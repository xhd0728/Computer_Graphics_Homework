
// Group3View.cpp: CGroup3View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Group3.h"
#endif

#include "Group3Doc.h"
#include "Group3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGroup3View

IMPLEMENT_DYNCREATE(CGroup3View, CView)

BEGIN_MESSAGE_MAP(CGroup3View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGroup3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_DRAW, &CGroup3View::OnMyDraw)
	ON_COMMAND(ID_FILL, &CGroup3View::OnMyFill)
	ON_COMMAND(ID_CLEAR, &CGroup3View::OnClear)
	ON_COMMAND(ID_CONFIG, &CGroup3View::OnMyConfig)
END_MESSAGE_MAP()

// CGroup3View 构造/析构

CGroup3View::CGroup3View() noexcept
{
	// TODO: 在此处添加构造代码
	// 设置初始参数
	TR = 1000;  // 视点距离
	d = 1000;   // 投影面到视点的距离
	Phi = 90;   // 俯仰角
	Theta = 60; // 方位角
	is_rotate = false;
	sleep_time = 100;
	change();
	
}

void CGroup3View::change() {
	Tk[1] = sin(PI * Theta / 180);
	Tk[2] = sin(PI * Phi / 180);
	Tk[3] = cos(PI * Theta / 180);
	Tk[4] = cos(PI * Phi / 180);
	Tk[5] = Tk[2] * Tk[3];
	Tk[6] = Tk[2] * Tk[1];
	Tk[7] = Tk[4] * Tk[3];
	Tk[8] = Tk[4] * Tk[1];
	ViewPointx = TR * Tk[6]; // 视点在x轴上的坐标
	ViewPointy = TR * Tk[4]; // 视点在y轴上的坐标
	ViewPointz = TR * Tk[5]; // 视点在z轴上的坐标

	// 设置材质参数
	Ia = 255;   // 环境光强度
	Ip = 255;   // 光源强度
	ka = 0.2;   // 环境反射系数
	kd = 0.6;   // 漫反射系数
	ks = 0.7;   // 镜面反射系数
	ns = 50;    // 镜面反射指数

	// 设置光源位置
	LPx = TR * sin(PI * 45 / 180) * sin(PI * 45 / 180); // 光源在x轴上的坐标
	LPy = TR * cos(PI * 45 / 180);                      // 光源在y轴上的坐标
	LPz = TR * sin(PI * 45 / 180) * cos(PI * 45 / 180); // 光源在z轴上的坐标
}


CGroup3View::~CGroup3View()
{
}

BOOL CGroup3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGroup3View 绘图
void CGroup3View::FillSphere(CDC* pDC, double R) {
	// 定义一个由四个 CPoint 对象组成的数组，用于表示一个多边形的顶点。
	CPoint rgnpoints[4];

	// 定义一个 COLORREF 变量，用于存储多边形的填充颜色。
	COLORREF fillColor;

	// 定义一个 CBrush 对象，用于填充多边形的颜色。
	CBrush brush;

	// 定义一个 CPen 对象，用于绘制多边形的轮廓。
	CPen NewPen;

	// 定义三个数组，分别存储多边形的顶点 x、y、z 坐标。
	double x[4], y[4], z[4];

	// 定义三个数组，分别存储变换后的多边形顶点的 x、y、z 坐标。
	double x1[4], y1[4], z1[4];

	// 定义一些变量，用于后续计算。
	int a1, a2, a3;
	double xn, yn, zn, vn, xv, yv, zv, costheta, mv, Id, Is, costhita1, cosphi, hx, hy, hz, lx, ly, lz;
	int C = 0;

	// 定义一些变量，用于后续计算。
	double px, py, ph, pH, pp;
	px = sqrt(2 * R * R * (1 - cos(72 * PI / 180)));
	ph = sqrt(R * R * (1 - 2 * cos(72 * PI / 180)));
	py = px / (2 + sqrt(2 - 2 * cos(108 * PI / 180)));
	pp = py * sin(36 * PI / 180);
	pH = sqrt(pow(px * sqrt(3) / 2, 2) + pp * pp) / 2;

	// 计算多边形的环境光强度。
	double Ie = Ia * ka;

	//floor-1
	a1 = 0;
	a3 = 72;

	// 循环 5 次，生成五个顶点的多边形
	for (int ii = 1; ii <= 5; ++ii)
	{
		// 计算多边形的三个顶点的 x、y、z 坐标
		x[0] = R * cos(a1 * PI / 180);
		y[0] = pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = 0;
		y[1] = pH + ph;
		z[1] = 0;
		x[2] = R * cos(a3 * PI / 180);
		y[2] = pH;
		z[2] = R * sin(a3 * PI / 180);

		// 计算多边形的法向量，并进行单位化
		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;

		// 计算从视点到多边形顶点的向量，并进行单位化
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;

		// 计算法向量与视线向量的夹角余弦值
		costheta = xn * xv + yn * yv + zn * zv;

		// 如果夹角余弦值大于等于 0，则进行光照计算
		if (costheta >= 0)
		{
			// 计算从光源到多边形顶点的向量，并进行单位化
			lx = LPx - x[0];
			ly = LPy - y[0];
			lz = LPz - z[0];
			mv = sqrt(lx * lx + ly * ly + lz * lz);
			lx = lx / mv;
			ly = ly / mv;
			lz = lz / mv;

			// 计算法向量与光源向量的夹角余弦值
			costhita1 = xn * lx + yn * ly + zn * lz;

			// 计算反射向量与视线向量的夹角余弦值
			hx = lx + xv;
			hy = ly + yv;
			hz = lz + zv;
			mv = sqrt(hx * hx + hy * hy + hz * hz);
			hx = hx / mv;
			hy = hy / mv;
			hz = hz / mv;
			cosphi = xn * hx + yn * hy + zn * hz;

			// 计算漫反射光强度和镜面反射光强度
			if (costhita1 >= 0)
				Id = Ip * kd * costhita1;
			else
				Id = 0;
			if (cosphi >= 0)
				Is = Ip * ks * pow(cosphi, ns);
			else
				Is = 0;

			// 计算多边形的最终颜色，并将其转换为 RGB 值
			C = (int)(Ie + Id + Is);
			if (C > 255) C = 255;

			// 对多边形的三个顶点进行变换，并填充多边形
			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;

				rgnpoints[k].x = x1[k];
				rgnpoints[k].y = y1[k];
			}
			
			// 创建填充多边形所需的画刷和画笔，并设置其颜色
			fillColor = RGB(C, C, C);
			if (!brush.CreateSolidBrush(fillColor)) return;
			if (!NewPen.CreatePen(PS_SOLID, 1, fillColor)) return;
			
			// 在设备上下文中选择画刷和画笔，并绘制填充多边形
			pDC->SelectObject(&NewPen);
			pDC->SelectObject(&brush);
			pDC->Polygon(rgnpoints, 3);
			
			// 释放画刷和画笔所占用的资源
			brush.DeleteObject();
			NewPen.DeleteObject();
		}

		// 将变量 a1 和 a3 分别加上 72，以便生成下一个多边形。
		a1 = (a1 + 72) % 360;
		a3 = (a3 + 72) % 360;
	}

	//floor-2
	a1 = 0;
	a2 = 36;
	a3 = 72;

	// 循环 5 次，生成五个顶点的多边形
	for (int i = 1; i <= 5; ++i)
	{
		// 计算3个点的坐标
		x[0] = R * cos(a1 * PI / 180);
		y[0] = pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = R * cos(a3 * PI / 180);
		y[1] = pH;
		z[1] = R * sin(a3 * PI / 180);
		x[2] = R * cos(a2 * PI / 180);
		y[2] = -pH;
		z[2] = R * sin(a2 * PI / 180);

		// 计算法向量
		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		
		// 计算观察点与物体表面上一点的向量
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		
		// 计算入射角
		costheta = xn * xv + yn * yv + zn * zv;
		
		// 如果入射角大于等于0，计算反射光
		if (costheta >= 0)
		{
			// 计算光源到物体表面上一点的向量，并归一化
			lx = LPx - x[0];
			ly = LPy - y[0];
			lz = LPz - z[0];
			mv = sqrt(lx * lx + ly * ly + lz * lz);
			lx = lx / mv;
			ly = ly / mv;
			lz = lz / mv;

			// 计算光源方向与法向量的夹角，即漫反射角
			costhita1 = xn * lx + yn * ly + zn * lz;
			
			// 计算半程向量，并归一化
			hx = lx + xv;
			hy = ly + yv;
			hz = lz + zv;
			mv = sqrt(hx * hx + hy * hy + hz * hz);
			hx = hx / mv;
			hy = hy / mv;
			hz = hz / mv;
			
			// 计算光线方向与半程向量的夹角，即镜面反射角
			cosphi = xn * hx + yn * hy + zn * hz;
			
			// 如果漫反射角大于等于0，计算漫反射光强度
			if (costhita1 >= 0)
				Id = Ip * kd * costhita1;
			else
				Id = 0;
			
			// 如果镜面反射角大于等于0，计算镜面反射光强度
			if (cosphi >= 0)
				Is = Ip * ks * pow(cosphi, ns);
			else
				Is = 0;

			// 计算物体表面的总光强度
			C = (int)(Ie + Id + Is);
			if (C > 255) C = 255;

			// 计算3个点在变换后的坐标系中的坐标，并设置填充颜色
			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;

				rgnpoints[k].x = x1[k];
				rgnpoints[k].y = y1[k];
			}
			fillColor = RGB(C, C, C);
			
			// 创建画刷和画笔，并用它们填充三角形
			if (!brush.CreateSolidBrush(fillColor)) return;
			if (!NewPen.CreatePen(PS_SOLID, 1, fillColor)) return;
			pDC->SelectObject(&NewPen);
			pDC->SelectObject(&brush);
			pDC->Polygon(rgnpoints, 3);
			brush.DeleteObject();
			NewPen.DeleteObject();
		}

		// 更新3个角度变量
		a1 = (a1 + 72) % 360;
		a2 = (a2 + 72) % 360;
		a3 = (a3 + 72) % 360;
	}

	//floor-3
	a1 = 36;
	a2 = 72;
	a3 = 108;

	// 循环 5 次，生成五个顶点的多边形
	for (int j = 1; j <= 5; ++j)
	{
		// 根据当前角度计算出每个点的坐标
		x[0] = R * cos(a1 * PI / 180);
		y[0] = -pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = R * cos(a2 * PI / 180);
		y[1] = pH;
		z[1] = R * sin(a2 * PI / 180);
		x[2] = R * cos(a3 * PI / 180);
		y[2] = -pH;
		z[2] = R * sin(a3 * PI / 180);

		// 计算出法向量
		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		
		// 计算出从视点到多边形顶点的单位向量
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		
		// 计算出法向量和视线向量的夹角
		costheta = xn * xv + yn * yv + zn * zv;
		
		// 如果夹角大于等于0，则可见
		if (costheta >= 0)
		{
			// 计算出从光源到多边形顶点的单位向量
			lx = LPx - x[0];
			ly = LPy - y[0];
			lz = LPz - z[0];
			mv = sqrt(lx * lx + ly * ly + lz * lz);
			lx = lx / mv;
			ly = ly / mv;
			lz = lz / mv;

			// 计算出光源向量和法向量的夹角，以及反射向量和视线向量的夹角
			costhita1 = xn * lx + yn * ly + zn * lz;
			hx = lx + xv; hy = ly + yv; hz = lz + zv;
			mv = sqrt(hx * hx + hy * hy + hz * hz);
			hx = hx / mv; hy = hy / mv; hz = hz / mv;
			cosphi = xn * hx + yn * hy + zn * hz;
			
			// 如果光源向量和法向量的夹角大于等于0，则计算出漫反射光照强度
			if (costhita1 >= 0)
				Id = Ip * kd * costhita1;
			else
				Id = 0;
			
			// 如果反射向量和视线向量的夹角大于等于0，则计算出镜面反射光照强度
			if (cosphi >= 0)
				Is = Ip * ks * pow(cosphi, ns);
			else
				Is = 0;

			// 计算出最终的颜色值
			C = (int)(Ie + Id + Is);
			if (C > 255) C = 255;

			// 将三角形的顶点坐标转换到屏幕坐标系中，填充颜色并绘制
			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;

				rgnpoints[k].x = x1[k];
				rgnpoints[k].y = y1[k];
			}
			fillColor = RGB(C, C, C);
			if (!brush.CreateSolidBrush(fillColor)) return;
			if (!NewPen.CreatePen(PS_SOLID, 1, fillColor)) return;
			pDC->SelectObject(&NewPen);
			pDC->SelectObject(&brush);
			pDC->Polygon(rgnpoints, 3);
			brush.DeleteObject();
			NewPen.DeleteObject();
		}

		// 更新角度值
		a1 = (a1 + 72) % 360;
		a2 = (a2 + 72) % 360;
		a3 = (a3 + 72) % 360;
	}

	//floor-4
	a1 = 36;
	a3 = 108;

	// 循环五次，生成五个顶点的多边形
	for (int jj = 1; jj <= 5; ++jj)
	{
		// 根据当前角度计算出每个点的坐标
		x[0] = R * cos(a1 * PI / 180);
		y[0] = -pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = R * cos(a3 * PI / 180);
		y[1] = -pH;
		z[1] = R * sin(a3 * PI / 180);
		x[2] = 0;
		y[2] = -pH - ph;
		z[2] = 0;

		// 计算出法向量
		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		
		// 计算出从视点到多边形顶点的单位向量
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		
		// 计算出法向量和视线向量的夹角
		costheta = xn * xv + yn * yv + zn * zv;
		
		// 如果夹角大于等于0，则可见
		if (costheta >= 0)
		{
			// 计算出从光源到多边形顶点的单位向量
			lx = LPx - x[0];
			ly = LPy - y[0];
			lz = LPz - z[0];
			mv = sqrt(lx * lx + ly * ly + lz * lz);
			lx = lx / mv;
			ly = ly / mv;
			lz = lz / mv;

			// 计算出光源向量和法向量的夹角，以及反射向量和视线向量的夹角
			costhita1 = xn * lx + yn * ly + zn * lz;
			hx = lx + xv;
			hy = ly + yv;
			hz = lz + zv;
			mv = sqrt(hx * hx + hy * hy + hz * hz);
			hx = hx / mv;
			hy = hy / mv;
			hz = hz / mv;
			cosphi = xn * hx + yn * hy + zn * hz;
			
			// 如果光源向量和法向量的夹角大于等于0，则计算出漫反射光照强度
			if (costhita1 >= 0)
				Id = Ip * kd * costhita1;
			else
				Id = 0;
			
			// 如果反射向量和视线向量的夹角大于等于0，则计算出镜面反射光照强度
			if (cosphi >= 0)
				Is = Ip * ks * pow(cosphi, ns);
			else
				Is = 0;

			// 计算出最终的颜色值
			C = (int)(Ie + Id + Is);
			if (C > 255) C = 255;

			// 将三角形的顶点坐标转换到屏幕坐标系中，填充颜色并绘制
			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;

				rgnpoints[k].x = x1[k];
				rgnpoints[k].y = y1[k];
			}
			
			// 创建填充和边框画刷，填充颜色为C
			fillColor = RGB(C, C, C);
			if (!brush.CreateSolidBrush(fillColor)) return;
			if (!NewPen.CreatePen(PS_SOLID, 1, fillColor)) return;
			
			// 选择画刷和边框画笔，并用多边形函数绘制三角形
			pDC->SelectObject(&NewPen);
			pDC->SelectObject(&brush);
			pDC->Polygon(rgnpoints, 3);
			brush.DeleteObject();
			NewPen.DeleteObject();
		}

		// 计算下一个多边形的顶点角度
		a1 = (a1 + 72) % 360;
		a3 = (a3 + 72) % 360;
	}
}

void CGroup3View::HideSphere(CDC* pDC, double R) {
	
	// 用于存储一个三角形的三个顶点的 x、y、z 坐标
	double x[4], y[4], z[4];
	
	// 经过变换后的三角形的顶点坐标
	double x1[4], y1[4], z1[4];
	
	// 用于存储角度值
	int a1, a2, a3;
	
	// 经过投影后的三角形的顶点坐标
	double sx[4], sy[4];
	
	// 用于计算光照效果的一些中间变量
	double xn, yn, zn, vn, xv, yv, zv, costheta, mv;

	Tk[1] = sin(PI * Theta / 180);
	Tk[2] = sin(PI * Phi / 180);
	Tk[3] = cos(PI * Theta / 180);
	Tk[4] = cos(PI * Phi / 180);
	Tk[5] = Tk[2] * Tk[3];
	Tk[6] = Tk[2] * Tk[1];
	Tk[7] = Tk[4] * Tk[3];
	Tk[8] = Tk[4] * Tk[1];

	// 用于计算三角形的初始坐标
	double px, py, ph, pH, pp;

	// 计算正五边形的边长
	px = sqrt(2 * R * R * (1 - cos(72 * PI / 180)));

	// 计算正五边形中心到边的距离
	ph = sqrt(R * R * (1 - 2 * cos(72 * PI / 180)));

	// 计算正五边形中心到顶点的距离
	py = px / (2 + sqrt(2 - 2 * cos(108 * PI / 180)));

	// 计算正五边形中心到顶点的垂直距离
	pp = py * sin(36 * PI / 180);

	// 计算正五边形中心到边的中点的距离
	pH = sqrt(pow(px * sqrt(3) / 2, 2) + pp * pp) / 2;

	//floor-1
	// 初始化角度值
	a1 = 0;
	a3 = 72;

	// 五个正五边形
	for (int ii = 1; ii <= 5; ++ii)
	{
		// 计算三角形的三个顶点坐标
		x[0] = R * cos(a1 * PI / 180);
		y[0] = pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = 0;
		y[1] = pH + ph;
		z[1] = 0;
		x[2] = R * cos(a3 * PI / 180);
		y[2] = pH;
		z[2] = R * sin(a3 * PI / 180);

		// 计算三角形的法向量
		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		
		// 计算光照效果
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		costheta = xn * xv + yn * yv + zn * zv;
		
		// 如果法向量和视线方向的夹角小于 90 度，则进行绘制
		if (costheta >= 0)
		{
			// 进行坐标变换
			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;
				sx[k] = x1[k];
				sy[k] = y1[k];
			}

			// 绘制三角形
			pDC->MoveTo((int)sx[0], (int)sy[0]);
			pDC->LineTo((int)sx[1], (int)sy[1]);
			pDC->LineTo((int)sx[2], (int)sy[2]);
			pDC->LineTo((int)sx[0], (int)sy[0]);
		}

		// 更新角度值
		a1 = (a1 + 72) % 360;
		a3 = (a3 + 72) % 360;
	}

	//floor-2
	// 初始化角度值
	a1 = 0;
	a2 = 36;
	a3 = 72;

	// 对每个正五边形进行绘制
	for (int i = 1; i <= 5; ++i)
	{
		// 计算三角形的三个顶点坐标
		x[0] = R * cos(a1 * PI / 180);
		y[0] = pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = R * cos(a3 * PI / 180);
		y[1] = pH;
		z[1] = R * sin(a3 * PI / 180);
		x[2] = R * cos(a2 * PI / 180);
		y[2] = -pH;
		z[2] = R * sin(a2 * PI / 180);

		// 计算三角形的法向量
		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		
		// 计算光照效果
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		costheta = xn * xv + yn * yv + zn * zv;
		
		// 如果法向量和视线方向的夹角小于 90 度，则进行绘制
		if (costheta >= 0)
		{
			// 进行坐标变换
			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;
				sx[k] = x1[k];
				sy[k] = y1[k];
			}
			
			// 绘制三角形
			pDC->MoveTo((int)sx[0], (int)sy[0]);
			pDC->LineTo((int)sx[1], (int)sy[1]);
			pDC->LineTo((int)sx[2], (int)sy[2]);
			pDC->LineTo((int)sx[0], (int)sy[0]);
		}
		
		// 更新角度值，准备绘制下一个正五边形
		a1 = (a1 + 72) % 360;
		a2 = (a2 + 72) % 360;
		a3 = (a3 + 72) % 360;
	}

	//floor-3
	//定义三个角度变量a1, a2, a3，分别为36度，72度和108度
	a1 = 36;
	a2 = 72;
	a3 = 108;

	//循环5次
	for (int j = 1; j <= 5; ++j)
	{
		// 计算三角形的三个顶点坐标
		x[0] = R * cos(a1 * PI / 180);
		y[0] = -pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = R * cos(a2 * PI / 180);
		y[1] = pH;
		z[1] = R * sin(a2 * PI / 180);
		x[2] = R * cos(a3 * PI / 180);
		y[2] = -pH;
		z[2] = R * sin(a3 * PI / 180);

		//计算法向量
		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		
		//计算视点向量
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		
		//计算法向量和视点向量的夹角余弦值
		costheta = xn * xv + yn * yv + zn * zv;
		
		//判断是否可见
		if (costheta >= 0)
		{
			//计算三个点的投影坐标
			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;
				sx[k] = x1[k];
				sy[k] = y1[k];
			}

			//绘制三角形
			pDC->MoveTo((int)sx[0], (int)sy[0]);
			pDC->LineTo((int)sx[1], (int)sy[1]);
			pDC->LineTo((int)sx[2], (int)sy[2]);
			pDC->LineTo((int)sx[0], (int)sy[0]);
		}

		//更新角度变量
		a1 = (a1 + 72) % 360;
		a2 = (a2 + 72) % 360;
		a3 = (a3 + 72) % 360;
	}

	//floor-4
	//定义两个角度变量a1和a3，分别为36度和108度
	a1 = 36;
	a3 = 108;

	// 对每个正五边形进行绘制
	for (int jj = 1; jj <= 5; ++jj)
	{
		//计算三角形上的三个点的坐标
		x[0] = R * cos(a1 * PI / 180);
		y[0] = -pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = R * cos(a3 * PI / 180);
		y[1] = -pH;
		z[1] = R * sin(a3 * PI / 180);
		x[2] = 0;
		y[2] = -pH - ph;
		z[2] = 0;

		//计算法向量
		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		
		//计算视点向量
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		
		//计算法向量和视点向量的夹角余弦值
		costheta = xn * xv + yn * yv + zn * zv;

		//判断是否可见
		if (costheta >= 0)
		{
			//计算三个点的投影坐标
			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;
				sx[k] = x1[k];
				sy[k] = y1[k];
			}
			
			//绘制三角形
			pDC->MoveTo((int)sx[0], (int)sy[0]);
			pDC->LineTo((int)sx[1], (int)sy[1]);
			pDC->LineTo((int)sx[2], (int)sy[2]);
			pDC->LineTo((int)sx[0], (int)sy[0]);
		}
		
		//更新角度变量
		a1 = (a1 + 72) % 360; a3 = (a3 + 72) % 360;
	}
}

void CGroup3View::OnDraw(CDC* pDC)
{
	CGroup3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	DrawXY();
}


// CGroup3View 打印


void CGroup3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGroup3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGroup3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CGroup3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CGroup3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGroup3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGroup3View 诊断

#ifdef _DEBUG
void CGroup3View::AssertValid() const
{
	CView::AssertValid();
}

void CGroup3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGroup3Doc* CGroup3View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGroup3Doc)));
	return (CGroup3Doc*)m_pDocument;
}
#endif //_DEBUG


// CGroup3View 消息处理程序
void CGroup3View::DrawXY() {
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(rect);//获取当前窗口区域大小
	pDC->SetMapMode(MM_ANISOTROPIC); //设置映射模式
	pDC->SetWindowExt(rect.Width(), rect.Height()); //设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height()); //设置视区范围
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2); //设置视区原点

	// 绘制x轴
	pDC->MoveTo(-rect.Width() / 2, 0);
	pDC->LineTo(rect.Width() / 2, 0);
	pDC->TextOutW(rect.Width() / 2 - 50, -20, L"x");

	// 绘制y轴
	pDC->MoveTo(0, -rect.Height() / 2);
	pDC->LineTo(0, rect.Height() / 2);
	pDC->TextOutW(-20, rect.Height() / 2 - 20, L"y");

	// 标出原点
	CBrush brush(RGB(0, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&brush);
	pDC->Ellipse(CRect(-3, -3, 3, 3));
	pDC->SelectObject(oldBrush);

	// 绘制箭头
	int arrowSize = 6;
	pDC->MoveTo(rect.Width() / 2 - arrowSize, arrowSize);
	pDC->LineTo(rect.Width() / 2, 0);
	pDC->LineTo(rect.Width() / 2 - arrowSize, -arrowSize);
	pDC->MoveTo(-arrowSize, rect.Height() / 2 - arrowSize);
	pDC->LineTo(0, rect.Height() / 2);
	pDC->LineTo(arrowSize, rect.Height() / 2 - arrowSize);
	ReleaseDC(pDC);
}

void CGroup3View::OnMyDraw()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	if (is_rotate) {
		for (int i = 0; i < 20; ++i) {
			OnClear();
			HideSphere(pDC, 100);
			Theta += 10;
			Phi += 10;
			change();
			Sleep(100);
		}
	}
	else {
		OnClear();
		HideSphere(pDC, 100);
	}
	ReleaseDC(pDC);
}


void CGroup3View::OnMyFill()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	if (is_rotate) {
		for (int i = 0; i < 20; ++i) {
			OnClear();
			FillSphere(pDC, 100);
			Theta += 10;
			Phi += 10;
			change();
			Sleep(100);
		}
	}
	else {
		OnClear();
		FillSphere(pDC, 100);
	}
	ReleaseDC(pDC);
}


void CGroup3View::OnClear()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	CBrush BkBrush;
	BkBrush.CreateSolidBrush(RGB(255, 255, 255));
	pDC->FillRect(rect, &BkBrush);
	DrawXY();
	ReleaseDC(pDC);
}


void CGroup3View::OnMyConfig()
{
	// TODO: 在此添加命令处理程序代码
	is_rotate = !is_rotate;
	CString strMessage;
	if (is_rotate) {
		strMessage.Format(L"已设置为可旋转");
	}
	else {
		strMessage.Format(L"已设置为不可旋转");
	}
	AfxMessageBox(strMessage);
}
