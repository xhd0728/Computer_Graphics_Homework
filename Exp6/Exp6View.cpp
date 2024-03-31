
// Exp6View.cpp: CExp6View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Exp6.h"
#endif

#include "Exp6Doc.h"
#include "Exp6View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExp6View

IMPLEMENT_DYNCREATE(CExp6View, CView)

BEGIN_MESSAGE_MAP(CExp6View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CExp6View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILL, &CExp6View::OnSphereFill)
	ON_COMMAND(ID_CLEAR, &CExp6View::OnSphereClear)
END_MESSAGE_MAP()

// CExp6View 构造/析构

CExp6View::CExp6View() noexcept
{
	// TODO: 在此处添加构造代码
	TR = 1000, d = 1000, Phi = 30, Theta = 60;

	Tk[1] = sin(PI * Theta / 180);
	Tk[2] = sin(PI * Phi / 180);
	Tk[3] = cos(PI * Theta / 180);
	Tk[4] = cos(PI * Phi / 180);
	Tk[5] = Tk[2] * Tk[3];
	Tk[6] = Tk[2] * Tk[1];
	Tk[7] = Tk[4] * Tk[3];
	Tk[8] = Tk[4] * Tk[1];
	ViewPointx = TR * Tk[6];
	ViewPointy = TR * Tk[4];
	ViewPointz = TR * Tk[5];

	Ia = 255;
	Ip = 255;
	ka = 0.2;
	kd = 0.6;
	ks = 0.7;
	ns = 50;
	LPx = TR * sin(PI * 45 / 180) * sin(PI * 45 / 180);
	LPy = TR * cos(PI * 45 / 180);
	LPz = TR * sin(PI * 45 / 180) * cos(PI * 45 / 180);

}

CExp6View::~CExp6View()
{
}

BOOL CExp6View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CExp6View 绘图


void CExp6View::OnDraw(CDC* /*pDC*/)
{
	CExp6Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	DrawXY();
}


// CExp6View 打印
void CExp6View::DrawXY() {
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);//获取当前窗口区域大小
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

void CExp6View::FillSphere(CDC* pDC, float R)
{
	CPoint rgnpoints[4];
	COLORREF fillColor;
	CBrush brush;
	CPen NewPen;
	double sx[4], sy[4];

	double x[4], y[4], z[4];
	double x1[4], y1[4], z1[4];
	double a1, a2, b1, b2;
	double xn, yn, zn, vn, xv, yv, zv, costheta, mv;
	double Ie, Id, Is, costhita1, cosphi, hx, hy, hz, lx, ly, lz;
	int C = 0;

	Ie = Ia * ka;

	int idelta = 1;
	for (double i = 0; i < 120; i += idelta)
	{
		double alpha = i * PI / 180;
		double delta_alpha = (i + 1) * PI / 180;
		for (int j = 0; j < 360; j += 1)
		{
			a1 = j * PI / 180;
			a2 = (j + 1) * PI / 180;

			x[0] = R * sin(alpha) * sin(a1);
			y[0] = R * cos(alpha);
			z[0] = R * sin(alpha) * cos(a1);

			x[1] = R * sin(delta_alpha) * sin(a1);
			y[1] = R * cos(delta_alpha);
			z[1] = R * sin(delta_alpha) * cos(a1);

			x[2] = R * sin(delta_alpha) * sin(a2);
			y[2] = R * cos(delta_alpha);
			z[2] = R * sin(delta_alpha) * cos(a2);

			x[3] = R * sin(alpha) * sin(a2);
			y[3] = R * cos(alpha);
			z[3] = R * sin(alpha) * cos(a2);

			xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
			yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
			zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);

			vn = sqrt(xn * xn + yn * yn + zn * zn);
			xn = xn / vn;
			yn = yn / vn;
			zn = zn / vn;

			xv = ViewPointx - x[0];
			yv = ViewPointy - y[0];
			zv = ViewPointz - z[0];

			mv = sqrt(xv * xv + yv * yv + zv * zv);
			xv = xv / mv;
			yv = yv / mv;
			zv = zv / mv;

			costheta = xn * xv + yn * yv + zn * zv;
			if (costheta >= -0.25)
			{
				lx = LPx - x[0]; 
				ly = LPy - y[0]; 
				lz = LPz - z[0];

				mv = sqrt(lx * lx + ly * ly + lz * lz);
				lx = lx / mv; ly = ly / mv; lz = lz / mv;

				costhita1 = xn * lx + yn * ly + zn * lz;

				hx = lx + xv; 
				hy = ly + yv; 
				hz = lz + zv;

				mv = sqrt(hx * hx + hy * hy + hz * hz);
				hx = hx / mv; hy = hy / mv; hz = hz / mv;
				cosphi = xn * hx + yn * hy + zn * hz;

				if (costhita1 >= 0)
					Id = Ip * kd * costhita1;
				else
					Id = 0;
				if (cosphi >= 0)
					Is = Ip * ks * pow(cosphi, ns);
				else
					Is = 0;

				C = (int)(Ie + Id + Is);

				if (C > 255) C = 255;

				for (int k = 0; k < 4; k++)
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
				pDC->Polygon(rgnpoints, 4);
				brush.DeleteObject();
				NewPen.DeleteObject();
			}
			Sleep(SLEEP_TIME);
		}
	}
}

void CExp6View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CExp6View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CExp6View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CExp6View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CExp6View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CExp6View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CExp6View 诊断

#ifdef _DEBUG
void CExp6View::AssertValid() const
{
	CView::AssertValid();
}

void CExp6View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CExp6Doc* CExp6View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExp6Doc)));
	return (CExp6Doc*)m_pDocument;
}
#endif //_DEBUG


// CExp6View 消息处理程序


void CExp6View::OnSphereFill()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	FillSphere(pDC, 300);
	ReleaseDC(pDC);
}


void CExp6View::OnSphereClear()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(rect);
	CBrush BkBrush;
	BkBrush.CreateSolidBrush(RGB(255, 255, 255));
	pDC->FillRect(rect, &BkBrush);
	DrawXY();
	ReleaseDC(pDC);
}
