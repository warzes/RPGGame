#include "stdafx.h"
#include "OGLTexture.h"
#include "Log.h"
//=============================================================================
inline ogl::TextureType ValueToEnum(GLenum type) noexcept
{
	switch (type)
	{
	case GL_TEXTURE_2D:       return ogl::TextureType::Texture2D;
	case GL_TEXTURE_CUBE_MAP: return ogl::TextureType::TextureCube;
	default: std::unreachable();
	}
}
//=============================================================================
constexpr uint32_t CalculateMipMapLevels(uint32_t p_width, uint32_t p_height)
{
	uint32_t maxDim = p_width > p_height ? p_width : p_height;
	uint32_t levels = 0;

	while (maxDim > 1)
	{
		maxDim >>= 1;
		++levels;
	}

	return levels ? levels + 1 : 1u;
}
//=============================================================================
constexpr bool IsValidMipMapFilter(ogl::TextureFilteringMode p_mode)
{
	return
		p_mode == ogl::TextureFilteringMode::NearestMipmapNearest ||
		p_mode == ogl::TextureFilteringMode::NearestMipmapLinear ||
		p_mode == ogl::TextureFilteringMode::LinearMipmapNearest ||
		p_mode == ogl::TextureFilteringMode::LinearMipmapLinear;
}
//=============================================================================
ogl::Texture::Texture(TextureType type, std::string_view debugName) 
	: m_type(EnumToValue(type))
{	
	glCreateTextures(m_type, 1, &m_id);
	m_debugName = debugName;
}
//=============================================================================
ogl::Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}
//=============================================================================
void ogl::Texture::Allocate(const TextureDesc& ndesc)
{
	auto& desc = m_desc;
	desc = ndesc;
	desc.width = std::max(1u, desc.width);
	desc.height = std::max(1u, desc.height);

	if (desc.mutableDesc.has_value())
	{
		const auto& mutableDesc = desc.mutableDesc.value();

		assert(m_type == GL_TEXTURE_2D && "Mutable textures are only supported for 2D textures");

		// No DSA version for glTexImage2D (mutable texture),
		// so we need to Bind/Unbind the texture.
		Bind();
		glTexImage2D(
			m_type,
			0,
			EnumToValue(desc.internalFormat),
			desc.width,
			desc.height,
			0,
			EnumToValue(mutableDesc.format),
			EnumToValue(mutableDesc.type),
			mutableDesc.data
		);
		Unbind();
	}
	else
	{
		// If the underlying texture is a cube map, this will allocate all 6 sides.
		// No need to iterate over each side.
		glTextureStorage2D(
			m_id,
			desc.useMipMaps ? CalculateMipMapLevels(desc.width, desc.height) : 1,
			EnumToValue(desc.internalFormat),
			desc.width,
			desc.height
		);
	}

	// Once the texture is allocated, we don't need to set the parameters again
	if (!m_allocated)
	{
		glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, EnumToValue(ndesc.horizontalWrap));
		glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, EnumToValue(ndesc.verticalWrap));
		glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, EnumToValue(ndesc.minFilter));
		glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, EnumToValue(ndesc.magFilter));
	}

	m_allocated = true;

}
//=============================================================================
void ogl::Texture::Upload(const void* data, Format format, PixelDataType type)
{
	assert(IsValid() && "Cannot upload data to a texture before it has been allocated");
	assert(data && "Cannot upload texture data from a null pointer");

	if (IsMutable())
	{
		m_desc.mutableDesc.value().data = data;
		Allocate(m_desc);
	}
	else
	{
		if (m_type == GL_TEXTURE_CUBE_MAP)
		{
			for (uint32_t i = 0; i < 6; ++i)
			{
				glTextureSubImage3D(
					m_id,
					0,
					0,
					0,
					0,
					m_desc.width,
					m_desc.height,
					i,
					EnumToValue(format),
					EnumToValue(type),
					data
				);
			}
		}
		else
		{
			glTextureSubImage2D(
				m_id,
				0,
				0,
				0,
				m_desc.width,
				m_desc.height,
				EnumToValue(format),
				EnumToValue(type),
				data
			);
		}
	}
}
//=============================================================================
void ogl::Texture::Resize(uint32_t width, uint32_t height)
{
	assert(IsValid() && "Cannot resize a texture before it has been allocated");
	assert(IsMutable() && "Cannot resize an immutable texture");

	auto& desc = m_desc;

	if (width != desc.width || height != desc.height)
	{
		desc.width = width;
		desc.height = height;
		Allocate(desc);
	}
}
//=============================================================================
const ogl::TextureDesc& ogl::Texture::GetDesc() const
{
	assert(IsValid() && "Cannot get the descriptor of a texture before it has been allocated");
	return m_desc;
}
//=============================================================================
void ogl::Texture::GenerateMipmaps() const
{
	assert(IsValid() && "Cannot generate mipmaps for a texture before it has been allocated");
	assert(m_desc.useMipMaps && "Cannot generate mipmaps for a texture that doesn't use them");

	if (IsValidMipMapFilter(m_desc.minFilter))
	{
		glGenerateTextureMipmap(m_id);
	}
	else
	{
		// In the event a user tries to generate mipmaps for a texture that doesn't use a valid mipmap filter
		Error("Cannot generate mipmaps for a texture that doesn't use a valid mipmap filter");
	}
}
//=============================================================================
void ogl::Texture::SetBorderColor(const glm::vec4& color)
{
	assert(IsValid() && "Cannot set border color for a texture before it has been allocated");
	glTextureParameterfv(m_id, GL_TEXTURE_BORDER_COLOR, &color.x);
}
//=============================================================================
void ogl::Texture::Bind(std::optional<uint32_t> slot) const
{
	if (slot.has_value())
	{
		glBindTextureUnit(slot.value(), m_id);
	}
	else
	{
		glBindTexture(m_type, m_id);
	}
}
//=============================================================================
void ogl::Texture::Bind(uint32_t slot, ImageAccessSpecifier access, InternalFormat format, uint32_t level, std::optional<uint32_t> layer) const
{
	const bool layered = !layer.has_value();
	const uint32_t l = layered ? 0 : layer.value();

	glBindImageTexture(slot, m_id, level, layered, l, EnumToValue(access), EnumToValue(format));
}
//=============================================================================
void ogl::Texture::Unbind() const
{
	glBindTexture(m_type, 0);
}
//=============================================================================
ogl::TextureType ogl::Texture::GetType() const
{
	return ValueToEnum(m_type);
}
//=============================================================================
const std::string& ogl::Texture::GetDebugName() const
{
	return m_debugName;
}
//=============================================================================
bool ogl::Texture::IsValid() const
{
	return m_allocated;
}
//=============================================================================
bool ogl::Texture::IsMutable() const
{
	assert(IsValid() && "Cannot check if a texture is mutable before it has been allocated");
	return m_desc.mutableDesc.has_value();
}
//=============================================================================