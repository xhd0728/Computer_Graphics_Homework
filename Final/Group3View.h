
// Group3View.h: CGroup3View 类的接口
//

#pragma once

#define PI 3.1415926

class CGroup3View : public CView
{
protected: // 仅从序列化创建
	CGroup3View() noexcept;
	DECLARE_DYNCREATE(CGroup3View)

// 特性
public:
	CGroup3Doc* GetDocument() const;

// 操作
public:

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
	double ViewPointz;
	double ViewPointy;
	double ViewPointx;
	double Tk[9];
	double TR, Theta, Phi, d;
	double Ia, Ip, ka, kd, ks, ns, LPx, LPy, LPz;

	BOOL is_rotate;
	int sleep_time;

	virtual ~CGroup3View();

	virtual void CGroup3View::HideSphere(CDC* pDC, double R);
	virtual void CGroup3View::FillSphere(CDC* pDC, double R);
	virtual void CGroup3View::DrawXY();
	virtual void CGroup3View::change();
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
	afx_msg void OnMyDraw();
	afx_msg void OnMyFill();
	afx_msg void OnClear();
	afx_msg void OnMyConfig();
};

#ifndef _DEBUG  // Group3View.cpp 中的调试版本
inline CGroup3Doc* CGroup3View::GetDocument() const
   { return reinterpret_cast<CGroup3Doc*>(m_pDocument); }
#endif

