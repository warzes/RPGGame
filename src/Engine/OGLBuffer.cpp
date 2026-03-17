#include "stdafx.h"
#include "OGLBuffer.h"
//=============================================================================
ogl::Buffer::Buffer()
{
	glCreateBuffers(1, &m_id);
}
//=============================================================================
ogl::Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_id);
}
//=============================================================================
void ogl::Buffer::Allocate(uint64_t size, AccessSpecifier usage)
{
	assert(IsValid() && "Cannot allocate memory for an invalid buffer");
	glNamedBufferData(m_id, size, nullptr, EnumToValue(usage));
	m_allocatedBytes = size;
}
//=============================================================================
void ogl::Buffer::Allocate(std::span<const std::byte> data, AccessSpecifier usage)
{
	assert(IsValid() && "Cannot allocate memory for an invalid buffer");
	glNamedBufferData(m_id, static_cast<GLsizeiptr>(data.size_bytes()), data.data(), EnumToValue(usage));
	m_allocatedBytes = static_cast<GLsizeiptr>(data.size_bytes());
}
//=============================================================================
void ogl::Buffer::Upload(const void* data, std::optional<BufferMemoryRange> range)
{
	assert(IsValid() && "Trying to upload data to an invalid buffer");
	assert(!IsEmpty() && "Trying to upload data to an empty buffer");
	glNamedBufferSubData(m_id, range ? range->offset : 0, range ? range->size : m_allocatedBytes, data);
}
//=============================================================================
void* ogl::Buffer::MapWrite(uint64_t offset, uint64_t size) const
{
	assert(IsValid() && "Trying to upload data to an invalid buffer");
	return glMapNamedBufferRange(m_id, offset, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
}
//=============================================================================
void ogl::Buffer::Unmap() const
{
	glUnmapNamedBuffer(m_id);
}
//=============================================================================
void ogl::Buffer::Bind(BufferType type, std::optional<uint32_t> index)
{
	assert(IsValid() && "Cannot bind an invalid buffer");

	if (index.has_value())
	{
		glBindBufferBase(EnumToValue(type), index.value(), m_id);
	}
	else
	{
		glBindBuffer(EnumToValue(type), m_id);
	}

	m_boundAs = type;
}
//=============================================================================
void ogl::Buffer::Unbind()
{
	assert(IsValid() && "Cannot unbind an invalid buffer");
	assert(m_boundAs.has_value() && "Cannot unbind a buffer that is not bound");
	glBindBuffer(EnumToValue(m_boundAs.value()), 0);
	m_boundAs.reset();
}
//=============================================================================
uint64_t ogl::Buffer::GetSize() const
{
	assert(IsValid() && "Cannot get size of an invalid buffer");
	return m_allocatedBytes;
}
//=============================================================================