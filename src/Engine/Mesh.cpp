#include "stdafx.h"
#include "Mesh.h"
//=============================================================================
Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices, std::optional<Material> material)
{
	assert(!vertices.empty());

	m_vertexCount = static_cast<uint32_t>(vertices.size());
	m_indicesCount = static_cast<uint32_t>(indices.size());
	m_material = material;

	// Buffers
	m_vbo.Allocate(vertices.size() * sizeof(MeshVertex), ogl::AccessSpecifier::StaticDraw);
	m_vbo.Upload(vertices.data());
	if (!indices.empty())
	{
		m_ebo.Allocate(indices.size() * sizeof(uint32_t), ogl::AccessSpecifier::StaticDraw);
		m_ebo.Upload(indices.data());
	}

	// VAO
	m_vao.SetLayoutOLD({
			{ ogl::DataType::Float, 3 }, // Position
			{ ogl::DataType::Float, 3 }, // Color
			{ ogl::DataType::Float, 3 }, // Normal
			{ ogl::DataType::Float, 2 }, // UV
			{ ogl::DataType::Float, 3 }, // Tangent
			{ ogl::DataType::Float, 3 }  // Bitangent
		}, m_vbo, m_ebo);

	initAABB(vertices, indices);
}
//=============================================================================
Mesh::~Mesh()
{
}
//=============================================================================
void Mesh::initAABB(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indexData)
{
	if (indexData.size() > 0)
	{
		for (size_t index_id = 0; index_id < indexData.size(); index_id++)
		{
			m_aabb.CombinePoint(vertices[indexData[index_id]].position);
		}
	}
	else
	{
		for (size_t vertex_id = 0; vertex_id < vertices.size(); vertex_id++)
		{
			m_aabb.CombinePoint(vertices[vertex_id].position);
		}
	}
}
//=============================================================================