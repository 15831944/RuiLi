#ifndef PICTURE_H
#ifndef picture_h

#define PICTURE_H
#define picture_h


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyPicture
{
public:
	CMyPicture();
	virtual ~CMyPicture();

public:
	BOOL LoadPicture(UINT nResource, LPCTSTR lpszResType)
	{return LoadPicture(MAKEINTRESOURCE(nResource), lpszResType);}
	BOOL LoadPictureFromFile(LPCTSTR lpszFileName);
	BOOL LoadPicture(LPCTSTR lpszResource,LPCTSTR lpszResType);
	//BOOL LoadPictureData(BYTE *pBuffer, int nSize);
	BOOL IsValid(){ return m_pPic!=NULL;}
	CSize GetSize(){return m_size;}
	void Draw(CDC* pDC, LPCRECT lprcDest, LPCRECT lprcSrc); 
	void Draw(CDC* pDC, int xDest,int yDest, int cxDest, int cyDest ,
		int xSrc ,int ySrc ,int cxSrc ,int cySrc); 

	void Release();

protected:
	IPicture*   m_pPic;

	OLE_XSIZE_HIMETRIC   _w_him;
	OLE_YSIZE_HIMETRIC   _h_him;

	CSize     m_size;

protected:
	void CalcSize();
};


#endif // define picture_h
#endif // define PICTURE_H


