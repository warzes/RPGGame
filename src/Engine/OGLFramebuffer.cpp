#include "stdafx.h"
#include "OGLFramebuffer.h"
#include "Log.h"
//=============================================================================
inline GLenum enumToValue(ogl::FramebufferAttachment format) noexcept
{
	switch (format)
	{
	case ogl::FramebufferAttachment::Color:        return GL_COLOR_ATTACHMENT0;
	case ogl::FramebufferAttachment::Depth:        return GL_DEPTH_ATTACHMENT;
	case ogl::FramebufferAttachment::Stencil:      return GL_STENCIL_ATTACHMENT;
	case ogl::FramebufferAttachment::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
	}
}
//=============================================================================
ogl::Framebuffer::Framebuffer(std::string_view debugName) 
	: m_debugName{ debugName }
{
	glCreateFramebuffers(1, &m_id);
}
//=============================================================================
ogl::Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_id);
}
//=============================================================================
void ogl::Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}
//=============================================================================
void ogl::Framebuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//=============================================================================
bool ogl::Framebuffer::Validate()
{
	const GLenum status = glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		Error("Framebuffer validation failed with status: " + std::to_string(status));
		return m_valid = false;
	}

	return m_valid = true;
}
//=============================================================================
void ogl::Framebuffer::Resize(uint16_t width, uint16_t height)
{
	assert(IsValid() && "Cannot resize an invalid framebuffer");

	for (auto& attachment : m_attachments)
	{
		if (const auto pval = std::get_if<std::shared_ptr<Texture>>(&attachment.second); pval && *pval)
		{
			(*pval)->Resize(width, height);
		}
		else if (const auto* pval = std::get_if<std::shared_ptr<Renderbuffer>>(&attachment.second); pval && *pval)
		{
			(*pval)->Resize(width, height);
		}
	}
}
//=============================================================================
template<>
void ogl::Framebuffer::Attach(std::shared_ptr<Renderbuffer> toAttach, FramebufferAttachment attachment, uint32_t index, std::optional<uint32_t> layer)
{
	assert(toAttach != nullptr && "Cannot attach a null renderbuffer");
	assert(!layer.has_value() && "Renderbuffer cannot use layers");

	const auto attachmentIndex = enumToValue(attachment) + static_cast<GLenum>(index);
	glNamedFramebufferRenderbuffer(m_id, attachmentIndex, GL_RENDERBUFFER, toAttach->GetID());
	m_attachments[attachmentIndex] = toAttach;
}
//=============================================================================
template<>
void ogl::Framebuffer::Attach(std::shared_ptr<Texture> toAttach, FramebufferAttachment attachment, uint32_t index, std::optional<uint32_t> layer)
{
	assert(toAttach != nullptr && "Cannot attach a null texture");

	const auto attachmentIndex = enumToValue(attachment) + static_cast<GLenum>(index);
	constexpr uint32_t k_mipMapLevel = 0;

	if (layer.has_value())
	{
		glNamedFramebufferTextureLayer(m_id, attachmentIndex, toAttach->GetID(), k_mipMapLevel, layer.value());
	}
	else
	{
		glNamedFramebufferTexture(m_id, attachmentIndex, toAttach->GetID(), k_mipMapLevel);
	}

	m_attachments[attachmentIndex] = toAttach;
}
//=============================================================================
template<>
std::optional<std::reference_wrapper<ogl::Texture>> ogl::Framebuffer::GetAttachment(FramebufferAttachment attachment, uint32_t index) const
{
	const auto attachmentIndex = enumToValue(attachment) + static_cast<GLenum>(index);

	if (m_attachments.contains(attachmentIndex))
	{
		auto attachment = m_attachments.at(attachmentIndex);

		if (auto pval = std::get_if<std::shared_ptr<Texture>>(&attachment); pval && *pval)
		{
			return **pval;
		}
	}

	return std::nullopt;
}
//=============================================================================
template<>
std::optional<std::reference_wrapper<ogl::Renderbuffer>> ogl::Framebuffer::GetAttachment(FramebufferAttachment attachment, uint32_t index) const
{
	const auto attachmentIndex = enumToValue(attachment) + static_cast<GLenum>(index);

	if (m_attachments.contains(attachmentIndex))
	{
		auto attachment = m_attachments.at(attachmentIndex);

		if (auto pval = std::get_if<std::shared_ptr<Renderbuffer>>(&attachment); pval && *pval)
		{
			return **pval;
		}
	}

	return std::nullopt;
}
//=============================================================================
void ogl::Framebuffer::SetTargetDrawBuffer(std::optional<uint32_t> index)
{
	assert(IsValid() && "Invalid framebuffer");

	if (index.has_value())
	{
		glNamedFramebufferDrawBuffer(m_id, GL_COLOR_ATTACHMENT0 + index.value());
	}
	else
	{
		glNamedFramebufferDrawBuffer(m_id, GL_NONE);
	}
}
//=============================================================================
void ogl::Framebuffer::SetTargetReadBuffer(std::optional<uint32_t> index)
{
	assert(IsValid() && "Invalid framebuffer");

	if (index.has_value())
	{
		glNamedFramebufferReadBuffer(m_id, GL_COLOR_ATTACHMENT0 + index.value());
	}
	else
	{
		glNamedFramebufferReadBuffer(m_id, GL_NONE);
	}
}
//=============================================================================
std::pair<uint16_t, uint16_t> ogl::Framebuffer::GetSize(FramebufferAttachment attachment) const
{
	assert(IsValid() && "Cannot get width of an invalid framebuffer");

	for (auto& attachment : m_attachments)
	{
		if (const auto pval = std::get_if<std::shared_ptr<Texture>>(&attachment.second); pval && *pval)
		{
			return {
				(*pval)->GetDesc().width,
				(*pval)->GetDesc().height
			};
		}
		else if (const auto* pval = std::get_if<std::shared_ptr<Renderbuffer>>(&attachment.second); pval && *pval)
		{
			return {
				(*pval)->GetWidth(),
				(*pval)->GetHeight()
			};
		}
	}

	return { {}, {} };
}
//=============================================================================
void ogl::Framebuffer::BlitToBackBuffer(uint16_t backBufferWidth, uint16_t backBufferHeight) const
{
	assert(IsValid(), "Cannot blit an invalid framebuffer");

	auto [width, height] = GetSize(FramebufferAttachment::Color);

	glBlitNamedFramebuffer(
		m_id, 0,
		0, 0, width, height,
		0, 0, backBufferWidth, backBufferHeight,
		GL_COLOR_BUFFER_BIT, GL_LINEAR
	);
}
//=============================================================================
void ogl::Framebuffer::ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, PixelDataFormat format, PixelDataType type, void* data) const
{
	assert(IsValid() && "Cannot read pixels from an invalid framebuffer");
	assert(width > 0 && height > 0 && "Invalid read size");

	Bind();
	glReadPixels(x, y, width, height, EnumToValue(format), EnumToValue(type), data);
	Unbind();
}
//=============================================================================
const std::string& ogl::Framebuffer::GetDebugName() const
{
	return m_debugName;
}
//=============================================================================
bool ogl::Framebuffer::IsValid() const
{
	return m_valid;
}
//=============================================================================