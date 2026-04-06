#pragma once

struct GameModel final
{
	Model     model;
	glm::mat4 modelMatrix{ glm::mat4(1.0f) };
	bool      visible{ true };
};