#include "stdafx.h"
//=============================================================================
namespace
{
	struct Particle
	{
		glm::vec2 position = { 0.0f, 0.0f };
		glm::vec2 velocity = { 0.0f, 0.0f };
		float life = 0.0f;

		// std430 requires the struct to be aligned based on the largest element (vec2: 8 bytes),
		// so adding a padding changes the struct size from 20 to 24 (multiple of 8).
		float padding = 0.0f;
	};
}
void ExampleCompute()
{
	if (engine::Init(1600, 900, "Game"))
	{
		constexpr uint32_t k_particleCount = 4096;
		std::vector<Particle> particles(k_particleCount); // Heap allocated, due to its size.

		// Storage buffer for particles
		ogl::Buffer particleBuffer;
		particleBuffer.Allocate(particles.size() * sizeof(Particle), ogl::AccessSpecifier::DynamicDraw);
		particleBuffer.Upload(particles.data());

		// Quad for rendering particles (instanced) - made slightly larger for visibility
		constexpr float quadVertices[] = {
			-0.005f, -0.005f,
			 0.005f, -0.005f,
			 0.005f,  0.005f,
			-0.005f,  0.005f
		};
		constexpr uint32_t quadIndices[] = { 0, 1, 2, 0, 2, 3 };

		ogl::Buffer vb;
		vb.Allocate(sizeof(quadVertices), ogl::AccessSpecifier::StaticDraw);
		vb.Upload(quadVertices);

		ogl::Buffer ib;
		ib.Allocate(sizeof(quadIndices), ogl::AccessSpecifier::StaticDraw);
		ib.Upload(quadIndices);

		ogl::VertexArray va;
		va.SetLayout({
			{ ogl::DataType::Float, 2 }
			}, vb, ib);

		// Compute shader for particle simulation
		ogl::ShaderStage computeShader(ogl::ShaderType::Compute);
		computeShader.Upload(R"(
#version 450 core
layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Particle {
	vec2 position;
	vec2 velocity;
	float life;
};

layout(std430, binding = 0) restrict buffer ParticleBuffer {
	 Particle particles[];
};

uniform float u_DeltaTime;
uniform float u_Time;

// Simple pseudo-random function
float random(vec2 st) {
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
	uint index = gl_GlobalInvocationID.x;
	if (index >= particles.length()) return;
	
	// Update particle position
	particles[index].position += particles[index].velocity * u_DeltaTime;
	
	// Decrease life
	particles[index].life -= u_DeltaTime;
	
	// Respawn particle if dead
	if (particles[index].life <= 0.0)
	{
		// Reset to center
		particles[index].position = vec2(0.0, 0.0);
		
		// Generate random angle for initial velocity
		vec2 seed = vec2(float(index), u_Time + float(index));
		float angle = random(seed) * 6.28318530718; // 2 * PI
		float speed = 0.1 + random(seed + vec2(1.0, 0.0)) * 0.2; // Speed between 0.1 and 0.3
		
		particles[index].velocity = vec2(
			cos(angle) * speed,
			sin(angle) * speed
		);
		
		// Random lifetime between 2 and 5 seconds
		particles[index].life = 2.0 + random(seed + vec2(0.0, 1.0)) * 3.0;
	}
	
	// Add gravity effect
	particles[index].velocity.y -= 0.05 * u_DeltaTime;
	
	// Add air resistance
	particles[index].velocity *= 0.999;
}
)");
		computeShader.Compile();

		ogl::ShaderProgram computeProgram;
		computeProgram.Attach(computeShader);
		computeProgram.Link();

		// Rendering shaders
		ogl::ShaderStage vertexShader(ogl::ShaderType::Vertex);
		vertexShader.Upload(R"(
#version 450 core
layout(location = 0) in vec2 a_Position;

struct Particle {
	vec2 position;
	vec2 velocity;
	float life;
};

layout(std430, binding = 0) restrict readonly buffer ParticleBuffer {
	Particle particles[];
};

out float v_Life;
out float v_MaxLife;

void main() {
	vec2 particlePos = particles[gl_InstanceID].position;
	vec2 worldPos = a_Position + particlePos;
	
	gl_Position = vec4(worldPos, 0.0, 1.0);
	v_Life = particles[gl_InstanceID].life;
	v_MaxLife = 5.0; // Maximum possible lifetime
}
)");
		vertexShader.Compile();

		ogl::ShaderStage fragmentShader(ogl::ShaderType::Fragment);
		fragmentShader.Upload(R"(
#version 450 core
in float v_Life;
in float v_MaxLife;
out vec4 FragColor;

void main() {
	// Skip dead particles
	if (v_Life <= 0.0) discard;
	
	// Calculate alpha based on remaining life
	float alpha = clamp(v_Life / v_MaxLife, 0.0, 1.0);
	
	// Color transition: white -> red as particle dies
	vec3 color = mix(vec3(1.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), alpha);
	
	FragColor = vec4(color, alpha * 0.8);
}
)");
		fragmentShader.Compile();

		ogl::ShaderProgram renderProgram;
		renderProgram.Attach(vertexShader);
		renderProgram.Attach(fragmentShader);
		renderProgram.Link();

		// Enable blending
		ogl::SetCapability(ogl::RenderingCapability::Blend, true);
		ogl::SetBlendingFunction(ogl::BlendFactor::SrcAlpha, ogl::BlendFactor::OneMinusSrcAlpha);

		while (!engine::ShouldClose())
		{
			engine::BeginFrame();

			ogl::SetViewport(0, 0, window::GetWidth(), window::GetHeight());
			ogl::SetClearColor(0.3f, 0.4f, 0.9f, 1.0f);
			ogl::Clear(true, true, true);

			// Compute shader dispatch
			computeProgram.Bind();
			particleBuffer.Bind(ogl::BufferType::ShaderStorage, 0);
			computeProgram.SetUniform("u_DeltaTime", engine::GetDeltaTime());
			computeProgram.SetUniform("u_Time", engine::GetCurrentTime()/1000.0f);
			constexpr uint32_t k_workGroupSize = 64;
			constexpr uint32_t k_dispatchSize = (k_particleCount + k_workGroupSize - 1) / k_workGroupSize;
			ogl::DispatchCompute(k_dispatchSize, 1, 1);
			ogl::MemoryBarrier(ogl::MemoryBarrierFlags::ShaderStorage);

			// Render particles
			renderProgram.Bind();
			particleBuffer.Bind(ogl::BufferType::ShaderStorage, 0);
			va.Bind();
			ogl::DrawElementsInstanced(ogl::PrimitiveMode::Triangles, 6, k_particleCount);
			va.Unbind();
			renderProgram.Unbind();

			engine::DrawFPS();
			engine::EndFrame();
		}
	}
	engine::Close();
}
//=============================================================================