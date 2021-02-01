#include "Painter.h"
#include <algorithm>
#include <limits.h>

Painter::Painter(int w, int h, unsigned char* fb, Color back_color) : width_(w), height_(h), framebuffer_(fb)
{
	back_color_ = back_color;
}

Painter::~Painter()
{
	
}

void Painter::clearFramebuffer()
{
	for (int i = 0; i < height_; ++i)
	{
		for (int j = 0; j < width_; ++j)
		{
			framebuffer_[(i * width_ + j) * 4 + 0] = back_color_.b;	// B
			framebuffer_[(i * width_ + j) * 4 + 1] = back_color_.g;	// G
			framebuffer_[(i * width_ + j) * 4 + 2] = back_color_.r;	// R
			framebuffer_[(i * width_ + j) * 4 + 3] = 255;		
		}
	}
}

inline void Painter::drawPixel(int x, int y, Color color)
{
	if (x < 0 || x >= width_ || y < 0 || y >= height_) return;	

	framebuffer_[(y * width_ + x) * 4 + 0] = color.b;	// B
	framebuffer_[(y * width_ + x) * 4 + 1] = color.g;	// G
	framebuffer_[(y * width_ + x) * 4 + 2] = color.r;	// R
	framebuffer_[(y * width_ + x) * 4 + 3] = 255;
}


// Bresenham 画线
void Painter::drawLine(int x0, int y0, int x1, int y1, Color color)
{
	bool steep = false;

	// 若斜率>1，选择y单增
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	
	// 保证从左到右
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror = std::abs(dy) * 2;
	int error = 0;

	for (int x = x0, y = y0; x <= x1; x++)
	{
		if (steep)
		{
			drawPixel(y, x, color);
		}
		else
		{
			drawPixel(x, y, color);
		}

		error += derror;
		if (error > dx)
		{
			y += (y1 > y0 ? 1 : -1);
			error -= dx * 2;
		}
	}
}

// 计算一个点在三角形中的重心坐标
Vec3f barycentric(Vec2i* vis, Vec2i p)
{
	Vec3f v1 = Vec3f(vis[2].x - vis[0].x, vis[1].x - vis[0].x, vis[0].x - p.x);
	Vec3f v2 = Vec3f(vis[2].y - vis[0].y, vis[1].y - vis[0].y, vis[0].y - p.y);

	Vec3f u = v1.cross(v2);
	if (std::abs(u.z) < 1)
	{
		return Vec3f(-1, 1, 1);
	}
	return Vec3f(1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void Painter::drawTriangleBC(Vec2i* vis, Color color)
{
	int x_min = width_ - 1, x_max = 0, y_min = height_ - 1, y_max = 0;

	// 计算包围盒
	for (int i = 0; i < 3; ++i)
	{
		x_max = std::max(vis[i].x, x_max);
		x_min = std::min(vis[i].x, x_min);
		y_max = std::max(vis[i].y, y_max);
		y_min = std::min(vis[i].y, y_min);
	}
	// 防止越界
	x_min = std::max(0, x_min);
	x_max = std::min(width_ - 1, x_max);
	y_min = std::max(0, y_min);
	y_max = std::min(height_ - 1, y_max);

	// 判断点是否在三角形内，是则绘制
	for (int x = x_min; x <= x_max; ++x)
	{
		for (int y = y_min; y <= y_max; ++y)
		{
			Vec3f bc = barycentric(vis, Vec2i(x, y));
			if (bc.x < 0 || bc.y < 0 || bc.z < 0)
			{
				continue;
			}
			drawPixel(x, y, color);
		}
	}
}