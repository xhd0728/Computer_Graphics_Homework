#pragma once
#include"T2.h"
class CMesh
{
public:
	CMesh(void);
	virtual ~CMesh(void);
public:
	CT2 BL, BR, TR, TL;//四边形的4个角点坐标
};
