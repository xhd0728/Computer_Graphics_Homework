#pragma once
#include "Patch.h"
#include"BezierPatch.h"

class CRevolution //回转类
{
public:
	CRevolution(void);
	virtual ~CRevolution(void);
	void ReadCubicBezierControlPoint(CP3 P[4]);//曲线顶点初始化
	CP3* GetVertexArrayName(void);//得到顶点数组名
	void DrawRevolutionSurface(CDC* pDC);//绘制回转体
private:
	void ReadVertex(void);//读入回转体控制多边形顶点
	void ReadPatch(void);//读入回转体双三次曲面片	
private:
	CP3 P[4];//来自曲线的4个三维控制点
	CP3 V[48];//回转体总顶点数组(4个面,共48个点)
	CPatch S[4];//回转体总曲面数组（一圈4个面）
};