#include "stdafx.h"
#include "resource.h"
/************************************************************************/
/* ��ͼƬ�ļ��浽���ݿ���                                               */
/************************************************************************/
BOOL  DBSavePic(CLongBinary * m_lobFile,CString FileFullName,CRecordset * m_Set,CString & error_info)
{
	CFile			f;
	//CFileStatus		fileStatus;
	CFileException	e;
	int             fenggefu;
	if (!f.Open(FileFullName,CFile::modeRead /*| CFile::typeBinary*/ ,&e))
	{
		fenggefu=FileFullName.ReverseFind('\\')==-1?FileFullName.ReverseFind('/'):FileFullName.ReverseFind('\\');
		if (fenggefu!=-1)
		{
			error_info.Format("�޷��ҵ�%s",FileFullName.Right(FileFullName.GetLength()-fenggefu+1));
		}
		else
		{
			error_info.Format("�޷��ҵ�%s",FileFullName);
		}
		return FALSE;
	}
	//f.GetStatus(fileStatus);
	DWORD dwSize = (DWORD)f.GetLength();
	try
	{
		m_lobFile->m_dwDataLength=dwSize;//(SQLUINTEGER)fileStatus.m_size;
		HGLOBAL hGlobal=GlobalAlloc(GMEM_MOVEABLE/*GPTR*/,dwSize/*(SIZE_T)fileStatus.m_size*/);
		m_lobFile->m_hData=::GlobalLock(hGlobal);
		f.Read(m_lobFile->m_hData,dwSize/*(UINT)fileStatus.m_size*/);
		m_Set->SetFieldDirty(m_lobFile);
		m_Set->SetFieldNull(m_lobFile,FALSE);
		::GlobalUnlock(hGlobal);
	}
	catch (...) {
		error_info=("����");
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* �����ݿ��ȡͼƬ                                                     */
/************************************************************************/
BOOL  DBOpenPic(CLongBinary * m_lobFile,CString  FileFullName/*����,����ΪNULL*/,CString & error_info,IPicture ** pic)
{
	LPSTR buffer=(LPSTR)GlobalLock(m_lobFile->m_hData);
	ASSERT(buffer!=NULL);
	//f.Write(buffer,m_Set.m_PIC.m_dwDataLength);			
	IStream *  pstm =NULL; // ��Ҫһ������stream��			
	CreateStreamOnHGlobal(buffer, TRUE, &pstm);
	ASSERT(pstm!=NULL);
	if (SUCCEEDED(OleLoadPicture(pstm,m_lobFile->m_dwDataLength,TRUE,IID_IPicture,(LPVOID*)pic)))
	{
		if (!FileFullName.IsEmpty())
		{
			DBOpenPicSaveAsFile(m_lobFile,FileFullName);
		}
		::GlobalUnlock(m_lobFile->m_hData);	
	}
	else{
		::GlobalUnlock(m_lobFile->m_hData);	
		pstm->Release();
		return FALSE;
	}
	pstm->Release();
	return TRUE;
}
/************************************************************************/
/* ���ļ����д洢                                                       */
/************************************************************************/
BOOL DBOpenPicSaveAsFile(CLongBinary * m_lobFile,CString  FileFullName/*����,����ΪNULL*/)
{
	CFile			f;
	CFileException	e;
	/*void */LPVOID buffer=(void *)GlobalLock(m_lobFile->m_hData);
	ASSERT(buffer!=NULL);
	if (!FileFullName.IsEmpty())
	{
		try
		{
			f.Open(FileFullName,CFile::modeCreate|CFile::modeWrite,&e);
			f.Write(buffer,m_lobFile->m_dwDataLength);
		}
		catch (CFileException * ee) {
			ee->ReportError(MB_OK);
		}			
	}
	::GlobalUnlock(m_lobFile->m_hData);	
	return TRUE;
}