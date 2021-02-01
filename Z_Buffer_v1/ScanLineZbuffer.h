#pragma once
#include "Painter.h"

struct TriTableNode;
struct EdgeTableNode;
struct EdgeDynamicNode;

// 扫描线z-buffer算法
class ScanLineZbuffer :
    public Painter
{
public:
    float* zbuffer_;
    std::vector<std::vector<TriTableNode>> tri_table_;

    ScanLineZbuffer(int w, int h, unsigned char* fb, Color back_color = Color(0, 0, 0));
    ~ScanLineZbuffer();


    virtual void drawMesh(std::vector<TriMesh*>& triMeshs) override;
    virtual void clearZbuffer() override;

    void ContructTriAndEdgeTable(std::vector<TriMesh*>& triMeshs);
};

struct EdgeTableNode
{
    int x;      // 边上端点的x坐标
    float dx;   // 相邻两条扫描线交点的x坐标差(-1/k) k是斜率
    int dy;     // 边跨越的扫描线数目
    float z;    // 边上端点的深度值
};

struct TriTableNode
{
    float a, b, c, d;   // 多边形平面ax + by + cz + d = 0
    int id;     // 多边形的序号
    int dy;     // 多边形跨越的扫描线数目
    Color back_color;   // 多边形的颜色
    EdgeTableNode *edgetable;     // 多边形的边
};

struct EdgeDynamicNode
{
    float xl;     // 左交点的x坐标
    float dxl;      // 左交点边上两相邻扫描线交点的x坐标之差
    int dyl;    // 边剩余跨越的扫描线数目

    float xr;     // 右交点的x坐标
    float dxr;  
    int dyr;

    float zl;   // 左交点处多边形所在平面的深度值
    float dzx;  // 沿扫描线向右增加一个像素时， 深度增量 dzx = - a / c
    float dzy;  // 沿y方向向下移过一根扫描线时，深度增量 dzy = b / c
    Color back_color;   // 背景颜色

    TriTableNode* triNode;  // 指向所属多边形的指针
};