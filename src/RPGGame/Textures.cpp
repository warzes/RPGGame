#include "stdafx.h"
#include "Textures.h"
//=============================================================================
struct TextureCache final
{
	bool operator==(const TextureCache&) const noexcept = default;

	std::string name;
	bool sRGB;
	bool flipVertical;
};
//=============================================================================
namespace std
{
	template<>
	struct hash<TextureCache>
	{
		std::size_t operator()(const TextureCache& tc) const noexcept
		{
			std::size_t h1 = std::hash<std::string>{}(tc.name);
			std::size_t h2 = std::hash<bool>{}(tc.sRGB);
			std::size_t h3 = std::hash<bool>{}(tc.flipVertical);
			std::size_t seed = 0;
			HashCombine(seed, h1, h2, h3);
			return seed;
		}
	};
}
//=============================================================================
namespace
{
	std::unordered_map<TextureCache, Texture2D> texturesMap;
	Texture2D defaultWhite2D;
	Texture2D defaultDiffuse2D;
	Texture2D defaultNormal2D;
	Texture2D defaultSpecular2D;
}
//=============================================================================
bool TexturesInit()
{
	// Create white texture
	{
		constexpr size_t SizeTexture = 1u;
		uint8_t data[SizeTexture][SizeTexture][3];
		for (size_t i = 0; i < SizeTexture; i++)
		{
			for (size_t j = 0; j < SizeTexture; j++)
			{
				data[i][j][0] = 255;
				data[i][j][1] = 255;
				data[i][j][2] = 255;
			}
		}

		ogl::MutableTextureDesc mutableDesc{};
		mutableDesc.format = ogl::Format::RGB;
		mutableDesc.type = ogl::PixelDataType::UnsignedByte;
		mutableDesc.data = data;

		ogl::TextureDesc desc{};
		desc.width = SizeTexture;
		desc.height = SizeTexture;
		desc.minFilter = ogl::TextureFilteringMode::LinearMipmapLinear;
		desc.magFilter = ogl::TextureFilteringMode::Linear;
		desc.internalFormat = ogl::InternalFormat::RGB;
		desc.useMipMaps = false;
		desc.mutableDesc = mutableDesc;

		defaultWhite2D.pixelFormat = ogl::Format::RGB;
		defaultWhite2D.width = SizeTexture;
		defaultWhite2D.height = SizeTexture;
		defaultWhite2D.id = new ogl::Texture(ogl::TextureType::Texture2D, "DefaultWhite2D");
		defaultWhite2D.id->Allocate(desc);
	}

	// Create default diffuse texture
	{
		constexpr size_t SizeTexture = 32u;
		uint8_t data[SizeTexture][SizeTexture][3];
		for (size_t i = 0; i < SizeTexture; i++)
		{
			for (size_t j = 0; j < SizeTexture; j++)
			{
				if ((i + j) % 2 == 0)
				{
					data[i][j][0] = 250;
					data[i][j][1] = 70;
					data[i][j][2] = 100;
				}
				else
				{
					data[i][j][0] = 150;
					data[i][j][1] = 50;
					data[i][j][2] = 200;
				}
			}
		}

		ogl::MutableTextureDesc mutableDesc{};
		mutableDesc.format = ogl::Format::RGB;
		mutableDesc.type = ogl::PixelDataType::UnsignedByte;
		mutableDesc.data = data;

		ogl::TextureDesc desc{};
		desc.width = SizeTexture;
		desc.height = SizeTexture;
		desc.minFilter = ogl::TextureFilteringMode::Nearest;
		desc.magFilter = ogl::TextureFilteringMode::Nearest;
		desc.internalFormat = ogl::InternalFormat::RGB;
		desc.useMipMaps = false;
		desc.mutableDesc = mutableDesc;

		defaultDiffuse2D.pixelFormat = ogl::Format::RGB;
		defaultDiffuse2D.width = SizeTexture;
		defaultDiffuse2D.height = SizeTexture;
		defaultDiffuse2D.id = new ogl::Texture(ogl::TextureType::Texture2D, "DefaultDiffuse2D");
		defaultDiffuse2D.id->Allocate(desc);
	}

	// Create default normal texture
	{
		constexpr size_t SizeTexture = 8u;
		uint8_t data[SizeTexture][SizeTexture][3];
		for (size_t i = 0; i < SizeTexture; i++)
		{
			for (size_t j = 0; j < SizeTexture; j++)
			{
				data[i][j][0] = 128;
				data[i][j][1] = 128;
				data[i][j][2] = 255;
			}
		}

		ogl::MutableTextureDesc mutableDesc{};
		mutableDesc.format = ogl::Format::RGB;
		mutableDesc.type = ogl::PixelDataType::UnsignedByte;
		mutableDesc.data = data;

		ogl::TextureDesc desc{};
		desc.width = SizeTexture;
		desc.height = SizeTexture;
		desc.minFilter = ogl::TextureFilteringMode::Nearest;
		desc.magFilter = ogl::TextureFilteringMode::Nearest;
		desc.internalFormat = ogl::InternalFormat::RGB;
		desc.useMipMaps = false;
		desc.mutableDesc = mutableDesc;

		defaultNormal2D.pixelFormat = ogl::Format::RGB;
		defaultNormal2D.width = SizeTexture;
		defaultNormal2D.height = SizeTexture;
		defaultNormal2D.id = new ogl::Texture(ogl::TextureType::Texture2D, "DefaultNormal2D");
		defaultNormal2D.id->Allocate(desc);
	}

	// Create default specular texture
	{
		constexpr size_t SizeTexture = 8u;
		uint8_t data[SizeTexture][SizeTexture][3];
		for (size_t i = 0; i < SizeTexture; i++)
		{
			for (size_t j = 0; j < SizeTexture; j++)
			{
				data[i][j][0] = 255;//Roughness
				data[i][j][1] = 255;//Metallic
				data[i][j][2] = 0;
			}
		}

		ogl::MutableTextureDesc mutableDesc{};
		mutableDesc.format = ogl::Format::RGB;
		mutableDesc.type = ogl::PixelDataType::UnsignedByte;
		mutableDesc.data = data;

		ogl::TextureDesc desc{};
		desc.width = SizeTexture;
		desc.height = SizeTexture;
		desc.minFilter = ogl::TextureFilteringMode::Nearest;
		desc.magFilter = ogl::TextureFilteringMode::Nearest;
		desc.internalFormat = ogl::InternalFormat::RGB;
		desc.useMipMaps = false;
		desc.mutableDesc = mutableDesc;

		defaultSpecular2D.pixelFormat = ogl::Format::RGB;
		defaultSpecular2D.width = SizeTexture;
		defaultSpecular2D.height = SizeTexture;
		defaultSpecular2D.id = new ogl::Texture(ogl::TextureType::Texture2D, "DefaultSpecular2D");
		defaultSpecular2D.id->Allocate(desc);
	}

	return true;
}
//=============================================================================
void TexturesClose()
{
	delete defaultWhite2D.id;
	defaultWhite2D.id = nullptr;
	delete defaultDiffuse2D.id;
	defaultDiffuse2D.id = nullptr;
	delete defaultNormal2D.id;
	defaultNormal2D.id = nullptr;
	delete defaultSpecular2D.id;
	defaultSpecular2D.id = nullptr;

	for (auto& it : texturesMap)
	{
		delete it.second.id;
	}
	texturesMap.clear();
}
//=============================================================================
Texture2D textures::GetWhiteTexture2D()
{
	return defaultWhite2D;
}
//=============================================================================
Texture2D textures::GetDefaultDiffuse2D()
{
	return defaultDiffuse2D;
}
//=============================================================================
Texture2D textures::GetDefaultNormal2D()
{
	return defaultNormal2D;
}
//=============================================================================
Texture2D textures::GetDefaultSpecular2D()
{
	return defaultSpecular2D;
}
//=============================================================================
Texture2D textures::LoadTexture2D(const std::string& fileName, bool flipVertical)
{
	TextureCache keyMap = { .name = fileName, .sRGB = false, .flipVertical = flipVertical };
	auto it = texturesMap.find(keyMap);
	if (it != texturesMap.end() && it->second.id != nullptr && it->second.id->IsValid())
	{
		return it->second;
	}
	else
	{
		bool hasTex = io::Exists(fileName);
		if (hasTex == false)
		{
			Error("Failed to load texture " + fileName);
			return GetDefaultDiffuse2D();
		}

		stbi_set_flip_vertically_on_load(flipVertical);

		int width, height, nrComponents;
		stbi_uc* pixels = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
		if (!pixels || nrComponents < 1 || nrComponents > 4 || width <= 0 || height <= 0)
		{
			stbi_image_free(pixels);
			Error("Failed to load texture " + fileName);
			return GetDefaultDiffuse2D();
		}

		ogl::InternalFormat internalFormat{};
		ogl::Format pixelFormat{ ogl::Format::RGB };
		if (nrComponents == 1)
		{
			internalFormat = ogl::InternalFormat::R8;
			pixelFormat = ogl::Format::RED;
		}
		else if (nrComponents == 2)
		{
			internalFormat = ogl::InternalFormat::RG8;
			pixelFormat = ogl::Format::RG;
		}
		else if (nrComponents == 3)
		{
			internalFormat = ogl::InternalFormat::RGB8;
			pixelFormat = ogl::Format::RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = ogl::InternalFormat::RGBA8;
			pixelFormat = ogl::Format::RGBA;
		}
		else
		{
			std::unreachable();
		}

		ogl::MutableTextureDesc mutableDesc{};
		mutableDesc.format = pixelFormat;
		mutableDesc.type = ogl::PixelDataType::UnsignedByte;
		mutableDesc.data = pixels;

		ogl::TextureDesc desc{};
		desc.width = width;
		desc.height = height;
		desc.minFilter = ogl::TextureFilteringMode::LinearMipmapLinear;
		desc.magFilter = ogl::TextureFilteringMode::Linear;
		desc.internalFormat = internalFormat;
		desc.useMipMaps = true;
		desc.mutableDesc = mutableDesc;

		ogl::Texture* texture = new ogl::Texture(ogl::TextureType::Texture2D, fileName);
		texture->Allocate(desc);
		texture->GenerateMipmaps();

		stbi_image_free(pixels);

		Debug("Load Texture: " + fileName);

		texturesMap[keyMap] = Texture2D{
			.id = texture,
			.pixelFormat = pixelFormat,
			.width = static_cast<uint32_t>(width),
			.height = static_cast<uint32_t>(height)
		};
		return texturesMap[keyMap];
	}
}
//=============================================================================
Texture2D textures::CreateTextureFromData(std::string_view name, aiTexture* embTex, bool flipVertical)
{
	TextureCache keyMap = { .name = name.data(), .sRGB = false, .flipVertical = flipVertical };
	auto it = texturesMap.find(keyMap);
	if (it != texturesMap.end())
	{
		return it->second;
	}
	else
	{
		stbi_set_flip_vertically_on_load(flipVertical);

		int width, height, nrComponents;
		stbi_uc* data{ nullptr };
		if (embTex->mHeight == 0)
			data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(embTex->pcData), static_cast<int>(embTex->mWidth), &width, &height, &nrComponents, 0);
		else
			data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(embTex->pcData), static_cast<int>(embTex->mWidth * embTex->mHeight), &width, &height, &nrComponents, 0);
		if (!data || nrComponents < 1 || nrComponents > 4 || width <= 0 || height <= 0)
		{
			stbi_image_free(data);
			Error("Error while trying to load embedded texture!");
			return GetDefaultDiffuse2D();
		}

		ogl::InternalFormat internalFormat{};
		ogl::Format pixelFormat{ ogl::Format::RGB };
		if (nrComponents == 1)
		{
			internalFormat = ogl::InternalFormat::R8;
			pixelFormat = ogl::Format::RED;
		}
		else if (nrComponents == 2)
		{
			internalFormat = ogl::InternalFormat::RG8;
			pixelFormat = ogl::Format::RG;
		}
		else if (nrComponents == 3)
		{
			internalFormat = ogl::InternalFormat::RGB8;
			pixelFormat = ogl::Format::RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = ogl::InternalFormat::RGBA8;
			pixelFormat = ogl::Format::RGBA;
		}
		else
		{
			std::unreachable();
		}

		ogl::MutableTextureDesc mutableDesc{};
		mutableDesc.format = pixelFormat;
		mutableDesc.type = ogl::PixelDataType::UnsignedByte;
		mutableDesc.data = data;

		ogl::TextureDesc desc{};
		desc.width = width;
		desc.height = height;
		desc.minFilter = ogl::TextureFilteringMode::LinearMipmapLinear;
		desc.magFilter = ogl::TextureFilteringMode::Linear;
		desc.internalFormat = internalFormat;
		desc.useMipMaps = true;
		desc.mutableDesc = mutableDesc;

		ogl::Texture* texture = new ogl::Texture(ogl::TextureType::Texture2D, std::string(name));
		texture->Allocate(desc);
		texture->GenerateMipmaps();

		stbi_image_free(data);

		Debug("Load Texture: " + std::string(name));
		texturesMap[keyMap] = Texture2D{ .id = texture, .width = (uint32_t)width, .height = (uint32_t)height };
		return texturesMap[keyMap];
	}
}
//=============================================================================