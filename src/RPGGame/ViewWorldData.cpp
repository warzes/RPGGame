#include "stdafx.h"
#include "ViewWorldData.h"
//=============================================================================
void ViewWorldData::Init()
{
	gameModels.reserve(5000);
}
//=============================================================================
void ViewWorldData::ResetFrame()
{
	camera = nullptr;
	countGameModels = 0;
}
//=============================================================================
void ViewWorldData::Bind(GameModel* go)
{
	if (countGameModels >= gameModels.size())
		gameModels.push_back(go);
	else
		gameModels[countGameModels] = go;

	countGameModels++;
}
//=============================================================================