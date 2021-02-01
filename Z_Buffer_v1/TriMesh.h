#pragma once
#include <vector>
#include "math/Vec.h"
#include <string>
#include "math/AABB.h"

struct Vertex3
{
	Vec3f pos;	// 顶点的几何信息
};

struct Tri
{
	unsigned int vid[3];	//三角形中顶点的序号
};

class TriMesh
{
public:
	std::vector<Vertex3> vtxs_;		// 存储顶点
	std::vector<Tri> tris_;		// 存储三角面片

	int num_vtxs_{ 0 };		// 顶点的数量
	int num_tris_{ 0 };		// 面片的数量

	AABB aabb;

	void LoadFile(std::string file_name);
	void translate(Vec3f vec3f);
	void scale(float s);
	void normalization();
};

