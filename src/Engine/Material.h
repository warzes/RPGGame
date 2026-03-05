#pragma once

#include "Textures.h"

struct Material final
{
	float     opacity{ 1.0f };
	glm::vec3 diffuseColor{ 1.0f };
	glm::vec3 specularColor{ 1.0f };
	glm::vec3 ambientColor{ 1.0f };

	std::vector<Texture2D> diffuseTextures;
	std::vector<Texture2D> specularTextures;
	std::vector<Texture2D> normalTextures;
};