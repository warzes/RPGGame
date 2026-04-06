#include "stdafx.h"
#include "GameScene.h"
#include "Camera.h"
//=============================================================================
bool GameScene::Init()
{
	m_data.Init();

	const auto wndWidth = window::GetWidth();
	const auto wndHeight = window::GetHeight();

	// Shaders
	m_vs = new ogl::ShaderStage(ogl::ShaderType::Vertex);
	m_vs->Upload(R"(
#version 450 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 vertexTexCoord;
layout(location = 4) in vec3 vertexTangent;
layout(location = 5) in vec3 vertexBitangent;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 v_Color;
out vec3 v_Normal;
out vec2 v_UV;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	v_Normal = normalize(mat3(modelMatrix) * vertexNormal);
	v_UV = vertexTexCoord;
	v_Color = vertexColor;
}
)");
	m_vs->Compile();

	m_fs = new ogl::ShaderStage(ogl::ShaderType::Fragment);
	m_fs->Upload(R"(
#version 450 core

in vec3 v_Color;
in vec3 v_Normal;
in vec2 v_UV;

out vec4 FragColor;
uniform vec3 u_Color = vec3(1.0, 1.0, 1.0);

layout(binding = 0) uniform sampler2D diffuseTexture;

const float alphaClippingThreshold = 0.1;

void main()
{
	float light = max(dot(v_Normal, vec3(0.0, 0.0, 1.0)), 0.2);

	vec4 diffuse = vec4(v_Color * u_Color, 1.0);
	//if (hasDiffuseTex) 
		diffuse = texture(diffuseTexture, v_UV) * diffuse;
	if (diffuse.a < alphaClippingThreshold) discard;

	FragColor = diffuse;
}
)");
	m_fs->Compile();

	m_program = new ogl::ShaderProgram;
	m_program->Attach(*m_vs);
	m_program->Attach(*m_fs);
	m_program->Link();
	
	if (!m_grid.Init())
		return false;

	if (!m_terra.Init())
		return false;

	return true;
}
//=============================================================================
void GameScene::Close()
{
	m_terra.Close();
	m_grid.Close();

	delete m_fs;
	delete m_vs;
	delete m_program;
}
//=============================================================================
void GameScene::Bind(Camera* camera)
{
	m_data.camera = camera;
}
//=============================================================================
void GameScene::Bind(GameModel* go)
{
	m_data.Bind(go);
}
//=============================================================================
void GameScene::Draw()
{
	if (!m_data.camera)
	{
		Warning("Not active camera");
		return;
	}
	beginDraw();
	draw();
	endDraw();
}
//=============================================================================
void GameScene::beginDraw()
{
	const auto wndWidth = window::GetWidth();
	const auto wndHeight = window::GetHeight();

	//glm::mat4 view = m_data.camera->GetViewMatrix();
	//glm::mat4 proj = glm::perspective(glm::radians(60.0f), window::GetAspect(), 0.1f, 1000.0f);
	//m_terra.Update(proj, view, m_data.camera->GetPosition());
}
//=============================================================================
void GameScene::draw()
{
	// Matrices
	const glm::mat4 view = m_data.camera->GetViewMatrix();
	const glm::mat4 proj = glm::perspective(glm::radians(60.0f), window::GetAspect(), 0.1f, 1000.0f);

	ogl::SetViewport({ window::GetWidth(), window::GetHeight() });
	ogl::SetClearColor(0.3f, 0.4f, 0.9f, 1.0f);
	ogl::Clear(true, true, true);

	m_grid.Draw(proj, view);
	//m_terra.Draw(proj, view);

	if (m_data.countGameModels == 0) return;

	m_program->Bind();
	m_program->SetUniform("viewMatrix", view);
	m_program->SetUniform("projectionMatrix", proj);
	m_program->SetUniform("u_Color", glm::vec3{ 1.0f, 1.0f, 1.0f });
	
	for (size_t i = 0; i < m_data.countGameModels; i++)
	{
		if (   !m_data.gameModels[i]
			|| !m_data.gameModels[i]->visible
			|| !m_data.gameModels[i]->model.Valid()
			||  m_data.gameModels[i]->model.GetMeshes().empty() )
			continue;

		auto& modelMatrix = m_data.gameModels[i]->modelMatrix;
		m_program->SetUniform("modelMatrix", modelMatrix);

		for (size_t m = 0; m < m_data.gameModels[i]->model.GetMeshes().size(); m++)
		{
			auto* mesh = m_data.gameModels[i]->model.GetMesh(m);
			if (mesh)
			{
				const auto& material = mesh->GetMaterial();
				if (material.has_value())
				{
					if (!material->diffuseTextures.empty())
					{
						material->diffuseTextures[0].id->Bind(0);
						m_program->SetUniform("diffuseTexture", 0);
					}
				}
				mesh->Draw();
			}
		}
	}
	m_program->Unbind();
}
//=============================================================================
void GameScene::endDraw()
{
	m_data.ResetFrame();
}
//=============================================================================
void GameScene::blittingToScreen(GLuint fbo, uint16_t srcWidth, uint16_t srcHeight)
{
}
//=============================================================================