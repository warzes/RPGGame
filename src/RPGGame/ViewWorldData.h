#pragma once

class Camera;
struct GameModel;

struct ViewWorldData final
{
	void Init();
	void ResetFrame();
	void Bind(GameModel* go);

	Camera*                 camera{ nullptr };
	std::vector<GameModel*> gameModels;
	size_t                  countGameModels{ 0 };
};