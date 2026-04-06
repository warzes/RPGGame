#include "stdafx.h"
#include "Terrain.h"
//=============================================================================
void EditorUpdate(double dt, const glm::mat4& proj, const glm::mat4& view, const glm::vec3& camPos, Terrain& terrain);
//=============================================================================
bool Terrain::Init(const std::string& filename)
{
	this->filename = filename;

	//Check if terrain is saved on disc
	if (!loadTerrain())
	{
		//Init with defaults
		height = DEFAULT_TERRAIN_HEIGHT;
		cell_size = DEFAULT_TERRAIN_CELL_SIZE;
		num_verts_x = DEFAULT_TERRAIN_NUM_X;
		num_verts_z = DEFAULT_TERRAIN_NUM_Z;
		width = DEFAULT_TERRAIN_CELL_SIZE * (DEFAULT_TERRAIN_NUM_X - 1);
		length = DEFAULT_TERRAIN_CELL_SIZE * (DEFAULT_TERRAIN_NUM_Z - 1);
		pgm_res = DEFAULT_TERRAIN_PGM_RES;
		point_count = num_verts_x * num_verts_z;
		vp = (float*)malloc(point_count * 3 * sizeof(float));

		int i = 0;
		float z_pos = -length / 2.0f;
		float x_pos;
		for (int r = 0; r < num_verts_z; r++)
		{
			x_pos = -width / 2.0f;
			for (int c = 0; c < num_verts_x; c++, i++)
			{
				assert(i < point_count);
				vp[3 * i] = x_pos;
				vp[3 * i + 1] = 0;
				vp[3 * i + 2] = z_pos;
				//print(vec3(x_pos,0,z_pos));
				x_pos += cell_size;
			}
			z_pos += cell_size;
		}
	}

	vn = (float*)malloc(point_count * 3 * sizeof(float));

	int num_quads = (num_verts_x - 1) * (num_verts_z - 1);
	num_indices = 3 * 2 * num_quads; //3 verts per tri, 2 tris per quad

	uint16_t* indices = (uint16_t*)calloc(num_indices, sizeof(uint16_t));
	{
		int i = 0;
		for (int v = num_verts_x; v < num_verts_x * num_verts_z; v++)
		{ //Add two triangles per quad
			if ((v + 1) % num_verts_x != 0)
			{
				indices[i++] = v;
				indices[i++] = v - num_verts_x + 1;
				indices[i++] = v - num_verts_x;

				indices[i++] = v;
				indices[i++] = v + 1;
				indices[i++] = v - num_verts_x + 1;
			}
		}
	}



	vbo = new ogl::Buffer;
	vbo->Allocate(sizeof(float) * point_count * 3, ogl::AccessSpecifier::StaticDraw);
	vbo->Upload(vp);

	vboNormal = new ogl::Buffer;
	recalculateNormals();

	ibo = new ogl::Buffer;
	ibo->Allocate(sizeof(uint16_t) * num_indices, ogl::AccessSpecifier::StaticDraw);
	ibo->Upload(indices);

	free(indices);

	vao = new ogl::VertexArray;

	vs = new ogl::ShaderStage(ogl::ShaderType::Vertex);
	vs->Upload(R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out float height;
out vec3 norm;

void main()
{
	height = aPos.y;
	norm = aNormal;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
}
)");
	vs->Compile();

	fs = new ogl::ShaderStage(ogl::ShaderType::Fragment);
	fs->Upload(R"(
#version 330 core

in float height;
in vec3 norm;
out vec4 frag_colour;

vec3 sun_dir = normalize(vec3(-1.0, -1.0, 0.3));

void main() {
	float x = dot(-sun_dir, norm);
	frag_colour = (x+0.2)*vec4((0.8-norm.y), 0.1*height+0.5, 0.1, 1.0);
}
)");
	fs->Compile();

	program = new ogl::ShaderProgram;
	program->Attach(*vs);
	program->Attach(*fs);
	program->Link();

	vao->SetVertexBuffer(0, vbo->GetID(), 0, sizeof(float) * 3);
	vao->SetVertexBuffer(1, vboNormal->GetID(), 0, sizeof(float) * 3);
	vao->SetElementBuffer(ibo->GetID());
	
	vao->EnableAttrib(0);
	vao->SetAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	vao->SetAttribBinding(0, 0);

	vao->EnableAttrib(1);
	vao->SetAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	vao->SetAttribBinding(1, 1);

	return false;
}
//=============================================================================
void Terrain::Close()
{
	if (vp) free(vp);
	if (vn) free(vn);

	delete vao;
	delete vbo;
	delete vboNormal;
	delete ibo;
	delete vs;
	delete fs;
	delete program;
}
//=============================================================================
void Terrain::Draw(const glm::mat4& proj, const glm::mat4& view)
{
	program->Bind();
	program->SetUniform("modelMatrix", glm::mat4(1.0f));
	program->SetUniform("viewMatrix", view);
	program->SetUniform("projectionMatrix", proj);

	vao->Bind();
	vao->SetVertexBuffer(0, vbo->GetID(), 0, sizeof(float) * 3);
	vao->SetVertexBuffer(1, vboNormal->GetID(), 0, sizeof(float) * 3);
	vao->SetElementBuffer(ibo->GetID());

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, 0);
	vao->Unbind();
}
//=============================================================================
void Terrain::Update(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& camPos)
{
	EditorUpdate(engine::GetDeltaTime(), proj, view, camPos, *this);
}
//=============================================================================
int Terrain::GetTerrainIndex(float x, float z)
{
	if (x<-width / 2 || x>width / 2 || z<-length / 2 || z>length / 2) return -1;
	int row = (z + length / 2) / cell_size;
	int col = (x + width / 2) / cell_size;
	int i = num_verts_x * row + col;

	float x_tl = vp[3 * i];
	float z_tl = vp[3 * i + 2];
	float x_t = (x - x_tl) / cell_size; // % along cell point is on x-axis
	float z_t = (z - z_tl) / cell_size; // % along cell point is on z-axis

	if (x_t > 0.5) i += 1;
	if (z_t > 0.5) i += num_verts_x;

	return i;
}
//=============================================================================
float Terrain::GetHeightInterp(float x, float z)
{
	if (x<-width / 2 || x>width / 2 || z<-length / 2 || z>length / 2) return -INFINITY;

	//Get index of top-left vert of quad containing x,z
	int row = (z + length / 2) / cell_size;
	int col = (x + width / 2) / cell_size;
	int i = num_verts_x * row + col;

	//Get heights of this quad's verts
	float y_tl = vp[3 * i + 1];
	float y_tr = vp[3 * (i + 1) + 1];
	float y_bl = vp[3 * (i + num_verts_x) + 1];
	float y_br = vp[3 * (i + num_verts_x + 1) + 1];

	//Get x,z position of top-left vert
	float x_tl = vp[3 * i];
	float z_tl = vp[3 * i + 2];
	float x_t = (x - x_tl) / cell_size; // % along cell point is on x-axis
	float z_t = (z - z_tl) / cell_size; // % along cell point is on z-axis

	// vec3 topleft  = vec3(x_tl, y_tl, z_tl);
	// vec3 topright = vec3(vp[3*(i+1)], y_tr, vp[3*(i+1)+2]);
	// vec3 botleft  = vec3(vp[3*(i+t.num_verts_x)], y_bl, vp[3*(i+t.num_verts_x)+2]);
	// vec3 botright = vec3(vp[3*(i+t.num_verts_x+1)], y_br, vp[3*(i+t.num_verts_x+1)+2]);

	// draw_point(topleft,  0.2, vec4(1,1,0,1));
	// draw_point(topright, 0.2, vec4(1,1,0,1));
	// draw_point(botleft,  0.2, vec4(1,1,0,1));
	// draw_point(botright, 0.2, vec4(1,1,0,1));

	//Barycentric Interpolation
	glm::vec3 a = glm::vec3(x_tl + cell_size, 0, z_tl); //tr
	glm::vec3 b = glm::vec3(x_tl, 0, z_tl + cell_size); //bl
	glm::vec3 c; //will be tl or br depending on x,z
	glm::vec3 p = glm::vec3(x, 0, z);
	if (x_t + z_t > 1)
		c = glm::vec3(x_tl + cell_size, 0, z_tl + cell_size); //br
	else
		c = glm::vec3(x_tl, 0, z_tl); //tl
	float tri_area = cell_size / 2;

	float u = (glm::length(glm::cross(b - p, c - p)) / (2 * cell_size)) / tri_area; //weight of a
	float v = (glm::length(glm::cross(a - p, c - p)) / (2 * cell_size)) / tri_area; //weight of b
	float w = (glm::length(glm::cross(a - p, b - p)) / (2 * cell_size)) / tri_area; //weight of c

	if (x_t + z_t > 1)
		return u * y_tr + v * y_bl + w * y_br;

	return u * y_tr + v * y_bl + w * y_tl;
}
//=============================================================================
glm::vec3 Terrain::GetNormalInterp(float x, float z)
{
	if (x<-width / 2 || x>width / 2 || z<-length / 2 || z>length / 2) return glm::vec3(-INFINITY, -INFINITY, -INFINITY);

	//Get index of top-left vert of quad containing x,z
	int row = (z + length / 2) / cell_size;
	int col = (x + width / 2) / cell_size;
	int i = num_verts_x * row + col;

	//Get x,z position of top-left vert
	float x_tl = vp[3 * i];
	float z_tl = vp[3 * i + 2];
	float x_t = (x - x_tl) / cell_size; // % along cell point is on x-axis
	float z_t = (z - z_tl) / cell_size; // % along cell point is on z-axis

	// Get surrounding normals
	glm::vec3 norm_a = glm::vec3(vn[3 * (i + 1)], vn[3 * (i + 1) + 1], vn[3 * (i + 1) + 2]); //tr
	glm::vec3 norm_b = glm::vec3(vn[3 * (i + num_verts_x)], vn[3 * (i + num_verts_x) + 1], vn[3 * (i + num_verts_x) + 2]); //bl
	glm::vec3 norm_c;
	if (x_t + z_t > cell_size) norm_c = glm::vec3(vn[3 * (i + num_verts_x + 1)], vn[3 * (i + num_verts_x + 1) + 1], vn[3 * (i + num_verts_x + 1) + 2]); //br
	else norm_c = glm::vec3(vn[3 * i], vn[3 * i + 1], vn[3 * i + 2]); //tl

	//Barycentric Interpolation
	glm::vec3 a = glm::vec3(x_tl + cell_size, 0, z_tl); //tr
	glm::vec3 b = glm::vec3(x_tl, 0, z_tl + cell_size); //bl
	glm::vec3 c; //will be tl or br depending on x,z
	glm::vec3 p = glm::vec3(x, 0, z);
	if (x_t + z_t > cell_size) c = glm::vec3(x_tl + cell_size, 0, z_tl + cell_size); //br
	else c = glm::vec3(x_tl, 0, z_tl); //tl
	float tri_area = cell_size / 2;

	float u = (glm::length(glm::cross(b - p, c - p)) / 2) / tri_area; //weight of a
	float v = (glm::length(glm::cross(a - p, c - p)) / 2) / tri_area; //weight of b
	float w = (glm::length(glm::cross(a - p, b - p)) / 2) / tri_area; //weight of c

	return glm::normalize(norm_a * u + norm_b * v + norm_c * w);
}
//=============================================================================
glm::vec3 Terrain::GetDisplacement(float x, float y, float z)
{
	if (x<-width / 2 || x>width / 2 || z<-length / 2 || z>length / 2) return glm::vec3(-INFINITY, -INFINITY, -INFINITY);

	//Get index of top-left vert of quad containing x,z
	int row = (z + length) / cell_size;
	int col = (x + width) / cell_size;
	int i = num_verts_x * row + col;

	//Get x,z position of top-left vert
	float x_tl = vp[3 * i];
	float z_tl = vp[3 * i + 2];
	float x_t = (x - x_tl) / cell_size; // % along cell point is on x-axis
	float z_t = (z - z_tl) / cell_size; // % along cell point is on z-axis

	// Get surrounding normals
	glm::vec3 norm_a = glm::vec3(vn[3 * (i + 1)], vn[3 * (i + 1) + 1], vn[3 * (i + 1) + 2]); //tr
	glm::vec3 norm_b = glm::vec3(vn[3 * (i + num_verts_x)], vn[3 * (i + num_verts_x) + 1], vn[3 * (i + num_verts_x) + 2]); //bl
	glm::vec3 norm_c;
	if (x_t + z_t > cell_size) norm_c = glm::vec3(vn[3 * (i + num_verts_x + 1)], vn[3 * (i + num_verts_x + 1) + 1], vn[3 * (i + num_verts_x + 1) + 2]); //br
	else norm_c = glm::vec3(vn[3 * i], vn[3 * i + 1], vn[3 * i + 2]); //tl

	//Barycentric Interpolation
	glm::vec3 a = glm::vec3(x_tl + cell_size, 0, z_tl); //tr
	glm::vec3 b = glm::vec3(x_tl, 0, z_tl + cell_size); //bl
	glm::vec3 c; //will be tl or br depending on x,z
	glm::vec3 p = glm::vec3(x, 0, z);
	if (x_t + z_t > cell_size) c = glm::vec3(x_tl + cell_size, 0, z_tl + cell_size); //br
	else c = glm::vec3(x_tl, 0, z_tl); //tl
	float tri_area = cell_size / 2;

	float u = (glm::length(glm::cross(b - p, c - p)) / 2) / tri_area; //weight of a
	float v = (glm::length(glm::cross(a - p, c - p)) / 2) / tri_area; //weight of b
	float w = (glm::length(glm::cross(a - p, b - p)) / 2) / tri_area; //weight of c

	glm::vec3 norm = glm::normalize(norm_a * u + norm_b * v + norm_c * w);
	glm::vec3 top_rgt = a;
	top_rgt.y = vp[3 * (i + 1) + 1];
	float disp = glm::dot(glm::vec3(x, y, z) - top_rgt, norm);

	//DEBUG: Draw tri's normals
	// a.y = t.vp[3*(i+1) + 1];
	// b.y = t.vp[3*(i+t_num_verts_x) + 1];
	// if(x_t+z_t > t.cell_size) c.y = t.vp[3*(i+t.num_verts_x+1) + 1]; //br
	// else c.y = t.vp[3*i + 1];; //tl

	// draw_vec(a, norm_a, vec4(0.8f, 0.7f, 0.1f, 1));
	// draw_vec(b, norm_b, vec4(0.8f, 0.7f, 0.1f, 1));
	// draw_vec(c, norm_c, vec4(0.8f, 0.7f, 0.1f, 1));

	return norm * disp;
}
//=============================================================================
void Terrain::SaveTerrain()
{
	FILE* fp;
	auto err = fopen_s(&fp, filename.c_str(), "w");
	fprintf(fp, "P2\n");
	fprintf(fp, "%d %d \n", num_verts_x, num_verts_z);
	fprintf(fp, "%d\n", pgm_res); //Colour resolution

	for (int i = 0; i < num_verts_z; i++)
	{
		for (int j = 0; j < num_verts_x; j++)
		{
			int index = num_verts_x * i + j;
			fprintf(fp, "%d ", int(pgm_res * vp[3 * index + 1] / height));
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}
//=============================================================================
//Utility function: reads past comments, spaces and newlines in a pgm file
void _pgm_skip_fluff(FILE* fp)
{
	char c;
	while ((c = fgetc(fp)) && ((c == ' ') || (c == '#') || (c == '\n')))
		if (c == '#') while ((c = fgetc(fp)) && (c != '\n'));

	fseek(fp, -1, SEEK_CUR); //rewind 1 char
}
//=============================================================================
bool Terrain::loadTerrain()
{
	FILE* fp;
	auto errr = fopen_s(&fp, filename.c_str(), "r");
	if (!fp) {
		//printf("Error reading %s, file does not exist\n", t->filename);
		return false;
	}

	//Read header
	{
		//Preamble (comments, spaces etc)
		_pgm_skip_fluff(fp);

		//Magic number (should be P2)
		char type[2];
		type[0] = fgetc(fp);
		type[1] = fgetc(fp);
		assert(type[0] == 'P' && type[1] == '2');
		_pgm_skip_fluff(fp);

		//Dimensions
		int w;
		{
			char c[16]; int i = 0;
			while ((c[i] = fgetc(fp)) && (c[i] != ' ') && (c[i] != '\n')) ++i;
			sscanf_s(c, "%d", &w);
			//printf("width: %d\n", w);
			_pgm_skip_fluff(fp);
		}
		int l;
		{
			char c[16]; int i = 0;
			while ((c[i] = fgetc(fp)) && (c[i] != ' ') && (c[i] != '\n')) ++i;
			sscanf_s(c, "%d", &l);
			//printf("length: %d\n", l);
			_pgm_skip_fluff(fp);
		}

		num_verts_x = w;
		num_verts_z = l;
		point_count = num_verts_x * num_verts_z;

		//Colour resolution
		int res;
		{
			char c[16]; int i = 0;
			while ((c[i] = fgetc(fp)) && (c[i] != ' ') && (c[i] != '\n')) ++i;
			sscanf_s(c, "%d", &res);
			//printf("res: %d\n", res);
			_pgm_skip_fluff(fp);
		}
		pgm_res = res;
	}

	vp = (float*)malloc(point_count * 3 * sizeof(float));

	height = DEFAULT_TERRAIN_HEIGHT;
	cell_size = DEFAULT_TERRAIN_CELL_SIZE;
	width = (num_verts_x - 1) * (cell_size);
	length = (num_verts_z - 1) * (cell_size);

	int i = 0;
	float z_pos = -length / 2.0f;
	float x_pos;
	for (int r = 0; r < num_verts_z; r++)
	{
		x_pos = -width / 2.0f;
		for (int c = 0; c < num_verts_x; c++, i++)
		{
			assert(i < point_count);

			int height_value;
			fscanf_s(fp, "%d", &height_value);
			vp[3 * i] = x_pos;
			vp[3 * i + 1] = height * (float)height_value / pgm_res;
			vp[3 * i + 2] = z_pos;
			x_pos += cell_size;
		}
		z_pos += cell_size;
	}

	fclose(fp);
	return true;
}
//=============================================================================
void Terrain::recalculateNormals()
{
	for (int i = 0; i < (point_count); i++)
	{
		float l, r, a, b; // 4 surrounding height values
		if (i % (num_verts_x))
			l = vp[3 * (i - 1) + 1];
		else l = vp[3 * i + 1];
		if ((i + 1) % num_verts_x)
			r = vp[3 * (i + 1) + 1];
		else r = vp[3 * i + 1];
		if (i > num_verts_x)
			a = vp[3 * (i - num_verts_x) + 1];
		else a = vp[3 * i + 1];
		if (i + num_verts_x < point_count)
			b = vp[3 * (i + num_verts_x) + 1];
		else b = vp[3 * i + 1];

		glm::vec3 d_x = glm::vec3(1, r - l, 0);
		glm::vec3 d_y = glm::vec3(0, b - a, 1);
		glm::vec3 norm = glm::normalize(cross(d_y, d_x));
		vn[3 * i] = norm.x;
		vn[3 * i + 1] = norm.y;
		vn[3 * i + 2] = norm.z;
	}

	vboNormal->Allocate(sizeof(float) * point_count * 3, ogl::AccessSpecifier::StaticDraw);
	vboNormal->Upload(vn);
}
//=============================================================================