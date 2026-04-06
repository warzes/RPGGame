#pragma once

#include "GameModel.h"
#include "GameWorldData.h"
#include "MapGrid.h"
#include "TerraTest.h"

class GameScene final
{
public:
	bool Init();
	void Close();
	void Draw();

	void Bind(Camera* camera);
	void Bind(GameModel* go);

private:
	void beginDraw();
	void draw();
	void endDraw();

	void blittingToScreen(GLuint fbo, uint16_t srcWidth, uint16_t srcHeight);

	GameWorldData m_data;
	MapGrid       m_grid;
	TerraTest     m_terra;

	// temp
	ogl::ShaderStage* vs;
	ogl::ShaderStage* fs;
	ogl::ShaderProgram* program;
};