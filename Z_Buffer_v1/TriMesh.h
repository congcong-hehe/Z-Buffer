#pragma once
#include <vector>
#include "math/Vec.h"
#include <string>
#include "math/AABB.h"

struct Vertex3
{
	Vec3f pos;	// ����ļ�����Ϣ
};

struct Tri
{
	unsigned int vid[3];	//�������ж�������
};

class TriMesh
{
public:
	std::vector<Vertex3> vtxs_;		// �洢����
	std::vector<Tri> tris_;		// �洢������Ƭ

	int num_vtxs_{ 0 };		// ���������
	int num_tris_{ 0 };		// ��Ƭ������

	AABB aabb;

	void LoadFile(std::string file_name);
	void translate(Vec3f vec3f);
	void scale(float s);
	void normalization();
};

