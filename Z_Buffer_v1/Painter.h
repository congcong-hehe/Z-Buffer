#pragma once
#include "Color.h"

// 填充framebuffer
#include <stdlib.h>
#include "TriMesh.h"

class Painter
{
public:
	int width_{ 800 };
	int height_{ 600 };
	unsigned char* framebuffer_{ nullptr };
	Color back_color_;

	Painter(int w, int h, unsigned char* fb, Color back_color);
	~Painter();
	void clearFramebuffer();

	inline void drawPixel(int x, int y, Color color);
	void drawLine(int x0, int y0, int x1, int y1, Color color);		// 网格线框
	void drawTriangleBC(Vec2i* vis, Color color);		// 利用重心坐标判断是否在三角形内
	virtual void drawMesh(std::vector<TriMesh *>& triMeshs) = 0;
	virtual void clearZbuffer() = 0;
};

