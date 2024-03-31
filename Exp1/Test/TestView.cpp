
// TestView.cpp: CTestView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Test.h"
#endif

#include "TestDoc.h"
#include "TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CTestView 构造/析构

CTestView::CTestView() noexcept
{
	// TODO: 在此处添加构造代码

}

CTestView::~CTestView()
{
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTestView 绘图

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
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

	// 设置画笔
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* oldPen = pDC->SelectObject(&pen);

	// 在第一象限画点
	int x, y;
	for (y = 100; y < 250; y += 2)
		for (x = 100; x < 250; x += 2)
			pDC->SetPixelV(x, y, RGB(rand() % 255, rand() % 255, rand() % 255));

	// 在第二象限画直线
	pDC->MoveTo(-100, 100);
	pDC->LineTo(-200, 200);
	pDC->MoveTo(-200, 100);
	pDC->LineTo(-300, 200);
	pDC->MoveTo(-300, 100);
	pDC->LineTo(-400, 200);

	// 在第三象限画椭圆
	pDC->Ellipse(-300, -200, -50, -50);

	// 在第四象限画三角形
	pDC->MoveTo(200, -50);
	pDC->LineTo(50, -250);
	pDC->MoveTo(50, -250);
	pDC->LineTo(350, -250);
	pDC->MoveTo(350, -250);
	pDC->LineTo(200, -50);

	// 恢复画笔
	pDC->SelectObject(oldPen);

}


// CTestView 打印

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CTestView 诊断

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestView 消息处理程序


void CTestView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	P0 = point;
	CView::OnLButtonDown(nFlags, point);
}


void CTestView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	P1 = point;
	CDC* pDC = GetDC();
	pDC->MoveTo(P0);
	pDC->LineTo(P1);
	ReleaseDC(pDC);

	CView::OnLButtonUp(nFlags, point);
}
