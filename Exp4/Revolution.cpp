#include "pch.h"
#include "Revolution.h"

CRevolution::CRevolution(void)
{
}

CRevolution::~CRevolution(void)
{
}

void CRevolution::ReadCubicBezierControlPoint(CP3 P[4])//三次Bezier曲线4个控制点初始化
{
	for (int i = 0; i < 4; i++)
		this->P[i] = P[i];
	//读入回转体的数据结构
	ReadVertex();
	ReadPatch();
}

CP3* CRevolution::GetVertexArrayName(void)//得到顶点数组名
{
	return	V;
}

void CRevolution::ReadVertex(void)//读入回转体的所有控制点
{
	const double m = 0.5523;//魔术常数
	//回转一圈需要4个面,第一个面16个点,第二个面12个点，第三个面12个点，第四个面8个点，共48个点
	//第一块面片yo
	V[0] = P[0];
	V[1] = P[1];
	V[2] = P[2];
	V[3] = P[3];
	V[4].x = V[0].x, V[4].y = V[0].y, V[4].z = -V[0].x * m;
	V[5].x = V[1].x, V[5].y = V[1].y, V[5].z = -V[1].x * m;
	V[6].x = V[2].x, V[6].y = V[2].y, V[6].z = -V[2].x * m;
	V[7].x = V[3].x, V[7].y = V[3].y, V[7].z = -V[3].x * m;
	V[8].x = V[0].x * m, V[8].y = V[0].y, V[8].z = -V[0].x;
	V[9].x = V[1].x * m, V[9].y = V[1].y, V[9].z = -V[1].x;
	V[10].x = V[2].x * m, V[10].y = V[2].y, V[10].z = -V[2].x;
	V[11].x = V[3].x * m, V[11].y = V[3].y, V[11].z = -V[3].x;
	V[12].x = V[0].z, V[12].y = V[0].y, V[12].z = -V[0].x;
	V[13].x = V[1].z, V[13].y = V[1].y, V[13].z = -V[1].x;
	V[14].x = V[2].z, V[14].y = V[2].y, V[14].z = -V[2].x;
	V[15].x = V[3].z, V[15].y = V[3].y, V[15].z = -V[3].x;
	//第二块面片
	V[16].x = -V[0].x * m, V[16].y = V[0].y, V[16].z = -V[0].x;
	V[17].x = -V[1].x * m, V[17].y = V[1].y, V[17].z = -V[1].x;
	V[18].x = -V[2].x * m, V[18].y = V[2].y, V[18].z = -V[2].x;
	V[19].x = -V[3].x * m, V[19].y = V[3].y, V[19].z = -V[3].x;
	V[20].x = -V[0].x, V[20].y = V[0].y, V[20].z = -V[0].x * m;
	V[21].x = -V[1].x, V[21].y = V[1].y, V[21].z = -V[1].x * m;
	V[22].x = -V[2].x, V[22].y = V[2].y, V[22].z = -V[2].x * m;
	V[23].x = -V[3].x, V[23].y = V[3].y, V[23].z = -V[3].x * m;
	V[24].x = -V[0].x, V[24].y = V[0].y, V[24].z = V[0].z;
	V[25].x = -V[1].x, V[25].y = V[1].y, V[25].z = V[1].z;
	V[26].x = -V[2].x, V[26].y = V[2].y, V[26].z = V[2].z;
	V[27].x = -V[3].x, V[27].y = V[3].y, V[27].z = V[3].z;
	//第三块面片
	V[28].x = -V[0].x, V[28].y = V[0].y, V[28].z = V[0].x * m;
	V[29].x = -V[1].x, V[29].y = V[1].y, V[29].z = V[1].x * m;
	V[30].x = -V[2].x, V[30].y = V[2].y, V[30].z = V[2].x * m;
	V[31].x = -V[3].x, V[31].y = V[3].y, V[31].z = V[3].x * m;
	V[32].x = -V[0].x * m, V[32].y = V[0].y, V[32].z = V[0].x;
	V[33].x = -V[1].x * m, V[33].y = V[1].y, V[33].z = V[1].x;
	V[34].x = -V[2].x * m, V[34].y = V[2].y, V[34].z = V[2].x;
	V[35].x = -V[3].x * m, V[35].y = V[3].y, V[35].z = V[3].x;
	V[36].x = V[0].z, V[36].y = V[0].y, V[36].z = V[0].x;
	V[37].x = V[1].z, V[37].y = V[1].y, V[37].z = V[1].x;
	V[38].x = V[2].z, V[38].y = V[2].y, V[38].z = V[2].x;
	V[39].x = V[3].z, V[39].y = V[3].y, V[39].z = V[3].x;
	//第四块面片
	V[40].x = V[0].x * m, V[40].y = V[0].y, V[40].z = V[0].x;
	V[41].x = V[1].x * m, V[41].y = V[1].y, V[41].z = V[1].x;
	V[42].x = V[2].x * m, V[42].y = V[2].y, V[42].z = V[2].x;
	V[43].x = V[3].x * m, V[43].y = V[3].y, V[43].z = V[3].x;
	V[44].x = V[0].x, V[44].y = V[0].y, V[44].z = V[0].x * m;
	V[45].x = V[1].x, V[45].y = V[1].y, V[45].z = V[1].x * m;
	V[46].x = V[2].x, V[46].y = V[2].y, V[46].z = V[2].x * m;
	V[47].x = V[3].x, V[47].y = V[3].y, V[47].z = V[3].x * m;
}

void CRevolution::ReadPatch(void)//曲面片表
{
	//第1卦限面片
	S[0].pIndex[0][0] = 0; S[0].pIndex[0][1] = 1; S[0].pIndex[0][2] = 2;  S[0].pIndex[0][3] = 3;
	S[0].pIndex[1][0] = 4; S[0].pIndex[1][1] = 5; S[0].pIndex[1][2] = 6;  S[0].pIndex[1][3] = 7;
	S[0].pIndex[2][0] = 8; S[0].pIndex[2][1] = 9; S[0].pIndex[2][2] = 10; S[0].pIndex[2][3] = 11;
	S[0].pIndex[3][0] = 12; S[0].pIndex[3][1] = 13; S[0].pIndex[3][2] = 14; S[0].pIndex[3][3] = 15;
	//第2卦限面片
	S[1].pIndex[0][0] = 12; S[1].pIndex[0][1] = 13; S[1].pIndex[0][2] = 14; S[1].pIndex[0][3] = 15;
	S[1].pIndex[1][0] = 16; S[1].pIndex[1][1] = 17; S[1].pIndex[1][2] = 18; S[1].pIndex[1][3] = 19;
	S[1].pIndex[2][0] = 20; S[1].pIndex[2][1] = 21; S[1].pIndex[2][2] = 22; S[1].pIndex[2][3] = 23;
	S[1].pIndex[3][0] = 24; S[1].pIndex[3][1] = 25; S[1].pIndex[3][2] = 26; S[1].pIndex[3][3] = 27;
	//第3卦限面片
	S[2].pIndex[0][0] = 24; S[2].pIndex[0][1] = 25; S[2].pIndex[0][2] = 26; S[2].pIndex[0][3] = 27;
	S[2].pIndex[1][0] = 28; S[2].pIndex[1][1] = 29; S[2].pIndex[1][2] = 30; S[2].pIndex[1][3] = 31;
	S[2].pIndex[2][0] = 32; S[2].pIndex[2][1] = 33; S[2].pIndex[2][2] = 34; S[2].pIndex[2][3] = 35;
	S[2].pIndex[3][0] = 36; S[2].pIndex[3][1] = 37; S[2].pIndex[3][2] = 38; S[2].pIndex[3][3] = 39;
	//第4卦限面片
	S[3].pIndex[0][0] = 36; S[3].pIndex[0][1] = 37; S[3].pIndex[0][2] = 38; S[3].pIndex[0][3] = 39;
	S[3].pIndex[1][0] = 40; S[3].pIndex[1][1] = 41; S[3].pIndex[1][2] = 42; S[3].pIndex[1][3] = 43;
	S[3].pIndex[2][0] = 44; S[3].pIndex[2][1] = 45; S[3].pIndex[2][2] = 46; S[3].pIndex[2][3] = 47;
	S[3].pIndex[3][0] = 0; S[3].pIndex[3][1] = 1; S[3].pIndex[3][2] = 2; S[3].pIndex[3][3] = 3;
}

void CRevolution::DrawRevolutionSurface(CDC* pDC)//绘制回转体曲面
{
	CP3 Point[4][4];//双三次曲面片的16个控制点	
	for (int nPatch = 0; nPatch < 4; nPatch++)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				Point[i][j] = V[S[nPatch].pIndex[i][j]];
		int nRecursiveDepth = 3;//递归深度
		CBezierPatch patch;//双三次曲面
		patch.ReadControlPoint(Point, nRecursiveDepth);
		patch.DrawCurvedPatch(pDC);
		//patch.DrawControlGrid(pDC);
	}
}