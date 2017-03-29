#include <ocidl.h> 
#include <olectl.h> 

// lpstrFile���ļ���������ǰ���·�����ڣ� 
// hWnd 
// nScrWidth�����ͼƬ�Ŀ�� 
// nScrHeight�����ͼƬ�ĸ߶� 
// nScrWidth��nScrHeight�����ͼƬ��ʵ�ʴ�С��������֮��ָ���ź�ĳߴ� 

HRESULT ShowPic(char *lpstrFile,HWND hWnd,int nScrWidth,int nScrHeight) 
{ 
	HDC hDC_Temp=GetDC(hWnd); 

	IPicture *pPic; 
	IStream *pStm; 

	BOOL bResult; 

	HANDLE hFile=NULL; 
	DWORD dwFileSize,dwByteRead; 

	//��Ӳ���е�ͼ���ļ� 
	hFile=CreateFile(lpstrFile,GENERIC_READ, 
		FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL); 

	if (hFile!=INVALID_HANDLE_value) 
	{ 
		dwFileSize=GetFileSize(hFile,NULL);//��ȡ�ļ��ֽ��� 

		if (dwFileSize==0xFFFFFFFF) 
			return E_FAIL; 
	} 
	else 
	{ 
		return E_FAIL; 
	} 


	//����ȫ�ִ洢�ռ� 
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize); 
	LPVOID pvData = NULL; 

	if (hGlobal == NULL) 
		return E_FAIL; 

	if ((pvData = GlobalLock(hGlobal)) == NULL)//���������ڴ�� 
		return E_FAIL; 

	ReadFile(hFile,pvData,dwFileSize,&dwByteRead,NULL);//���ļ������ڴ滺���� 

	GlobalUnlock(hGlobal); 

	CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 

	//װ��ͼ���ļ� 
	bResult=OleLoadPicture(pStm,dwFileSize,TRUE,IID_IPicture,(LPVOID*)&pPic); 

	if(FAILED(bResult)) 
		return E_FAIL; 

	OLE_XSIZE_HIMETRIC hmWidth;//ͼƬ����ʵ��� 
	OLE_YSIZE_HIMETRIC hmHeight;//ͼƬ����ʵ�߶� 
	pPic->get_Width(&hmWidth); 
	pPic->get_Height(&hmHeight); 


	//��ͼ���������Ļ�ϣ��е���BitBlt�� 
	bResult=pPic->Render(hDC_Temp,0,0,nScrWidth,nScrHeight, 
		0,hmHeight,hmWidth,-hmHeight,NULL); 

	pPic->Release(); 

	CloseHandle(hFile);//�رմ򿪵��ļ� 

	if (SUCCEEDED(bResult)) 
	{ 
		return S_OK; 
	} 
	else 
	{ 
		return E_FAIL; 
	} 
} 
