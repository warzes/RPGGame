#include "stdafx.h"
#include "MapGrid.h"
//=============================================================================
std::vector<float> generateGrid(float size, float step)
{
	std::vector<float> vertices;

	for (float i = -size; i <= size; i += step) {
		// Горизонтальные линии
		vertices.insert(vertices.end(), {
			-size + 0.5f, 0.0f, i + 0.5f,
			 size + 0.5f, 0.0f, i + 0.5f,
			});

		// Вертикальные линии
		vertices.insert(vertices.end(), {
			i + 0.5f, 0.0f, -size + 0.5f,
			i + 0.5f, 0.0f,  size + 0.5f,
			});
	}

	return vertices;
}
//=============================================================================
bool MapGrid::Init()
{
	float gridSize = 100.0f;
	float gridStep = 1.0f;
	auto gridVertices = generateGrid(gridSize, gridStep);
	m_vertSize = gridVertices.size();

	m_vb = new ogl::Buffer;
	m_vb->Allocate(sizeof(float) * m_vertSize, ogl::AccessSpecifier::StaticDraw);
	m_vb->Upload(gridVertices.data());

	m_va = new ogl::VertexArray;
	//m_va->SetLayoutOLD({
	//		{ ogl::DataType::Float, 3 }
	//	}, *m_vb);

	// Shaders
	m_vs = new ogl::ShaderStage(ogl::ShaderType::Vertex);
	m_vs->Upload(R"(
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
}
)");
	m_vs->Compile();

	m_fs = new ogl::ShaderStage(ogl::ShaderType::Fragment);
	m_fs->Upload(R"(
#version 330 core

out vec4 color;

void main()
{
	color = vec4(vec3(0.0f), 1.0f);
}
)");
	m_fs->Compile();

	m_program = new ogl::ShaderProgram;
	m_program->Attach(*m_vs);
	m_program->Attach(*m_fs);
	m_program->Link();

	m_va->SetVertexBuffer(0, m_vb->GetID(), 0, sizeof(float) * 3);
	m_va->EnableAttrib(0);
	m_va->SetAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	m_va->SetAttribBinding(0, 0);

	return false;
}
//=============================================================================
void MapGrid::Close()
{
	delete m_vs; m_vs = nullptr;
	delete m_fs; m_fs = nullptr;
	delete m_program; m_program = nullptr;
	delete m_vb; m_vb = nullptr;
	delete m_va; m_va = nullptr;
}
//=============================================================================
void MapGrid::Draw(const glm::mat4& proj, const glm::mat4& view)
{
	ogl::SetRasterizationMode(ogl::RasterizationMode::Line);

	m_program->Bind();
	m_program->SetUniform("modelMatrix", glm::mat4(1.0f));
	m_program->SetUniform("viewMatrix", view);
	m_program->SetUniform("projectionMatrix", proj);

	m_va->Bind();
	ogl::DrawArrays(ogl::PrimitiveMode::Lines, m_vertSize / 3);
	m_va->Unbind();

	ogl::SetRasterizationMode(ogl::RasterizationMode::Fill);
}
//=============================================================================