#pragma once

namespace input
{
	const glm::vec2& GetCursorPos() noexcept;
	const glm::vec2& GetCursorOffset() noexcept;
	const glm::vec2& GetScrollOffset() noexcept;

	bool IsKeyDown(RGFW_key key) noexcept;
	bool IsKeyUp(RGFW_key key) noexcept;
	bool IsKeyPressed(RGFW_key key) noexcept;
	bool IsKeyReleased(RGFW_key key) noexcept;
	bool IsMouseDown(RGFW_mouseButton key) noexcept;
	bool IsMouseUp(RGFW_mouseButton key) noexcept;
	bool IsMousePressed(RGFW_mouseButton key) noexcept;
	bool IsMouseReleased(RGFW_mouseButton key) noexcept;

	void SetCursorVisible(bool state);
} // namespace input