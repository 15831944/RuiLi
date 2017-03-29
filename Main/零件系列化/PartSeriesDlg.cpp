// PartSeriesDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "PartSeriesDlg.h"
#include "AddMdlSeriesDlg.h"
#include "MdlSeriesPicShowDlg.h"
//extern int DlgIcon;
// CPartSeriesDlg �Ի���

IMPLEMENT_DYNAMIC(CPartSeriesDlg, CDialog)
CPartSeriesDlg::CPartSeriesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartSeriesDlg::IDD, pParent)
	, mdlNumber(0)
	, p_db(NULL)
	, m_current_item(-1)
	,m_InsTableSet(NULL)
	, combColumn(-1)
{
}

CPartSeriesDlg::~CPartSeriesDlg()
{
	if (m_InsTableSet!=NULL) {
		delete m_InsTableSet;
	}
}

void CPartSeriesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_ADD_BUTTON, m_AddButton);
	DDX_Control(pDX, IDC_BUTTON1, m_OkButton);
	DDX_Control(pDX, IDC_BUTTON2, m_CancelAddButton);
	DDX_Control(pDX, IDC_MOD_BUTTON, m_ModifyButton);
}


BEGIN_MESSAGE_MAP(CPartSeriesDlg, CDialog)
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)	
	ON_BN_CLICKED(IDC_ADD_BUTTON, OnBnClickedAddButton)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_NOTIFY(LVN_HOTTRACK, IDC_LIST1, OnLvnHotTrackList1)
	ON_BN_CLICKED(IDC_DEL_BUTTON, OnBnClickedDelButton)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_MOD_BUTTON, OnBnClickedModButton)
	ON_BN_CLICKED(IDC_VIEW_BUTTON, OnBnClickedViewButton)
END_MESSAGE_MAP()


// CPartSeriesDlg ��Ϣ�������

BOOL CPartSeriesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(p_db==NULL) return FALSE;
	//���ð�ť״̬
	m_AddButton.EnableWindow(TRUE);
	m_CancelAddButton.EnableWindow(FALSE);
	m_OkButton.EnableWindow(FALSE);
	//������ʾ��Ϣ
	EnableToolTips(TRUE);		
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	CWnd* pW=GetDlgItem(IDC_LIST1);
	m_tooltip.AddTool(pW,"ģ�͵�ϵ�б�");
	//�����б��
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_TRACKSELECT);
    InitListByMdlInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CPartSeriesDlg::SetMdlNumberAndDB(long num,CDatabase * db)
{
	mdlNumber=num;
	p_db=db;
	m_InsTableSet=new CPartInsTableSet(p_db);
	//ȷ�����ݿ�����
	if(!p_db->IsOpen()) 
	{
		CString strConnect=m_InsTableSet->GetDefaultConnect();
		p_db->Open(NULL,FALSE,FALSE,strConnect,FALSE);
	}
}
/************************************************************************/
/* ����ģ�͵����ܲ������������Ա���Ϣ                                   */
/************************************************************************/
void CPartSeriesDlg::InitListByMdlInfo(void)
{
	int        tempColNum=0;
	ItemProp   temp;
	CString    sql_lang;
	CpartVariantTableSet      m_mdlVariantSet(p_db);
	CPartMdlRecordSet         m_MdlInfoSet(p_db);
	CpartPerfparaTableSet     m_mdlPerfparaSet(p_db);
	MdlInsInfo mdlInfotemp; //��ʱ��ϵ����Ϣ

	for(int i=0;i<(int)p_ColProp.GetCount();i++)
	{
		m_List.DeleteColumn(0);
	}
	p_ColProp.RemoveAll();
	p_InsInfo.RemoveAll();
	m_List.DeleteAllItems();

	try{
		//�б������� 0����� 1������  2������   ����  n�����ܲ���	
		//1.��0�е�0��
		m_List.InsertColumn(tempColNum,"���",LVCFMT_LEFT,30,-1);
		temp.para_num=-1;temp.note=" ģ��ϵ�еı��";temp.type=-1;
		p_ColProp.Add(temp);
		m_List.InsertItem(0,"");
		
		//2.��0�е�1��   ���ģ�͵�����		
		m_MdlInfoSet.m_strFilter.Format("[Number]=%d",mdlNumber);		
		if(!m_MdlInfoSet.Open())
		{
			AfxMessageBox("���ݿ�����ʧ�ܣ�");
			return;
		}
		if (m_MdlInfoSet.IsBOF()) 		{
			
			return;
		}
		m_MdlInfoSet.MoveFirst();
		//����������
		tempColNum=1;temp.para_num=-1;temp.note="ģ�ͼ�ϵ�е�����";	temp.type=-1;
		p_ColProp.Add(temp);
		//���ģ�͵Ļ�����Ϣ
		mdlInfotemp.Name=m_MdlInfoSet.m_Name;mdlInfotemp.Code=m_MdlInfoSet.m_MdlCode;
		mdlInfotemp.Person=m_MdlInfoSet.m_MdlerName;mdlInfotemp.Dept=m_MdlInfoSet.m_MdlerDepartment;
		mdlInfotemp.m_time=m_MdlInfoSet.m_Datatime;
		p_InsInfo.Add(mdlInfotemp);
		m_List.InsertColumn(tempColNum,"����",LVCFMT_LEFT,50,-1);
		m_List.SetItemText(0,tempColNum,m_MdlInfoSet.m_Name);
		//m_MdlInfoSet.Close();



		//�����������Ա����
		//if(m_mdlVariantSet.IsOpen()) m_mdlVariantSet.Close();
		//sql_lang.Format("SELECT * FROM [dbo].[partVariantTable] WHERE [PrtNum]=%d",mdlNumber);
		m_mdlVariantSet.m_strFilter.Format("[PrtNum]=%d",mdlNumber);
		if(m_mdlVariantSet.Open())
		{
			if (!m_mdlVariantSet.IsBOF()) {
				m_mdlVariantSet.MoveFirst();

				while (!m_mdlVariantSet.IsEOF()) {
					tempColNum++;

					temp.para_num=m_mdlVariantSet.m_Number;
					temp.note=m_mdlVariantSet.m_Note;

					if (m_mdlVariantSet.m_SubType=="PRO_PARAM_DOUBLE" || m_mdlVariantSet.m_Type=="dim") {
						temp.type=1;  //������
					}
					else if (m_mdlVariantSet.m_SubType=="PRO_PARAM_INTEGER") {
						temp.type=2;  //����
					}
					else if (m_mdlVariantSet.m_SubType=="PRO_PARAM_BOOLEAN") {
						temp.type=3;  //������
					}
					else {
						temp.type=0;  //�ַ���
					}
	
					p_ColProp.Add(temp);
					m_List.InsertColumn(tempColNum,m_mdlVariantSet.m_Name,LVCFMT_LEFT,50,-1);
					m_List.SetItemText(0,tempColNum,m_mdlVariantSet.m_Value);
					m_mdlVariantSet.MoveNext();
				}

			}	
			//m_mdlVariantSet.Close();
		}
		else
		{
			AfxMessageBox("����������Ա����ֵʱ����Ϊ���ݿ�����ʧ�ܣ�û����ʾ�������Ա���Ϣ��");
			return;
		}

		//�м�����һ��
		tempColNum++;
		temp.para_num=-1;
		temp.note="";
		temp.type=-1;
		p_ColProp.Add(temp);
		m_List.InsertColumn(tempColNum,"",LVCFMT_LEFT,10,-1);
		m_List.SetItemText(0,tempColNum,"");
		//�����������Ա����
		//if(m_mdlPerfparaSet.IsOpen()) m_mdlPerfparaSet.Close();
		//sql_lang.Format("SELECT * FROM [dbo].[partPerfparaTable] WHERE [PrtNum]=%d",mdlNumber);
		m_mdlPerfparaSet.m_strFilter.Format("[PrtNum]=%d",mdlNumber);
		if(m_mdlPerfparaSet.Open())
		{

			if (!m_mdlPerfparaSet.IsBOF()) {
				m_mdlPerfparaSet.MoveFirst();
				while (!m_mdlPerfparaSet.IsEOF()) {
					tempColNum++;

					temp.para_num=m_mdlPerfparaSet.m_Number;
					temp.note=m_mdlPerfparaSet.m_Note;
					temp.type=0;
					p_ColProp.Add(temp);

					m_List.InsertColumn(tempColNum,m_mdlPerfparaSet.m_Name,LVCFMT_LEFT,50,-1);
					m_List.SetItemText(0,tempColNum,m_mdlPerfparaSet.m_Value);
					m_mdlPerfparaSet.MoveNext();

				}
			}	
			//m_mdlPerfparaSet.Close();
		}
		else
		{
			AfxMessageBox("������ܱ����ֵʱ����Ϊ���ݿ�����ʧ�ܣ�û����ʾ�������Ա���Ϣ��");
			return;
		}

	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		delete e;
		//if(m_MdlInfoSet.IsOpen()) m_MdlInfoSet.Close();
		//if(m_mdlPerfparaSet.IsOpen()) m_mdlPerfparaSet.Close();
		//if(m_mdlVariantSet.IsOpen())  m_mdlVariantSet.Close();
		return;
	}


    //���ϵ��
    AddSeriesData();

}

/************************************************************************/
/* ���ģ�͵�ϵ��                                                       */
/************************************************************************/
void CPartSeriesDlg::AddSeriesData(void)
{
	int        itemNum=1;  //�У��ӵڶ��У�1����ʼ���
	CString    csInsNum;
	int        ColNum=0;
	MdlInsInfo temp; //��ʱ��ϵ����Ϣ
	CpartInsValueTableSet temp_ins_value_Set(p_db);
	CpartInsPerfValueTableSet temp_ins_Perf_Set(p_db);
	try
	{
		if(m_InsTableSet->IsOpen()) m_InsTableSet->Close();
		m_InsTableSet->m_strFilter.Format("[PrtNum]=%d",mdlNumber);
		m_InsTableSet->Open();
		if(m_InsTableSet->IsBOF()) 
		{
			m_InsTableSet->Close();
			return;
		}

		    m_InsTableSet->MoveFirst();
			//�ӵڶ��п�ʼ�������
			while (!m_InsTableSet->IsEOF()) {
				//����0��1�е�ֵ
				csInsNum.Format("%d",itemNum);				
				m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,itemNum,csInsNum,0,0,0,m_InsTableSet->m_InsNum);
				m_List.SetItemText(itemNum,1,m_InsTableSet->m_InsName);
				//���ʵ���Ļ�����Ϣ
				temp.Name=m_InsTableSet->m_InsName;
				temp.Code=m_InsTableSet->m_InsCode;
				temp.Person=m_InsTableSet->m_InsCreaterName;
				temp.Dept=m_InsTableSet->m_InsCreateDpt;
				temp.Note=m_InsTableSet->m_InsNote;
				temp.mdlNum=mdlNumber;
				temp.m_time=m_InsTableSet->m_InsCreateTime;
				p_InsInfo.Add(temp);
				//��������������Ա�
				ColNum=2;
				while (p_ColProp[ColNum].para_num!=-1) {
					temp_ins_value_Set.m_strFilter.Format("[PrtNum]=%d AND [InsNum]=%d AND [Num]=%d"
						,mdlNumber,m_InsTableSet->m_InsNum,p_ColProp[ColNum].para_num);
					temp_ins_value_Set.Open();
					if (!temp_ins_value_Set.IsBOF()) {
						temp_ins_value_Set.MoveFirst();
						m_List.SetItemText(itemNum,ColNum,temp_ins_value_Set.m_Value);
					}
					temp_ins_value_Set.Close();
					ColNum++;
				}
				ColNum++;
				//������ܲ���				
				while (ColNum<(int)p_ColProp.GetCount()) {
					temp_ins_Perf_Set.m_strFilter.Format("[PrtNum]=%d AND [InsNum]=%d AND [Num]=%d"
						,mdlNumber,m_InsTableSet->m_InsNum,p_ColProp[ColNum].para_num);
					temp_ins_Perf_Set.Open();
					if (!temp_ins_Perf_Set.IsBOF()) {
						temp_ins_Perf_Set.MoveFirst();
						m_List.SetItemText(itemNum,ColNum,temp_ins_Perf_Set.m_Value);
					}
					temp_ins_Perf_Set.Close();
					ColNum++;
				}

               //�ƶ�����һ����¼
				m_InsTableSet->MoveNext();
				itemNum++;
				ColNum=2;
		     
	        }
	}
	catch (CDBException * e) {
        AfxMessageBox(e->m_strError);
		delete e;
		return;
	}

}
/************************************************************************/
/* ���ϵ��                                                             */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedAddButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAddMdlSeriesDlg dlg;
	CString          sql_lang;
	int              colNum=0;
	int              itemNum=0;
	CString          str="";
	CStringArray     exitSerieName;
	CPartMdlRecordSet         m_MdlInfoSet(p_db);
	m_MdlInfoSet.m_strFilter.Format("[Number]=%d",mdlNumber);	
	m_MdlInfoSet.Open();
	if (m_MdlInfoSet.IsBOF()) 
	{
		m_MdlInfoSet.Close();
		return;
	}
	//����Ѿ��е�ϵ�е����ƣ�������,ʹ����������ƺͱ��벻�ܹ�ͬ�Ѿ��е���ͬ
	dlg.SetMdlNumberAndCode(mdlNumber,m_MdlInfoSet.m_MdlCode,&p_InsInfo);
	m_MdlInfoSet.Close();
	//������ϵ�е����ơ����롢�ȵ���Ϣ
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}    
	//ȷ��֮�󣬸ı䰴ť��״̬��ʹ��Ӱ�ť��Ч��ʹ�ύ�ͳ�����ť��Ч
	m_AddButton.EnableWindow(FALSE);
	m_OkButton.EnableWindow(TRUE);
	m_CancelAddButton.EnableWindow(TRUE);
	//�����Ϣ
	newIns=dlg.newMdl;
	//���µ�����
    //������ϵ�е�Ĭ��ֵ
	itemNum=m_List.GetItemCount();
	colNum=m_List.GetColumnCounts();
	str.Format("%d",itemNum);
	//��������ӵ���Ϊ��ǰ��
	m_current_item=itemNum;
    m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,itemNum,str,0,0,0,-1);
	m_List.SetItemText(itemNum,1,newIns.Name);
	for(int i=2;i<colNum;i++)
	{
		m_List.SetItemText(itemNum,i,m_List.GetItemText(0,i));
	}
	m_AddButton.EnableWindow(FALSE);
	m_CancelAddButton.EnableWindow(TRUE);
	m_OkButton.EnableWindow(TRUE);
}
/************************************************************************/
/* �ύ��ϵ��                                                           */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int      newInsNum=0;
	CString  sql_lang;
	int      ColNum=0;
	int      ColCurrent=0;

	CpartInsValueTableSet     m_InsValueTableSet(p_db);
	CpartInsPerfValueTableSet m_InsPerfValueTableSet(p_db);

	if(!CheckNewSeries()) return;
	newInsNum=GetMdlSeriesMaxNumber()+1;
	if(m_InsTableSet->IsOpen())           m_InsTableSet->Close();
	if (p_db->CanTransact()) {
		if(!m_InsTableSet->Open()) return;
		if(!m_InsValueTableSet.Open()) return;
		if(!m_InsPerfValueTableSet.Open()) return;

		if(!p_db->BeginTrans())  return;
		//���ϵ����Ϣ
		try{
			m_InsTableSet->AddNew();
			m_InsTableSet->m_PrtNum=mdlNumber;
			m_InsTableSet->m_InsNum=newInsNum;
			m_InsTableSet->m_InsName=newIns.Name;
			m_InsTableSet->m_InsCode=newIns.Code;
			m_InsTableSet->m_InsNote=newIns.Note;
			m_InsTableSet->m_InsCreaterName=newIns.Person;
			m_InsTableSet->m_InsCreateDpt=newIns.Dept;
			CTime  time=CTime::GetCurrentTime();
			m_InsTableSet->m_InsCreateTime=time;
			m_InsTableSet->Update();

			ColNum=m_List.GetColumnCounts();		

			for(ColCurrent=2;ColCurrent<ColNum;ColCurrent++)
			{
				//�����ǰ��������еĲ�����Ϊ-1				
				if(p_ColProp[ColCurrent].para_num==-1 ) break;
				m_InsValueTableSet.AddNew();
				m_InsValueTableSet.m_PrtNum=mdlNumber;
				m_InsValueTableSet.m_InsNum=newInsNum;
				m_InsValueTableSet.m_Num=p_ColProp[ColCurrent].para_num;
				m_InsValueTableSet.m_Value=m_List.GetItemText(m_current_item,ColCurrent);
				m_InsValueTableSet.Update();
			}

			ColCurrent+=1;
			for(;ColCurrent<ColNum;ColCurrent++)
			{				
				m_InsPerfValueTableSet.AddNew();
				m_InsPerfValueTableSet.m_PrtNum=mdlNumber;
				m_InsPerfValueTableSet.m_InsNum=newInsNum;
				m_InsPerfValueTableSet.m_Num=p_ColProp[ColCurrent].para_num;
				m_InsPerfValueTableSet.m_Value=m_List.GetItemText(m_current_item,ColCurrent);
				m_InsPerfValueTableSet.Update();
			}
			p_db->CommitTrans();

		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			delete e;
			p_db->Rollback();
			if(m_InsTableSet->IsOpen())           m_InsTableSet->Close();	
			return;
		}
		m_InsTableSet->Close();	
        p_InsInfo.Add(newIns);

	}

	//���ð�ť
	m_AddButton.EnableWindow(TRUE);
	m_CancelAddButton.EnableWindow(FALSE);
	m_OkButton.EnableWindow(FALSE);
	//���ø���Ϊ���ɱ༭	
	m_current_item=-1;
	//�б����Ϊ-1
	combColumn=-1;
	//����������Ա�
	//������ܲ���

}
/************************************************************************/
/* ��������ӵ�ϵ��                                                     */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_AddButton.EnableWindow(TRUE);
	m_OkButton.EnableWindow(FALSE);
	m_CancelAddButton.EnableWindow(FALSE);
	m_List.DeleteItem(m_current_item);
	m_current_item=-1;
	combColumn=-1;
}
/************************************************************************/
/* ������ϵ�к�                                                       */
/************************************************************************/
long CPartSeriesDlg::GetMdlSeriesMaxNumber(void)
{
	long      maxNum=0;
	CString   sql_lang;
	if(m_InsTableSet->IsOpen()) m_InsTableSet->Close();
	m_InsTableSet->m_strFilter.Format("[PrtNum]=%d",mdlNumber);	
	m_InsTableSet->Open();
	if (m_InsTableSet->IsBOF()) {
		m_InsTableSet->Close();
		return maxNum;
	}
	m_InsTableSet->MoveFirst();
	while (!m_InsTableSet->IsEOF()) {
		maxNum=maxNum>=m_InsTableSet->m_InsNum?maxNum:m_InsTableSet->m_InsNum;
		m_InsTableSet->MoveNext();
	}
	m_InsTableSet->Close();
	return maxNum;
	return 0;
}
/************************************************************************/
/* ������ӵ�ϵ�н��м��                                               */
/************************************************************************/
bool CPartSeriesDlg::CheckNewSeries(void)
{
      
  for(int i=2;i<m_List.GetColumnCounts();i++)
	{		
		//�����в���Ϊ��
		if(p_ColProp[i].para_num==-1) continue;
		if(m_List.GetItemText(m_current_item,i).IsEmpty())
		{
			CString  temp;
			LVCOLUMN temp_colprop;
			m_List.GetColumn(i,&temp_colprop);			
			temp.Format("����%s,����Ϊ�գ�",temp_colprop.pszText);
			AfxMessageBox(temp);
			return false;
		}
	}
	return true;
}

/************************************************************************/
/* ���õ�ǰ��ӵ��п��Ա༭                                             */
/************************************************************************/
void CPartSeriesDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int      item=0;
    int      ColNum=0;
	CString  str;
	item=(int)m_List.GetHotItem();	
    //���ǵ�ǰ�еĶ����ɱ༭
	if (item!=m_current_item || m_current_item==-1) {
		ColNum=m_List.GetColumnCounts();
		for(int i=2;i<ColNum;i++)
		{
			if(m_List.IsCombo(i))     m_List.SetComboColumns(i,false);
			if(!m_List.IsReadOnly(i))  m_List.SetReadOnlyColumns(i,true);
		}
	}	
	else
	{
		ColNum=m_List.GetColumnCounts();
		for(int i=2;i<ColNum;i++)
		{
			if(m_List.IsCombo(i)) m_List.SetComboColumns(i,false);
			m_List.SetReadOnlyColumns(i,true);
			switch(p_ColProp[i].type) {
				case 0:
					str="";				
					m_List.SetReadOnlyColumns(i,false);
					m_List.SetColumnValidEditCtrlCharacters(str,i);		
					break;
				case 1:
					str="987654321.0";				
					m_List.SetReadOnlyColumns(i,false);
					m_List.SetColumnValidEditCtrlCharacters(str,i);		
					break;
				case 2:
					str="1230456789";				
					m_List.SetReadOnlyColumns(i,false);
					m_List.SetColumnValidEditCtrlCharacters(str,i);				
					break;
				case 3: //����ǲ����ͣ�����Ӹ����б���ǡ���
					m_List.SetReadOnlyColumns(i,true);
					m_List.SetComboColumns(i,true);
					combColumn=i;
					break;
				default:
					break;
			}
		}

	}	
	*pResult = 0;
}

LRESULT CPartSeriesDlg::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;
	// TODO: Add your control notification handler code here

	return 0;
}
LRESULT CPartSeriesDlg::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
	// Get the Combobox window pointer
	CComboBox* pInPlaceCombo = static_cast<CComboBox*> (GetFocus());

	// Get the inplace combbox top left
	CRect obWindowRect;

	pInPlaceCombo->GetWindowRect(&obWindowRect);

	CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft()); 

	// Get the active list
	// Get the control window rect
	// If the inplace combobox top left is in the rect then
	// The control is the active control
	m_List.GetWindowRect(&obWindowRect);

	int iColIndex = (int )wParam;

	CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
	pComboList->RemoveAll(); 

	if (obWindowRect.PtInRect(obInPlaceComboTopLeft)) 
	{				
		 
		if(iColIndex==combColumn)
		{
			pComboList->AddTail("1");
			pComboList->AddTail("0");		
		}			
	}
	return true;
}


void CPartSeriesDlg::OnLvnHotTrackList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWnd* pW=GetDlgItem(IDC_LIST1);  
	LVHITTESTINFO lvhti;
	POINT myPoint=GetCurrentMessage()->pt;
    ScreenToClient(&myPoint);
    lvhti.pt = myPoint;
	m_List.SubItemHitTest(&lvhti);
	
	if (lvhti.iItem==-1) {
		m_tooltip.UpdateTipText("",pW,0);
		return;
	}
	if (lvhti.iSubItem==-1) {
		m_tooltip.UpdateTipText("",pW,0);
		return;
	}
	//������м�յ�����Ϊ
	if (lvhti.iSubItem>1 && p_ColProp[lvhti.iSubItem].para_num==-1) {
        m_tooltip.UpdateTipText("",pW,0);
		return;
	}
	m_tooltip.UpdateTipText(p_ColProp[lvhti.iSubItem].note,pW,0);	
	*pResult = 0;
}

BOOL CPartSeriesDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
    m_tooltip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}
/************************************************************************/
/* ɾ��ϵ��                                                             */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedDelButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int     itemNum=0;
	int     selectedItem=0;
	CString temp;
	itemNum=m_List.GetItemCount();
	for(int i=1;i<(m_current_item==-1?itemNum:itemNum-1);i++)
	{
         if(m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED) selectedItem=i;
	}
	if(selectedItem==0) return;	
	temp.Format("ȷ��ɾ��ϵ��%s",p_InsInfo[selectedItem].Name);
	
    if(AfxMessageBox(temp,MB_OK)==IDOK)
	{
		//ɾ��ʵ��
		try{
			if(m_InsTableSet->IsOpen()) m_InsTableSet->Close();
			m_InsTableSet->m_strFilter.Format("[PrtNum]=%d AND [InsName]='%s'",mdlNumber,p_InsInfo[selectedItem].Name);
			m_InsTableSet->Open();
			if (!m_InsTableSet->IsBOF()) {
				temp.Format("DELETE FROM [dbo].[partInsPerfValueTable] WHERE [PrtNum]=%d AND [InsNum]=%d",mdlNumber,m_InsTableSet->m_InsNum);
				p_db->ExecuteSQL(temp);
                temp.Format("DELETE FROM [dbo].[partInsValueTable] WHERE [PrtNum]=%d AND [InsNum]=%d",mdlNumber,m_InsTableSet->m_InsNum);
				p_db->ExecuteSQL(temp);
				m_InsTableSet->Delete();
			}
	
		}
		catch (CDBException * e) {
			AfxMessageBox( e->m_strError );
		}
		m_InsTableSet->Close();
        //�����б�
		InitListByMdlInfo();

	}

    
}
/************************************************************************/
/* �鿴���ԣ���Ϣ                                                       */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
     CAddMdlSeriesDlg dlg;
	 dlg.newMdl=p_InsInfo[0];
	 dlg.strCaption="�鿴��ģ����Ϣ";
	 dlg.SetMdlNumberAndCode(-1,"",NULL);
	 dlg.DoModal();
}
/************************************************************************/
/* �鿴ģ�͵���ͼ                                                       */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    //CPartMdlRecordSet m_mdlInfoSet(p_db);
	CMdlSeriesPicShowDlg dlg;
	dlg.SetMdlNumber(mdlNumber,p_db);
	dlg.DoModal();
    
}
/************************************************************************/
/* �޸�ϵ�еĲ���ֵ                                                     */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedModButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString                   windowtext;
	CString                   sql;
	int                       itemNum=0;
	int                       selectedItem=-1;
	CpartInsValueTableSet     m_InsValueTableSet(p_db);
	CpartInsPerfValueTableSet m_InsPerfValueTableSet(p_db);
	m_ModifyButton.GetWindowText(windowtext);
	if (windowtext=="�޸�") {
		if (m_current_item!=-1) {
			AfxMessageBox("�����ύ������ǰ��ӵ�ϵ�У�");
			return;
		}
		itemNum=m_List.GetItemCount();
		for(int i=1;i<itemNum;i++)
		{
			
			if(m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED) selectedItem=i;
		}
		if(selectedItem==-1) return;	
		m_current_item=selectedItem;
		m_ModifyButton.SetWindowText("ȷ��");
		m_AddButton.EnableWindow(FALSE);
	}
	else
	{
		//�޸�
		if(!CheckNewSeries()) return;
		try{
				if(m_InsTableSet->IsOpen()) m_InsTableSet->Close();
				m_InsTableSet->m_strFilter.Format("[PrtNum]=%d AND [InsName]='%s'",mdlNumber,p_InsInfo[m_current_item].Name);
				m_InsTableSet->Open();
				if (m_InsTableSet->IsBOF()) {
					m_InsTableSet->Close();
					InitListByMdlInfo();
					m_current_item=-1;
					m_ModifyButton.SetWindowText("�޸�");
					m_AddButton.EnableWindow(TRUE);
					return;
				}
				
				int  ColNum=0;
				int ColCurrent=0;
				ColNum=m_List.GetColumnCounts();
				for(ColCurrent=2;ColCurrent<ColNum;ColCurrent++)
				{
					//�����ǰ��������еĲ�����Ϊ-1				
					if(p_ColProp[ColCurrent].para_num==-1 ) break;
					m_InsValueTableSet.m_strFilter.Format("[PrtNum]=%d AND [InsNum]=%d  AND [Num]=%d"
						,mdlNumber,m_InsTableSet->m_InsNum,p_ColProp[ColCurrent].para_num );
					m_InsValueTableSet.Open();
					m_InsValueTableSet.Edit();
					m_InsValueTableSet.m_Value=m_List.GetItemText(m_current_item,ColCurrent);
					m_InsValueTableSet.Update();
					m_InsValueTableSet.Close();
				}

				ColCurrent+=1;
				for(;ColCurrent<ColNum;ColCurrent++)
				{	
					m_InsPerfValueTableSet.m_strFilter.Format("[PrtNum]=%d AND [InsNum]=%d  AND [Num]=%d"
						,mdlNumber,m_InsTableSet->m_InsNum,p_ColProp[ColCurrent].para_num );
					m_InsPerfValueTableSet.Open();
					m_InsPerfValueTableSet.Edit();
					m_InsPerfValueTableSet.m_Value=m_List.GetItemText(m_current_item,ColCurrent);
					m_InsPerfValueTableSet.Update();
					m_InsPerfValueTableSet.Close();
				}
		  }
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			m_InsTableSet->Close();
			return;
		}
		m_InsTableSet->Close();

		InitListByMdlInfo();
		m_current_item=-1;
		m_ModifyButton.SetWindowText("�޸�");
		m_AddButton.EnableWindow(TRUE);

	}

}
/************************************************************************/
/* �鿴ʵ����Ϣ                                                         */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedViewButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAddMdlSeriesDlg dlg;
	int     itemNum=0;
	int     selectedItem=-1;
	CString temp;
	itemNum=m_List.GetItemCount();
	for(int i=0;i<(m_current_item==-1?itemNum:itemNum-1);i++)
	{
		if(m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED) selectedItem=i;
	}
	if(selectedItem==-1) return;	
	dlg.newMdl=p_InsInfo[selectedItem];
	dlg.strCaption="�鿴ϵ����Ϣ";
	dlg.SetMdlNumberAndCode(-1,"",NULL);
	dlg.DoModal();
}
