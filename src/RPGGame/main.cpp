#include "stdafx.h"
//=============================================================================
#if defined(_MSC_VER)
#	pragma comment( lib, "3rdparty.lib" )
#	pragma comment( lib, "Engine.lib" )
#endif
//=============================================================================
void ExampleTriangle();
void ExampleCube();
void ExampleFramebuffer();
void ExampleCompute();

https://www.youtube.com/shorts/rwHFLTLYvbk?feature=share
эффекты тумана сделать аналог

void GameApp();
//=============================================================================
int main(
	[[maybe_unused]] int   argc,
	[[maybe_unused]] char* argv[])
{
#if 0
	//ExampleTriangle();
	ExampleCube();
	//ExampleFramebuffer();
	//ExampleCompute();
#else
	GameApp();
#endif
}
//=============================================================================