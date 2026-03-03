#pragma once

#include "OGLBuffer.h"

namespace ogl
{
	struct VertexAttribute final
	{
		DataType type{ DataType::Float };
		uint8_t count{ 4 };
		bool normalized{ false };
	};

	using VertexAttributeLayout = std::initializer_list<const VertexAttribute>;

	class VertexArray final
	{
	public:
		VertexArray();
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray(VertexArray&&) = delete;
		VertexArray& operator=(VertexArray&&) = delete;
		~VertexArray();

		void SetLayout(VertexAttributeLayout attributes, Buffer& vertexBuffer);
		void SetLayout(VertexAttributeLayout attributes, Buffer& vertexBuffer, Buffer& indexBuffer);
		void ResetLayout();

		void Bind() const;
		void Unbind() const;

		bool IsValid() const;
		GLuint GetID() const { return m_id; }
	private:
		GLuint m_id{ 0 };
		uint32_t m_attributeCount{ 0 };
	};
} // namespace ogl