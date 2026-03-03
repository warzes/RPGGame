#include "stdafx.h"
#include "OGLVertexArray.h"
//=============================================================================
uint32_t GetDataTypeSizeInBytes(ogl::DataType type)
{
	switch (type)
	{
	case ogl::DataType::Byte: return sizeof(GLbyte);
	case ogl::DataType::UnsignedByte: return sizeof(GLubyte);
	case ogl::DataType::Short: return sizeof(GLshort);
	case ogl::DataType::UnsignedShort: return sizeof(GLushort);
	case ogl::DataType::Int: return sizeof(GLint);
	case ogl::DataType::UnsignedInt: return sizeof(GLuint);
	case ogl::DataType::Float: return sizeof(GLfloat);
	case ogl::DataType::Double: return sizeof(GLdouble);
	default: return 0;
	}
}
//=============================================================================
uint32_t CalculateTotalVertexSize(const std::initializer_list<const ogl::VertexAttribute>& attributes)
{
	uint32_t result = 0;
	for (const auto& attribute : attributes)
	{
		result += GetDataTypeSizeInBytes(attribute.type) * attribute.count;
	}
	return result;
}
//=============================================================================
ogl::VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_id);
}
//=============================================================================
ogl::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_id);
}
//=============================================================================
void ogl::VertexArray::SetLayout(VertexAttributeLayout attributes, Buffer& vertexBuffer)
{
	assert(!IsValid() && "Vertex array layout already set");

	Bind();
	vertexBuffer.Bind(BufferType::Vertex);

	uint32_t attributeIndex = 0;

	const uint32_t totalSize = CalculateTotalVertexSize(attributes);
	intptr_t currentOffset = 0;

	for (const auto& attribute : attributes)
	{
		assert(attribute.count >= 1 && attribute.count <= 4 && "Attribute count must be between 1 and 4");

		glEnableVertexAttribArray(attributeIndex);

		glVertexAttribPointer(
			static_cast<GLuint>(attributeIndex),
			static_cast<GLint>(attribute.count),
			EnumToValue(attribute.type),
			static_cast<GLboolean>(attribute.normalized),
			static_cast<GLsizei>(totalSize),
			reinterpret_cast<const GLvoid*>(currentOffset)
		);

		const uint64_t typeSize = GetDataTypeSizeInBytes(attribute.type);
		const uint64_t attributeSize = typeSize * attribute.count;
		currentOffset += attributeSize;
		++attributeIndex;
		++m_attributeCount;
	}

	Unbind();
	vertexBuffer.Unbind();
}
//=============================================================================
void ogl::VertexArray::SetLayout(VertexAttributeLayout attributes, Buffer& vertexBuffer, Buffer& indexBuffer)
{
	assert(!IsValid() && "Vertex array layout already set");

	Bind();
	indexBuffer.Bind(BufferType::Index);
	vertexBuffer.Bind(BufferType::Vertex);

	uint32_t attributeIndex = 0;

	const uint32_t totalSize = CalculateTotalVertexSize(attributes);
	intptr_t currentOffset = 0;

	for (const auto& attribute : attributes)
	{
		assert(attribute.count >= 1 && attribute.count <= 4 && "Attribute count must be between 1 and 4");

		glEnableVertexAttribArray(attributeIndex);

		glVertexAttribPointer(
			static_cast<GLuint>(attributeIndex),
			static_cast<GLint>(attribute.count),
			EnumToValue(attribute.type),
			static_cast<GLboolean>(attribute.normalized),
			static_cast<GLsizei>(totalSize),
			reinterpret_cast<const GLvoid*>(currentOffset)
		);

		const uint64_t typeSize = GetDataTypeSizeInBytes(attribute.type);
		const uint64_t attributeSize = typeSize * attribute.count;
		currentOffset += attributeSize;
		++attributeIndex;
		++m_attributeCount;
	}

	Unbind();
	indexBuffer.Unbind();
	vertexBuffer.Unbind();
}
//=============================================================================
void ogl::VertexArray::ResetLayout()
{
	assert(IsValid() && "Vertex array layout not already set");

	Bind();
	for (uint32_t i = 0; i < m_attributeCount; ++i)
	{
		glDisableVertexAttribArray(i);
	}
	m_attributeCount = 0;
	Unbind();
}
//=============================================================================
void ogl::VertexArray::Bind() const
{
	glBindVertexArray(m_id);
}
//=============================================================================
void ogl::VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
//=============================================================================
bool ogl::VertexArray::IsValid() const
{
	return m_attributeCount > 0;
}
//=============================================================================