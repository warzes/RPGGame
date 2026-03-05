#pragma once

constexpr float CAMERA_SENSITIVITY = 0.1f;

enum class GameState : uint8_t
{
	Start,
	Editor,
	MainMenu,
	GameLogic
};

inline GameState gGameState = GameState::Editor;