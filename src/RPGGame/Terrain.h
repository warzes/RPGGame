#pragma once

class Terrain final
{
public:
	bool Init(const std::string& filename);
	void Draw(const glm::mat4& proj, const glm::mat4& view);

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

private:
	bool loadTerrain();
	void recalculateNormals();
};