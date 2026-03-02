#pragma once

#include "OGLCore.h"

namespace ogl
{
	struct MutableTextureDesc final
	{
		Format format = Format::RGBA;
		PixelDataType type = PixelDataType::UnsignedByte;
		const void* data = nullptr;
	};

	struct TextureDesc final
	{
		uint32_t width = 0;
		uint32_t height = 0;
		TextureFilteringMode minFilter = TextureFilteringMode::LinearMipmapLinear;
		TextureFilteringMode magFilter = TextureFilteringMode::Linear;
		TextureWrapMode horizontalWrap = TextureWrapMode::Repeat;
		TextureWrapMode verticalWrap = TextureWrapMode::Repeat;
		InternalFormat internalFormat = InternalFormat::RGBA;
		bool useMipMaps = true;
		std::optional<MutableTextureDesc> mutableDesc = std::nullopt;
	};

	class Texture final
	{
	public:
		Texture(TextureType type, std::string_view debugName = std::string_view{});
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture(Texture&&) = delete;
		Texture& operator=(Texture&&) = delete;
		~Texture();

		void Allocate(const TextureDesc& desc);

		void Upload(const void* data, Format format, PixelDataType type);

		void Resize(uint32_t width, uint32_t height);

		const TextureDesc& GetDesc() const;

		void GenerateMipmaps() const;

		void SetBorderColor(const glm::vec4& color);

		void Bind(std::optional<uint32_t> slot = std::nullopt) const;
		void Bind(uint32_t slot, ImageAccessSpecifier access, InternalFormat format, uint32_t level = 0, std::optional<uint32_t> layer = std::nullopt) const;
		void Unbind() const;

		TextureType GetType() const;

		const std::string& GetDebugName() const;

		bool IsValid() const;
		bool IsMutable() const;
		GLuint GetID() const { return m_id; }
	private:
		TextureDesc    m_desc;
		bool           m_allocated{ false };
		std::string    m_debugName;
		GLuint         m_id{ 0 };
		const uint32_t m_type;
	};
} // namespace ogl