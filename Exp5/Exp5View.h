
// Exp5View.h: CExp5View 类的接口
//

#pragma once

#define PI 3.1415926
#define SLEEP_TIME 0

class CExp5View : public CView
{
protected: // 仅从序列化创建
	CExp5View() noexcept;
	DECLARE_DYNCREATE(CExp5View)

// 特性
public:
	CExp5Doc* GetDocument() const;

// 操作
public:
	double ViewPointz;
	double ViewPointy;
	double ViewPointx;
	double Tk[9];
	double TR, Theta, Phi, d;//视点在用户坐标系中的球坐标
	double Ia, Ip, ka, kd, ks, ns, LPx, LPy, LPz;

	void CExp5View::HideSphere(CDC* pDC, float R, bool is_Hide);
	void CExp5View::DrawXY();

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CExp5View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSphereDraw();
	afx_msg void OnSphereDraw2();
	afx_msg void OnSphereClear();
};

#ifndef _DEBUG  // Exp5View.cpp 中的调试版本
inline CExp5Doc* CExp5View::GetDocument() const
   { return reinterpret_cast<CExp5Doc*>(m_pDocument); }
#endif

