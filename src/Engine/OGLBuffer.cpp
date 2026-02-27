#include "stdafx.h"
#include "OGLBuffer.h"
//=============================================================================
inline GLenum EnumToValue(BufferUsage mode) noexcept
{
	switch (mode) {
	case BufferUsage::StaticDraw:  return GL_STATIC_DRAW;
	case BufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
	case BufferUsage::StreamDraw:  return GL_STREAM_DRAW;
	case BufferUsage::StaticRead:  return GL_STATIC_READ;
	case BufferUsage::DynamicRead: return GL_DYNAMIC_READ;
	case BufferUsage::StreamRead:  return GL_STREAM_READ;
	case BufferUsage::StaticCopy:  return GL_STATIC_COPY;
	case BufferUsage::DynamicCopy: return GL_DYNAMIC_COPY;
	case BufferUsage::StreamCopy:  return GL_STREAM_COPY;
	default: std::unreachable();
	}
}
//=============================================================================
GLuint GetCurrentVertexBindBuffer()
{
	GLuint currentBuffer{ 0 };
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&currentBuffer);
	return currentBuffer;
}
//=============================================================================
GLuint GetCurrentIndexBindBuffer()
{
	GLuint currentBuffer{ 0 };
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*)&currentBuffer);
	return currentBuffer;
}
//=============================================================================
VertexBufferHandle CreateVertexBuffer(BufferUsage usage, size_t size, const void* data)
{
	const GLuint currentBuffer = GetCurrentVertexBindBuffer();
	VertexBufferHandle buffer{};
	glGenBuffers(1, &buffer.handle);
	glBindBuffer(GL_ARRAY_BUFFER, buffer.handle);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, EnumToValue(usage));
	glBindBuffer(GL_ARRAY_BUFFER, currentBuffer);
	return buffer;
}
//=============================================================================
IndexBufferHandle CreateIndexBuffer(BufferUsage usage, size_t size, const void* data)
{
	const GLuint currentBuffer = GetCurrentIndexBindBuffer();
	IndexBufferHandle buffer{};
	glGenBuffers(1, &buffer.handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, EnumToValue(usage));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentBuffer);
	return buffer;
}
//=============================================================================
UniformBufferHandle CreateUniformBuffer(BufferUsage usage, size_t size, const void* data)
{
	// TODO: с UBO нельзя получить текущий бинд, как у вершинных/индексных буферов, так как оно привязывается к слотам (через glBindBufferBase)
	// поэтому рабочий вариант - это делать свою функцию BindUbo в которой кешировать текущие ubo

	UniformBufferHandle buffer{};
	glGenBuffers(1, &buffer.handle);
	glBindBuffer(GL_UNIFORM_BUFFER, buffer.handle);
	glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(size), data, EnumToValue(usage));
	return buffer;
}
//=============================================================================
void DestroyBuffer(VertexBufferHandle& handle)
{
	if (handle.handle)
	{
		const GLuint currentBuffer = GetCurrentVertexBindBuffer();
		if (currentBuffer == handle.handle)
			glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &handle.handle);
	}
	handle.handle = 0;
}
//=============================================================================
void DestroyBuffer(IndexBufferHandle& handle)
{
	if (handle.handle)
	{
		const GLuint currentBuffer = GetCurrentIndexBindBuffer();
		if (currentBuffer == handle.handle)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &handle.handle);
	}
	handle.handle = 0;
}
//=============================================================================
void DestroyBuffer(UniformBufferHandle& handle)
{
	if (handle.handle)
	{
		glDeleteBuffers(1, &handle.handle);
	}
	handle.handle = 0;
}
//=============================================================================
void BufferSubData(VertexBufferHandle bufferId, GLintptr offset, GLsizeiptr size, const void* data)
{
	GLuint currentBuffer = GetCurrentVertexBindBuffer();

	glBindBuffer(GL_ARRAY_BUFFER, bufferId.handle);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, currentBuffer);
}
//=============================================================================
void BufferSubData(IndexBufferHandle bufferId, GLintptr offset, GLsizeiptr size, const void* data)
{
	GLuint currentBuffer = GetCurrentIndexBindBuffer();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId.handle);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentBuffer);
}
//=============================================================================
void BufferSubData(UniformBufferHandle bufferId, GLintptr offset, GLsizeiptr size, const void* data)
{
	// TODO: с UBO нельзя получить текущий бинд, как у вершинных/индексных буферов, так как оно привязывается к слотам (через glBindBufferBase)
	// поэтому рабочий вариант - это делать свою функцию BindUbo в которой кешировать текущие ubo

	glBindBuffer(GL_UNIFORM_BUFFER, bufferId.handle);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}
//=============================================================================
void Bind(VertexBufferHandle bufferId)
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferId.handle);
}
//=============================================================================
void Bind(IndexBufferHandle bufferId)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId.handle);
}
//=============================================================================
void Bind(UniformBufferHandle bufferId, std::optional<uint32_t> slot)
{
	if (slot.has_value())
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, slot.value(), bufferId.handle);
	}
	else
	{
		glBindBuffer(GL_UNIFORM_BUFFER, bufferId.handle);
	}
}
//=============================================================================