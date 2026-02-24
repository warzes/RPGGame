#include "stdafx.h"
#include "Input.h"
#include "Window.h"
#include "Log.h"
//=============================================================================
/*
TODO:
	if (pressed == RGFW_TRUE) {
		ImGui_ImplRgfw_CharCallback(window, keyChar);
	}

*/
//=============================================================================
namespace input
{
	void Init();
	void Update();
}
//=============================================================================
RGFW_event windowEvent{};
glm::vec2  cursorPos{};
glm::vec2  cursorOffset{};
glm::vec2  scrollOffset{};
//=============================================================================
void mouseNotifyFunc(RGFW_window* win, i32 x, i32 y, u8 status) noexcept
{
	ImGui_ImplRgfw_CursorEnterCallback(win, x, y, status);

	if (status) Debug("mouse enter " + std::to_string(x) + ":" + std::to_string(y));
	else Debug("mouse leave");
}
//=============================================================================
void mousePosFunc(RGFW_window* window, i32 xPos, i32 yPos, float vecX, float vecY) noexcept
{
	ImGui_ImplRgfw_CursorPosCallback(window, xPos, yPos, vecX, vecY);
	cursorPos.x = static_cast<float>(xPos);
	cursorPos.y = static_cast<float>(yPos);
	cursorOffset.x = vecX;
	cursorOffset.y = -vecY; // Reversed Y since y-coordinates go from bottom to left
}
//=============================================================================
void mouseButtonFunc(RGFW_window* win, u8 button, u8 pressed) noexcept
{
	ImGui_ImplRgfw_MouseButtonCallback(win, button, pressed);
}
//=============================================================================
void scrollFunc(RGFW_window* win, float xOffset, float yOffset) noexcept
{
	ImGui_ImplRgfw_MouseScrollCallback(win, xOffset, yOffset);
	scrollOffset.x = xOffset;
	scrollOffset.y = yOffset;
}
//=============================================================================
void keyFunc(RGFW_window* win, u8 key, RGFW_keymod mod, RGFW_bool repeat, RGFW_bool pressed) noexcept
{
	ImGui_ImplRgfw_KeyCallback(win, key, mod, repeat, pressed);
}
//=============================================================================
void input::Init()
{
	cursorPos = cursorOffset = scrollOffset = glm::vec2(0.0f);

	i32 xpos, ypos;
	RGFW_window_getMouse(window::handle, &xpos, &ypos);
	cursorOffset.x = static_cast<float>(xpos);
	cursorOffset.y = static_cast<float>(ypos);
}
//=============================================================================
void input::Update()
{
	scrollOffset = glm::vec2(0);
	cursorOffset = glm::vec2(0);

	while (RGFW_window_checkEvent(window::handle, &windowEvent))
	{
		if (windowEvent.type == RGFW_quit)
		{
			window::Quit();
		}
		// TODO: ???
	}
}
//=============================================================================
void input::SetCursorVisible(bool state)
{
	bool isHoldingMouse = RGFW_window_isRawMouseMode(window::handle) && RGFW_window_isCaptured(window::handle);
	if (state)
	{
		if (isHoldingMouse)
		{
			RGFW_window_showMouse(window::handle, 1);
			RGFW_window_captureRawMouse(window::handle, RGFW_FALSE);
		}
	}
	else
	{
		if (!isHoldingMouse)
		{
			RGFW_window_showMouse(window::handle, 0);
			RGFW_window_captureRawMouse(window::handle, RGFW_TRUE);
		}
	}
}
//=============================================================================
const glm::vec2& input::GetCursorPos() noexcept { return cursorPos; }
const glm::vec2& input::GetCursorOffset() noexcept { return cursorOffset; }
const glm::vec2& input::GetScrollOffset() noexcept { return scrollOffset; }
//=============================================================================
bool input::IsKeyDown(RGFW_key key) noexcept { return RGFW_isKeyDown(key); }
bool input::IsKeyUp(RGFW_key key) noexcept { return !RGFW_isKeyDown(key); }
bool input::IsKeyPressed(RGFW_key key) noexcept { return RGFW_isKeyPressed(key); }
bool input::IsKeyReleased(RGFW_key key) noexcept { return RGFW_isKeyReleased(key); }
bool input::IsMouseDown(RGFW_mouseButton key) noexcept { return RGFW_isMouseDown(key); }
bool input::IsMouseUp(RGFW_mouseButton key) noexcept { return !RGFW_isMouseDown(key); }
bool input::IsMousePressed(RGFW_mouseButton key) noexcept { return RGFW_isMousePressed(key); }
bool input::IsMouseReleased(RGFW_mouseButton key) noexcept { return RGFW_isMouseReleased(key); }
//=============================================================================