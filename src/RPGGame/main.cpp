#include "stdafx.h"
//=============================================================================
#if defined(_MSC_VER)
#	pragma comment( lib, "3rdparty.lib" )
#	pragma comment( lib, "Engine.lib" )
#endif
//=============================================================================
/*
мир строится из блоков. пример редактора - halftimber
также примеры - это стратегия pharaoh и timberborn
https://v3x3d.itch.io/mini-medieval
*/
int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
	if (engine::Init(1600, 900, "Game"))
	{
		while (!engine::ShouldClose())
		{
			engine::BeginFrame();

			glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			engine::DrawFPS();
			engine::EndFrame();
		}
	}
	engine::Close();
}
//=============================================================================