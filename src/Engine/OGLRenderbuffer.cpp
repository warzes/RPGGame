#include "stdafx.h"
#include "OGLRenderbuffer.h"
//=============================================================================
ogl::Renderbuffer::Renderbuffer()
{
	glCreateRenderbuffers(1, &m_id);
}
//=============================================================================
ogl::Renderbuffer::~Renderbuffer()
{
	glDeleteRenderbuffers(1, &m_id);
}
//=============================================================================
void ogl::Renderbuffer::Bind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_id);
}
//=============================================================================
void ogl::Renderbuffer::Unbind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
//=============================================================================
void ogl::Renderbuffer::Allocate(uint16_t width, uint16_t height, InternalFormat format)
{
	m_width = width;
	m_height = height;
	m_format = format;
	glNamedRenderbufferStorage(m_id, EnumToValue(m_format), m_width, m_height);
	m_allocated = true;
}
//=============================================================================
void ogl::Renderbuffer::Resize(uint16_t width, uint16_t height)
{
	assert(IsValid() && "Cannot resize a renderbuffer that has not been allocated");
	Allocate(width, height, m_format);
}
//=============================================================================
uint16_t ogl::Renderbuffer::GetWidth() const
{
	assert(IsValid() && "Cannot get width of an invalid renderbuffer");
	return m_width;
}
//=============================================================================
uint16_t ogl::Renderbuffer::GetHeight() const
{
	assert(IsValid() && "Cannot get height of an invalid renderbuffer");
	return m_height;
}
//=============================================================================
bool ogl::Renderbuffer::IsValid() const
{
	return m_allocated;
}
//=============================================================================