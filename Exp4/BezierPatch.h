#pragma once
#include"Mesh.h"
#include"P3.h"

class CBezierPatch
{
public:
	CBezierPatch(void);
	virtual ~CBezierPatch(void);
	void ReadControlPoint(CP3 CtrPt[4][4], int ReNumber);//读入16个控制点和递归深度
	void DrawCurvedPatch(CDC* pDC);//绘制曲面
	void DrawControlGrid(CDC* pDC);//绘制控制网格
private:
	void Recursion(CDC* pDC, int ReNumber, CMesh Mesh);//递归函数
	void Tessellation(CMesh Mesh);//细分函数
	void DrawFacet(CDC* pDC);//绘制四边形网格
	void LeftMultiplyMatrix(double M[4][4], CP3 P[4][4]);//左乘顶点矩阵
	void RightMultiplyMatrix(CP3 P[4][4], double M[4][4]);//右乘顶点矩阵
	void TransposeMatrix(double M[4][4]);//转置矩阵
private:
	int ReNumber;//递归深度
	CP3 quadrP[4];//四边形网格点
	CP3 CtrPt[4][4];//曲面的16个控制点
};
