#pragma once

#define DEFAULT_TERRAIN_HEIGHT 20
#define DEFAULT_TERRAIN_CELL_SIZE 2
#define DEFAULT_TERRAIN_NUM_X 8
#define DEFAULT_TERRAIN_NUM_Z 8
#define DEFAULT_TERRAIN_PGM_RES 255

class Terrain final
{
public:
	bool Init(const std::string& filename);
	void Close();
	void Draw(const glm::mat4& proj, const glm::mat4& view);
	void Update(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& camPos);

	// Returns index for closest terrain point to world pos x,z
	int GetTerrainIndex(float x, float z);
	// Returns height at world pos x,z interpolated from closest verts in height field
	float GetHeightInterp(float x, float z);
	// Returns normal at world pos x,z interpolated from closest verts in height field
	glm::vec3 GetNormalInterp(float x, float z);
	// Returns vector to closest point on ground from point x,y,z along surface normal
	glm::vec3 GetDisplacement(float x, float y, float z);

	// Save height data to file
	void SaveTerrain();

	std::string filename;
	//vec3 pos;
	float width, length, height;
	float cell_size;
	int num_verts_x, num_verts_z;

	float* vp, * vn;
	int point_count, num_indices;

	ogl::VertexArray* vao;
	ogl::Buffer* vbo;
	ogl::Buffer* vboNormal;
	ogl::Buffer* ibo;

	ogl::ShaderStage* vs;
	ogl::ShaderStage* fs;
	ogl::ShaderProgram* program;

	int pgm_res; //resolution of height data for pgm storage

//private:
	bool loadTerrain();
	void recalculateNormals();
};