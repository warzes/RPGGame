#include "stdafx.h"
//=============================================================================
void ExampleTriangle()
{
	if (engine::Init(1600, 900, "Game"))
	{
		const float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,	// Bottom left
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	// Bottom right
			 0.0f,  0.5f, 0.0f, 0.5f, 1.0f	// Top center
		};
		const uint32_t indices[] = { 0, 1, 2 };

		ogl::Buffer vb;
		vb.Allocate(sizeof(vertices), ogl::AccessSpecifier::StaticDraw);
		vb.Upload(vertices);
		ogl::Buffer ib;
		ib.Allocate(sizeof(indices), ogl::AccessSpecifier::StaticDraw);
		ib.Upload(indices);

		ogl::VertexArray va;
		va.SetLayout({
				{ ogl::DataType::Float, 3 },
				{ ogl::DataType::Float, 2 }
			}, vb, ib);

		// Shaders
		ogl::ShaderStage vs(ogl::ShaderType::Vertex);
		vs.Upload(R"(
#version 450 core
layout(location = 0) in vec3 geo_Pos;
layout(location = 1) in vec2 geo_UV;
out vec2 v_UV;
void main() {
	gl_Position = vec4(geo_Pos, 1.0);
	v_UV = geo_UV;
}
)");
		vs.Compile();

		ogl::ShaderStage fs(ogl::ShaderType::Fragment);
		fs.Upload(R"(
#version 450 core
in vec2 v_UV;
out vec4 FragColor;
void main() {
	FragColor = vec4(v_UV, 0.5, 1.0);
}
)");
		fs.Compile();

		ogl::ShaderProgram program;
		program.Attach(vs);
		program.Attach(fs);
		program.Link();

		while (!engine::ShouldClose())
		{
			engine::BeginFrame();

			ogl::SetViewport(0, 0, window::GetWidth(), window::GetHeight());
			ogl::SetClearColor(0.3f, 0.4f, 0.9f, 1.0f);
			ogl::Clear(true, true, true);

			program.Bind();
			va.Bind();
			ogl::DrawElements(ogl::PrimitiveMode::Triangles, 3);
			va.Unbind();
			program.Unbind();

			engine::DrawFPS();
			engine::EndFrame();
		}
	}
	engine::Close();
}
//=============================================================================