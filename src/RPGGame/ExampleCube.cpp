#include "stdafx.h"
//=============================================================================
void ExampleCube()
{
	if (engine::Init(1600, 900, "Game"))
	{
		// Cube vertices (position, uv, normal)
		float vertices[] = {
			// Front face
			-1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			// Back face
			-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			 1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			-1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			// Left face
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			-1.0f,  1.0f,  1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			// Right face
			 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 // Top face
			 -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			  1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 // Bottom face
			 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			  1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			  1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
		};

		uint32_t indices[] = {
			0, 1, 2,   0, 2, 3,     // Front
			4, 5, 6,   4, 6, 7,     // Back
			8, 9, 10,  8, 10, 11,   // Left
			12, 13, 14, 12, 14, 15, // Right
			16, 17, 18, 16, 18, 19, // Top
			20, 21, 22, 20, 22, 23  // Bottom
		};

		ogl::Buffer vb;
		vb.Allocate(sizeof(vertices), ogl::AccessSpecifier::StaticDraw);
		vb.Upload(vertices);

		ogl::Buffer ib;
		ib.Allocate(sizeof(indices), ogl::AccessSpecifier::StaticDraw);
		ib.Upload(indices);

		ogl::VertexArray va;
		va.SetLayoutOLD({
			{ ogl::DataType::Float, 3 }, // Position
			{ ogl::DataType::Float, 2 }, // UV
			{ ogl::DataType::Float, 3 }  // Normal
		}, vb, ib);

		// Shaders
		ogl::ShaderStage vs(ogl::ShaderType::Vertex);
		vs.Upload(R"(
#version 450 core
layout(location = 0) in vec3 geo_Pos;
layout(location = 1) in vec2 geo_UV;
layout(location = 2) in vec3 geo_Normal;
uniform mat4 u_Model = mat4(1.0);
uniform mat4 u_View = mat4(1.0);
uniform mat4 u_Projection = mat4(1.0);
out vec3 v_Normal;
void main() {
	gl_Position = u_Projection * u_View * u_Model * vec4(geo_Pos, 1.0);
	v_Normal = normalize(mat3(u_Model) * geo_Normal);
}
)");
		vs.Compile();

		ogl::ShaderStage fs(ogl::ShaderType::Fragment);
		fs.Upload(R"(
#version 450 core
in vec3 v_Normal;
out vec4 FragColor;
uniform vec3 u_Color = vec3(1.0, 1.0, 0.0);
void main() {
	float light = max(dot(v_Normal, vec3(0.0, 0.0, 1.0)), 0.2);
	FragColor = vec4(u_Color * light, 1.0);
}
)");
		fs.Compile();

		ogl::ShaderProgram program;
		program.Attach(vs);
		program.Attach(fs);
		program.Link();

		ogl::SetCapability(ogl::RenderingCapability::DepthTest, true);

		// Camera
		glm::vec3 camPos = { 0.0f, 0.0f, 5.0f };

		while (!engine::ShouldClose())
		{
			engine::BeginFrame();

			// Input
			const float speed = 10.0f * engine::GetDeltaTime();
			if (input::IsKeyDown(RGFW_w)) camPos.z -= speed;
			if (input::IsKeyDown(RGFW_s)) camPos.z += speed;
			if (input::IsKeyDown(RGFW_a)) camPos.x -= speed;
			if (input::IsKeyDown(RGFW_d)) camPos.x += speed;
			if (input::IsKeyDown(RGFW_q)) camPos.y -= speed;
			if (input::IsKeyDown(RGFW_e)) camPos.y += speed;
			if (input::IsKeyDown(RGFW_escape)) break;

			ogl::SetViewport(0, 0, window::GetWidth(), window::GetHeight());
			ogl::SetClearColor(0.3f, 0.4f, 0.9f, 1.0f);
			ogl::Clear(true, true, true);

			// Matrices
			glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 view = glm::lookAt(camPos, camPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 proj = glm::perspective(glm::radians(60.0f), window::GetAspect(), 0.1f, 100.0f);

			program.Bind();
			program.SetUniform("u_Model", model);
			program.SetUniform("u_View", view);
			program.SetUniform("u_Projection", proj);
			program.SetUniform("u_Color", glm::vec3{ 1.0f, 1.0f, 0.0f });

			va.Bind();
			ogl::DrawElements(ogl::PrimitiveMode::Triangles, 36);
			va.Unbind();
			program.Unbind();

			engine::DrawFPS();
			engine::EndFrame();
		}
	}
	engine::Close();
}
//=============================================================================