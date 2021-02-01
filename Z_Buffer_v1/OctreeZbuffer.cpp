#include "OctreeZbuffer.h"
#include <math.h>
#include <time.h>

OctreeZbuffer::OctreeZbuffer(int w, int h, unsigned char* fb, Color back_color) : Painter(w, h, fb, back_color)
{
	int w_temp = w;
	// ������zbuffer���ڴ�
	while (w_temp >= 1)
	{
		float* zbuffer = (float*)malloc(w_temp * w_temp * sizeof(float));
		zbuffers_.push_back(zbuffer);
		zbuffer_len_.push_back(std::make_pair(w_temp * w_temp, w_temp));
		w_temp /= 2;
	}
	zbuffer_height_ = zbuffers_.size();
}

OctreeZbuffer::~OctreeZbuffer()
{
	for (int i = 0; i < zbuffers_.size(); ++i)
	{
		free(zbuffers_[i]);
	}
}

void OctreeZbuffer::clearZbuffer()
{
	for (int i = 0; i < zbuffers_.size(); ++i)
	{
		float* zbuffer = zbuffers_[i];
		for (int j = 0; j < zbuffer_len_[i].first; ++j)
		{
			zbuffer[j] = -FLT_MAX;
		}
	}
}

// �õ���άƽ��������ε�����zbuffer���Ĳ���������, ���Ϊ0��
static int getTriHeight(int x_max, int x_min, int y_max, int y_min, int& x, int& y)
{
	int height_x = 0, height_y = 0;
	while (x_max != x_min)
	{
		x_max /= 2;
		x_min /= 2;
		height_x++;
	}
	while (y_max != y_min)
	{
		y_max /= 2;
		y_min /= 2;
		height_y++;
	}
	x = x_max;
	y = y_max;

	int height = 0;
	if (height_x > height_y)
	{
		height = height_x;
		for (int i = 0; i < height - height_y; ++i)
		{
			y /= 2;
		}
	}
	else
	{
		height = height_y;
		for (int i = 0; i < height - height_x; ++i)
		{
			x /= 2;
		}
	}

	return height;
}


void OctreeZbuffer::drawTriangle(Vec2i* vis, float* Z, Color color)
{
	int x_max = 0, x_min = width_ - 1, y_max = 0, y_min = height_ - 1;
	float z_max = -FLT_MAX;		// ������о����ӵ������zֵ
	for (int i = 0; i < 3; ++i)
	{
		x_max = std::max(vis[i].x, x_max);
		x_min = std::min(vis[i].x, x_min);
		y_max = std::max(vis[i].y, y_max);
		y_min = std::min(vis[i].y, y_min);

		z_max = std::max(Z[i], z_max);
	}

	// ��ֹԽ��
	if (x_max == width_) x_max--;
	if (y_max == height_) y_max--;

	int x, y;
	int height = getTriHeight(x_max - 1, x_min + 1, y_max - 1, y_min + 1, x, y);

	// �������ξ����ӵ��zС�ڲ��zbuffer���е�z����ô��������
	if (z_max < zbuffers_[height][y * zbuffer_len_[height].second + x])
	{
		hiden_num++;
		return;
	}

	// ����y�����С�������ζ�������
	if (vis[0].y > vis[1].y)
	{
		std::swap(vis[0], vis[1]);
		std::swap(Z[0], Z[1]);
	}
	if (vis[0].y > vis[2].y)
	{
		std::swap(vis[0], vis[2]);
		std::swap(Z[0], Z[2]);
	}
	if (vis[1].y > vis[2].y)
	{
		std::swap(vis[1], vis[2]);
		std::swap(Z[1], Z[2]);
	}

	int total_height = vis[2].y - vis[0].y + 1;		// ����Ϊ0������Ҫ+1
	// ���ƴ�vis[0] �� vis[1]�Ĳ���
	for (int y = vis[0].y; y <= vis[1].y; ++y)
	{
		int part_height = vis[1].y - vis[0].y + 1;
		float step1 = (float)(y - vis[0].y) / part_height;
		float step2 = (float)(y - vis[0].y) / total_height;

		Vec2i v1 = vis[0] + (vis[1] - vis[0]) * step1;
		Vec2i v2 = vis[0] + (vis[2] - vis[0]) * step2;
		float z1 = Z[0] + (Z[1] - Z[0]) * step1;
		float z2 = Z[0] + (Z[2] - Z[0]) * step2;

		if (v1.x > v2.x)
		{
			std::swap(z1, z2);
			std::swap(v1, v2);
		}

		float dz = (z2 - z1) / (v2.x - v1.x);
		float z = z1;
		for (int x = v1.x; x <= v2.x; ++x)
		{
			if (z > zbuffers_[0][x + y * width_])
			{
				zbuffers_[0][x + y * width_] = z;
				drawPixel(x, y, color);
			}
			z += dz;
		}
	}

	// ���ƴ�vis[1] �� vis[2]�Ĳ���
	for (int y = vis[1].y; y <= vis[2].y; ++y)
	{
		int part_height = vis[2].y - vis[1].y + 1;
		float step1 = (float)(y - vis[1].y) / part_height;
		float step2 = (float)(y - vis[0].y) / total_height;

		Vec2i v1 = vis[1] + (vis[2] - vis[1]) * step1;
		Vec2i v2 = vis[0] + (vis[2] - vis[0]) * step2;
		float z1 = Z[1] + (Z[2] - Z[1]) * step1;
		float z2 = Z[0] + (Z[2] - Z[0]) * step2;

		if (v1.x > v2.x)
		{
			std::swap(z1, z2);
			std::swap(v1, v2);
		}

		float dz = (z2 - z1) / (v2.x - v1.x);
		float z = z1;
		for (int x = v1.x; x <= v2.x; ++x)
		{
			if (z > zbuffers_[0][x + y * width_])
			{
				zbuffers_[0][x + y * width_] = z;
				drawPixel(x, y, color);
			}
			z += dz;
		}
	}
}

static float zMin(float z1, float z2, float z3, float z4)
{
	float zmin = FLT_MAX;
	zmin = std::min(zmin, z1);
	zmin = std::min(zmin, z2);
	zmin = std::min(zmin, z3);
	zmin = std::min(zmin, z4);
	return zmin;
}

// ��д���������ص�ʱ����²��z-buffer
void OctreeZbuffer::drawPixel(int x, int y, Color color)
{
	if (x < 0 || x >= width_ || y < 0 || y >= height_) return;

	framebuffer_[(y * width_ + x) * 4 + 0] = color.b;	// B
	framebuffer_[(y * width_ + x) * 4 + 1] = color.g;	// G
	framebuffer_[(y * width_ + x) * 4 + 2] = color.r;	// R
	framebuffer_[(y * width_ + x) * 4 + 3] = 255;

	// �ӵ�1�㿪ʼ������0��
	for (int i = 1; i < zbuffer_height_; ++i)
	{

		if (x % 2 == 1) x--;
		if (y % 2 == 1) y--;

		// ȡ�ĸ��Ӹ����е���Сֵ
		zbuffers_[i][(y / 2) * zbuffer_len_[i].second + x / 2] = zMin(
			zbuffers_[i - 1][y * zbuffer_len_[i - 1].second + x],
			zbuffers_[i - 1][y * zbuffer_len_[i - 1].second + x + 1],
			zbuffers_[i - 1][(y + 1) * zbuffer_len_[i - 1].second + x],
			zbuffers_[i - 1][(y + 1) * zbuffer_len_[i - 1].second + x + 1]
		);

		x /= 2;
		y /= 2;
	}
}

// ���ư˲����нڵ�������Ƭ�ĵݹ麯��
void OctreeZbuffer::drawOctreeNode(OctreeNode* current_node, int depth)
{
	if (current_node == nullptr)
	{
		return;
	}

	// ���ݰ˲����ڵ��еİ�Χ�е����zֵ���п����޳�
	int x = (current_node->aabb.x_min + 1.0f) * width_ / 2.0f;	//��ά����ת��Ϊ��ά��Ļ����
	int y = height_ - ((current_node->aabb.y_min + 1.0f) * height_ / 2.0f);	//��ά����ת��Ϊ��ά��Ļ����

	for (int i = 0; i < zbuffer_height_ - depth; ++i)
	{
		x /= 2;
		y /= 2;
	}

	if (current_node->aabb.z_max < zbuffers_[zbuffer_height_ - depth][y * int(pow(2, depth-1)) + x])
	{
		oct_hiden_num += 1;
		return;
	}

	for (int i = 0; i < 8; ++i)
	{
		drawOctreeNode(current_node->nodes[i], depth + 1);
	}

	// ����������
	auto &tris = current_node->tris;
	for (int i = 0; i < tris.size(); ++i)
	{
		Vec2i vec2is[3];	// ��Ļ�е�����
		float Z[3];

		for (int j = 0; j < 3; ++j)
		{
			vec2is[j] = Vec2i((tris[i].vec3f[j].x + 1.0f) * width_ / 2.0f, height_ - ((tris[i].vec3f[j].y + 1.0f) * height_ / 2.0f));
			Z[j] = tris[i].vec3f[j].z;
		}

		drawTriangle(vec2is, Z, tris[i].color);
	}
}

void OctreeZbuffer::drawMesh(std::vector<TriMesh*>& triMeshs)
{
	clearZbuffer();
	hiden_num = 0;
	oct_hiden_num = 0;

	clock_t begin = clock();
	octree.OctreeCreate(triMeshs);	// ��������Ƭ����˲����ṹ��
	clock_t end = clock();
	printf("build octree cost %d ms\n", end - begin);

	drawOctreeNode(octree.root, 1);


	printf("oct_hiden_num = %d hiden_num = %d\n", oct_hiden_num, hiden_num);
	octree.OctreeDestroy();		// ���ٰ˲���
}
