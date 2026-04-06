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
}
//=============================================================================
void TerraTest::Draw(const glm::mat4& proj, const glm::mat4& view)
{
	m_terra.Draw(proj, view);
}
//=============================================================================