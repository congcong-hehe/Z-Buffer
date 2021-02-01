#pragma once

#include "math/AABB.h"
#include <vector>
#include "TriMesh.h"
#include "Color.h"

struct TriInfo
{
    Vec3f vec3f[3];     // 所绘制三角形的位置信息
    Color color;    // 三角形的颜色
};

struct OctreeNode
{
    AABB aabb;
    std::vector<TriInfo> tris;    // 存储在节点中的三角形的信息

    // r--right l--left t--top d--down n--near f--far
    // 0-rtn 1-ltn 2-ldn 3-rdn 4-rtf 5-ltf 6-ldf  7-rdf
    OctreeNode* nodes[8];

    OctreeNode()
    {
        for (int i = 0; i < 8; ++i)
        {
            nodes[i] = nullptr;
        }
    }

    ~OctreeNode()
    {
        for (int i = 0; i < 8; ++i)
        {
            if (nodes[i] != nullptr)
            {
                delete nodes[i];
            }
        }
    }
};

class Octree
{
public:
    OctreeNode* root{ nullptr };

    Octree();
    ~Octree();

    void OctreeCreate(std::vector<TriMesh*> triMeshs);
    void OctreeDestroy();

};
