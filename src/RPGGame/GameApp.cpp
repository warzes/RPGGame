#include "stdafx.h"
#include "Camera.h"
#include "GameScene.h"
#include "Map.h"
//=============================================================================
bool TexturesInit();
void TexturesClose();
//=============================================================================
/*
мир строится из блоков. пример редактора - halftimber
также примеры - это стратегия pharaoh и timberborn
https://v3x3d.itch.io/mini-medieval
*/

/*
TODO:
- вывод трехмерных примитивов (куб, сфера, цилиндр)
- генерация тайлов
- режим игры/редактора
*/
//=============================================================================
bool GameAppInit()
{
	if (!engine::Init(1600, 900, "Game"))
		return false;

	if (!TexturesInit())
		return false;

	return true;
}
//=============================================================================
void GameAppClose()
{
	TexturesClose();
	engine::Close();
}
//=============================================================================
void LoadMap(Map& map)
{
	TileInfo  tempTile;
	tempTile.type = TileGeometryType::Block00;
	tempTile.textureWall = textures::LoadTexture2D("data/tiles/grass01_wall.png", true);
	tempTile.textureCeil = textures::LoadTexture2D("data/tiles/grass01_ceil.png");
	tempTile.textureFloor = textures::LoadTexture2D("data/tiles/grass01.png");
	auto tempTileId = TileBank::AddTileInfo(tempTile);

	for (size_t x = 0; x < 30; x++)
	{
		for (size_t y = 0; y < 30; y++)
		{
			map.SetTile(tempTileId, x, 14 + y, 0);
		}
	}

	map.RecreateGeometry();
}
//=============================================================================
void GameApp()
{
	if (GameAppInit())
	{
		ogl::SetCapability(ogl::RenderingCapability::DepthTest, true);

		// Camera
		Camera camera({ 0.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

		GameScene scene;
		scene.Init();

		GameModel modelLevel;
		modelLevel.model.Load("data/models/spaceCompound/spaceCompound.obj");

		Map map;
		LoadMap(map);

		while (!engine::ShouldClose())
		{
			engine::BeginFrame();

			// Input
			const float speed = 10.0f * engine::GetDeltaTime();
			if (input::IsKeyDown(RGFW_w)) camera.Move(CameraForward, speed);
			if (input::IsKeyDown(RGFW_s)) camera.Move(CameraBackward, speed);
			if (input::IsKeyDown(RGFW_a)) camera.Move(CameraLeft, speed);
			if (input::IsKeyDown(RGFW_d)) camera.Move(CameraRight, speed);
			if (input::IsKeyDown(RGFW_q)) camera.Move(CameraDown, speed);
			if (input::IsKeyDown(RGFW_e)) camera.Move(CameraUp, speed);
			if (input::IsKeyDown(RGFW_escape)) break;

			if (input::IsMouseDown(RGFW_mouseRight))
			{
				input::SetCursorVisible(false);
				camera.Rotate(input::GetCursorOffset().y * CAMERA_SENSITIVITY, input::GetCursorOffset().x * CAMERA_SENSITIVITY, 0.0f);
			}
			else if (input::IsMouseReleased(RGFW_mouseRight))
			{
				input::SetCursorVisible(true);
			}

			scene.Bind(&camera);
			//scene.Bind(&modelLevel);
			//scene.Bind(map.GetModel());
			scene.Draw();

			// ui

			{
				if (const ImGuiViewport* v = ImGui::GetMainViewport())
				{
					ImGui::SetNextWindowPos({ v->WorkPos.x + 15.0f, v->WorkPos.y + 15.0f }, ImGuiCond_Always, { 0.0f, 0.0f });
				}
				ImGui::SetNextWindowBgAlpha(0.30f);
				ImGui::SetNextWindowSize(ImVec2(ImGui::CalcTextSize("VertexCount : __________").x, 0));
				if (ImGui::Begin("##Map", nullptr,
					ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
					ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove))
				{
					ImGui::Text("Map Info :");
					ImGui::Text("VertexCount : %i", (int)map.GetVertexCount());
					ImGui::Text("IndexCount  : %i", (int)map.GetIndexCount());
				}
				ImGui::End();
			}

			engine::DrawFPS();
			engine::EndFrame();
		}
	}
	GameAppClose();
}
//=============================================================================