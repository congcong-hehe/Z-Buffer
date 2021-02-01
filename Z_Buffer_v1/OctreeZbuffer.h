#pragma once
#include "Painter.h"
#include "Octree.h"

// 带八叉树的层次zbuffer算法
class OctreeZbuffer :
    public Painter
{
public:
    std::vector<float*> zbuffers_;
    std::vector<std::pair<int, int>> zbuffer_len_;
    int zbuffer_height_{ 0 };
    int hiden_num{ 0 };  // 画面剔除的面数
    int oct_hiden_num{ 0 }; // 通过八叉树剔除的面数
    Octree octree;

    OctreeZbuffer(int w, int h, unsigned char* fb, Color back_color = Color(0, 0, 0));
    ~OctreeZbuffer();

    virtual void clearZbuffer() override;
    virtual void drawMesh(std::vector<TriMesh*>& triMeshs) override;
    void drawPixel(int x, int y, Color color);
    void drawTriangle(Vec2i* vis, float* Z, Color color);
    void drawOctreeNode(OctreeNode* current_node, int depth);
};

