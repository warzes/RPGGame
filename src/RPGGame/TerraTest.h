#pragma once

#include "Terrain.h"

class TerraTest final
{
public:
	bool Init();
	void Close();

	void Draw(const glm::mat4& proj, const glm::mat4& view);

private:
	Terrain m_terra;
};