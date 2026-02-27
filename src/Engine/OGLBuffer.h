#pragma once

// TODO: UBO нужно переписать.

enum class BufferUsage : uint8_t
{
	StaticDraw,
	DynamicDraw,
	StreamDraw,
	StaticRead,
	DynamicRead,
	StreamRead,
	StaticCopy,
	DynamicCopy,
	StreamCopy
};

struct VertexBufferHandle final 
{ 
	GLuint handle{ 0u }; 
};

struct IndexBufferHandle final
{
	GLuint handle{ 0u };
};

struct UniformBufferHandle final
{
	GLuint handle{ 0u };
};

GLuint GetCurrentVertexBindBuffer();
GLuint GetCurrentIndexBindBuffer();

VertexBufferHandle CreateVertexBuffer(BufferUsage usage, size_t size, const void* data);
IndexBufferHandle CreateIndexBuffer(BufferUsage usage, size_t size, const void* data);
UniformBufferHandle CreateUniformBuffer(BufferUsage usage, size_t size, const void* data);

void DestroyBuffer(VertexBufferHandle& handle);
void DestroyBuffer(IndexBufferHandle& handle);
void DestroyBuffer(UniformBufferHandle& handle);

void BufferSubData(VertexBufferHandle& bufferId, GLintptr offset, GLsizeiptr size, const void* data);
void BufferSubData(IndexBufferHandle& bufferId, GLintptr offset, GLsizeiptr size, const void* data);
void BufferSubData(UniformBufferHandle& bufferId, GLintptr offset, GLsizeiptr size, const void* data);

void Bind(VertexBufferHandle bufferId);
void Bind(IndexBufferHandle bufferId);
void Bind(UniformBufferHandle bufferId, std::optional<uint32_t> slot = std::nullopt);