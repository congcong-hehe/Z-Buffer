#include "Window.h"
#include <iostream>
#include "Painter.h"
#include "TriMesh.h"
#include "BaseZbuffer.h"
#include "HieZbuffer.h"
#include "ScanLineZbuffer.h"
#include "OctreeZbuffer.h"
#include <time.h>

int main()
{
	clock_t begin, end;

	int width = 512, height = 512;
	unsigned char* framebuffer = NULL;
	framebuffer = (unsigned char*)malloc(width * height * 4);

	windowInit(width, height);

	// BaseZbuffer	����z-buffer�㷨
	// HieZbuffer ���z-buffer�㷨
	// OctreeZbuffer ���z-buffer�㷨 + �˲��������ָ�
	// ScanLineZbuffer ɨ����z-buffer�㷨
	Painter *painter = new OctreeZbuffer(width, height, framebuffer, Color(0, 0, 0));

	std::vector<TriMesh* > triMeshs;
	TriMesh* triMesh0 = new TriMesh;
	triMesh0->LoadFile("../model/miku.obj");
	triMesh0->normalization();	// ��һ��
	//triMesh0->scale(0.5f);
	triMesh0->translate(Vec3f(0.0f, 0.0f, -0.5f));
	triMeshs.push_back(triMesh0);

	TriMesh* triMesh1 = new TriMesh;
	triMesh1->LoadFile("../model/helmet.obj");
	triMesh1->scale(0.5f);
	triMesh1->translate(Vec3f(0.0f, 0.0f, 0.5f));
	triMeshs.push_back(triMesh1);

	while (my_window->window_close_ == false)
	{
		painter->clearFramebuffer();

		begin = clock();
		painter->drawMesh(triMeshs);
		end = clock();

		std::cout << "Render cost time : " << end - begin << "ms" << std::endl;
		
		windowDraw(framebuffer);
		Sleep(0);
	}
	windowClose();
	free(framebuffer);

	for (int i = 0; i < triMeshs.size(); ++i)
	{
		delete triMeshs[i];
	}

	return 0;
}