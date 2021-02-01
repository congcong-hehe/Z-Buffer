#include "BaseZbuffer.h"

BaseZbuffer::BaseZbuffer(int w, int h, unsigned char* fb, Color back_color) : Painter(w, h, fb, back_color)
{
	zbuffer_ = (float*)malloc(w * h * sizeof(float));
}

BaseZbuffer::~BaseZbuffer()
{
	free(zbuffer_);
}

void BaseZbuffer::clearZbuffer()
{
	for (int i = 0; i < width_ * height_; ++i)
	{
		zbuffer_[i] = -FLT_MAX;
	}
}

void BaseZbuffer::drawTriangle(Vec2i* vis, float* Z, Color color)
{
	// 按照y坐标大小对三角形顶点排序
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

	int total_height = vis[2].y - vis[0].y + 1;		// 可能为0，所以要+1
	// 绘制从vis[0] 到 vis[1]的部分
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
			if (z > zbuffer_[x + y * width_])
			{
				zbuffer_[x + y * width_] = z;
				drawPixel(x, y, color);
			}
			z += dz;
		}
	}

	// 绘制从vis[1] 到 vis[2]的部分
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
			if (z > zbuffer_[x + y * width_])
			{
				zbuffer_[x + y * width_] = z;
				drawPixel(x, y, color);
			}
			z += dz;
		}
	}
}


void BaseZbuffer::drawMesh(std::vector<TriMesh*>& triMeshs)
{
	clearZbuffer();
	for (TriMesh* triMesh : triMeshs)
	{
		Vec3f light(0, 0, 1);

		for (int i = 0; i < triMesh->num_tris_; ++i)
		{
			unsigned int* vids = triMesh->tris_[i].vid;	// 三角形中的顶点序号
			Vec3f vec3fs[3];	// 空间中的坐标
			Vec2i vec2is[3];	// 屏幕中的坐标
			float Z[3];

			for (int j = 0; j < 3; ++j)
			{
				vec3fs[j] = triMesh->vtxs_[vids[j]].pos;
				vec2is[j] = Vec2i((vec3fs[j].x + 1.0f) * width_ / 2.0f, height_ - ((vec3fs[j].y + 1.0f) * height_ / 2.0f));
				Z[j] = vec3fs[j].z;
			}

			Vec3f n = (vec3fs[1] - vec3fs[0]).cross(vec3fs[2] - vec3fs[0]);		// 三角形是逆时针
			n.normalization();
			float intensity = n.dot(light);

			if (intensity > 0)
			{
				drawTriangle(vec2is, Z, Color(255 * intensity, 255 * intensity, 255 * intensity));
			}
		}
	}
}
