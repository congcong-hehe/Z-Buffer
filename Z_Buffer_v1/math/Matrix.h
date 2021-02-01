#pragma once

struct Matrix4
{
	float mat[4][4];

	Matrix4()
	{
		mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0; mat[0][3] = 0;
		mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0; mat[1][3] = 0;
		mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1; mat[2][3] = 0;
		mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;
	}

	// 两个矩阵相加
	Matrix4 operator + (const Matrix4& m)
	{
		Matrix4 ans;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				ans.mat[i][j] = mat[i][j] + m.mat[i][j];
			}
		}
		return ans;
	}

	// 两个矩阵相减
	Matrix4 operator - (const Matrix4& m)
	{
		Matrix4 ans;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				ans.mat[i][j] = mat[i][j] - m.mat[i][j];
			}
		}
		return ans;
	}

	Matrix4 operator * (const Matrix4& m)
	{
		Matrix4 ans;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				ans.mat[i][j] = mat[i][0] * m.mat[0][j] + mat[i][1] * m.mat[1][j] 
					+ mat[i][2] * m.mat[2][j] + mat[i][3] * m.mat[3][j];
			}
		}
		return ans;
	}
};
