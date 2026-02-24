#pragma once

enum class BufferTarget : uint8_t
{
	Array,
	ElementArray,
	Uniform
};

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

struct BufferHandle final 
{ 
	GLuint       handle{ 0u }; 
	BufferTarget target{ BufferTarget::Array }; // TODO:
};

GLuint GetCurrentBindBuffer(BufferTarget target);

BufferHandle CreateBuffer(BufferTarget target, BufferUsage usage, size_t size, const void* data);
void DestroyBuffer(BufferHandle& handle);

void BufferSubData(BufferHandle bufferId, GLintptr offset, GLsizeiptr size, const void* data);