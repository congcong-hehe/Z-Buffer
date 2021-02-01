#include "TriMesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <assert.h>

void TriMesh::LoadFile(std::string file_name)
{
	std::ifstream in(file_name);
	
	assert(in);

	std::string line;
	while (std::getline(in, line))
	{
		std::string tag;
		std::istringstream is(line);

		is >> tag;

		if (tag == "v")
		{
			Vertex3 vertex3;
			is >> vertex3.pos.x >> vertex3.pos.y >> vertex3.pos.z;

			aabb.x_min = std::min(vertex3.pos.x, aabb.x_min);
			aabb.x_max = std::max(vertex3.pos.x, aabb.x_max);
			aabb.y_min = std::min(vertex3.pos.y, aabb.y_min);
			aabb.y_max = std::max(vertex3.pos.y, aabb.y_max);
			aabb.z_min = std::min(vertex3.pos.z, aabb.z_min);
			aabb.z_max = std::max(vertex3.pos.z, aabb.z_max);

			vtxs_.push_back(vertex3);
		}
		if (tag == "f")
		{
			Tri tri;
			for (int i = 0; i < 3; ++i)
			{
				char trash;
				int pos_id, uv_id, norm_id;
				is >> pos_id >> trash >> uv_id >> trash >> norm_id;
				tri.vid[i] = pos_id - 1;
			}
			tris_.push_back(tri);
		}
	}
	num_tris_ = tris_.size();
	num_vtxs_ = vtxs_.size();

	std::cout << "Load succees! num of vtx: " << num_vtxs_ << " num of tris: " << num_tris_ << std::endl;

}

void TriMesh::translate(Vec3f vec3f)
{
	for (int i = 0; i < num_vtxs_; ++i)
	{
		vtxs_[i].pos = vtxs_[i].pos + vec3f;
	}
}

void TriMesh::scale(float s)
{
	for (int i = 0; i < num_vtxs_; ++i)
	{
		vtxs_[i].pos = vtxs_[i].pos * s;
	}
}

void TriMesh::normalization()
{
	float x_diam = aabb.x_max - aabb.x_min;
	float y_diam = aabb.y_max - aabb.y_min;
	float z_diam = aabb.z_max - aabb.z_min;

	float diam = x_diam;
	diam = std::max(diam, y_diam);
	diam = std::max(diam, z_diam);

	diam += diam * 0.1f;
	float scale = 2.0f / diam;

	Vec3f center = Vec3f((aabb.x_max + aabb.x_min) / 2.0f, (aabb.y_max + aabb.y_min) / 2.0f, (aabb.z_max + aabb.z_min) / 2.0f);

	// 将网格平移到中点并缩小scale倍
	for (int i = 0; i < num_vtxs_; ++i)
	{
		vtxs_[i].pos = (vtxs_[i].pos - center) * scale;
	}
}