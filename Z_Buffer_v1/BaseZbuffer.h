#pragma once
#include "Painter.h"

// »ù±¾z-bufferËã·¨
class BaseZbuffer :
    public Painter
{
public:
    float* zbuffer_{ nullptr };

    BaseZbuffer(int w, int h, unsigned char* fb, Color back_color = Color(0, 0, 0));
    ~BaseZbuffer();

    void drawTriangle(Vec2i* vis, float* Z, Color color);
    virtual void drawMesh(std::vector<TriMesh*>& triMeshs) override;
    virtual void clearZbuffer() override;
};

