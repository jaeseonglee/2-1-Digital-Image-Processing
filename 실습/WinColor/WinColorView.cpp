
// WinColorView.cpp: CWinColorView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "WinColor.h"
#endif

#include "WinColorDoc.h"
#include "WinColorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinColorView

IMPLEMENT_DYNCREATE(CWinColorView, CScrollView)

BEGIN_MESSAGE_MAP(CWinColorView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_EDIT_COPY, &CWinColorView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CWinColorView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CWinColorView::OnUpdateEditPaste)
	ON_COMMAND(IDM_REVERSE_IMG, &CWinColorView::OnReverseImg)
	ON_UPDATE_COMMAND_UI(IDM_REVERSE_IMG, &CWinColorView::OnUpdateReverseImg)
	ON_COMMAND(IDM_CHANNELSPILT_RGB, &CWinColorView::OnChannelspiltRgb)
	ON_UPDATE_COMMAND_UI(IDM_CHANNELSPILT_RGB, &CWinColorView::OnUpdateChannelspiltRgb)
	ON_COMMAND(IDM_GRAY_IMG, &CWinColorView::OnGrayImg)
	ON_UPDATE_COMMAND_UI(IDM_GRAY_IMG, &CWinColorView::OnUpdateGrayImg)
	ON_COMMAND(IDM_FRM_ADD, &CWinColorView::OnFrmAdd)
	ON_UPDATE_COMMAND_UI(IDM_FRM_ADD, &CWinColorView::OnUpdateFrmAdd)
	ON_COMMAND(IDM_LUT_MUL, &CWinColorView::OnLutMul)
	ON_UPDATE_COMMAND_UI(IDM_LUT_MUL, &CWinColorView::OnUpdateLutMul)
	ON_COMMAND(IDM_IMGHISTO, &CWinColorView::OnImghisto)
	ON_UPDATE_COMMAND_UI(IDM_IMGHISTO, &CWinColorView::OnUpdateImghisto)
	ON_COMMAND(IDM_BINARIZATION, &CWinColorView::OnBinarization)
	ON_UPDATE_COMMAND_UI(IDM_BINARIZATION, &CWinColorView::OnUpdateBinarization)
	ON_COMMAND(IDM_HISTO_EQUAL, &CWinColorView::OnHistoEqual)
	ON_UPDATE_COMMAND_UI(IDM_HISTO_EQUAL, &CWinColorView::OnUpdateHistoEqual)
	ON_COMMAND(IDM_HISTO_STRETCH, &CWinColorView::OnHistoStretch)
	ON_UPDATE_COMMAND_UI(IDM_HISTO_STRETCH, &CWinColorView::OnUpdateHistoStretch)
	ON_COMMAND(IDM_BOX_FILTER, &CWinColorView::OnBoxFilter)
	ON_COMMAND(IDM_GAUSSIAN_FILTER, &CWinColorView::OnGaussianFilter)
	ON_COMMAND(IDM_SHARPENING, &CWinColorView::OnSharpening)
	ON_COMMAND(IDM_LAPLACIAN, &CWinColorView::OnLaplacian)
	ON_COMMAND(IDM_EDGE_SOBEL, &CWinColorView::OnEdgeSobel)
	ON_COMMAND(IDM_EDGE_PREWITT, &CWinColorView::OnEdgePrewitt)
	ON_COMMAND(IDM_MEDIAN_FILTER, &CWinColorView::OnMedianFilter)
	ON_COMMAND(IDM_MORPHOLOGY_BINARY_EROSION, &CWinColorView::OnMorphologyBinaryErosion)
	ON_COMMAND(IDM_MORPHOLOGY_BINARY_DILATION, &CWinColorView::OnMorphologyBinaryDilation)
	ON_COMMAND(IDM_MORPHOLOGY_GRAY_EROSION, &CWinColorView::OnMorphologyGrayErosion)
	ON_COMMAND(IDM_MORPHOLOGY_GRAY_DILATION, &CWinColorView::OnMorphologyGrayDilation)
END_MESSAGE_MAP()
// CWinColorView 생성/소멸

CWinColorView::CWinColorView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	for (int i = 0; i < 256; i++) {
		BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbBlue = i;
		BmInfo->bmiColors[i].rgbReserved = 0;
	}
}

CWinColorView::~CWinColorView()
{
	if (BmInfo)
		delete BmInfo;
}

BOOL CWinColorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CWinColorView 그리기

void CWinColorView::OnDraw(CDC* pDC)
{
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if (pDoc->m_InImg == NULL)
		return;
	height = pDoc->dibHi.biHeight;
	width = pDoc->dibHi.biWidth;
	rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount*pDoc->dibHi.biWidth);
	BmInfo->bmiHeader = pDoc->dibHi;

	SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height, 0, 0, 0, height, pDoc->m_InImg, BmInfo, DIB_RGB_COLORS);

}

void CWinColorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CWinColorView 인쇄

BOOL CWinColorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CWinColorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CWinColorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CWinColorView 진단

#ifdef _DEBUG
void CWinColorView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWinColorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWinColorDoc* CWinColorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinColorDoc)));
	return (CWinColorDoc*)m_pDocument;
}
#endif //_DEBUG


// CWinColorView 메시지 처리기


void CWinColorView::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->CopyCilpboard(pDoc->m_InImg, height, width, pDoc->dibHi.biBitCount);
}


void CWinColorView::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CWinColorView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(IsClipboardFormatAvailable(CF_DIB));
}


void CWinColorView::OnReverseImg()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int i, j, k, index;
	float r, g, b;
	for (i = 0; i < height; i++) {
		index = (height - i - 1)*rwsize;
		for (j = 0; j < width; j++) {
			r = (float)pDoc->m_InImg[index + 3 * j + 2];
			g = (float)pDoc->m_InImg[index + 3 * j + 1];
			b = (float)pDoc->m_InImg[index + 3 * j];

			r = 255 - r;
			g = 255 - g;
			b = 255 - b;

			pDoc->m_OutImg[index + 3 * j + 2] = (unsigned char)r;
			pDoc->m_OutImg[index + 3 * j + 1] = (unsigned char)g;
			pDoc->m_OutImg[index + 3 * j] = (unsigned char)b;
		}

	}
	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 24);
	AfxGetMainWnd() -> SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CWinColorView::OnUpdateReverseImg(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount == 24);
}


void CWinColorView::OnChannelspiltRgb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int i, j, k, index;
	for (k = 2; k >= 0; k--) {
		for (i = 0; i < height*rwsize; i++) {
			pDoc->m_OutImg[i] = 0;
		}

		for (i = 0; i < height; i++) {
			index = (height - i - 1)*rwsize;
			for (j = 0; j < width; j++) {
				pDoc->m_OutImg[index + 3 * j + k] = pDoc->m_InImg[index + 3 * j + k];
			}
		}
		pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 24);
		AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
	}
}


void CWinColorView::OnUpdateChannelspiltRgb(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount == 24);
}


void CWinColorView::OnGrayImg()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int i, j, k, index;
	float r, g, b;
	for (i = 0; i < height; i++) {
		index = (height - i - 1)*rwsize;
		for (j = 0; j < width; j++) {		
			r = (float)pDoc->m_InImg[index + 3 * j + 2];
			g = (float)pDoc->m_InImg[index + 3 * j + 1];
			b = (float)pDoc->m_InImg[index + 3 * j];

			// 흑백이미지의 rgb컬러 값은 동일함.
			// r,g,b 각각의 값을 동일하게 하기 위해 (r+g+b)/3 
			// 그렇게 저장된 인덱스는 명암도가 된다.?

			r = g = b = (r + g + b) / 3;

			// 출력할 이미지의 rgb값에 넣어준다.
			pDoc->m_OutImg[index + 3 * j + 2] = (unsigned char)r;
			pDoc->m_OutImg[index + 3 * j + 1] = (unsigned char)g;
			pDoc->m_OutImg[index + 3 * j] = (unsigned char)b;
		}

	}
	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 24);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);

}


void CWinColorView::OnUpdateGrayImg(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount == 24);
}

void CWinColorView::OnFrmAdd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->ImgLord();
	
	int i, j, index;
	float r, g, b;
	for (i = 0; i < height; i++) {
		index = (height - i - 1)*rwsize;
		for (j = 0; j < width; j++) {
			r = pDoc->m_InImg[index + 3 * j + 2] + pDoc->m_InImg2[index + 3 * j + 2];
			g = pDoc->m_InImg[index + 3 * j + 1] + pDoc->m_InImg2[index + 3 * j + 1];
			b = pDoc->m_InImg[index + 3 * j ] + pDoc->m_InImg2[index + 3 * j ];

			r = r > 255 ? 255 : r;
			g = g > 255 ? 255 : g;
			b = b > 255 ? 255 : b;

			pDoc->m_OutImg[index + 3 * j + 2] = (unsigned)r;
			pDoc->m_OutImg[index + 3 * j + 1] = (unsigned)g;
			pDoc->m_OutImg[index + 3 * j ] = (unsigned)b;
		}
	}

	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 24);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CWinColorView::OnUpdateFrmAdd(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount == 24);
}


void CWinColorView::OnLutMul()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	unsigned char LUT[256];

	for (int i = 0; i < 256; i++) {
		int temp = (int)((float)i*1.4);
		LUT[i] = temp > 255 ? 255 : temp;
	}

	int i, j, index;
	float r, g, b;
	for (i = 0;  i < height; i++) {

		index = (height - i - 1)*rwsize;
		for (j = 0; j < width; j++) {
			pDoc->m_OutImg[index + 3 * j + 2] = LUT[pDoc->m_InImg[index + 3 * j + 2]];
			pDoc->m_OutImg[index + 3 * j + 1] = LUT[pDoc->m_InImg[index + 3 * j + 1]];
			pDoc->m_OutImg[index + 3 * j] = LUT[pDoc->m_InImg[index + 3 * j ]];

		}

	}
	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 24);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);

}


void CWinColorView::OnUpdateLutMul(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount == 24);
}


void CWinColorView::OnImghisto()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_ImgHisto(height, width);
	pDoc->CopyCilpboard(pDoc->m_HistImg, 256, 256, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);

	delete[] pDoc->m_HistImg;
}


void CWinColorView::OnUpdateImghisto(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount == 8);
}


void CWinColorView::OnBinarization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	for (int i = 0; i < height; i++) {
		int index = (height - i - 1)*rwsize;
		for (int j = 0; j < width; j++) {
			if (pDoc->m_InImg[index + j] > 100)
				pDoc->m_OutImg[index + j] = 255;
			else
				pDoc->m_OutImg[index + j] = 0;
		}
	}

	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CWinColorView::OnUpdateBinarization(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount == 8);
}


void CWinColorView::OnHistoEqual()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_HistoEqual(height, width);
	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CWinColorView::OnUpdateHistoEqual(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount == 8);
}


void CWinColorView::OnHistoStretch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_HistoStretch(height, width);

	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CWinColorView::OnUpdateHistoStretch(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount == 8);
}


void CWinColorView::OnBoxFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_SmoothingBox(height, width);
	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}





void CWinColorView::OnGaussianFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_SmoothingGaussian(height, width);
	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CWinColorView::OnSharpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_Sharpening(height, width);
	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CWinColorView::OnLaplacian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_Laplacian(height, width);
	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CWinColorView::OnEdgeSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_EdgeSobel(height, width);
	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CWinColorView::OnEdgePrewitt()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_EdgePrewitt(height, width);
	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CWinColorView::OnMedianFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_MedianFilter(height, width);
	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void CWinColorView::OnMorphologyBinaryErosion()
{
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_AllocStructureElementBinary(4);
	pDoc->m_SetStructureElementBinary(0, 4, 0);
	pDoc->m_SetStructureElementBinary(1, 0, -1);
	pDoc->m_SetStructureElementBinary(2, 0, 0);
	pDoc->m_SetStructureElementBinary(3, 0, 1);

	pDoc->m_MorphologyBinaryErosion(height, width);

	pDoc->m_FreeStructureElemmentBinary();

	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CWinColorView::OnMorphologyBinaryDilation() {
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_AllocStructureElementBinary(4);
	pDoc->m_SetStructureElementBinary(0, 4, 0);
	pDoc->m_SetStructureElementBinary(1, 0, -1);
	pDoc->m_SetStructureElementBinary(2, 0, 0);
	pDoc->m_SetStructureElementBinary(3, 0, 1);

	pDoc->m_MorphologyBinaryDilation(height, width);

	pDoc->m_FreeStructureElemmentBinary();

	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);

}

void CWinColorView::OnMorphologyGrayErosion() {
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_AllocStructureElementGray(6);
	pDoc->m_SetStructureElementGray(0, 6, 0, 0);
	pDoc->m_SetStructureElementGray(1, -1, 0, 1);
	pDoc->m_SetStructureElementGray(2, 0, -1, 1);
	pDoc->m_SetStructureElementGray(3, 0, 0, 2);
	pDoc->m_SetStructureElementGray(4, 0, 1, 1);
	pDoc->m_SetStructureElementGray(5, 1, 0, 1);

	pDoc->m_MorphologyGrayErosion(height, width);

	pDoc->m_FreeStructureElemmentGray();

	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void CWinColorView::OnMorphologyGrayDilation() {
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_AllocStructureElementGray(6);
	pDoc->m_SetStructureElementGray(0, 6, 0, 0);
	pDoc->m_SetStructureElementGray(1, -1, 0, 1);
	pDoc->m_SetStructureElementGray(2, 0, -1, 1);
	pDoc->m_SetStructureElementGray(3, 0, 0, 2);
	pDoc->m_SetStructureElementGray(4, 0, 1, 1);
	pDoc->m_SetStructureElementGray(5, 1, 0, 1);

	pDoc->m_MorphologyGrayDilation(height, width);

	pDoc->m_FreeStructureElemmentGray();

	pDoc->CopyCilpboard(pDoc->m_OutImg, height, width, 8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);

}