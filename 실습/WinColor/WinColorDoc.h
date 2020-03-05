
// WinColorDoc.h: CWinColorDoc 클래스의 인터페이스
//


#pragma once
#define WIDTHBYTES(bits)	( ( (bits) +31) /32*4)


class CWinColorDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CWinColorDoc() noexcept;
	DECLARE_DYNCREATE(CWinColorDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CWinColorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	BITMAPFILEHEADER dibHf;
	BITMAPINFOHEADER dibHi;
	RGBQUAD palRGB[256];
	unsigned char *m_InImg;
	unsigned char *m_OutImg;
	int height;
	int width;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	void CopyCilpboard(unsigned char *m_CpyImg, int height, int width, int biBitCount);
	unsigned char *m_InImg2;
	void ImgLord();
	int m_HistoArr[256];
	void m_ImgHisto(int height, int width);
	unsigned char *m_HistImg;
	void m_HistoEqual(int height, int width);
	void m_HistoStretch(int height, int width);
	void m_SmoothingBox(int height, int width);
	void m_SmoothingGaussian(int height, int width);
	void m_Sharpening(int height, int width);
	void m_Laplacian(int height, int width);
	void m_EdgeSobel(int height, int width);
	void m_EdgePrewitt(int height, int width);
	void m_MedianFilter(int height, int width);
	void m_AllocStructureElementBinary(int HowMany);
	void m_FreeStructureElemmentBinary();
	void m_SetStructureElementBinary(int which, int row, int col);
	void m_MorphologyBinaryErosion(int height, int width);
	void m_MorphologyBinaryDilation(int height, int width);
	void m_AllocStructureElementGray(int HowMany);
	void m_FreeStructureElemmentGray();
	void m_SetStructureElementGray(int which, int row, int col, int grayval);
	void m_MorphologyGrayErosion(int height, int width);
	void m_MorphologyGrayDilation(int height, int width);
};
