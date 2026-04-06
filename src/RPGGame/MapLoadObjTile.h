#pragma once

struct TempBlockInfo final
{
	std::string modelPath;
	glm::vec3 color{ 1.0f };

	glm::vec3 center{ 0.0f };
	glm::vec3 rotate{ 0.0f }; // Порядок вращения: Z (roll), Y (yaw), X (pitch) в радианах

	Texture2D textureFloor;
	Texture2D textureCeil;
	Texture2D textureWall;

	bool forwardVisible{ true };
	bool backVisible{ true };
	bool rightVisible{ true };
	bool leftVisible{ true };
	bool topVisible{ true };
	bool bottomVisible{ true };
};

void AddObjModel(const TempBlockInfo& modelInfo, std::vector<MeshInfo>& meshInfo);