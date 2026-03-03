#pragma once

#include "Model.h"

class MapGrid final
{
public:
	bool Init();
	void Close();

	void Draw(const glm::mat4& proj, const glm::mat4& view);

private:
	ogl::Buffer*        m_vb;
	ogl::VertexArray*   m_va;
	ogl::ShaderStage*   m_vs;
	ogl::ShaderStage*   m_fs;
	ogl::ShaderProgram* m_program;
	size_t              m_vertSize;
};