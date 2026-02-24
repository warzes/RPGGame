#include "stdafx.h"
#include "Window.h"
#include "Log.h"
//=============================================================================
bool       windowQuit{ true };
uint16_t   windowWidth{ 0 };
uint16_t   windowHeight{ 0 };
float      windowAspect{ 1.0f };
//=============================================================================
namespace window
{
	bool Init(uint16_t width, uint16_t height, std::string_view title, bool vsync = false, bool resizable = true, bool maximized = false);
	void Close() noexcept;
	bool WindowShouldClose() noexcept;
	void Swap();
}
//=============================================================================
// Input.cpp
void mouseNotifyFunc(RGFW_window* win, i32 x, i32 y, u8 status) noexcept;
void mousePosFunc(RGFW_window* window, i32 xPos, i32 yPos, float vecX, float vecY) noexcept;
void mouseButtonFunc(RGFW_window* win, u8 button, u8 pressed) noexcept;
void scrollFunc(RGFW_window* win, float xOffset, float yOffset) noexcept;
void keyFunc(RGFW_window* win, u8 key, RGFW_keymod mod, RGFW_bool repeat, RGFW_bool pressed) noexcept;
//=============================================================================
void errorFunc(RGFW_debugType type, RGFW_errorCode err, const char* msg) noexcept
{
	if (type != RGFW_typeError || err == RGFW_noError) return; /* disregard non-errors */
	Error("RGFW ERROR: " + std::string(msg));
}
//=============================================================================
void windowResizeFunc(RGFW_window* win, i32 width, i32 height) noexcept
{
	if (width < 0 || height < 0) return;
	windowWidth = static_cast<uint16_t>(std::max(width, 1));
	windowHeight = static_cast<uint16_t>(std::max(height, 1));
	windowAspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}
//=============================================================================
void windowMinimizeFunc(RGFW_window*) noexcept
{
	Debug("window minimize");
}
//=============================================================================
void windowMaximizeFunc(RGFW_window*, i32 x, i32 y, i32 w, i32 h) noexcept
{
	RGFW_UNUSED(x); RGFW_UNUSED(y);
	Debug("window maximize: " + std::to_string(w) + ":" + std::to_string(h));
}
//=============================================================================
void windowRestoreFunc(RGFW_window*, i32 x, i32 y, i32 w, i32 h) noexcept
{
	RGFW_UNUSED(x); RGFW_UNUSED(y);
	Debug("window restore: " + std::to_string(w) + ":" + std::to_string(h));
}
//=============================================================================
void windowQuitFunc(RGFW_window*) noexcept
{}
//=============================================================================
void focusFunc(RGFW_window* win, u8 inFocus) noexcept
{
	ImGui_ImplRgfw_WindowFocusCallback(win, inFocus);
	if (inFocus) Debug("window in focus");
	else Debug("window out of focus");
}
//=============================================================================
bool window::Init(uint16_t width, uint16_t height, std::string_view title, bool vsync, bool resizable, bool maximized)
{
	if (!RGFW_init())
	{
		Fatal("Error Initialising RGFW");
		return false;
	}
	RGFW_glHints* hints = RGFW_getGlobalHints_OpenGL();
	hints->depth = 32;
	hints->stencil = 8;
#if ENABLE_SRGB
	hints->sRGB = true;
#endif
#if defined(_DEBUG)
	hints->debug = true;
#else
	hints->noError = true;
#endif
	hints->major = 3;
	hints->minor = 3;

	RGFW_setGlobalHints_OpenGL(hints);

	RGFW_windowFlags windowFlags = RGFW_windowCenter | RGFW_windowOpenGL;
	if (!resizable) windowFlags |= RGFW_windowNoResize;
	if (maximized) windowFlags |= RGFW_windowMaximize;

	handle = RGFW_createWindow(title.data(), 0, 0, width, height, windowFlags);
	if (!handle)
	{
		Fatal("Failed to create RGFW window");
		return false;
	}

	// Window callbacks
	RGFW_setDebugCallback(errorFunc);
	RGFW_setWindowResizedCallback(windowResizeFunc);
	RGFW_setWindowMinimizedCallback(windowMinimizeFunc);
	RGFW_setWindowRestoredCallback(windowRestoreFunc);
	RGFW_setWindowMaximizedCallback(windowMaximizeFunc);
	RGFW_setWindowQuitCallback(windowQuitFunc);
	RGFW_setFocusCallback(focusFunc);
	//// Mouse callbacks
	RGFW_setMouseNotifyCallback(mouseNotifyFunc);
	RGFW_setMousePosCallback(mousePosFunc);
	RGFW_setMouseButtonCallback(mouseButtonFunc);
	RGFW_setMouseScrollCallback(scrollFunc);
	//// Key callbacks
	RGFW_setKeyCallback(keyFunc);

	// Get buffer size information
	int displayW, displayH;
	RGFW_window_getSize(handle, &displayW, &displayH);
	windowWidth = static_cast<uint16_t>(displayW);
	windowHeight = static_cast<uint16_t>(displayH);
	windowAspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

	// Set the current context
	RGFW_window_makeCurrentContext_OpenGL(handle);

	RGFW_window_swapInterval_OpenGL(handle, vsync ? 1 : 0);

	windowQuit = false;

	return true;
}
//=============================================================================
void window::Close() noexcept
{
	windowQuit = true;
	if (handle) RGFW_window_close(handle);
	handle = nullptr;
	RGFW_deinit();
}
//=============================================================================
bool window::WindowShouldClose() noexcept
{
	return RGFW_window_shouldClose(handle) == RGFW_TRUE || windowQuit;
}
//=============================================================================
void window::Swap()
{
	RGFW_window_swapBuffers_OpenGL(handle);
}
//=============================================================================
void window::Quit()
{
	windowQuit = true;
}
//=============================================================================
uint16_t window::GetWidth() noexcept { return windowWidth; }
uint16_t window::GetHeight() noexcept { return windowHeight; }
float window::GetAspect() noexcept { return windowAspect; }
//=============================================================================