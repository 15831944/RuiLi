#include <ocidl.h> 
#include <olectl.h> 

// lpstrFile：文件名（最好是包含路径在内） 
// hWnd 
// nScrWidth：输出图片的宽度 
// nScrHeight：输出图片的高度 
// nScrWidth、nScrHeight是输出图片的实际大小，换而言之是指缩放后的尺寸 

HRESULT ShowPic(char *lpstrFile,HWND hWnd,int nScrWidth,int nScrHeight) 
{ 
	HDC hDC_Temp=GetDC(hWnd); 

	IPicture *pPic; 
	IStream *pStm; 

	BOOL bResult; 

	HANDLE hFile=NULL; 
	DWORD dwFileSize,dwByteRead; 

	//打开硬盘中的图形文件 
	hFile=CreateFile(lpstrFile,GENERIC_READ, 
		FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL); 

	if (hFile!=INVALID_HANDLE_value) 
	{ 
		dwFileSize=GetFileSize(hFile,NULL);//获取文件字节数 

		if (dwFileSize==0xFFFFFFFF) 
			return E_FAIL; 
	} 
	else 
	{ 
		return E_FAIL; 
	} 


	//分配全局存储空间 
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize); 
	LPVOID pvData = NULL; 

	if (hGlobal == NULL) 
		return E_FAIL; 

	if ((pvData = GlobalLock(hGlobal)) == NULL)//锁定分配内存块 
		return E_FAIL; 

	ReadFile(hFile,pvData,dwFileSize,&dwByteRead,NULL);//把文件读入内存缓冲区 

	GlobalUnlock(hGlobal); 

	CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 

	//装入图形文件 
	bResult=OleLoadPicture(pStm,dwFileSize,TRUE,IID_IPicture,(LPVOID*)&pPic); 

	if(FAILED(bResult)) 
		return E_FAIL; 

	OLE_XSIZE_HIMETRIC hmWidth;//图片的真实宽度 
	OLE_YSIZE_HIMETRIC hmHeight;//图片的真实高度 
	pPic->get_Width(&hmWidth); 
	pPic->get_Height(&hmHeight); 


	//将图形输出到屏幕上（有点像BitBlt） 
	bResult=pPic->Render(hDC_Temp,0,0,nScrWidth,nScrHeight, 
		0,hmHeight,hmWidth,-hmHeight,NULL); 

	pPic->Release(); 

	CloseHandle(hFile);//关闭打开的文件 

	if (SUCCEEDED(bResult)) 
	{ 
		return S_OK; 
	} 
	else 
	{ 
		return E_FAIL; 
	} 
} 
