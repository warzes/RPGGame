#include "stdafx.h"
#include "Model.h"
#include "Log.h"
#include "File.h"
//=============================================================================
bool Model::Load(const std::string& fileName)
{
#define ASSIMP_LOAD_FLAGS (aiProcess_JoinIdenticalVertices |    \
                           aiProcess_Triangulate |              \
                           aiProcess_GenSmoothNormals |         \
                           aiProcess_LimitBoneWeights |         \
                           aiProcess_SplitLargeMeshes |         \
                           aiProcess_ImproveCacheLocality |     \
                           aiProcess_RemoveRedundantMaterials | \
                           aiProcess_FindDegenerates |          \
                           aiProcess_FindInvalidData |          \
                           aiProcess_GenUVCoords |              \
                           aiProcess_FlipUVs |                  \
                           aiProcess_CalcTangentSpace |         \
                           aiProcess_SortByPType |              \
                           aiProcess_OptimizeMeshes)

	Free();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName.c_str(), ASSIMP_LOAD_FLAGS);
	if (!scene || !scene->HasMeshes() || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Error("Not load mesh: " + fileName + "\n\tError: " + importer.GetErrorString());
		return false;
	}

	m_name = fileName;

	std::string directory = io::GetFileDirectory(fileName);
	processNode(scene, scene->mRootNode, directory);

	computeAABB();

	// TODO: центрировать модель, так как бывают не от центра

	return true;
}
//=============================================================================
void Model::Create(const MeshInfo& ci)
{
	Free();
	Mesh* mesh = new Mesh(ci.vertices, ci.indices, ci.material);
	m_meshes.emplace_back(mesh);
	computeAABB();

	// TODO: центрировать модель, так как бывают не от центра
}
//=============================================================================
void Model::Create(const std::vector<MeshInfo>& meshes)
{
	Free();
	for (size_t i = 0; i < meshes.size(); i++)
	{
		if (meshes[i].vertices.empty()) continue;
		m_meshes.emplace_back(new Mesh(meshes[i].vertices, meshes[i].indices, meshes[i].material));
	}
	computeAABB();

	// TODO: центрировать модель, так как бывают не от центра
}
//=============================================================================
void Model::Free()
{
	for (Mesh* mesh : m_meshes)
	{
		delete mesh;
	}
	m_meshes.clear();
}
//=============================================================================
void Model::processNode(const aiScene* scene, aiNode* node, std::string_view directory)
{
	for (unsigned i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.emplace_back(processMesh(scene, aimesh, directory));
	}

	for (unsigned i = 0; i < node->mNumChildren; i++)
	{
		processNode(scene, node->mChildren[i], directory);
	}
}
//=============================================================================
Mesh* Model::processMesh(const aiScene* scene, struct aiMesh* mesh, std::string_view directory)
{
	// Process vertices
	std::vector<MeshVertex> vertices(mesh->mNumVertices);
	for (unsigned i = 0; i < mesh->mNumVertices; i++)
	{
		MeshVertex& v = vertices[i];

		v.position.x = mesh->mVertices[i].x;
		v.position.y = mesh->mVertices[i].y;
		v.position.z = mesh->mVertices[i].z;

		if (mesh->HasVertexColors(0))
		{
			v.color.x = mesh->mColors[0][i].r;
			v.color.y = mesh->mColors[0][i].g;
			v.color.z = mesh->mColors[0][i].b;
		}

		if (mesh->HasNormals())
		{
			v.normal.x = mesh->mNormals[i].x;
			v.normal.y = mesh->mNormals[i].y;
			v.normal.z = mesh->mNormals[i].z;
		}

		if (mesh->HasTextureCoords(0))
		{
			v.texCoord.x = mesh->mTextureCoords[0][i].x;
			v.texCoord.y = mesh->mTextureCoords[0][i].y;
		}

		if (mesh->HasTangentsAndBitangents())
		{
			v.tangent.x = mesh->mTangents[i].x;
			v.tangent.y = mesh->mTangents[i].y;
			v.tangent.z = mesh->mTangents[i].z;

			v.bitangent.x = mesh->mBitangents[i].x;
			v.bitangent.y = mesh->mBitangents[i].y;
			v.bitangent.z = mesh->mBitangents[i].z;
		}
	}

	// Process indices
	std::vector<uint32_t> indices;
	indices.reserve(mesh->mNumFaces * 3);
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];

		// Assume the model has only triangles.
		indices.emplace_back(face.mIndices[0]);
		indices.emplace_back(face.mIndices[1]);
		indices.emplace_back(face.mIndices[2]);
	}

	// Process material
	std::optional<Material> material{};

	//if (m_materialType == ModelMaterialType::BlinnPhong)
	{
		// TODO: по одной текстуре грузится, а тут есть возможность нескольих текстур
		aiMaterial* mesh_material = scene->mMaterials[mesh->mMaterialIndex];

		aiColor3D colorDiffuse;
		mesh_material->Get(AI_MATKEY_COLOR_DIFFUSE, colorDiffuse);
		aiColor3D colorSpecular;
		mesh_material->Get(AI_MATKEY_COLOR_SPECULAR, colorSpecular);
		aiColor3D colorAmbient;
		mesh_material->Get(AI_MATKEY_COLOR_AMBIENT, colorAmbient);
		float opacity{ 0.0f };
		mesh_material->Get(AI_MATKEY_OPACITY, opacity);
		float shininess{ 0.0f };
		mesh_material->Get(AI_MATKEY_SHININESS, shininess);
		float roughness{ 0.0f };
		//mesh_material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, roughness);
		float metallic{ 0.0f };
		//mesh_material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, metallic);

		material = Material();
		material->diffuseColor = glm::vec3(colorDiffuse.r, colorDiffuse.g, colorDiffuse.b);
		material->specularColor = glm::vec3(colorSpecular.r, colorSpecular.g, colorSpecular.b);
		material->ambientColor = glm::vec3(colorAmbient.r, colorAmbient.g, colorAmbient.b);

		material->opacity = opacity;

		// DIFFUSE TEXTURES
		material->diffuseTextures = loadMaterialTextures(directory, scene, mesh_material, aiTextureType_DIFFUSE);
		if (material->diffuseTextures.size() > 1)
			Warning("More than one diffuse texture loaded. Engine does not support multiple diffuse textures");

		// SPECULAR TEXTURES
		material->specularTextures = loadMaterialTextures(directory, scene, mesh_material, aiTextureType_SPECULAR);
		if (material->specularTextures.size() > 1)
			Warning("More than one specular texture loaded. Engine does not support multiple specular textures");

		// NORMAL TEXTURES
		material->normalTextures = loadMaterialTextures(directory, scene, mesh_material, aiTextureType_NORMALS);
		if (material->normalTextures.empty())
			material->normalTextures = loadMaterialTextures(directory, scene, mesh_material, aiTextureType_HEIGHT);
		if (material->normalTextures.size() > 1)
			Warning("More than one normal texture loaded. Engine does not support multiple normal textures");

		//// SHININESS TEXTURES
		//material->shininessTextures = loadMaterialTextures(directory, scene, mesh_material, aiTextureType_SHININESS);
		//if (material->shininessTextures.size() > 1)
		//	Warning("More than one shininess texture loaded. Engine does not support multiple shininessMaps textures");

		//// EMISSIVE TEXTURES
		//material->emissionTextures = loadMaterialTextures(directory, scene, mesh_material, aiTextureType_EMISSIVE);
		//if (material->emissionTextures.size() > 1)
		//	Warning("More than one emission texture loaded. Engine does not support multiple emissionMaps textures");

		//// OPACITY TEXTURES
		//material->opacityTextures = loadMaterialTextures(directory, scene, mesh_material, aiTextureType_OPACITY);
		//if (material->opacityTextures.size() > 1)
		//	Warning("More than one opacity texture loaded. Engine does not support multiple opacityMaps textures");
	}
	
	return new Mesh(vertices, indices, material);
}
//=============================================================================
std::vector<Texture2D> Model::loadMaterialTextures(std::string_view directory, const aiScene* scene, aiMaterial* mat, aiTextureType type)
{
	std::vector<Texture2D> texs;

	for (unsigned i{ 0 }; i < mat->GetTextureCount(type); ++i)
	{
		aiString path;
		mat->GetTexture(type, i, &path);

		size_t index = std::string(path.C_Str()).find_last_of("/");
		std::string texName = std::string(path.C_Str()).substr(index + 1);

		Texture2D texture;
		if (texName.at(0) == '*' && index == std::string::npos)
		{
			int embedded{ static_cast<int>(texName.at(1) - '0') };
			aiTexture* embTex = scene->mTextures[embedded];
			//aiTexel* texData = embTex->pcData;
			std::string name = m_name + " --- " + std::string(embTex->mFilename.C_Str()) + " --- " + texName;
			texture = textures::CreateTextureFromData(name, embTex, false);
		}
		else
		{
			std::string texPath = std::string(directory) + texName;
			texture = textures::LoadTexture2D(texPath);
		}
		bool isFind{ false };
		for (size_t j = 0; j < texs.size(); j++)
		{
			if (texs[j] == texture)
			{
				isFind = true;
				break;
			}
		}
		if (!isFind)
			texs.push_back(texture);
	}

	return texs;
}
//=============================================================================
void Model::computeAABB()
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		m_aabb.CombineAABB(m_meshes[i]->GetAABB());
	}
}
//=============================================================================