
// WinColorDoc.cpp: CWinColorDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "WinColor.h"
#endif

#include "WinColorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWinColorDoc

IMPLEMENT_DYNCREATE(CWinColorDoc, CDocument)

BEGIN_MESSAGE_MAP(CWinColorDoc, CDocument)
END_MESSAGE_MAP()


// CWinColorDoc 생성/소멸

CWinColorDoc::CWinColorDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_InImg = NULL;
	m_OutImg = NULL;

	for (int i = 0; i < 256; i++) {
		palRGB[i].rgbBlue = palRGB[i].rgbGreen = palRGB[i].rgbRed = i;
		palRGB[i].rgbReserved = 0;
	}
}

CWinColorDoc::~CWinColorDoc()
{
	if (m_InImg)
		delete[]m_InImg;
	if (m_OutImg)
		delete[]m_OutImg;

}

BOOL CWinColorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.
	::OpenClipboard(NULL);
	if (!IsClipboardFormatAvailable(CF_DIB))
		return FALSE;
	HGLOBAL m_hImage = ::GetClipboardData(CF_DIB);
	::CloseClipboard();

	LPSTR pDIB = (LPSTR) ::GlobalLock((HGLOBAL)m_hImage);

	memcpy(&dibHi, pDIB, sizeof(BITMAPINFOHEADER));
	height = dibHi.biHeight;
	width = dibHi.biWidth;
	int rwsize = WIDTHBYTES(dibHi.biBitCount*width);
	DWORD dwBitsSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + rwsize * height * sizeof(char);
	m_InImg = new unsigned char[dibHi.biSizeImage];
	m_OutImg = new unsigned char[dibHi.biSizeImage];

	if (dibHi.biBitCount == 8) {
		memcpy(palRGB, pDIB + sizeof(BITMAPINFOHEADER), sizeof(RGBQUAD) * 256);
		memcpy(m_InImg, pDIB + dwBitsSize - dibHi.biSizeImage, dibHi.biSizeImage);
	}
	else
		memcpy(m_InImg, pDIB + sizeof(BITMAPINFOHEADER), dibHi.biSizeImage);


	dibHf.bfType = 0x4d42;
	dibHf.bfSize = dwBitsSize + sizeof(BITMAPFILEHEADER);
	if (dibHi.biBitCount == 24)
		dibHf.bfSize -= sizeof(RGBQUAD) * 256;
	dibHf.bfOffBits = dibHf.bfSize - rwsize * height * sizeof(char);
	dibHf.bfReserved1 = dibHf.bfReserved2 = 0;

	return TRUE;
}




// CWinColorDoc serialization

void CWinColorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CWinColorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CWinColorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWinColorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWinColorDoc 진단

#ifdef _DEBUG
void CWinColorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWinColorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWinColorDoc 명령


BOOL CWinColorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CFile hFile;
	hFile.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);
	hFile.Read(&dibHf, sizeof(BITMAPFILEHEADER));

	if (dibHf.bfType != 0x4D42)	{
		AfxMessageBox(L"Not BMP FILE!!");
		return FALSE;
	}
	hFile.Read(&dibHi, sizeof(BITMAPINFOHEADER));
	if (dibHi.biBitCount != 8 && dibHi.biBitCount != 24) {
		AfxMessageBox(L"Gray / True Color Possible!!");
		return FALSE;
	}
	if (dibHi.biBitCount == 8)
		hFile.Read(palRGB, sizeof(RGBQUAD) * 256);

	int ImgSize;
	if (dibHi.biBitCount == 8)
		ImgSize = hFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) - sizeof(RGBQUAD) * 256;
	else if (dibHi.biBitCount == 24)
		ImgSize = hFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
	
	m_InImg = new unsigned char[ImgSize];
	m_OutImg = new unsigned char[ImgSize];
	hFile.Read(m_InImg, ImgSize);
	hFile.Close();
	height = dibHi.biHeight;
	width = dibHi.biWidth;

	return TRUE;
}


BOOL CWinColorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CFile hFile;
	if (!hFile.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		return FALSE;
	hFile.Write(&dibHf, sizeof(BITMAPFILEHEADER));
	hFile.Write(&dibHi, sizeof(BITMAPINFOHEADER));
	if (dibHi.biBitCount == 8)
		hFile.Write(palRGB, sizeof(RGBQUAD) * 256);
	hFile.Write(m_InImg, dibHi.biSizeImage);
	hFile.Close();
	return TRUE;

	//return CDocument::OnSaveDocument(lpszPathName);
}


void CWinColorDoc::CopyCilpboard(unsigned char *m_CpyImg, int height, int width, int biBitCount)
{
	// TODO: 여기에 구현 코드 추가.
	int rwsize = WIDTHBYTES(biBitCount*width);
	DWORD dwBitsSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + rwsize * height * sizeof(char);

	HGLOBAL m_hImage = (HGLOBAL)::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	LPSTR pDIB = (LPSTR)::GlobalLock((HGLOBAL)m_hImage);

	BITMAPINFOHEADER dibCpyHi;
	memcpy(&dibCpyHi, &dibHi, sizeof(BITMAPINFOHEADER));
	dibCpyHi.biBitCount = biBitCount;
	dibCpyHi.biHeight = height;
	dibCpyHi.biWidth = width;
	dibCpyHi.biSizeImage = height * rwsize;
	if (biBitCount == 8)
		dibCpyHi.biClrUsed = dibCpyHi.biClrImportant = 256;

	memcpy(pDIB, &dibCpyHi, sizeof(BITMAPINFOHEADER));
	if (biBitCount == 8) {
		memcpy(pDIB + sizeof(BITMAPINFOHEADER), palRGB, sizeof(RGBQUAD) * 256);
		memcpy(pDIB + dwBitsSize - dibCpyHi.biSizeImage, m_CpyImg, dibCpyHi.biSizeImage);
	}
	else
		memcpy(pDIB + sizeof(BITMAPINFOHEADER), m_CpyImg, dibCpyHi.biSizeImage);


	::OpenClipboard(NULL);
	::SetClipboardData(CF_DIB, m_hImage);
	::CloseClipboard();

	::GlobalUnlock((HGLOBAL)m_hImage);
	GlobalFree(m_hImage);
}


void CWinColorDoc::ImgLord()
{
	// TODO: 여기에 구현 코드 추가.
	CFile file;

	CFileDialog opendlg(TRUE);
	if (opendlg.DoModal() == IDOK) {

		file.Open(opendlg.GetPathName(), CFile::modeRead);
		file.Read(&dibHf, sizeof(BITMAPFILEHEADER));
		file.Read(&dibHi, sizeof(BITMAPINFOHEADER));
		if (dibHi.biBitCount == 8)
			file.Read(palRGB, sizeof(RGBQUAD) * 256);
		int ImgSize;
		if (dibHi.biBitCount == 8)
			ImgSize = file.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) - 256 * sizeof(RGBQUAD);
		else if (dibHi.biBitCount == 24)
			ImgSize = file.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) ;

		m_InImg2 = new unsigned char[ImgSize];
		file.Read(m_InImg2, ImgSize);
		file.Close();
	}

	




}


void CWinColorDoc::m_ImgHisto(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int size = 256 * 256;
	m_HistImg = new unsigned char[size];
	
	int rwsize = WIDTHBYTES(dibHi.biBitCount*dibHi.biWidth);
	int gv, vmax, vmin;

	for (int i = 0; i < 256; i++) {
		m_HistoArr[i] = 0;
	}

	for (int i = 0; i < height; i++) {
		int index = (height - i - 1)*rwsize;
		for (int j = 0; j < width; j++) {
			gv = (int)m_InImg[index + j];
			m_HistoArr[gv]++;
		}

	}

	vmin = 1000000;
	vmax = 0;
	for (int i = 0; i < 256; i++) {
		if (m_HistoArr[i] <= vmin)
			vmin = m_HistoArr[i];
		if (m_HistoArr[i] >= vmax)
			vmax = m_HistoArr[i];
	}

	if (vmax == vmin)
		return;

	float vd = (float)(vmax - vmin);
	for (int i = 0; i < 256; i++) {
		m_HistoArr[i] = (int)(((float)m_HistoArr[i] - vmin) *255.0 / vd);
	}
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			m_HistImg[i * 256 + j] = 255;
		}
	}
	for (int j = 0; j < 256; j++) {
		for (int i = 0; i < m_HistoArr[j]; i++) {
			m_HistImg[i * 256 + j] = 0;
		}
	}

}


void CWinColorDoc::m_HistoEqual(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	unsigned int *histogram = new unsigned int[256];
	unsigned int *sum_hist = new unsigned int[256];

	for (int i = 0; i < 256; i++) {
		histogram[i] = 0;
	}

	int rwsize = WIDTHBYTES(dibHi.biBitCount*dibHi.biWidth);
	int index, gv;

	for (int i = 0; i < height; i++) {
		index = (height - i - 1)*rwsize;
		for (int j = 0; j < width; j++) {
			gv = (int)m_InImg[index + j];
			histogram[gv]++;
		}
	}

	int sum = 0;
	float scale_factor = 255.0f / (float)(height*width);

	for (int i = 0; i < 256; i++) {
		sum += histogram[i]++;
		sum_hist[i] = (int)((sum*scale_factor) + 0.5);
	}

	for (int i = 0; i < height; i++) {
		index = (height - i - 1)*rwsize;
		for (int j = 0; j < width; j++) {
			m_OutImg[index + j] = sum_hist[m_InImg[index + j]];
		}
	}

	delete[] histogram;
	delete[] sum_hist;

}


void CWinColorDoc::m_HistoStretch(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int lowvalue = 255, highvalue = 0;
	int index;
	int rwsize = WIDTHBYTES(dibHi.biBitCount*dibHi.biWidth);

	for (int i = 0; i < height; i++) {
		index = (height - i - 1)*rwsize;
		for (int j = 0; j < width; j++) {
			if (m_InImg[index + j] < lowvalue)
				lowvalue = m_InImg[index + j];
		}
	}


	for (int i = 0; i < height; i++) {
		index = (height - i - 1) * rwsize;
		for (int j = 0; j < width; j++) {
			if (m_InImg[index + j] > highvalue)
				highvalue = m_InImg[index + j];
		}
	}

	float mult = 255.0f / (float)(highvalue - lowvalue);
	for (int i = 0; i < height; i++) {
		index = (height - i - 1)*rwsize;
		for (int j = 0; j < width; j++) {
			m_OutImg[index + j] = (unsigned char)((m_InImg[index + j] - lowvalue) *mult);
		}
	}


}


void CWinColorDoc::m_SmoothingBox(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int MaskBox[3][3] = { {1, 1, 1 },
						  {1, 1, 1 },
						  {1, 1, 1 } };
	int mr, mc;
	int newValue;
	int divValue;

	int rwsize = WIDTHBYTES(dibHi.biBitCount*dibHi.biWidth);
	int index1, index2;
	for (int i = 0; i < height; i++) {
		index1 = (height - i - 1)*rwsize;
		for (int j = 0; j < width; j++) {
			m_OutImg[index1 + j] = m_InImg[index1 + j];
		}
	}

	for (int i = 1; i < height - 1; i++) {
		index1 = (height - i - 1)*rwsize;
		for (int j = 1; j < width - 1; j++) {
			newValue = 0;
			divValue = 0;
			for (mr = 0; mr < 3; mr++) {
				index2 = (height - (i + mr - 1) - 1)*rwsize;
				for (mc = 0; mc < 3; mc++) {
					newValue += (MaskBox[mr][mc] * m_InImg[index2 + (j + mc - 1)]);
					divValue += MaskBox[mr][mc];
				}
			}
			newValue /= divValue;
			m_OutImg[index1 + j] = (unsigned char)newValue;
		}
	}

}


void CWinColorDoc::m_SmoothingGaussian(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int MaskBox[3][3] = { {1, 2, 1 },
						  {2, 4, 2 },
						  {1, 2, 1 } };
	int mr, mc;
	int newValue;
	int divValue;

	int rwsize = WIDTHBYTES(dibHi.biBitCount*dibHi.biWidth);
	int index1, index2;
	for (int i = 0; i < height; i++) {
		index1 = (height - i - 1)*rwsize;
		for (int j = 0; j < width; j++) {
			m_OutImg[index1 + j] = m_InImg[index1 + j];
		}
	}

	for (int i = 1; i < height - 1; i++) {
		index1 = (height - i - 1)*rwsize;
		for (int j = 1; j < width - 1; j++) {
			newValue = 0;
			divValue = 0;
			for (mr = 0; mr < 3; mr++) {
				index2 = (height - (i + mr - 1) - 1)*rwsize;
				for (mc = 0; mc < 3; mc++) {
					newValue += (MaskBox[mr][mc] * m_InImg[index2 + (j + mc - 1)]);
					divValue += MaskBox[mr][mc];
				}
			}
			newValue /= divValue;
			m_OutImg[index1 + j] = (unsigned char)newValue;
		}
	}
}


void CWinColorDoc::m_Sharpening(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.

	int MaskBox[3][3] = { {-1, -1, -1 },
						  {-1, 9, -1 },
						  {-1, -1, -1 } };
	int mr, mc;
	int newValue;

	int rwsize = WIDTHBYTES(dibHi.biBitCount*dibHi.biWidth);
	int index1, index2;
	for (int i = 0; i < height; i++) {
		index1 = (height - i - 1)*rwsize;
		for (int j = 0; j < width; j++) {
			m_OutImg[index1 + j] = m_InImg[index1 + j];
		}
	}

	for (int i = 1; i < height - 1; i++) {
		index1 = (height - i - 1)*rwsize;
		for (int j = 1; j < width - 1; j++) {
			newValue = 0;
			for (mr = 0; mr < 3; mr++) {
				index2 = (height - (i + mr - 1) - 1)*rwsize;
				for (mc = 0; mc < 3; mc++) {
					newValue += (MaskBox[mr][mc] * m_InImg[index2 + (j + mc - 1)]);

				}
			}
			if (newValue > 255)
				newValue = 255;
			else if (newValue < 0)
				newValue = 0;
			m_OutImg[index1 + j] = (unsigned char)newValue;
		}
	}
}


void CWinColorDoc::m_Laplacian(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int MaskBox[3][3] = { {-1, -1, -1 },
						  {-1, 8, -1 },
						  {-1, -1, -1 } };
	unsigned char *pTmpImg;

	pTmpImg = new unsigned char[height*width];

	int rwsize = WIDTHBYTES(dibHi.biBitCount*dibHi.biWidth);
	int index1, index2;
	for (int i = 0; i < height; i++) {
		index1 = (height - i - 1)*rwsize;
		for (int j = 0; j < width; j++) {
			m_OutImg[index1 + j] = 0;
			pTmpImg[i*width + j] = 0;
		}
	}

	int newValue;
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width; j++) {
			newValue = 0;
			for(int mr = 0; mr<3; mr++) {
				index2 = (height - (i + mr - 1) - 1)*rwsize;
				for (int mc = 0; mc < 3; mc++) {
					newValue += (MaskBox[mr][mc] * m_InImg[index2 + (j + mc - 1)]);
				}
			}
			//값을 양수로 변환
			if (newValue < 0)
				newValue = -newValue;
			pTmpImg[i*width + j] = newValue;
		}
	}

	//디스플레이를 위해서 0에서 255사이로 값의 범위를 매핑
	//이를 위해 먼저 최대, 최소값으 찾은 후 이를 이용하여 매핑한다.
	int min = (int) 10e+10;
	int max = (int) -10e+10;
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	for (int i = 1; i < heightm1; i++) {
		for (int j = 0; j < widthm1; j++) {
			newValue = pTmpImg[i*width + j];
			if (newValue < min)
				min = newValue;
			if (newValue > max)
				max = newValue;
		}
	}

	float constVal1 = (float)(255.0 / (max - min));
	float constVal2 = (float)(-255.0*min / (max - min));
	for (int i = 1; i < heightm1; i++) {
		int index = (height - i - 1)*rwsize;
		for (int j = 1; j < widthm1; j++) {
			newValue = pTmpImg[i*width + j];
			newValue = constVal1 * newValue + constVal2;
			m_OutImg[index + j] = (unsigned char)newValue;
		}
	}

	delete[] pTmpImg;
}


void CWinColorDoc::m_EdgeSobel(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int MaskSobelX[3][3] = { {-1, 0, 1 },
							 {-2, 0, 2 },
							 {-1, 0, 1 } };
	int MaskSobelY[3][3] = { { 1, 2, 1 },
							 { 0, 0, 0 },
							 {-1,-2, -1 } };

	int rwsize = WIDTHBYTES(dibHi.biBitCount*dibHi.biWidth);
	int *pImgSobelX, *pImgSobelY;
	int newValue;

	pImgSobelX = new int[height*width];
	pImgSobelY = new int[height*width];


	for (int i = 0; i < height; i++) {
		int index = (height - i - 1)* rwsize;
		for (int j = 0; j < width; j++) {
			m_OutImg[index + j] = 0;
			pImgSobelX[i*width + j] = 0;
			pImgSobelY[i*width + j] = 0;
		}
	}

	for (int i = 1; i < height; i++) {
		for (int j = 1; j < width - 1; j++) {
			newValue = 0;
			for (int mr = 0; mr < 3; mr++) {
				int maskIdx = (height - (i + mr - 1) - 1)*rwsize;
				for (int mc = 0; mc < 3; mc++) {
					newValue += (MaskSobelX[mr][mc] * m_InImg[maskIdx + (j + mc - 1)]);
				}
			}
			pImgSobelX[i*width + j] = newValue;
		}
	}
	// Y방향
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			newValue = 0;
			for (int mr = 0; mr < 3; mr++) {
				int maskIdx = (height - (i + mr - 1) - 1)*rwsize;
				for (int mc = 0; mc < 3; mc++) {
					newValue += (MaskSobelY[mr][mc] * m_InImg[maskIdx + (j + mc - 1)]);
				}
			}
			pImgSobelY[i*width + j] = newValue;
		}
	}

	float constVal1, constVal2;
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			constVal1 = pImgSobelX[i*width + j];
			constVal2 = pImgSobelY[i*width + j];
			if (constVal1 < 0)
				constVal1 = -constVal1;
			if (constVal2 < 0)
				constVal2 = -constVal2;
			pImgSobelX[i*width + j] = constVal1 + constVal2;
		}
	}


	int min = 999999;
	int max = -999999;
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			newValue = pImgSobelX[i*width + j];
			if (newValue < min)
				min = newValue;
			if (newValue > max)
				max = newValue;
		}
	}

	constVal1 = (float)(255.0 / (max - min));
	constVal2 = (float)(-255.0*min / (max - min));
	for (int i = 1; i < height - 1; i++) {
		int index = (height - i - 1) *rwsize;
		for (int j = 1; j < width - 1; j++) {
			newValue = pImgSobelX[i*width + j];
			newValue = constVal1 * newValue + constVal2;
			m_OutImg[index + j] = (unsigned char)newValue;
		}
	}
	delete[] pImgSobelX;
	delete[] pImgSobelY;
}


void CWinColorDoc::m_EdgePrewitt(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int MaskPrewittX[3][3] = { {-1, 0, 1 },
						 	   {-1, 0, 1 },
							   {-1, 0, 1 } };
	int MaskPrewittY[3][3] = { { 1, 1, 1 },
							   { 0, 0, 0 },
							   {-1,-1, -1 } };

	int rwsize = WIDTHBYTES(dibHi.biBitCount*dibHi.biWidth);
	int *pImgPrewittX, *pImgPrewittY;
	int newValue;

	pImgPrewittX = new int[height*width];
	pImgPrewittY = new int[height*width];

	for (int i = 0; i < height; i++) {
		int index = (height - i - 1)* rwsize;
		for (int j = 0; j < width; j++) {
			m_OutImg[index + j] = 0;
			pImgPrewittX[i*width + j] = 0;
			pImgPrewittY[i*width + j] = 0;
		}
	}

	for (int i = 1; i < height; i++) {
		for (int j = 1; j < width - 1; j++) {
			newValue = 0;
			for (int mr = 0; mr < 3; mr++) {
				int maskIdx = (height - (i + mr - 1) - 1)*rwsize;
				for (int mc = 0; mc < 3; mc++) {
					newValue += (MaskPrewittX[mr][mc] * m_InImg[maskIdx + (j + mc - 1)]);
				}
			}
			pImgPrewittX[i*width + j] = newValue;
		}
	}
	//Y방향
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			newValue = 0;
			for (int mr = 0; mr < 3; mr++) {
				int maskIdx = (height - (i + mr - 1) - 1)*rwsize;
				for (int mc = 0; mc < 3; mc++) {
					newValue += (MaskPrewittY[mr][mc] * m_InImg[maskIdx + (j + mc - 1)]);
				}
			}
			pImgPrewittY[i*width + j] = newValue;
		}
	}

	float constVal1, constVal2;
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			constVal1 = pImgPrewittX[i*width + j];
			constVal2 = pImgPrewittY[i*width + j];
			if (constVal1 < 0)
				constVal1 = -constVal1;
			if (constVal2 < 0)
				constVal2 = -constVal2;
			pImgPrewittX[i*width + j] = constVal1 + constVal2;
		}
	}


	int min = 999999;
	int max = -999999;
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			newValue = pImgPrewittX[i*width + j];
			if (newValue < min)
				min = newValue;
			if (newValue > max)
				max = newValue;
		}
	}

	constVal1 = (float)(255.0 / (max - min));
	constVal2 = (float)(-255.0*min / (max - min));
	for (int i = 1; i < height - 1; i++) {
		int index = (height - i - 1) *rwsize;
		for (int j = 1; j < width - 1; j++) {
			newValue = pImgPrewittX[i*width + j];
			newValue = constVal1 * newValue + constVal2;
			m_OutImg[index + j] = (unsigned char)newValue;
		}
	}
	delete[] pImgPrewittX;
	delete[] pImgPrewittY;

}


void CWinColorDoc::m_MedianFilter(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int median[9] = { 0 };
	int rwsize = WIDTHBYTES(dibHi.biBitCount*dibHi.biWidth);
	int temp;

	for (int i = 1; i < height - 1; i++) {
		//int indexm1 = (height - i - 2) *rwsize;
		int index = (height - i - 1) *rwsize;
		//int indexp1 = (height - i)*rwsize;
		for (int j = 1; j < width-1; j++) {
			for (int mr = 0; mr < 3; mr++) {
				int index2 = (height - (i + mr - 1) - 1)*rwsize;
				for (int mc = 0; mc < 3; mc++) {
					median[mr * 3 + mc] = m_InImg[index2 + (j + mc - 1)];
				}
			}
			for (int k = 8; k > 0; k--) {
				for (int w = 0; w < k; w++) {
					if (median[w] > median[w + 1]) {
						temp = median[w + 1];
						median[w + 1] = median[w];
						median[w] = temp;
					}
				}
			}
			m_OutImg[index + j] = median[4];
		}

	}
}

#define BACKGROUND 255
#define FOREGROUND 0

typedef struct {
	int row, col;
}	StructureElementBinary, * pStructureElementBinary;
pStructureElementBinary m_pSEBinary;

void CWinColorDoc::m_AllocStructureElementBinary(int HowMany)
{	
	// TODO: 여기에 구현 코드 추가.
	m_pSEBinary = new StructureElementBinary[HowMany];
}


void CWinColorDoc::m_FreeStructureElemmentBinary()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_pSEBinary != NULL)
		delete[] m_pSEBinary;
}


void CWinColorDoc::m_SetStructureElementBinary(int which, int row, int col)
{
	// TODO: 여기에 구현 코드 추가.
	m_pSEBinary[which].row = row;
	m_pSEBinary[which].col = col;
}


void CWinColorDoc::m_MorphologyBinaryErosion(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int flagPassed;

	int mx, my;

	float sum = 0;
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			sum += m_InImg[r * width + c];
		}
	}
	float mean = sum / (height * width);

	
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			if (m_InImg[r * width + c] > mean)
				m_InImg[r * width + c] = FOREGROUND;
			else
				m_InImg[r * width + c] = BACKGROUND;
		}
	}

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			m_OutImg[r * width + c] = BACKGROUND;
		}
	}

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			flagPassed = 1;
			for (int i = 1; i < m_pSEBinary[0].row; i++) {
				mx = c + m_pSEBinary[i].col;
				my = r + m_pSEBinary[i].row;
				if(mx>= 0 && mx<width && my >= 0 && my< height)
					if (m_InImg[my * width + mx] == BACKGROUND) {
						flagPassed = 0;
						break;
					}
			}
			if (flagPassed)
				m_OutImg[r * width + c] = FOREGROUND;
		}
	}
}



void CWinColorDoc::m_MorphologyBinaryDilation(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int flagPassed;

	int mx, my;

	float sum = 0;
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			sum += m_InImg[r * width + c];
		}
	}
	float mean = sum / (height * width);
	
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			if (m_InImg[r * width + c] > mean)
				m_InImg[r * width + c] = FOREGROUND;
			else
				m_InImg[r * width + c] = BACKGROUND;
		}
	}

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			m_OutImg[r * width + c] = BACKGROUND;
		}
	}

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			flagPassed = 0;
			for (int i = 1; i < m_pSEBinary[0].row; i++) {
				mx = c + m_pSEBinary[i].col;
				my = r + m_pSEBinary[i].row;
				if (mx >= 0 && mx < width && my >= 0 && my < height)
					if (m_InImg[my * width + mx] == FOREGROUND) {
						flagPassed = 1;
						break;
					}
			}
			if (flagPassed)
				m_OutImg[r * width + c] = FOREGROUND;
		}
	}
}

typedef struct {
	int row, col;
	int grayval;
}StructureElementGray, * pStructureElementGray;
pStructureElementGray m_pSEGray;

void CWinColorDoc::m_AllocStructureElementGray(int HowMany)
{
	// TODO: 여기에 구현 코드 추가.
	m_pSEGray = new StructureElementGray[HowMany];
}

void CWinColorDoc::m_FreeStructureElemmentGray()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_pSEGray != NULL)
		delete[] m_pSEGray;
}


void CWinColorDoc::m_SetStructureElementGray(int which, int row, int col, int grayval)
{
	// TODO: 여기에 구현 코드 추가.
	m_pSEGray[which].row = row;
	m_pSEGray[which].col = col;
	m_pSEGray[which].grayval = grayval;
}


void CWinColorDoc::m_MorphologyGrayErosion(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int mx, my;
	int min, max, diff;

	int* pTmpImg = new int[height * width];

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			pTmpImg[r*width + c] = 0;
		}
	}

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			min = m_InImg[r * width + c];

			for (int i = 1; i < m_pSEGray[0].row; i++) {
				mx = c + m_pSEGray[i].col;
				my = r + m_pSEGray[i].row;

				if (mx >= 0 && mx < width && my >= 0 && my < height) {
					diff = m_InImg[my * width + mx] - m_pSEGray[i].grayval;
					if (diff < min)
						min = diff;
				}
			}
			pTmpImg[r * width + c] = min;
		}
	}

	min = 999999;
	max = -999999;
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			diff = pTmpImg[r * width + c];
			if (diff < min)
				min = diff;
			if (diff > max)
				max = diff;
		}
	}

	float constVal1 = (float)(255.0 / (max - min));
	float constVal2 = (float)(-255.0 / (max - min));
	float newValue;
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			newValue = pTmpImg[r * width + c];
			newValue = constVal1 * newValue +constVal2;
			m_OutImg[r * width + c] = (unsigned char)newValue;
		}
	}

	delete[] pTmpImg;
}


void CWinColorDoc::m_MorphologyGrayDilation(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int mx, my;
	int min, max, sum;

	int* pTmpImg = new int[height * width];

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			pTmpImg[r * width + c] = 0;
		}
	}

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			max = m_InImg[r * width + c];
			for (int i = 1; i < m_pSEGray[0].row; i++) {
				mx = c + m_pSEGray[i].col;
				my = r + m_pSEGray[i].row;

				if (mx >= 0 && mx < width && my >= 0 && my < height) {
					sum = m_InImg[my * width + mx] - m_pSEGray[i].grayval;
					if (sum > max)
						max = sum;
				}
			}
			pTmpImg[r * width + c] = max;
		}
	}

	min = 999999;
	max = -999999;
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			sum = pTmpImg[r * width + c];
			if (sum < min)
				min = sum;
			if (sum > max)
				max = sum;
		}
	}

	float constVal1 = (float)(255.0 / (max - min));
	float constVal2 = (float)(-255.0 / (max - min));
	float newValue;
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			newValue = pTmpImg[r * width + c];
			newValue = constVal1 * newValue + constVal2;
			m_OutImg[r * width + c] = (unsigned char)newValue;
		}
	}

	delete[] pTmpImg;
}
