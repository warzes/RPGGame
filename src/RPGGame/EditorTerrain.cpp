#include "stdafx.h"
#include "Terrain.h"
//=============================================================================
enum EDIT_MODE_CURRENT_BRUSH
{
	BRUSH_PAINT,
	BRUSH_FLATTEN,
	EDIT_MODE_NUM_BRUSHES
};
EDIT_MODE_CURRENT_BRUSH g_current_brush = BRUSH_PAINT;
//=============================================================================
void paint_terrain(Terrain& t, int terrain_index, float edit_speed, float paint_radius, double dt)
{
	assert(paint_radius >= 0);
	assert(terrain_index >= 0);
	//Paint terrain
	for (int j = -paint_radius; j <= paint_radius; j++)
	{
		for (int k = -paint_radius; k <= paint_radius; k++)
		{
			int idx = terrain_index + k + j * t.num_verts_x;
			if (input::IsMouseDown(RGFW_mouseLeft))
			{
				t.vp[3 * idx + 1] = glm::min(t.vp[3 * idx + 1] + edit_speed * dt, (double)DEFAULT_TERRAIN_HEIGHT);
			}
			if (input::IsMouseDown(RGFW_mouseMiddle))
			{
				t.vp[3 * idx + 1] = glm::max(t.vp[3 * idx + 1] - edit_speed * dt, 0.0);
			}

			//Draw vertices
			float x = float(paint_radius - abs(j)) / paint_radius;
			float z = float(paint_radius - abs(k)) / paint_radius;
			float factor = (x + z) / 2;
			//draw_point(glm::vec3(t.vp[3 * idx], t.vp[3 * idx + 1], t.vp[3 * idx + 2]), (1 + factor) * 0.05f, glm::vec4(0.8, factor * 0.8, 0, 1));
		}
	}
	t.vbo->Allocate(sizeof(float) * t.point_count * 3, ogl::AccessSpecifier::StaticDraw);
	t.vbo->Upload(t.vp);
	t.recalculateNormals();
}

void flatten_terrain(Terrain& t, int terrain_index, float edit_speed, float paint_radius, double dt)
{
	assert(paint_radius >= 0);
	assert(terrain_index >= 0);

	float target_min = 999999;
	float target_max = -99999;
	for (int j = -paint_radius; j <= paint_radius; j++)
	{
		for (int k = -paint_radius; k <= paint_radius; k++)
		{
			int idx = terrain_index + k + j * t.num_verts_x;
			float h = t.vp[3 * idx + 1];
			if (h > target_max) target_max = h;
			if (h < target_min) target_min = h;
		}
	}

	for (int j = -paint_radius; j <= paint_radius; j++)
	{
		for (int k = -paint_radius; k <= paint_radius; k++)
		{
			int idx = terrain_index + k + j * t.num_verts_x;
			if (input::IsMouseDown(RGFW_mouseLeft))
			{
				t.vp[3 * idx + 1] += edit_speed * dt;
				if (t.vp[3 * idx + 1] > target_max) t.vp[3 * idx + 1] = target_max;
			}
			if (input::IsMouseDown(RGFW_mouseMiddle))
			{
				t.vp[3 * idx + 1] -= edit_speed * dt;
				if (t.vp[3 * idx + 1] < target_min) t.vp[3 * idx + 1] = target_min;
			}

			//Draw vertices
			float x = float(paint_radius - abs(j)) / paint_radius;
			float z = float(paint_radius - abs(k)) / paint_radius;
			float factor = (x + z) / 2;
			//draw_point(vec3(t->vp[3 * idx], t->vp[3 * idx + 1], t->vp[3 * idx + 2]), (1 + factor) * 0.05f, vec4(0, 0.8, factor * 0.8, 1));
		}
	}

	t.vbo->Allocate(sizeof(float) * t.point_count * 3, ogl::AccessSpecifier::StaticDraw);
	t.vbo->Upload(t.vp);
	t.recalculateNormals();
}
//=============================================================================
void EditorUpdate(double dt, const glm::mat4& proj, const glm::mat4& view, const glm::vec3& camPos, Terrain& terrain)
{
	//Data
	static int paint_radius = 2;
	static float edit_speed = 5.0f;

	//Hotkeys
	{
		//Number keys to choose brush
		for (int i = 0; i < EDIT_MODE_NUM_BRUSHES; i++)
		{
			if (input::IsKeyPressed(RGFW_1 + i))
				g_current_brush = (EDIT_MODE_CURRENT_BRUSH)(BRUSH_PAINT + i);
		}
	}

	//Ray picking to raise/lower ground
	//if (g_mouse.is_in_window)
	{
		float x_nds = /*(cam_mouse_controls) ?*/ 0 /*: (2 * g_mouse.xpos / window::GetWidth()) - 1*/;
		float y_nds = /*(cam_mouse_controls) ?*/ 0 /*: 1 - (2 * g_mouse.ypos) / window::GetHeight()*/;
		//vec3 ray_nds = vec3(x_nds, y_nds, 1.0f);
		glm::vec4 ray_clip = glm::vec4(x_nds, y_nds, -1.0f, 1.0f);
		glm::vec4 ray_eye = glm::inverse(proj) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
		glm::vec3 ray_world = glm::vec3(glm::inverse(view) * ray_eye);
		ray_world = glm::normalize(ray_world);

		const int num_ray_marches = 64;
		const float ray_step_size = 0.5f;
		// static vec3 ray_positions[num_ray_marches-10];
		// static vec3 ground_positions[num_ray_marches-10];
		// static int num_vec_draws = 0;

		for (int i = 0; i < num_ray_marches; ++i)
		{
			glm::vec3 ray_pos = camPos + ray_world * (float)i * ray_step_size;

			int terrain_index = terrain.GetTerrainIndex(ray_pos.x, ray_pos.z);
			//float ground_y = (terrain_index<0)? -INFINITY : g_terrain.height*g_terrain.vp[3*terrain_index+1]/g_terrain.pgm_res; 
			float ground_y = terrain.GetHeightInterp(ray_pos.x, ray_pos.z);

			// ------------------------------------------------------------
			//DEBUG DRAWING STUFF
			// static bool k_was_pressed = false;
			// static bool freeze_ray_vis = false;
			// if(glfwGetKey(window, GLFW_KEY_K)) {
			// 	if(!k_was_pressed) {
			//         freeze_ray_vis = !freeze_ray_vis;
			// 		k_was_pressed = true;
			// 	}
			// }
			// else k_was_pressed = false;

			// if(i>=10 && !freeze_ray_vis) 
			// {
			//     ray_positions[i-10] = ray_pos;
			//     //ground_positions[i-10] = vec3(g_terrain.vp[3*terrain_index], g_terrain.vp[3*terrain_index+1], g_terrain.vp[3*terrain_index+2]);
			//     ground_positions[i-10] = vec3(ray_pos.x, ground_y, ray_pos.z);
			//     num_vec_draws = i-10;
			// }
			// ------------------------------------------------------------

			//if we've gone "underground" without entering the terrain's xz perimeter, break
			//avoids the case where you arrive underground without ever hitting the surface:
			/* \  ___ground___
			//   \
			//     \  <- Like this */
			if (ray_pos.y < 0)
			{
				if (ray_pos.x < -terrain.width / 2)  break;
				if (ray_pos.x > terrain.width / 2)  break;
				if (ray_pos.z < -terrain.length / 2) break;
				if (ray_pos.z > terrain.length / 2) break;
			}

			//if(fabs(ray_pos.y-ground_y)<0.1f) 
			if (ray_pos.y < ground_y)
			{
				switch (g_current_brush) {
				case BRUSH_PAINT:
					paint_terrain(terrain, terrain_index, edit_speed, paint_radius, dt);
					break;

				case BRUSH_FLATTEN:
					flatten_terrain(terrain, terrain_index, edit_speed, paint_radius, dt);
					break;

				default:
					printf("Warning in editor_update, unknown brush type: %u\n", g_current_brush);
					break;
				}
				//draw_vec(ray_pos, vec3(0,3,0), vec4(1,0,0,1));
				break;
			}
		}//endfor

		// ------------------------------------------------------------
		// for(int i=0; i<num_vec_draws; ++i)
		// {
		//     draw_point(ray_positions[i],    0.05, vec4(0.8, 0, 0.8, 1));
		//     draw_point(ground_positions[i], 0.05, vec4(0.8, 0.6, 0, 1));
		// }
		// ------------------------------------------------------------

	}
}
//=============================================================================