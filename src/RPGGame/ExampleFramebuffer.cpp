#include "stdafx.h"
//=============================================================================
void ExampleFramebuffer()
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
		va.SetLayoutOLD({
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

		// Texture creation (framebuffer color attachment)
		std::shared_ptr<ogl::Texture> colorBuffer = std::make_shared<ogl::Texture>(ogl::TextureType::Texture2D);
		colorBuffer->Allocate(ogl::TextureDesc{
			.width = 1,
			.height = 1,
			.internalFormat = ogl::InternalFormat::RGBA32F,
			.useMipMaps = false,
			.mutableDesc = ogl::MutableTextureDesc{
				.format = ogl::Format::RGBA,
				.type = ogl::PixelDataType::Float
			}
			});

		// Renderbuffer creation (framebuffer depth attachment)
		std::shared_ptr<ogl::Renderbuffer> depthBuffer = std::make_shared<ogl::Renderbuffer>();
		depthBuffer->Allocate(1, 1, ogl::InternalFormat::DEPTH_COMPONENT);

		ogl::Framebuffer framebuffer;
		framebuffer.Attach(colorBuffer, ogl::FramebufferAttachment::Color);
		framebuffer.Attach(depthBuffer, ogl::FramebufferAttachment::Depth);
		assert(framebuffer.Validate());

		while (!engine::ShouldClose())
		{
			engine::BeginFrame();

			// Draw to framebuffer
			framebuffer.Bind();
			framebuffer.Resize(window::GetWidth(), window::GetHeight());
			ogl::SetViewport(0, 0, window::GetWidth(), window::GetHeight());
			ogl::SetClearColor(0.3f, 0.4f, 0.9f, 1.0f);
			ogl::Clear(true, true, true);
			program.Bind();
			va.Bind();
			ogl::DrawElements(ogl::PrimitiveMode::Triangles, 3);
			va.Unbind();
			program.Unbind();
			framebuffer.Unbind();

			// Blit framebuffer to backbuffer
			framebuffer.BlitToBackBuffer(window::GetWidth(), window::GetHeight());

			engine::DrawFPS();
			engine::EndFrame();
		}
	}
	engine::Close();
}
//=============================================================================