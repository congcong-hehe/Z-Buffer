#include "ScanLineZbuffer.h"
#include <list>
#include <time.h>

ScanLineZbuffer::ScanLineZbuffer(int w, int h, unsigned char* fb, Color back_color) : Painter(w ,h, fb, back_color)
{
	zbuffer_ = (float*)malloc(w * sizeof(float));
	tri_table_.resize(h);
}
ScanLineZbuffer::~ScanLineZbuffer()
{
	free(zbuffer_);
	for (int i = 0; i < height_; ++i)
	{
		for (int j = 0; j < tri_table_[i].size(); ++i)
		{
			delete[]tri_table_[i][j].edgetable;
		}
	}
}

void ScanLineZbuffer::clearZbuffer()
{
	for (int i = 0; i < width_; ++i)
	{
		zbuffer_[i] = -FLT_MAX;
	}
}

// ��������α�ͱ߱�
void ScanLineZbuffer::ContructTriAndEdgeTable(std::vector<TriMesh*>& triMeshs)
{
	tri_table_.clear();
	tri_table_.resize(height_);

	Vec3f light(0, 0, 1);

	for (TriMesh* triMesh : triMeshs)
	{
		for (int i = 0; i < triMesh->num_tris_; ++i)
		{
			unsigned int* vids = triMesh->tris_[i].vid;	// �������еĶ������
			Vec3f vec3fs[3];	// �ռ��е�����
			Vec2i1f vec2i1fs[3];	// ��Ļ�е�����

			for (int j = 0; j < 3; ++j)
			{
				vec3fs[j] = triMesh->vtxs_[vids[j]].pos;
				vec2i1fs[j] = Vec2i1f((vec3fs[j].x + 1.0f) * width_ / 2.0f, int(height_ - ((vec3fs[j].y + 1.0f) * height_ / 2.0f)),
					(vec3fs[j].z + 1.0f) * width_ / 2.0f);		// zֵ�����x��y����ͬ���ı任�������ſ��Ժ�����ȷ����z������
			}

			Vec3f n = (vec3fs[1] - vec3fs[0]).cross(vec3fs[2] - vec3fs[0]);		// ����������ʱ��
			n.normalization();

			float intensity = n.dot(light);
			if (intensity > 0)		// �����޳�
			{
				// ����y�����С�������ζ�����������
				if (vec2i1fs[0].y > vec2i1fs[1].y) std::swap(vec2i1fs[0], vec2i1fs[1]);
				if (vec2i1fs[0].y > vec2i1fs[2].y) std::swap(vec2i1fs[0], vec2i1fs[2]);
				if (vec2i1fs[1].y > vec2i1fs[2].y) std::swap(vec2i1fs[1], vec2i1fs[2]);

				// �����������α�
				TriTableNode triTableNode;
				triTableNode.a = n.x;
				triTableNode.b = n.y;
				triTableNode.c = n.z;

				triTableNode.d = -n.dot(vec3fs[0]);
				triTableNode.id = i;
				triTableNode.back_color = Color(255 * intensity, 255 * intensity, 255 * intensity);
				int ymax = vec2i1fs[2].y;
				triTableNode.dy = ymax - vec2i1fs[0].y;

				// ��������߱�
				triTableNode.edgetable = new EdgeTableNode[3];
				EdgeTableNode edgeTableNode20;
				edgeTableNode20.x = vec2i1fs[2].x;
				edgeTableNode20.z = vec2i1fs[2].z;
				edgeTableNode20.dy = vec2i1fs[2].y - vec2i1fs[0].y;
				edgeTableNode20.dx = -(float)(vec2i1fs[2].x - vec2i1fs[0].x) / edgeTableNode20.dy;
				triTableNode.edgetable[0] = edgeTableNode20;
				EdgeTableNode edgeTableNode21;
				edgeTableNode21.x = vec2i1fs[2].x;
				edgeTableNode21.z = vec2i1fs[2].z;
				edgeTableNode21.dy = vec2i1fs[2].y - vec2i1fs[1].y;
				edgeTableNode21.dx = -(float)(vec2i1fs[2].x - vec2i1fs[1].x) / edgeTableNode21.dy;
				triTableNode.edgetable[1] = edgeTableNode21;
				EdgeTableNode edgeTableNode10;
				edgeTableNode10.x = vec2i1fs[1].x;
				edgeTableNode10.z = vec2i1fs[1].z;
				edgeTableNode10.dy = vec2i1fs[1].y - vec2i1fs[0].y;
				edgeTableNode10.dx = -(float)(vec2i1fs[1].x - vec2i1fs[0].x) / edgeTableNode10.dy;
				triTableNode.edgetable[2] = edgeTableNode10;

				tri_table_[ymax].push_back(triTableNode);
			}
		}
	}
}

void ScanLineZbuffer::drawMesh(std::vector<TriMesh*>& triMeshs)
{
	clock_t begin = clock();
	ContructTriAndEdgeTable(triMeshs);	// �����������α�ͷ���߱�
	clock_t end = clock();
	printf("build tritable cost %d ms\n", end - begin);

	std::list<EdgeDynamicNode> dynamic_edgeTable;		//	��߱����ͶӰ�������ɨ�����ཻ�ı߽�

	for (int y = height_ - 1; y >= 0; --y) // ��������ɨ��
	{
		clearZbuffer();		// ÿһ�ж�Ҫ����zbuffer

		for (int i = 0; i < tri_table_[y].size(); ++i)
		{
			// �µĶ���μ�������α�
			TriTableNode* triNode = &tri_table_[y][i];

			// �¼���Ķ���εı߼���
			EdgeDynamicNode edgeNode;
			edgeNode.back_color = triNode->back_color;
			edgeNode.dzx = -triNode->a / triNode->c;		// c����Ϊ0?
			edgeNode.dzy = triNode->b / triNode->c;
			if (triNode->edgetable[1].dx > triNode->edgetable[0].dx)
			{
				edgeNode.xl = triNode->edgetable[0].x;
				edgeNode.dxl = triNode->edgetable[0].dx;
				edgeNode.dyl = triNode->edgetable[0].dy;

				edgeNode.xr = triNode->edgetable[1].x;
				edgeNode.dxr = triNode->edgetable[1].dx;
				edgeNode.dyr = triNode->edgetable[1].dy;

				edgeNode.zl = triNode->edgetable[0].z;
			}
			else
			{
				edgeNode.xl = triNode->edgetable[1].x;
				edgeNode.dxl = triNode->edgetable[1].dx;
				edgeNode.dyl = triNode->edgetable[1].dy;

				edgeNode.xr = triNode->edgetable[0].x;
				edgeNode.dxr = triNode->edgetable[0].dx;
				edgeNode.dyr = triNode->edgetable[0].dy;

				edgeNode.zl = triNode->edgetable[1].z;
			}

			edgeNode.triNode = triNode;
			dynamic_edgeTable.push_back(edgeNode);
		}

		// ���ݻ�߱����
		for (auto itr = dynamic_edgeTable.begin(); itr != dynamic_edgeTable.end(); ++itr)
		{
			float z = itr->zl;

  			for (int x = (int(itr->xl) > 0 ? int(itr->xl) : 0); x <= (int(itr->xr)-1 < width_ - 1 ? (itr->xr)-1 : width_ - 1); ++x, z += itr->dzx)
			{
				if (z > zbuffer_[x])
				{
					drawPixel(x, y, itr->back_color);
					zbuffer_[x] = z;
				}
			}
		}

		// ��߱���Ԫ�ص��޸�
		for (auto itr = dynamic_edgeTable.begin(); itr != dynamic_edgeTable.end();)
		{
			itr->dyl--;
			itr->dyr--;

			// ��������߶���ʣɨ����
			if (itr->dyl < 0 && itr->dyr < 0)
			{
				itr = dynamic_edgeTable.erase(itr);
			}
			else
			{
				// �����һ���߲�ʣ��ɨ���ߣ����滻
				TriTableNode* triNode = itr->triNode;

				if (itr->dyl < 0)
				{
					itr->xl = triNode->edgetable[2].x;
					itr->dxl = triNode->edgetable[2].dx;
					itr->dyl = triNode->edgetable[2].dy - 1;
				}
				if (itr->dyr < 0)
				{
					itr->xr = triNode->edgetable[2].x;
					itr->dxr = triNode->edgetable[2].dx;
					itr->dyr = triNode->edgetable[2].dy - 1;
				}

				itr->xl += itr->dxl;
				itr->xr += itr->dxr;
				itr->zl += itr->dzy + itr->dzx * itr->dxl;

				itr++;
			}

		}
	}
	
}
