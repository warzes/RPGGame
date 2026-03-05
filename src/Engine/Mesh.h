#pragma once

#include "Material.h"
#include "AABB.h"
#include "OGLBuffer.h"
#include "OGLVertexArray.h"

struct MeshVertex final
{
	glm::vec3 position{ 0.0f };
	glm::vec3 color{ 1.0f };
	glm::vec3 normal{ 0.0f };
	glm::vec2 texCoord{ 0.0f };
	glm::vec3 tangent{ 0.0f };
	glm::vec3 bitangent{ 0.0f };
};

struct MeshInfo final
{
	std::vector<MeshVertex> vertices;
	std::vector<uint32_t>   indices;
	std::optional<Material> material{};
};

class Mesh final
{
public:
	Mesh(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices, std::optional<Material> material);
	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) = delete;
	~Mesh();

	Mesh& operator=(const Mesh&) = delete;
	Mesh& operator=(Mesh&&) = delete;

	auto GetVertexCount() const noexcept { return m_vertexCount; }
	auto GetIndexCount() const noexcept { return m_indicesCount; }
	auto GetMaterial() const noexcept { return m_material; }
	const AABB& GetAABB() const noexcept { return m_aabb; }
	ogl::VertexArray* GetVAO() noexcept { return &m_vao; }

private:
	void initAABB(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices);

	uint32_t                   m_vertexCount{ 0 };
	uint32_t                   m_indicesCount{ 0 };
	ogl::VertexArray           m_vao{};
	ogl::Buffer                m_vbo{};
	ogl::Buffer                m_ebo{};
	std::optional<Material>    m_material{};
	AABB                       m_aabb{};
};