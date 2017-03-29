// ToleranceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ansys.h"
#include "ToleranceDlg.h"
#include "Tolerance.h"


// CToleranceDlg �Ի���

IMPLEMENT_DYNAMIC(CToleranceDlg, CDialog)
CToleranceDlg::CToleranceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToleranceDlg::IDD, pParent)
{
	m_PageIndex=0;
}

CToleranceDlg::~CToleranceDlg()
{
}

void CToleranceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_COMBO1, m_SmallDia);
	DDX_Control(pDX, IDC_COMBO3, m_BigDia);
	DDX_Control(pDX, IDC_COMBO2, m_NameCombo);
}


BEGIN_MESSAGE_MAP(CToleranceDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnCbnSelchangeCombo3)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CToleranceDlg ��Ϣ�������

BOOL CToleranceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//this->OnBnClickedButtonLoad();

	this->LoadAll();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

int CToleranceDlg::LoadAll()
{
	int status;
	status=m_Tol.Load();
	if(status==0) return 0;
	CString str;
	
    int PageCount=m_Tol.m_PageCount;
	if(PageCount<=0) return 0;
	for(int i=0;i<PageCount;i++)
	{
	   str=m_Tol.m_PageArray[i]->Name;
	   m_NameCombo.AddString(str);
	   if(i==0)
		   m_NameCombo.SelectString(0,str);
	}
	m_PageIndex=0;
    this->LoadPage(m_PageIndex);
	return 1;
}


int CToleranceDlg::LoadPage(int pageIndex)
{
	CString str;
	CStringArray & strArray=m_Tol.m_PageArray[pageIndex]->strArray;
    m_Cols=m_Tol.m_PageArray[pageIndex]->cols;
	m_Rows=m_Tol.m_PageArray[pageIndex]->rows;
	m_StartCol=1;m_EndCol=m_Cols-1;
	m_SmallDia.ResetContent();
	m_BigDia.ResetContent();
	for(int i=1;i<m_Cols;i++)///��һ���ǻ����ߴ磬����ʾ
	{
		//�����ߴ�
         str=strArray[i];
		 m_SmallDia.AddString(str);
		 m_BigDia.AddString(str);
		 if(i==1)
		 	m_SmallDia.SelectString(0,str);
		 if(i==m_Cols-1)
		 	m_BigDia.SelectString(0,str);
	}
	int status=this->LoadList(pageIndex,m_StartCol,m_EndCol);
	return 1;
}

int CToleranceDlg::LoadList(int pageIndex,int startcol,int endcol)
{
	CString str;
	CStringArray & strArray=m_Tol.m_PageArray[pageIndex]->strArray;
	// Delete all of the columns.
	m_List.DeleteAllItems();
	int nColumnCount =m_List.GetHeaderCtrl()->GetItemCount();
	for (int i=0;i < nColumnCount;i++){m_List.DeleteColumn(0);}
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);


	str=strArray[0];
	m_List.InsertColumn(0,str,LVCFMT_LEFT,80,-1);

	for(int i=startcol;i<=endcol;i++)
	{
	    str=strArray[i];
		m_List.InsertColumn(i-startcol+1,str,LVCFMT_LEFT,80,-1);
	}

	for(int i=1;i<m_Rows;i++)///��һ�л����ߴ磬��Ϊ��ͷ
	{
		str=strArray[i*m_Cols+0];
		m_List.InsertItem(i-1,str);
		for(int j=startcol;j<=endcol;j++)
		{
			str=strArray[i*m_Cols+j];///��һ�л����ߴ磬��Ϊ��ͷ
			m_List.SetItemText(i-1,j-startcol+1,str);
		}
	}


	for(int i=0;i<m_List.GetItemCount();i++)
	{
		m_List.SetCheck(i,TRUE);
	}
	return 1;
}





void CToleranceDlg::OnCbnSelchangeCombo1()
{
	CStringArray & strArray=m_Tol.m_PageArray[m_PageIndex]->strArray;
	int curIndex;
	CString text;
	curIndex=m_SmallDia.GetCurSel();
	m_SmallDia.GetLBText(curIndex,text);
	CString str;
	for(int j=1;j<m_Cols;j++)
	{
		str=strArray[j];
		if(text==str)
		{
			if(j>m_EndCol)
			{
				AfxMessageBox("��Сֱ�����ô������ֱ��");
				str=strArray[m_StartCol];
				m_SmallDia.SelectString(0,str);
				return;

			}
			m_StartCol=j;
			this->LoadList(m_PageIndex,m_StartCol,m_EndCol);
			return;
		}

	}

}

void CToleranceDlg::OnCbnSelchangeCombo3()
{
	CStringArray & strArray=m_Tol.m_PageArray[m_PageIndex]->strArray;
	int curIndex;
	CString text;
	curIndex=m_BigDia.GetCurSel();
	m_BigDia.GetLBText(curIndex,text);
	CString str;
	for(int j=1;j<m_Cols;j++)
	{
		str=strArray[j];
		if(text==str)
		{
			if(j<m_StartCol)
			{
				AfxMessageBox("���ֱ������С����Сֱ��");
				str=strArray[m_EndCol];
				m_BigDia.SelectString(0,str);
				return;

			}
			m_EndCol=j;
			this->LoadList(m_PageIndex,m_StartCol,m_EndCol);
			return;
		}

	}
}

/************************************************************************/
/* ҳ���л�                                                             */
/************************************************************************/
void CToleranceDlg::OnCbnSelchangeCombo2()
{
	int curIndex;
	CString text;
	curIndex=m_NameCombo.GetCurSel();
	m_NameCombo.GetLBText(curIndex,text);
	CString str;
	int PageCount=m_Tol.m_PageCount;
	if(PageCount<=0) return ;
	for(int i=0;i<PageCount;i++)
	{
		str=m_Tol.m_PageArray[i]->Name;
		if(str==text)
		{
			m_PageIndex=i;
			this->LoadPage(m_PageIndex);
			return ;
		}

	}
	
}



void CToleranceDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
   int status=this->GetList();
   if(status==0) return ;
	OnOK();
}

int  CToleranceDlg::GetList()
{
	CString str;
	CStringArray & strArray=m_Tol.m_PageArray[m_PageIndex]->strArray;
	m_OutPage.strArray.RemoveAll();
	m_OutPage.Name=m_Tol.m_PageArray[m_PageIndex]->Name;
	int cols,rows;
    cols=m_EndCol-m_StartCol+2;
	str=strArray[0];
    m_OutPage.strArray.Add(str);

	for(int i=m_StartCol;i<=m_EndCol;i++)
	{
		str=strArray[i];
		m_OutPage.strArray.Add(str);
	}

	rows=1;
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		if(m_List.GetCheck(i)!=TRUE)
		{
			continue;
		}
		for(int j=0;j<cols;j++)
		{
			str=m_List.GetItemText(i,j);
			m_OutPage.strArray.Add(str);
		}
		rows++;
	}

	m_OutPage.rows=rows;
	m_OutPage.cols=cols;
	m_OutPage.PageIndex=m_PageIndex;
	return 1;
}
