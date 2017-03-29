// PropPage5.cpp : ʵ���ļ�
//ͼƬ

#include "stdafx.h"
#include "Main.h"
#include "PropPage5.h"
#include "PicViewDlg.h"

// CPropPage5 �Ի���

IMPLEMENT_DYNAMIC(CPropPage5, CPropertyPage)
CPropPage5::CPropPage5()
	: CPropertyPage(CPropPage5::IDD)
	, m_PicFileName(_T(""))
	, m_PicNote(_T(""))
	,p_temp_UsrPicture(NULL)
	,mdl(NULL)
	, m_index(-1)
{
}

CPropPage5::~CPropPage5()
{
}

void CPropPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_PicFileName);
	DDX_Text(pDX, IDC_EDIT2, m_PicNote);
	DDX_Control(pDX, IDC_MYIMAGE, m_pic);
	DDX_Control(pDX, IDC_BUTTON1, m_ViewPic);
}


BEGIN_MESSAGE_MAP(CPropPage5, CPropertyPage)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
//	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPropPage5 ��Ϣ�������

int CPropPage5::SetValue(ProMdl solid,CArray<UsrPicture,UsrPicture>* pp_temp_UsrPicture)
{
	p_temp_UsrPicture=pp_temp_UsrPicture;
    mdl=solid;
	return 0;
}

int CPropPage5::SetList(void)
{
	int num=0;
	int indexNo=0;
	m_List.DeleteAllItems();
	for(int i=0;i<2;i++)
	{
		m_List.DeleteColumn(0);
	}
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"�ļ���",LVCFMT_LEFT,120,-1);
	m_List.InsertColumn(1,"˵��",LVCFMT_LEFT,120,-1);
	
	num=(int)p_temp_UsrPicture->GetCount();
	int temp=0;
	for(int i=0;i<num;i++)
	{
		if(p_temp_UsrPicture->GetAt(i).owner!=mdl) continue;
		if(p_temp_UsrPicture->GetAt(i).isCheck!=1) continue;
		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,temp++,p_temp_UsrPicture->GetAt(i).fileName,0,0,0,i);		
		m_List.SetItemText(indexNo,1,p_temp_UsrPicture->GetAt(i).Note);
	}
	return 0;
}

BOOL CPropPage5::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
    m_ViewPic.EnableWindow(FALSE);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    SetList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* �����б����ʾͼƬ��ͬʱ���·�������Ϣ                             */
/************************************************************************/
void CPropPage5::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int num=0;
	CString file;
	num=(int)m_List.GetItemCount();
	for(int i=0;i<num;i++)
	{
		if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;
		file=p_temp_UsrPicture->GetAt((int)m_List.GetItemData(i)).filePath;
		file=file+p_temp_UsrPicture->GetAt((int)m_List.GetItemData(i)).fileName;
        ShowPic(file);
		m_PicFileName=p_temp_UsrPicture->GetAt((int)m_List.GetItemData(i)).fileName;
		m_PicNote=p_temp_UsrPicture->GetAt((int)m_List.GetItemData(i)).Note;

		m_index=(int)m_List.GetItemData(i);
		m_ViewPic.EnableWindow(TRUE);
		UpdateData(FALSE);
	}
	*pResult = 0;
}
/************************************************************************/
/* ��ͼƬ�ؼ�����ʾָ����ͼƬ                                           */
/************************************************************************/
int CPropPage5::ShowPic(CString file)
{

	m_pic.SetFile(file);
	m_pic.OnPaint();
	return 0;
}

void CPropPage5::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CPropertyPage::OnPaint()
	
}
/************************************************************************/
/* ɾ��ͼƬ                                                             */
/************************************************************************/
void CPropPage5::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString temp;
	if(m_index==-1) return;

	temp.Format("ȷ��ɾ�����ܲ��������%s����",p_temp_UsrPicture->GetAt(m_index).fileName);
	if(AfxMessageBox(temp)!=IDOK) return;
	//p_temp_UsrPicture->GetAt(m_index).isCheck=0;	
	DeleteFile(_T(p_temp_UsrPicture->GetAt(m_index).filePath+p_temp_UsrPicture->GetAt(m_index).fileName));
	p_temp_UsrPicture->RemoveAt(m_index);
    ShowPic("");
	m_ViewPic.EnableWindow(FALSE);
	m_index=-1;
	//ˢ��
	SetList();
}
/************************************************************************/
/* �޸���Ϣ                                                             */
/************************************************************************/
void CPropPage5::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_index==-1) return;
    p_temp_UsrPicture->GetAt(m_index).Note=m_PicNote;
	//m_index=-1;	
    SetList();
}
/************************************************************************/
/* ��ʾ��ʵͼƬ                                                         */
/************************************************************************/
void CPropPage5::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    CString temp_file;
	if(m_index==-1) return;
	temp_file=p_temp_UsrPicture->GetAt(m_index).filePath+p_temp_UsrPicture->GetAt(m_index).fileName;
	dlg= new CPicViewDlg();
	dlg->Create(IDD_DIALOG5);
	dlg->SetFile(temp_file);
	dlg->ShowWindow(SW_SHOW);
}

