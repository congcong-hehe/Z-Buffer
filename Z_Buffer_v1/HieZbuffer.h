#pragma once
#include "Painter.h"
// 不带八叉树的层次z-buffer算法
class HieZbuffer :
    public Painter
{
public:
    std::vector<float*> zbuffers_;  //存储每一层的zbuffer
    std::vector<std::pair<int, int>> zbuffer_len_;  // 每一层z-buffer的宽度
    int zbuffer_height_{ 0 };  
    int hiden_num{ 0 };  // 剔除的面数

    HieZbuffer(int w, int h, unsigned char* fb, Color back_color = Color(0, 0, 0));
    ~HieZbuffer();

    virtual void clearZbuffer() override;
    virtual void drawMesh(std::vector<TriMesh*>& triMeshs) override;
    void drawPixel(int x, int y, Color color);
    void drawTriangle(Vec2i* vis, float* Z, Color color);
};

