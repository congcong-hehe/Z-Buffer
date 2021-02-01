#pragma once
#include "Vec.h"
#include <iostream>
#include <limits.h>
// axis-aligned bounding box
struct AABB
{
	float x_min, x_max, y_min, y_max, z_min, z_max;

	AABB(float _x_min, float _x_max, float _y_ymin, float _y_max, float _z_min, float _z_max) :
		x_min(_x_min), x_max(_x_max), y_min(_x_min), y_max(_y_max), z_min(_z_min), z_max(_z_max) {}

	AABB() : x_min(FLT_MAX), x_max(-FLT_MAX), y_min(FLT_MAX), y_max(-FLT_MAX), z_min(FLT_MAX), z_max(-FLT_MAX) {}

	AABB(Vec3f vec_min, Vec3f vec_max)
	{
		x_min = vec_min.x;
		x_max = vec_max.x;
		y_min = vec_min.y;
		y_max = vec_max.y;
		z_min = vec_min.z;
		z_max = vec_max.z;
	}

	bool containTri(Vec3f* vec3fs)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (vec3fs[i].x > x_max || vec3fs[i].x <= x_min)
			{
				return false;
			}
			if (vec3fs[i].y > y_max || vec3fs[i].y <= y_min)
			{
				return false;
			}
			if (vec3fs[i].z > z_max || vec3fs[i].z <= z_min)
			{
				return false;
			}
		}
		return true;
	}
};
