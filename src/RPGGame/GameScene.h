#pragma once

#include "GameModel.h"
#include "ViewWorldData.h"
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

	ViewWorldData m_data;
	MapGrid       m_grid;
	TerraTest     m_terra;

	ogl::ShaderStage*   m_vs;
	ogl::ShaderStage*   m_fs;
	ogl::ShaderProgram* m_program;
};