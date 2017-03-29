// PreviewThread.cpp : ʵ���ļ�
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


// CPreviewThread ��Ա����
int CPreviewThread::Begin()
{
	if(m_ModalFlag==0) return 0;
	if(m_strID=="") return 0;

	AfxMessageBox("HELLO,YOU GOT IT!");
	
	pPreviewDlg=new CPreViewDlg();
	pPreviewDlg->m_ModalFlag=m_ModalFlag;//���ģ��
	pPreviewDlg->m_2DFlag=m_2DFlag;//��άͼ
	pPreviewDlg->m_strID=m_strID;//ģ��ID

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
	//dlg.m_ModalFlag=pThread->m_ModalFlag;//���ģ��
	//dlg.m_2DFlag=pThread->m_2DFlag;//��άͼ
	//dlg.m_strID=pThread->m_strID;//ģ��ID
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