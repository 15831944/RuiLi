#include "stdafx.h"
#include "Picture.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyPicture::CMyPicture()
:m_pPic(NULL), _h_him(0), _w_him(0), m_size(0,0)
{
}

CMyPicture::~CMyPicture()
{
	Release();
}

void CMyPicture::Release()
{
	if(m_pPic != NULL)
	{
		m_pPic->Release();
		m_pPic = NULL;
		_h_him = _w_him = 0;
		m_size.cx = m_size.cy = 0;
	}
}

BOOL CMyPicture::LoadPicture(LPCTSTR lpszResource, LPCTSTR lpszResType)
{
	Release();

	HINSTANCE  hInst = AfxFindResourceHandle(lpszResource, lpszResType);
	HRSRC hRsrc = ::FindResource(hInst, lpszResource, lpszResType);
	if(hRsrc == NULL) return FALSE;
	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);

	if(hGlobal == NULL) return FALSE;


	DWORD dwSize = SizeofResource(hInst, hRsrc);

	HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
	if(hMem == NULL) return  FALSE;

	LPVOID  pSrc = ::LockResource(hGlobal);
	if(pSrc == NULL) {
		::GlobalFree(hMem);
		return FALSE;
	}

	LPVOID pDes = ::GlobalLock(hMem);
	if(pDes == NULL){
		//::GlobalUnlock(hGlobal);
		::GlobalFree(hMem);
		return FALSE;
	}

	memcpy(pDes, pSrc, dwSize);

	//GlobalUnlock(hGlobal);
	GlobalUnlock(hMem);

	::FreeResource(hGlobal);

	IStream* pStm = NULL;
	CreateStreamOnHGlobal(hMem, TRUE, &pStm);

	if(!SUCCEEDED(OleLoadPicture(pStm,dwSize,TRUE,IID_IPicture,(LPVOID*)&m_pPic)))
	{
		pStm -> Release();
		::GlobalFree(hMem);
		pStm = NULL;
		return FALSE;
	}

	pStm->Release();
	::GlobalFree(hMem);

	CalcSize();
	return TRUE;

}

BOOL CMyPicture::LoadPictureFromFile(LPCTSTR lpszFileName)
{
	Release();

	CFile   file;
	if(!file.Open(lpszFileName, CFile::modeRead))
		return FALSE;

	DWORD dwSize = (DWORD)file.GetLength();

	HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
	if(hMem == NULL) return  FALSE;

	LPVOID pDes = ::GlobalLock(hMem);
	if(pDes == NULL){
		::GlobalFree(hMem);
		return FALSE;
	}

	file.Read(pDes, dwSize);

	file.Close();

	GlobalUnlock(hMem);

	IStream* pStm = NULL;
	CreateStreamOnHGlobal(hMem, TRUE, &pStm);

	if(!SUCCEEDED(OleLoadPicture(pStm,dwSize,TRUE,IID_IPicture,(LPVOID*)&m_pPic)))
	{
		pStm -> Release();
		::GlobalFree(hMem);
		pStm = NULL;
		return FALSE;
	}

	pStm->Release();
	::GlobalFree(hMem);

	CalcSize();
	return TRUE;

}


void CMyPicture::CalcSize()
{
	if(m_pPic == NULL) return;

	m_pPic->get_Width(&_w_him);
	m_pPic->get_Height(&_h_him);

	CDC* pDC = CWnd::GetDesktopWindow()->GetDC();
	m_size.cx = _w_him;
	m_size.cy = _h_him;

	pDC->HIMETRICtoDP(&m_size);

	CWnd::GetDesktopWindow()->ReleaseDC(pDC);

}

void CMyPicture::Draw(CDC* pDC, LPCRECT lprcDest, LPCRECT lprcSrc)
{
	if(m_pPic)
	{
		CSize  szOrig(lprcSrc->left, lprcSrc->top);
		CSize  szSrc(lprcSrc->right - lprcSrc->left, lprcSrc->bottom - lprcSrc->top);
		pDC->DPtoHIMETRIC(&szOrig);
		pDC->DPtoHIMETRIC(&szSrc);

		m_pPic->Render(*pDC, lprcDest->left,lprcDest->top,lprcDest->right-lprcDest->left,
			lprcDest->bottom-lprcDest->top, szOrig.cx, _h_him-szOrig.cy, szSrc.cx,
			-szSrc.cy, NULL);
	}
}

void CMyPicture::Draw(CDC* pDC, int xDest,int yDest, int cxDest, int cyDest ,
					int xSrc ,int ySrc ,int cxSrc ,int cySrc)
{
	Draw(pDC, CRect(xDest, yDest, xDest+cxDest, yDest+cyDest), CRect(xSrc, ySrc, xSrc+cxSrc, ySrc+cySrc));
}

