#pragma once

#include "OGLCore.h"

namespace ogl
{
	class Renderbuffer final
	{
	public:
		Renderbuffer();
		Renderbuffer(const Renderbuffer&) = delete;
		Renderbuffer& operator=(const Renderbuffer&) = delete;
		Renderbuffer(Renderbuffer&&) = delete;
		Renderbuffer& operator=(Renderbuffer&&) = delete;
		~Renderbuffer();

		void Bind() const;
		void Unbind() const;

		void Allocate(uint16_t width, uint16_t height, InternalFormat format);

		void Resize(uint16_t width, uint16_t height);

		uint16_t GetWidth() const;
		uint16_t GetHeight() const;

		bool IsValid() const;
		GLuint GetID() const { return m_id; }
	private:
		GLuint m_id{ 0 };
		uint16_t m_width = 0;
		uint16_t m_height = 0;
		InternalFormat m_format = InternalFormat::RGBA;
		bool m_allocated = false;
	};
} // namespace ogl