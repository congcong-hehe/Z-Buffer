#include "Octree.h"

Octree::Octree()
{

}

Octree::~Octree()
{
	OctreeDestroy();
}

void Octree::OctreeCreate(std::vector<TriMesh*> triMeshs)
{
	root = new OctreeNode;
	root->aabb = AABB(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	for (TriMesh* triMesh : triMeshs)
	{
		// ���������������˲���
		Vec3f light(0, 0, 1);
		for (int i = 0; i < triMesh->num_tris_; ++i)
		{

			unsigned int* vids = triMesh->tris_[i].vid;	// �������еĶ������
			Vec3f vec3fs[3];	// �ռ��е�����

			for (int j = 0; j < 3; ++j)
			{
				vec3fs[j] = triMesh->vtxs_[vids[j]].pos;
			}

			Vec3f n = (vec3fs[1] - vec3fs[0]).cross(vec3fs[2] - vec3fs[0]);		// ����������ʱ��
			n.normalization();

			float intensity = n.dot(light);
			if (intensity > 0)		// �����޳�
			{
				OctreeNode* current_node = root;
				int depth = 10;
				while (depth-- > 0)
				{
					if (depth == 0)
					{
						TriInfo triInfo;
						for (int i = 0; i < 3; ++i)
						{
							triInfo.vec3f[i] = vec3fs[i];
						}
						triInfo.color = Color(255 * intensity, 255 * intensity, 255 * intensity);
						current_node->tris.push_back(triInfo);
						break;
					}

					// �ָ������ڵ�Ϊ�˸�С�飬����С���У���ǰ�ڵ�ת��ΪС�飬����ֱ�Ӵ��뵱ǰ�ڵ�
					AABB& aabb = current_node->aabb;
					Vec3f center = Vec3f((aabb.x_max + aabb.x_min) / 2.0f, (aabb.y_max + aabb.y_min) / 2.0f, (aabb.z_max + aabb.z_min) / 2.0f);	// ���ӵ����ĵ�
					float r = (aabb.x_max - aabb.x_min) / 2.0f;		// ���ӵİ뾶

					AABB sub_aabb;

					sub_aabb = AABB(center,center + Vec3f(r, r, r));
					if (sub_aabb.containTri(vec3fs))
					{
						if (current_node->nodes[0] == nullptr)
						{
							current_node->nodes[0] = new OctreeNode;
							current_node->nodes[0]->aabb = sub_aabb;
						}
						current_node = current_node->nodes[0];
						continue;
					}

					sub_aabb = AABB(center + Vec3f(-r, 0, 0), center + Vec3f(0, r, r));
					if (sub_aabb.containTri(vec3fs))
					{
						if (current_node->nodes[1] == nullptr)
						{
							current_node->nodes[1] = new OctreeNode;
							current_node->nodes[1]->aabb = sub_aabb;
						}
						current_node = current_node->nodes[1];
						continue;
					}

					sub_aabb = AABB(center + Vec3f(-r, -r, 0), center + Vec3f(0, 0, r));
					if (sub_aabb.containTri(vec3fs))
					{
						if (current_node->nodes[2] == nullptr)
						{
							current_node->nodes[2] = new OctreeNode;
							current_node->nodes[2]->aabb = sub_aabb;
						}
						current_node = current_node->nodes[2];
						continue;
					}

					sub_aabb = AABB(center + Vec3f(0, -r, 0), center + Vec3f(r, 0, r));
					if (sub_aabb.containTri(vec3fs))
					{
						if (current_node->nodes[3] == nullptr)
						{
							current_node->nodes[3] = new OctreeNode;
							current_node->nodes[3]->aabb = sub_aabb;
						}
						current_node = current_node->nodes[3];
						continue;
					}

					sub_aabb = AABB(center + Vec3f(0, 0, -r), center + Vec3f(r, r, 0));
					if (sub_aabb.containTri(vec3fs))
					{
						if (current_node->nodes[4] == nullptr)
						{
							current_node->nodes[4] = new OctreeNode;
							current_node->nodes[4]->aabb = sub_aabb;
						}
						current_node = current_node->nodes[4];
						continue;
					}

					sub_aabb = AABB(center + Vec3f(-r, 0, -r), center + Vec3f(0, r, 0));
					if (sub_aabb.containTri(vec3fs))
					{
						if (current_node->nodes[5] == nullptr)
						{
							current_node->nodes[5] = new OctreeNode;
							current_node->nodes[5]->aabb = sub_aabb;
						}
						current_node = current_node->nodes[5];
						continue;
					}

					sub_aabb = AABB(center + Vec3f(-r, -r, -r), center + Vec3f(0, 0, 0));
					if (sub_aabb.containTri(vec3fs))
					{
						if (current_node->nodes[6] == nullptr)
						{
							current_node->nodes[6] = new OctreeNode;
							current_node->nodes[6]->aabb = sub_aabb;
						}
						current_node = current_node->nodes[6];
						continue;
					}

					sub_aabb = AABB(center + Vec3f(0, -r, -r), center + Vec3f(r, 0, 0));
					if (sub_aabb.containTri(vec3fs))
					{
						if (current_node->nodes[7] == nullptr)
						{
							current_node->nodes[7] = new OctreeNode;
							current_node->nodes[7]->aabb = sub_aabb;
						}
						current_node = current_node->nodes[7];
						continue;
					}

					// ������ڷ��ѵ�С�����У���ô���뵱ǰ���ӣ�������ѭ��
					TriInfo triInfo;
					for (int i = 0; i < 3; ++i)
					{
						triInfo.vec3f[i] = vec3fs[i];
					}
					triInfo.color = Color(255 * intensity, 255 * intensity, 255 * intensity);
					current_node->tris.push_back(triInfo);
					break;
				}
			}
		}
	}
}

void Octree::OctreeDestroy()
{
	if (root != nullptr)
	{
		delete root;
		root = nullptr;
	}
}