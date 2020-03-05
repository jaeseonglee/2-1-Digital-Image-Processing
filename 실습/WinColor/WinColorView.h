
// WinColorView.h: CWinColorView 클래스의 인터페이스
//

#pragma once


class CWinColorView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CWinColorView() noexcept;
	DECLARE_DYNCREATE(CWinColorView)

// 특성입니다.
public:
	CWinColorDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CWinColorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	BITMAPINFO *BmInfo;
	int height;
	int width;
	int rwsize;
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnReverseImg();
	afx_msg void OnUpdateReverseImg(CCmdUI *pCmdUI);
	afx_msg void OnChannelspiltRgb();
	afx_msg void OnUpdateChannelspiltRgb(CCmdUI *pCmdUI);
	afx_msg void OnGrayImg();
	afx_msg void OnUpdateGrayImg(CCmdUI *pCmdUI);
	afx_msg void OnFrmAdd();
	afx_msg void OnUpdateFrmAdd(CCmdUI *pCmdUI);
	afx_msg void OnLutMul();
	afx_msg void OnUpdateLutMul(CCmdUI *pCmdUI);
	afx_msg void OnImghisto();
	afx_msg void OnUpdateImghisto(CCmdUI *pCmdUI);
	afx_msg void OnBinarization();
	afx_msg void OnUpdateBinarization(CCmdUI *pCmdUI);
	afx_msg void OnHistoEqual();
	afx_msg void OnUpdateHistoEqual(CCmdUI *pCmdUI);
	afx_msg void OnHistoStretch();
	afx_msg void OnUpdateHistoStretch(CCmdUI *pCmdUI);
	afx_msg void OnBoxFilter();
	afx_msg void OnUpdateBoxFilter(CCmdUI *pCmdUI);
	afx_msg void OnGaussianFilter();
	afx_msg void OnSharpening();
	afx_msg void OnLaplacian();
	afx_msg void OnEdgeSobel();
	afx_msg void OnEdgePrewitt();
	afx_msg void OnMedianFilter();
	afx_msg void OnMorphologyBinaryErosion();
	afx_msg void OnMorphologyBinaryDilation();
	afx_msg void OnMorphologyGrayErosion();
	afx_msg void OnMorphologyGrayDilation();
};

#ifndef _DEBUG  // WinColorView.cpp의 디버그 버전
inline CWinColorDoc* CWinColorView::GetDocument() const
   { return reinterpret_cast<CWinColorDoc*>(m_pDocument); }
#endif

