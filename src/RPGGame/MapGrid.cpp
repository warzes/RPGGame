#include "stdafx.h"
#include "MapGrid.h"
//=============================================================================
std::vector<float> generateGrid(float size, float step)
{
	std::vector<float> vertices;

	for (float i = -size; i <= size; i += step) {
		// Горизонтальные линии
		vertices.insert(vertices.end(), {
			-size + 0.5f, 0.0f, i + 0.5f,
			 size + 0.5f, 0.0f, i + 0.5f,
			});

		// Вертикальные линии
		vertices.insert(vertices.end(), {
			i + 0.5f, 0.0f, -size + 0.5f,
			i + 0.5f, 0.0f,  size + 0.5f,
			});
	}

	return vertices;
}
//=============================================================================
bool MapGrid::Init()
{
	m_vb = new ogl::Buffer;
	m_vb->Allocate(sizeof(vertices), ogl::AccessSpecifier::StaticDraw);
	vb.Upload(vertices);
	ogl::Buffer ib;
	ib.Allocate(sizeof(indices), ogl::AccessSpecifier::StaticDraw);
	ib.Upload(indices);

	ogl::VertexArray va;
	va.SetLayout({
			{ ogl::DataType::Float, 3 },
			{ ogl::DataType::Float, 2 }
		}, vb, ib);



	return false;
}
//=============================================================================
void MapGrid::Close()
{
}
//=============================================================================
void MapGrid::Draw(const glm::mat4& proj, const glm::mat4& view)
{
}
//=============================================================================