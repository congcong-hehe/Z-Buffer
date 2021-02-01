#pragma once
#include "Painter.h"
// �����˲����Ĳ��z-buffer�㷨
class HieZbuffer :
    public Painter
{
public:
    std::vector<float*> zbuffers_;  //�洢ÿһ���zbuffer
    std::vector<std::pair<int, int>> zbuffer_len_;  // ÿһ��z-buffer�Ŀ��
    int zbuffer_height_{ 0 };  
    int hiden_num{ 0 };  // �޳�������

    HieZbuffer(int w, int h, unsigned char* fb, Color back_color = Color(0, 0, 0));
    ~HieZbuffer();

    virtual void clearZbuffer() override;
    virtual void drawMesh(std::vector<TriMesh*>& triMeshs) override;
    void drawPixel(int x, int y, Color color);
    void drawTriangle(Vec2i* vis, float* Z, Color color);
};

