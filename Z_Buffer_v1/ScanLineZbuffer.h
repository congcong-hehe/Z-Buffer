#pragma once
#include "Painter.h"

struct TriTableNode;
struct EdgeTableNode;
struct EdgeDynamicNode;

// ɨ����z-buffer�㷨
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
    int x;      // ���϶˵��x����
    float dx;   // ��������ɨ���߽����x�����(-1/k) k��б��
    int dy;     // �߿�Խ��ɨ������Ŀ
    float z;    // ���϶˵�����ֵ
};

struct TriTableNode
{
    float a, b, c, d;   // �����ƽ��ax + by + cz + d = 0
    int id;     // ����ε����
    int dy;     // ����ο�Խ��ɨ������Ŀ
    Color back_color;   // ����ε���ɫ
    EdgeTableNode *edgetable;     // ����εı�
};

struct EdgeDynamicNode
{
    float xl;     // �󽻵��x����
    float dxl;      // �󽻵����������ɨ���߽����x����֮��
    int dyl;    // ��ʣ���Խ��ɨ������Ŀ

    float xr;     // �ҽ����x����
    float dxr;  
    int dyr;

    float zl;   // �󽻵㴦���������ƽ������ֵ
    float dzx;  // ��ɨ������������һ������ʱ�� ������� dzx = - a / c
    float dzy;  // ��y���������ƹ�һ��ɨ����ʱ��������� dzy = b / c
    Color back_color;   // ������ɫ

    TriTableNode* triNode;  // ָ����������ε�ָ��
};