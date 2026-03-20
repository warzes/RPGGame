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
	//m_vao.SetLayoutOLD({
	//		{ ogl::DataType::Float, 3 }, // Position
	//		{ ogl::DataType::Float, 3 }, // Color
	//		{ ogl::DataType::Float, 3 }, // Normal
	//		{ ogl::DataType::Float, 2 }, // UV
	//		{ ogl::DataType::Float, 3 }, // Tangent
	//		{ ogl::DataType::Float, 3 }  // Bitangent
	//	}, m_vbo, m_ebo);

	m_vao.SetVertexBuffer(0, m_vbo.GetID(), 0, sizeof(MeshVertex));
	m_vao.SetElementBuffer(m_ebo.GetID());
	m_vao.EnableAttrib(0); // Position
	m_vao.SetAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(MeshVertex, position));
	m_vao.EnableAttrib(1); // Color
	m_vao.SetAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(MeshVertex, color));
	m_vao.EnableAttrib(2); // Normal
	m_vao.SetAttribFormat(2, 3, GL_FLOAT, GL_FALSE, offsetof(MeshVertex, normal));
	m_vao.EnableAttrib(3); // UV
	m_vao.SetAttribFormat(3, 2, GL_FLOAT, GL_FALSE, offsetof(MeshVertex, texCoord));
	m_vao.EnableAttrib(4); // Tangent
	m_vao.SetAttribFormat(4, 3, GL_FLOAT, GL_FALSE, offsetof(MeshVertex, tangent));
	m_vao.EnableAttrib(5); // Bitangent
	m_vao.SetAttribFormat(5, 3, GL_FLOAT, GL_FALSE, offsetof(MeshVertex, bitangent));

	m_vao.SetAttribBinding(0, 0);
	m_vao.SetAttribBinding(1, 0);
	m_vao.SetAttribBinding(2, 0);
	m_vao.SetAttribBinding(3, 0);
	m_vao.SetAttribBinding(4, 0);
	m_vao.SetAttribBinding(5, 0);

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