#include "stdafx.h"
#include "TerraTest.h"
//=============================================================================
bool TerraTest::Init()
{
	m_terra.Init("data/terrain.pgm");

	return true;
}
//=============================================================================
void TerraTest::Close()
{
	m_terra.Close();
}
//=============================================================================
void TerraTest::Update(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& camPos)
{
	m_terra.Update(proj, view, camPos);
}
//=============================================================================
void TerraTest::Draw(const glm::mat4& proj, const glm::mat4& view)
{
	m_terra.Draw(proj, view);
}
//=============================================================================