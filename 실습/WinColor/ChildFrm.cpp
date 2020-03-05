﻿
// ChildFrm.cpp: CChildFrame 클래스의 구현
//

#include "stdafx.h"
#include "WinColor.h"

#include "ChildFrm.h"
#include "WinColorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()

// CChildFrame 생성/소멸

CChildFrame::CChildFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame 진단

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 메시지 처리기


void CChildFrame::ActivateFrame(int nCmdShow)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CRect rect, rectC;
	GetWindowRect(&rect);
	GetClientRect(&rectC);

	CSize sizeImg;

	sizeImg.cx = ((CWinColorDoc *)GetActiveDocument())->width;
	sizeImg.cy = ((CWinColorDoc *)GetActiveDocument())->height;

	int cx = sizeImg.cx + rect.Width() - rectC.Width() + 4;
	int cy = sizeImg.cy + rect.Height() - rectC.Height() + 4;

	SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_SHOWWINDOW);

	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}
