// PicForDBDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "PicForDBDlg.h"
#include "partPicSet.h"
#include "CAsmPicSet.h"
#include "Part_Temp_Pic.h"
#include "AsmTempPic.h"
// CPicForDBDlg 对话框

IMPLEMENT_DYNAMIC(CPicForDBDlg, CDialog)
CPicForDBDlg::CPicForDBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicForDBDlg::IDD, pParent)
	, pic_dlg(NULL)
	, p_Picture(NULL)
	, current_pic(-1)
	, p_db(NULL)
{
}

CPicForDBDlg::~CPicForDBDlg()
{
	if (p_Picture!=NULL)
	{
		p_Picture->Release();
		p_Picture=NULL;
	}
	if (pic_dlg!=NULL)
	{
		delete pic_dlg;
	}
}

void CPicForDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, m_pic);
	DDX_Control(pDX, IDC_BUTTON1, m_btnPrePic);
	DDX_Control(pDX, IDC_BUTTON2, m_btnNextPic);
	DDX_Control(pDX, IDC_BUTTON3, m_viewPicInDlg);
}


BEGIN_MESSAGE_MAP(CPicForDBDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CPicForDBDlg 消息处理程序
/************************************************************************/
/* 上一张                                                               */
/************************************************************************/
void CPicForDBDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (current_pic>0)
	{
		if (p_Picture!=NULL)
		{
			p_Picture->Release();
			p_Picture=NULL;
		}
		ShowPicItem(&p_PicItemList[--current_pic]);
		m_btnNextPic.EnableWindow(TRUE);
	}
	if (current_pic==0)
	{
		//if (p_Picture!=NULL)
		//{
		//	p_Picture->Release();
		//	p_Picture=NULL;
		//}
		m_btnPrePic.EnableWindow(FALSE);
	}
}
/************************************************************************/
/* 下一张                                                               */
/************************************************************************/
void CPicForDBDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (current_pic<p_PicItemList.GetCount()-1)
	{
		ShowPicItem(&p_PicItemList[++current_pic]);
		m_btnPrePic.EnableWindow(TRUE);
	}
	if (current_pic==p_PicItemList.GetCount()-1)
	{
		m_btnNextPic.EnableWindow(FALSE);
	}
}
bool CPicForDBDlg::ShowPicItem(pic_item * temp)
{
	CpartPicSet		m_partPic(p_db);
	CAsmPicSet		m_asmPic(p_db);
	CPart_Temp_Pic  m_partTempPic(p_db);
	CAsmTempPic     m_asmTempPic(p_db);
	CString         error_info;
	if(p_db==NULL) return false;
	switch(temp->setType) {
	case 0:
		m_partPic.m_strFilter.Format("[PrtNum]=%ld AND [Number]=%ld",temp->mdlNum,temp->picNum);
		m_partPic.Open();
		if (!m_partPic.IsBOF())
		{
			m_partPic.MoveFirst();
			DBOpenPic(&m_partPic.m_File,"",error_info,&p_Picture);
		}
		break;
	case 1:
		m_asmPic.m_strFilter.Format("[AsmNum]=%ld AND [Number]=%ld",temp->mdlNum,temp->picNum);
		m_asmPic.Open();
		if (!m_asmPic.IsBOF())
		{
			m_asmPic.MoveFirst();
			DBOpenPic(&m_asmPic.m_File,"",error_info,&p_Picture);
		}
		break;
	case 2:
		m_partTempPic.m_strFilter.Format("[PrtNum]=%ld AND [Number]=%ld",temp->mdlNum,temp->picNum);
		m_partTempPic.Open();
		if (!m_partTempPic.IsBOF())
		{
			m_partTempPic.MoveFirst();
			DBOpenPic(&m_partTempPic.m_File,"",error_info,&p_Picture);
		}
		break;
	case 3:
		m_asmTempPic.m_strFilter.Format("[AsmNum]=%ld AND [Number]=%ld",temp->mdlNum,temp->picNum);
		m_asmTempPic.Open();
		if (!m_asmTempPic.IsBOF())
		{
			m_asmTempPic.MoveFirst();
			DBOpenPic(&m_asmTempPic.m_File,"",error_info,&p_Picture);
		}
		break;
	default:
		break;
	}
	SendMessage(WM_PAINT,0,0);
	return true;
}
/************************************************************************/
/* 根据制定的类型（组件，零件）和模型号，获得图片                       */
/************************************************************************/
void CPicForDBDlg::GetMdlPicSet(int type,long mdlNum,CDatabase * i_db)
{
	p_db=i_db;
	CpartPicSet		m_partPic(p_db);
	CAsmPicSet		m_asmPic(p_db);
	CPart_Temp_Pic  m_partTempPic(p_db);
	CAsmTempPic     m_asmTempPic(p_db);
	pic_item		temp;
	if (p_Picture!=NULL)
	{
		p_Picture->Release();
		p_Picture=NULL;
	}
	p_PicItemList.RemoveAll();
	m_btnNextPic.EnableWindow(FALSE);
	m_btnPrePic.EnableWindow(FALSE);
	m_viewPicInDlg.EnableWindow(FALSE);
	current_pic=-1;

	switch(type) {
	case 0:  //零件模型
		m_partPic.m_strFilter.Format("[PrtNum]=%ld",mdlNum);
		m_partPic.Open();
		if (!m_partPic.IsBOF())
		{
			m_partPic.MoveFirst();
			do {
				temp.setType=0;
				temp.mdlNum=mdlNum;
				temp.picNum=m_partPic.m_Number;
				p_PicItemList.Add(temp);
				m_partPic.MoveNext();
			} while(!m_partPic.IsEOF());
		}
		break;
	case 1:   //组件模型
		m_asmPic.m_strFilter.Format("[AsmNum]=%ld",mdlNum);
		m_asmPic.Open();
		if (!m_asmPic.IsBOF())
		{
			m_asmPic.MoveFirst();
			do {
				temp.setType=1;
				temp.mdlNum=mdlNum;
				temp.picNum=m_asmPic.m_Number;
				p_PicItemList.Add(temp);
				m_asmPic.MoveNext();
			} while(!m_asmPic.IsEOF());
		}
		break;
	case 2:  //临时零件模型
		m_partTempPic.m_strFilter.Format("[PrtNum]=%ld",mdlNum);
		m_partTempPic.Open();
		if (!m_partTempPic.IsBOF())
		{
			m_partTempPic.MoveFirst();
			do {
				temp.setType=2;
				temp.mdlNum=mdlNum;
				temp.picNum=m_partTempPic.m_Number;
				p_PicItemList.Add(temp);
				m_partTempPic.MoveNext();
			} while(!m_partTempPic.IsEOF());
		}
		break;
	case 3:
		m_asmTempPic.m_strFilter.Format("[AsmNum]=%ld",mdlNum);
		m_asmTempPic.Open();
		if (!m_asmTempPic.IsBOF())
		{
			m_asmTempPic.MoveFirst();
			do {
				temp.setType=3;
				temp.mdlNum=mdlNum;
				temp.picNum=m_asmTempPic.m_Number;
				p_PicItemList.Add(temp);
				m_asmTempPic.MoveNext();
			} while(!m_asmTempPic.IsEOF());
		}
		break;
	default:
		break;
	}
	if (p_PicItemList.GetCount()>0)
	{
		ShowPicItem(&p_PicItemList[0]);
		current_pic=0;
		if (p_PicItemList.GetCount()>1)
		{
			m_btnNextPic.EnableWindow(TRUE);
		}
		else
		{
			m_btnNextPic.EnableWindow(FALSE);
		}
		m_btnPrePic.EnableWindow(FALSE);
		m_viewPicInDlg.EnableWindow(TRUE);
	}
	else
	{
		CRect rect;
		m_pic.GetClientRect(&rect);
		m_pic.ClientToScreen(&rect);
		ScreenToClient(&rect);
		RedrawWindow(&rect,NULL,RDW_ERASE|RDW_INVALIDATE);
	}
}
void CPicForDBDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	if (p_Picture!=NULL)
	{
		CSize m_size;
		CRect rect;
		m_pic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		p_Picture->get_Width(&m_size.cx);
		p_Picture->get_Height(&m_size.cy);
		m_pic.MoveWindow(&rect,TRUE);
		p_Picture->Render(*GetDC(),rect.left,rect.bottom,rect.Width(),-rect.Height(),0,0,m_size.cx,m_size.cy,NULL);
	}
}
/************************************************************************/
/* 窗口浏览                                                             */
/************************************************************************/
void CPicForDBDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (pic_dlg==NULL)
	{
		pic_dlg=new CPicViewDlg();
		pic_dlg->Create(IDD_DIALOG5);
	}
	else
	{
		delete pic_dlg;
		pic_dlg=new CPicViewDlg();
		pic_dlg->Create(IDD_DIALOG5);
	}
	pic_dlg->SetIpicture(p_Picture);
	pic_dlg->ShowWindow(TRUE);
}