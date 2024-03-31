
// Exp2View.h: CExp2View 类的接口
//

#pragma once


class CExp2View : public CView
{
protected: // 仅从序列化创建
	CExp2View() noexcept;
	DECLARE_DYNCREATE(CExp2View)

// 特性
public:
	CExp2Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual void DrawCube(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CExp2View();
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
	afx_msg void DrawCube();
	afx_msg void OnADraw();
};

#ifndef _DEBUG  // Exp2View.cpp 中的调试版本
inline CExp2Doc* CExp2View::GetDocument() const
   { return reinterpret_cast<CExp2Doc*>(m_pDocument); }
#endif

