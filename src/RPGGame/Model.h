#pragma once

#include "Mesh.h"

class Model final
{
public:
	bool Load(const std::string& fileName);
	void Create(const MeshInfo& meshCreateInfo);
	void Create(const std::vector<MeshInfo>& meshes);

	void Free();

	const std::vector<Mesh*>& GetMeshes() const noexcept { return m_meshes; }
	Mesh* GetMesh(size_t id) noexcept { return m_meshes[id]; }
	const AABB& GetAABB() const noexcept { return m_aabb; }

	bool Valid() const noexcept { return !m_meshes.empty(); }

private:
	void processNode(const aiScene* scene, aiNode* node, std::string_view directory);
	Mesh* processMesh(const aiScene* scene, struct aiMesh* mesh, std::string_view directory);
	std::vector<Texture2D> loadMaterialTextures(std::string_view directory, const aiScene* scene, aiMaterial* mat, aiTextureType type);
	void computeAABB();

	std::vector<Mesh*> m_meshes;
	AABB               m_aabb;
	std::string        m_name;
};