#pragma once

#include "OGLTexture.h"

struct Texture2D final
{
	bool operator==(const Texture2D& rhs) const
	{
		return id == rhs.id
			&& id != nullptr
			&& id->GetID() == rhs.id->GetID()
			&& pixelFormat == rhs.pixelFormat
			&& width == rhs.width
			&& height == rhs.height;
	}
	bool operator!=(const Texture2D& rhs) const { return !(*this == rhs); }

	ogl::Texture* id{ 0 };
	ogl::Format   pixelFormat{ ogl::Format::RGBA };
	uint32_t      width{ 0 };
	uint32_t      height{ 0 };
};

namespace textures
{
	Texture2D GetWhiteTexture2D();
	Texture2D GetDefaultDiffuse2D();
	Texture2D GetDefaultNormal2D();
	Texture2D GetDefaultSpecular2D();
	Texture2D LoadTexture2D(const std::string& fileName, bool flipVertical = false);
	Texture2D CreateTextureFromData(std::string_view name, aiTexture* embTex, bool flipVertical = false);
} // namespace textures