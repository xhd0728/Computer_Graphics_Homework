
// Exp5View.cpp: CExp5View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Exp5.h"
#endif

#include "Exp5Doc.h"
#include "Exp5View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExp5View

IMPLEMENT_DYNCREATE(CExp5View, CView)

BEGIN_MESSAGE_MAP(CExp5View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CExp5View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_DRAW, &CExp5View::OnSphereDraw)
	ON_COMMAND(ID_DRAW2, &CExp5View::OnSphereDraw2)
	ON_COMMAND(ID_CLEAR, &CExp5View::OnSphereClear)
END_MESSAGE_MAP()

// CExp5View 构造/析构

CExp5View::CExp5View() noexcept
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

CExp5View::~CExp5View()
{
}

BOOL CExp5View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CExp5View 绘图
void CExp5View::HideSphere(CDC* pDC, float R, bool is_Hide)
{

	double x[4], y[4], z[4];
	double x1[4], y1[4], z1[4];
	double a1, a2;
	double sx[4], sy[4];
	double xn, yn, zn, vn, xv, yv, zv, costheta, mv;
	Tk[1] = sin(PI * Theta / 180);
	Tk[2] = sin(PI * Phi / 180);
	Tk[3] = cos(PI * Theta / 180);
	Tk[4] = cos(PI * Phi / 180);
	Tk[5] = Tk[2] * Tk[3];
	Tk[6] = Tk[2] * Tk[1];
	Tk[7] = Tk[4] * Tk[3];
	Tk[8] = Tk[4] * Tk[1];

	int idelta = 10;

	for (double i = 0; i < 180; i += idelta)
	{
		double alpha = i * PI / 180;
		double delta_alpha = (i + 10) * PI / 180;
		for (int j = 0; j < 360; j += 5)
		{
			a1 = j * PI / 180;
			a2 = (j + 5) * PI / 180;

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
			if (costheta >= -0.1 || is_Hide)
			{
				for (int k = 0; k < 4; k++)
				{
					x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
					y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
					z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;
					sx[k] = x1[k];
					sy[k] = y1[k];
				}
				pDC->MoveTo((int)sx[0], (int)sy[0]);
				pDC->LineTo((int)sx[1], (int)sy[1]);
				pDC->LineTo((int)sx[2], (int)sy[2]);
				pDC->LineTo((int)sx[3], (int)sy[3]);
				pDC->LineTo((int)sx[0], (int)sy[0]);
			}
			Sleep(SLEEP_TIME);
		}
	}
}

void CExp5View::OnDraw(CDC* pDC)
{
	CExp5Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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
}


// CExp5View 打印


void CExp5View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CExp5View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CExp5View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CExp5View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CExp5View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CExp5View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CExp5View 诊断

#ifdef _DEBUG
void CExp5View::AssertValid() const
{
	CView::AssertValid();
}

void CExp5View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CExp5Doc* CExp5View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExp5Doc)));
	return (CExp5Doc*)m_pDocument;
}
#endif //_DEBUG


// CExp5View 消息处理程序
void CExp5View::DrawXY() {
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


void CExp5View::OnSphereDraw()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	HideSphere(pDC, 200, false);
	ReleaseDC(pDC);
}


void CExp5View::OnSphereDraw2()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	HideSphere(pDC, 200, true);
	ReleaseDC(pDC);
}


void CExp5View::OnSphereClear()
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
