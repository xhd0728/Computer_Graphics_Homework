
// Exp2View.cpp: CExp2View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Exp2.h"
#endif

#include "Exp2Doc.h"
#include "Exp2View.h"

#include "Transform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExp2View

IMPLEMENT_DYNCREATE(CExp2View, CView)

BEGIN_MESSAGE_MAP(CExp2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CExp2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_ABC, &CExp2View::DrawCube)
	ON_COMMAND(ID_32773, &CExp2View::OnADraw)
END_MESSAGE_MAP()

// CExp2View 构造/析构

CExp2View::CExp2View() noexcept
{
	// TODO: 在此处添加构造代码

}

CExp2View::~CExp2View()
{
}

BOOL CExp2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CExp2View 绘图

void CExp2View::OnDraw(CDC* pDC)
{
	CExp2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	// 
	//绘制坐标轴
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

	//DrawCube(pDC);

}


void CExp2View::DrawCube(CDC* pDC) {

	//
	int i;
	double k[9];
	double Theta, Phi, R;//旋转角度
	Theta = 3.14 / 6;
	Phi = 3.14 * 3 / 4;
	R = 1000;
	int vex_s_x[12] = { 200,200,200,200,  0,  0,  0,  0,200,200,200,200 };
	int vex_s_y[12] = { 0  ,200,200,  0,  0,200,200,  0,  0,200,200,0 };
	int vex_s_z[12] = { 0,  0,200,200,  0,  0,200,200,  0,  0,200,200 };
	//正方体12条边起点坐标
	int vex_e_x[12] = { 200,200,200,200,  0,  0,  0,  0,  0,  0,  0, 0 };
	int vex_e_y[12] = { 200,200,  0,  0,200,200,  0,  0,  0,200,200,0 };
	int vex_e_z[12] = { 0,200,200,  0,  0,200,200,  0,  0,  0,200,200 };
	//正方体12条边终点坐标
	int newvex_s_x, newvex_s_y, newvex_s_z, newvex_e_x, newvex_e_y, newvex_e_z;
	//旋转变换后起点和终点坐标
	k[1] = sin(Theta); //初始化三角函数
	k[2] = sin(Phi);
	k[3] = cos(Theta);
	k[4] = cos(Phi);
	k[5] = k[2] * k[3];
	k[6] = k[2] * k[1];
	k[7] = k[4] * k[3];
	k[8] = k[4] * k[1];

	for (i = 0; i < 12; i++)
	{//观察变换
		newvex_s_x = k[3] * vex_s_x[i] - k[1] * vex_s_z[i]; //观察坐标系三维点
		newvex_s_y = -k[8] * vex_s_x[i] + k[2] * vex_s_y[i] - k[7] * vex_s_z[i];
		newvex_s_z = -k[6] * vex_s_x[i] - k[4] * vex_s_y[i] - k[5] * vex_s_z[i] + R;

		newvex_e_x = k[3] * vex_e_x[i] - k[1] * vex_e_z[i]; //观察坐标系三维点
		newvex_e_y = -k[8] * vex_e_x[i] + k[2] * vex_e_y[i] - k[7] * vex_e_z[i];
		newvex_e_z = -k[6] * vex_e_x[i] - k[4] * vex_e_y[i] - k[5] * vex_e_z[i] + R;

		//根据变换后的顶点，画出12条直线

		pDC->MoveTo(newvex_s_x, newvex_s_y);//光标移到起点
		pDC->LineTo(newvex_e_x, newvex_e_y);//在起点和终点之间画直线
	}

}

// CExp2View 打印


void CExp2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CExp2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CExp2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CExp2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CExp2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CExp2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CExp2View 诊断

#ifdef _DEBUG
void CExp2View::AssertValid() const
{
	CView::AssertValid();
}

void CExp2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CExp2Doc* CExp2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExp2Doc)));
	return (CExp2Doc*)m_pDocument;
}
#endif //_DEBUG


// CExp2View 消息处理程序


void CExp2View::DrawCube()
{
	// TODO: 在此添加命令处理程序代码

	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);//获取当前窗口区域大小
	pDC->SetMapMode(MM_ANISOTROPIC); //设置映射模式
	pDC->SetWindowExt(rect.Width(), rect.Height()); //设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height()); //设置视区范围
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2); //设置视区原点

	double k[9];
	double Theta, Phi, R;//旋转角度
	Theta = 3.14 / 6;
	Phi = 3.14  / 4;
	R = 1000;
	int vex_s_x[12] = { 200,200,200,200,  0,  0,  0,  0,200,200,200,200 };
	int vex_s_y[12] = { 0  ,200,200,  0,  0,200,200,  0,  0,200,200,0 };
	int vex_s_z[12] = { 0,  0,200,200,  0,  0,200,200,  0,  0,200,200 };
	//正方体12条边起点坐标
	int vex_e_x[12] = { 200,200,200,200,  0,  0,  0,  0,  0,  0,  0, 0 };
	int vex_e_y[12] = { 200,200,  0,  0,200,200,  0,  0,  0,200,200,0 };
	int vex_e_z[12] = { 0,200,200,  0,  0,200,200,  0,  0,  0,200,200 };
	//正方体12条边终点坐标
	int newvex_s_x, newvex_s_y, newvex_s_z, newvex_e_x, newvex_e_y, newvex_e_z;
	//旋转变换后起点和终点坐标

	for (int t = 1; t <= 200; ++t) {

		double theta_1 = Theta * t / 45;
		double phi_1 = Phi * t / 45;

		k[1] = sin(theta_1);
		k[2] = sin(phi_1);
		k[3] = cos(theta_1);
		k[4] = cos(phi_1);
		k[5] = k[2] * k[3];
		k[6] = k[2] * k[1];
		k[7] = k[4] * k[3];
		k[8] = k[4] * k[1];

		for (int i = 0; i < 12; i++)
		{//观察变换
			newvex_s_x = k[3] * vex_s_x[i] - k[1] * vex_s_z[i]; //观察坐标系三维点
			newvex_s_y = -k[8] * vex_s_x[i] + k[2] * vex_s_y[i] - k[7] * vex_s_z[i];
			newvex_s_z = -k[6] * vex_s_x[i] - k[4] * vex_s_y[i] - k[5] * vex_s_z[i] + R;

			newvex_e_x = k[3] * vex_e_x[i] - k[1] * vex_e_z[i]; //观察坐标系三维点
			newvex_e_y = -k[8] * vex_e_x[i] + k[2] * vex_e_y[i] - k[7] * vex_e_z[i];
			newvex_e_z = -k[6] * vex_e_x[i] - k[4] * vex_e_y[i] - k[5] * vex_e_z[i] + R;

			//根据变换后的顶点，画出12条直线

			pDC->MoveTo(newvex_s_x, newvex_s_y);//光标移到起点
			pDC->LineTo(newvex_e_x, newvex_e_y);//在起点和终点之间画直线
		}

		Sleep(50);
		RedrawWindow();
	}
	
}


void CExp2View::OnADraw()
{
	// TODO: 在此添加命令处理程序代码
	int x[] = { 20,50,50,20 };
	int y[] = { 20,20,50,50 };

	CDC* pDC = GetDC();
	CTransform trans;
	CRect rect;
	GetClientRect(&rect);//获取当前窗口区域大小
	pDC->SetMapMode(MM_ANISOTROPIC); //设置映射模式
	pDC->SetWindowExt(rect.Width(), rect.Height()); //设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height()); //设置视区范围
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2); //设置视区原点

	trans.num = 4;
	CP2* pArray = new CP2[trans.num];

	for (int i = 0; i < trans.num; ++i) {
		pArray[i].x = x[i];
		pArray[i].y = y[i];
	}

	trans.POld = pArray;
	for (int t = 0; t < 200; ++t) {
		for (int i = 0; i < 4; ++i) {
			pDC->MoveTo(pArray[i % 4].x, pArray[i % 4].y);
			pDC->LineTo(pArray[(i + 1) % 4].x, pArray[(i + 1) % 4].y);
		}

		trans.Rotate(10);
		trans.Scale(1.01, 1.01);
		Sleep(20);
		RedrawWindow();
	}
	

	ReleaseDC(pDC);
	delete[] pArray;

}
