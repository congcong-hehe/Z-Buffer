#pragma once
#include <math.h>

struct Vec2i
{
	int x, y;

	Vec2i(int _x = 0, int _y = 0) : x(_x), y(_y) {}

	Vec2i operator + (const Vec2i& v)
	{
		return Vec2i(x + v.x, y + v.y);
	}

	Vec2i operator - (const Vec2i& v)
	{
		return Vec2i(x - v.x, x - v.y);
	}

	Vec2i operator * (const float a)
	{
		return Vec2i(x * a, y * a);
	}
};

struct Vec2i1f
{
	int x, y;	
	float z;

	Vec2i1f(int _x = 0, int _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}

	Vec2i1f operator + (const Vec2i1f& v)
	{
		return Vec2i1f(x + v.x, y + v.y, z + v.z);
	}

	Vec2i1f operator - (const Vec2i1f& v)
	{
		return Vec2i1f(x - v.x, y - v.y, z - v.z);
	}

	Vec2i1f operator * (const int a)
	{
		return Vec2i1f(x * a, y * a, z * a);
	}
};

struct Vec3f
{
	float x, y, z;

	Vec3f(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}

	Vec3f operator + (const Vec3f& v)
	{
		return Vec3f(x + v.x, y + v.y, z + v.z);
	}

	Vec3f operator - (const Vec3f& v)
	{
		return Vec3f(x - v.x, y - v.y, z - v.z);
	}

	Vec3f operator / (const float a)
	{
		return Vec3f(x / a, y / a, z / a);
	}

	Vec3f operator * (const float a)
	{
		return Vec3f(x * a, y * a, z * a);
	}

	float norm()
	{
		return (float)sqrt(x * x + y * y + z * z);
	}

	Vec3f &normalization()
	{
		*this = (*this) / norm();
		return *this;
	}

	float dot(const Vec3f& v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3f cross(const Vec3f& v)
	{
		return Vec3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};
