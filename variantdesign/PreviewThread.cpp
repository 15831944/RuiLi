// PreviewThread.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "PreViewDlg.h"
#include "PreviewThread.h"

UINT MyPreviewThreadProc(LPVOID pParam);


// CPreviewThread

CPreviewThread::CPreviewThread()
{
	m_ModalFlag=0;
	m_2DFlag=0;
	m_strID="";
	pPreviewDlg=NULL;
}

CPreviewThread::~CPreviewThread()
{
}


// CPreviewThread 成员函数
int CPreviewThread::Begin()
{
	if(m_ModalFlag==0) return 0;
	if(m_strID=="") return 0;

	AfxMessageBox("HELLO,YOU GOT IT!");
	
	pPreviewDlg=new CPreViewDlg();
	pPreviewDlg->m_ModalFlag=m_ModalFlag;//零件模型
	pPreviewDlg->m_2DFlag=m_2DFlag;//二维图
	pPreviewDlg->m_strID=m_strID;//模型ID

	pPreviewDlg->Create(IDD_DIALOG_PREVIEW,NULL);
	pPreviewDlg->ShowWindow(SW_SHOW);
	pPreviewDlg->UpdateWindow();

	//AfxBeginThread(MyPreviewThreadProc,(LPVOID)this);
	return 1;
}



UINT MyPreviewThreadProc(LPVOID pParam)
{
	CPreviewThread * pThread=(CPreviewThread *)pParam;
	AfxMessageBox("HELLO,YOU GOT IT!");
	//CPreViewDlg dlg;
	//dlg.m_ModalFlag=pThread->m_ModalFlag;//零件模型
	//dlg.m_2DFlag=pThread->m_2DFlag;//二维图
	//dlg.m_strID=pThread->m_strID;//模型ID
	//dlg.DoModal();

CPreViewDlg * pPreviewDlg=NULL;

   //  CPreViewDlg * pPreviewDlg=pThread->pPreviewDlg;
	//if(pPreviewDlg!=NULL)
	//{
	// delete pPreviewDlg;
	//}

	pPreviewDlg=new CPreViewDlg();
	pPreviewDlg->Create(IDD_DIALOG_PREVIEW,NULL);
	pPreviewDlg->ShowWindow(SW_SHOW);
	pPreviewDlg->UpdateWindow();


	//Sleep(100);
	return false;
}