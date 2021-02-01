#pragma once

struct Color
{
	char r{ 0 };
	char g{ 0 };
	char b{ 0 };

	Color() {}
	Color(int _r, int _g, int _b) : r((char)_r), g((char)_g), b((char)_b) {}
};
