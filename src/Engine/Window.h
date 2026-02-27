#pragma once

namespace window
{
	void     Quit();

	uint16_t GetWidth() noexcept;
	uint16_t GetHeight() noexcept;
	float    GetAspect() noexcept;
} // namespace window