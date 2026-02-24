#pragma once

namespace window
{
	void     Quit();

	uint16_t GetWidth() noexcept;
	uint16_t GetHeight() noexcept;
	float    GetAspect() noexcept;
	

	inline RGFW_window* handle{ nullptr };

} // namespace window