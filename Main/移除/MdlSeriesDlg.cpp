// MdlSeriesDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "MdlSeriesDlg.h"
#include "AddMdlSeriesDlg.h"

// CMdlSeriesDlg �Ի���

IMPLEMENT_DYNAMIC(CMdlSeriesDlg, CDialog)
CMdlSeriesDlg::CMdlSeriesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMdlSeriesDlg::IDD, pParent)
	, mdlNumber(0)
	, m_currentCol(-1)	
	
{
	newIns.Name="";
	newIns.Code="";
	newIns.Person="";
	newIns.Dept="";
	newIns.Note="";
	newIns.mdlNum=-1;
	
}

CMdlSeriesDlg::~CMdlSeriesDlg()
{
}

void CMdlSeriesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_VSFLEXARRAYCTRL2, m_Flex);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_ADD_BUTTON, m_AddButton);
	DDX_Control(pDX, IDC_BUTTON1, m_OkAddButton);
	DDX_Control(pDX, IDC_BUTTON2, m_CancelAddButton);
} 

BEGIN_MESSAGE_MAP(CMdlSeriesDlg, CDialog)
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)	
	ON_BN_CLICKED(IDC_ADD_BUTTON, OnBnClickedAddButton)    
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_NOTIFY(LVN_HOTTRACK, IDC_LIST1, OnLvnHotTrackList1)
//	ON_WM_MEASUREITEM()
ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
ON_WM_MEASUREITEM()
END_MESSAGE_MAP()


// CMdlSeriesDlg ��Ϣ�������

BOOL CMdlSeriesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_TRACKSELECT);
	//���ñ��͹�����ʾ
    EnableToolTips(TRUE);		//
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	CWnd* pW=GetDlgItem(IDC_LIST1);//�õ�����ָ��
	m_tooltip.AddTool(pW,"ģ�͵�ϵ�б�");//���
    //���ø�����ť�ĳ�ʼ״̬
	//�½�����
	m_OkAddButton.EnableWindow(FALSE);
	m_CancelAddButton.EnableWindow(FALSE);
	m_AddButton.EnableWindow(TRUE);
	p_ItemProp.FreeExtra();
    SetMdlSWTXAndPerfList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ���öԻ�����Ҫ���в�����ģ�͵ı�ʶ��                                 */
/************************************************************************/
int CMdlSeriesDlg::SetMdlNumber(long Number)
{
	mdlNumber=Number;
	return 0;
}
/************************************************************************/
/* ��ģ�͵ĳ�ʼֵ��ӵ��б�ؼ���                                       */
/************************************************************************/
void CMdlSeriesDlg::SetMdlSWTXAndPerfList(void)
{
	CString    sql_lang;
	int        indexNo=0;
	CString    csIndex;
	int        perfIndexNo=0;
	int        num;
    CString    csNum;
	ItemProp   temp;
	if (mdlNumber==0) {
		return;
	}
	m_List.DeleteAllItems();
	m_List.DeleteAllColumn();
	m_List.InsertColumn(0,"���",LVCFMT_LEFT,40,-1);
	m_List.SetReadOnlyColumns(0);
	m_List.InsertColumn(1,"����",LVCFMT_LEFT,50,-1);
	m_List.SetReadOnlyColumns(1);
    m_List.InsertColumn(2,"���",LVCFMT_LEFT,50,-1);		
	m_List.SetReadOnlyColumns(2);
	m_List.InsertColumn(3,"��ʼֵ",LVCFMT_LEFT,50,-1);
	m_List.SetReadOnlyColumns(3);
	m_List.InsertItem(0,"");


	sql_lang.Format("SELECT * FROM [dbo].[PartManageTable] WHERE [Number]=%d",mdlNumber);
	//if(m_MdlInfoSet.IsOpen()) m_MdlInfoSet.Close();
	try{
		m_MdlInfoSet.Open(CRecordset::dynaset,_T(sql_lang),CRecordset::none);
		//{
		//	//AfxMessageBox("���ݿ�����ʧ�ܣ�");
		//	//return;
		//}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		delete e;		
		return;
	}

	//if(!m_MdlInfoSet.Open(CRecordset::dynaset,_T(sql_lang),CRecordset::none))
	//{
	//	AfxMessageBox("���ݿ�����ʧ�ܣ�");
	//	return;
	//}

	if (m_MdlInfoSet.IsBOF()) {
		m_MdlInfoSet.Close();
		return;
	}
	m_MdlInfoSet.MoveFirst();
	m_List.SetItemText(0,1,"ϵ����");
	m_List.SetItemText(0,2,"");
	m_List.SetItemText(0,3,m_MdlInfoSet.m_Name);
    m_MdlInfoSet.Close();	

	temp.para_num=-1;
	temp.type=-1;
	temp.note="����ϵ�е�����";
	p_ItemProp.Add(temp);

//    ����������Ա����
	if (m_mdlVariantSet.IsOpen()) {
		m_mdlVariantSet.Close();
	}
    sql_lang.Format("SELECT * FROM [dbo].[partVariantTable] WHERE [PrtNum]=%d",mdlNumber);
	if(!m_mdlVariantSet.Open(AFX_DB_USE_DEFAULT_TYPE,_T(sql_lang),CRecordset::none))
	{
		AfxMessageBox("����������Ա����ֵʱ����Ϊ���ݿ�����ʧ�ܣ�û����ʾ�������Ա���Ϣ��");
		return;
	}

	if (!m_mdlVariantSet.IsBOF()) {

		m_mdlVariantSet.MoveFirst();
		while (!m_mdlVariantSet.IsEOF()) {
			indexNo+=1;		

			csIndex.Format("%d",indexNo);
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
			num=(int)p_ItemProp.Add(temp);

			m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,indexNo,csIndex,0,0,0,num);
			m_List.SetItemText(indexNo,1,m_mdlVariantSet.m_Name);
			m_List.SetItemText(indexNo,2,"��������");
			m_List.SetItemText(indexNo,3,m_mdlVariantSet.m_Value);
			m_mdlVariantSet.MoveNext();
	     }
	}
	m_mdlVariantSet.Close();


    //����������Ա����
	if (m_mdlPerfparaSet.IsOpen()) {
		m_mdlPerfparaSet.Close();
	}
	sql_lang.Format("SELECT * FROM [dbo].[partPerfparaTable] WHERE [PrtNum]=%d",mdlNumber);
	if(!m_mdlPerfparaSet.Open(AFX_DB_USE_DEFAULT_TYPE,_T(sql_lang),CRecordset::none))
	{
		AfxMessageBox("������ܱ����ֵʱ����Ϊ���ݿ�����ʧ�ܣ�û����ʾ�������Ա���Ϣ��");
		return;
	}
	if (!m_mdlPerfparaSet.IsBOF()) {
		m_mdlPerfparaSet.MoveFirst();
		temp.note="";
		temp.para_num=-1;
		temp.type=-1;
		num=(int)p_ItemProp.Add(temp);
		m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,++indexNo,"",0,0,0,num);
		
		while (!m_mdlPerfparaSet.IsEOF()) {
			indexNo+=1;
			perfIndexNo+=1;
			csIndex.Format("%d",perfIndexNo);

			temp.para_num=m_mdlPerfparaSet.m_Number;
			temp.type=0;
			temp.note=m_mdlPerfparaSet.m_Note;
			num=(int)p_ItemProp.Add(temp);
			m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,indexNo,csIndex,0,0,0,num);
			m_List.SetItemText(indexNo,1,m_mdlPerfparaSet.m_Name);
			m_List.SetItemText(indexNo,2,"���ܲ���");
			m_List.SetItemText(indexNo,3,m_mdlPerfparaSet.m_Value);
			m_mdlPerfparaSet.MoveNext();
		}
	}	
	m_mdlPerfparaSet.Close();

    InitListWithMdlSeries();
}


LRESULT CMdlSeriesDlg::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;
	// TODO: Add your control notification handler code here

	return 0;
}
LRESULT CMdlSeriesDlg::PopulateComboList(WPARAM wParam, LPARAM lParam)
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
		if(iColIndex==m_currentCol)
		{
			pComboList->AddTail("1");
			pComboList->AddTail("0");		
		}			
	}
	return true;
}
/************************************************************************/
/* ���б�ؼ������ϵ�в���                                             */
/************************************************************************/
void CMdlSeriesDlg::InitListWithMdlSeries(void)
{
	int           itemNum=1;   //�б����ţ��ӵڶ��ʼ
	int           insNum=0;
	CString       sql_lang="";	
	int           insNo=4;
	long          itemData=0;
	try{
		//��ѯģ�͵�����ϵ��
		sql_lang.Format("SELECT * FROM [dbo].[PartInsTable] WHERE [PrtNum]=%d",mdlNumber);
		if(m_InsTableSet.IsOpen()) m_InsTableSet.Close();
		if(!m_InsTableSet.Open(AFX_DB_USE_DEFAULT_TYPE,_T(sql_lang),CRecordset::none))
		{
			AfxMessageBox("�����ݿ��л�ȡģ�͵�ϵ����Ϣʱ����Ϊ���ݿ�����ԭ���޷�������ȡ��");
			return;
		}
		if(m_InsTableSet.IsBOF()) 
		{
			m_InsTableSet.Close();
			return;
		}	

		//ѭ���ҳ�����ϵ�е��������Ա�ֵ�����ܲ������ֵ
		CpartInsValueTableSet temp_ins_value_Set(m_InsTableSet.m_pDatabase);
		CpartInsPerfValueTableSet temp_ins_Perf_Set(m_InsTableSet.m_pDatabase);

		m_InsTableSet.MoveFirst();
		while (!m_InsTableSet.IsEOF()) {
			    sql_lang.Format("%d",insNo-3);
			    m_List.InsertColumn(insNo,sql_lang,LVCFMT_LEFT,50,-1);
                m_List.SetReadOnlyColumns(insNo,true);
				m_List.SetItemText(0,insNo,m_InsTableSet.m_InsName);
			//�������Ա����ֵ
			do {
				//��ò�����
				
				itemData=p_ItemProp[(int)m_List.GetItemData(itemNum)].para_num;
				//sql_lang.Format("%d",(int)m_List.GetItemData(itemNum));
				//AfxMessageBox(sql_lang);
    //            if(itemData==-1) continue;
				sql_lang.Format("SELECT * FROM [dbo].[partInsValueTable] WHERE [PrtNum]=%d AND [InsNum]=%d AND [Num]=%d"
					,mdlNumber,m_InsTableSet.m_InsNum,itemData);
				temp_ins_value_Set.Open(AFX_DB_USE_DEFAULT_TYPE,_T(sql_lang),CRecordset::none);
				if (!temp_ins_value_Set.IsBOF()) {
					temp_ins_value_Set.MoveFirst();
					m_List.SetItemText(itemNum,insNo,temp_ins_value_Set.m_Value);
				}			
				temp_ins_value_Set.Close();
				itemNum+=1;
			} while(p_ItemProp[(int)m_List.GetItemData(itemNum)].para_num!=-1);


			itemNum+=1;//�ո����ӹ�

			//���ܲ�����
			do {
				//��ò�����
				itemData=p_ItemProp[(int)m_List.GetItemData(itemNum)].para_num;

				sql_lang.Format("SELECT * FROM [dbo].[partInsPerfValueTable] WHERE [PrtNum]=%d AND [InsNum]=%d AND [Num]=%d"
					,mdlNumber,m_InsTableSet.m_InsNum,itemData);

				temp_ins_Perf_Set.Open(AFX_DB_USE_DEFAULT_TYPE,_T(sql_lang),CRecordset::none);
				if (!temp_ins_Perf_Set.IsBOF()) {
					temp_ins_Perf_Set.MoveFirst();
					m_List.SetItemText(itemNum,insNo,temp_ins_Perf_Set.m_Value);
				}			
				temp_ins_Perf_Set.Close();
				itemNum+=1;

			} while(itemNum<(int)m_List.GetItemCount());		

			m_InsTableSet.MoveNext();
			itemNum=1;
			insNo+=1;
		}

		m_InsTableSet.Close();	

	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		if(m_InsTableSet.IsOpen()) m_InsTableSet.Close();
		if(m_InsValueTableSet.IsOpen()) m_InsValueTableSet.Close();
		if(m_InsPerfValueTableSet.IsOpen()) m_InsPerfValueTableSet.Close();
		return;
	}

}
/************************************************************************/
/* ���ϵ��                                                             */
/************************************************************************/
void CMdlSeriesDlg::OnBnClickedAddButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAddMdlSeriesDlg dlg;
	CString          sql_lang;
	int              colNum=0;
	int              itemNum=0;
	CString          str="";
	if(m_MdlInfoSet.IsOpen()) m_MdlInfoSet.Close();
	sql_lang.Format("SELECT * FROM [dbo].[PartManageTable] WHERE [Number]=%d",mdlNumber);
	m_MdlInfoSet.Open(AFX_DB_USE_DEFAULT_TYPE,_T(sql_lang),CRecordset::none);
	if (m_MdlInfoSet.IsBOF()) 
	{
		m_MdlInfoSet.Close();
		return;
	}
	dlg.SetMdlNumberAndCode(mdlNumber,m_MdlInfoSet.m_MdlCode);
	m_MdlInfoSet.Close();
	//������ϵ�е����ơ����롢�ȵ���Ϣ
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}
	//ȷ��֮�󣬸ı䰴ť��״̬��ʹ��Ӱ�ť��Ч��ʹ�ύ�ͳ�����ť��Ч
	m_AddButton.EnableWindow(FALSE);
	m_OkAddButton.EnableWindow(TRUE);
	m_CancelAddButton.EnableWindow(TRUE);
	//�����Ϣ
	newIns=dlg.newMdl;	
	//������һ��ϵ��Ϊ���ɱ༭
	if (m_currentCol!=-1) {
		if(!m_List.IsReadOnly(m_currentCol)) m_List.SetReadOnlyColumns(m_currentCol,true);
		if(m_List.IsCombo(m_currentCol)) m_List.SetComboColumns(m_currentCol,false);
	}
	
	//�����µ���
	colNum=(int)m_List.GetColumnCounts();
	str.Format("%d",colNum-3);
	m_List.InsertColumn(colNum,str,LVCFMT_LEFT,50,-1);
	m_List.SetItemText(0,colNum,newIns.Name); 
    itemNum=(int)m_List.GetItemCount();
	for(int i=1;i<itemNum;i++)
	{
		m_List.SetItemText(i,colNum,m_List.GetItemText(i,3));
	}
	m_currentCol=colNum;//���õ�ǰ���Ա༭���к�
	//m_List.SetReadOnlyColumns(m_currentCol,true);
	
}
/************************************************************************/
/* �����б����ݲ��������ͣ����õ�Ԫ������                             */
/************************************************************************/
void CMdlSeriesDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int      item=0;
	
	CString  str;
	item=(int)m_List.GetHotItem();	
	
	if (item==0) {
		m_List.SetReadOnlyColumns(m_currentCol,true);
		if(m_List.IsCombo(m_currentCol)) m_List.SetComboColumns(m_currentCol,false);
	}	
	else
	{
		if(m_List.IsCombo(m_currentCol)) m_List.SetComboColumns(m_currentCol,false);
        m_List.SetReadOnlyColumns(m_currentCol,true);
		switch(p_ItemProp[(int)m_List.GetItemData(item)].type) {
			case 0:
				str="";				
					m_List.SetReadOnlyColumns(m_currentCol,false);
					m_List.SetColumnValidEditCtrlCharacters(str,m_currentCol);		
				break;
			case 1:
				str="987654321.0";				
					m_List.SetReadOnlyColumns(m_currentCol,false);
					m_List.SetColumnValidEditCtrlCharacters(str,m_currentCol);		
				break;
			case 2:
				str="1230456789";				
					m_List.SetReadOnlyColumns(m_currentCol,false);
					m_List.SetColumnValidEditCtrlCharacters(str,m_currentCol);				
				break;
			case 3: //����ǲ����ͣ�����Ӹ����б���ǡ���
				m_List.SetReadOnlyColumns(m_currentCol,true);
				m_List.SetComboColumns(m_currentCol,true);
				break;
			default:
				break;
		}

	}	
	
	
	*pResult = 0;
}
/************************************************************************/
/* Ԥ��������ı�                                                       */
/************************************************************************/
BOOL CMdlSeriesDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	
	m_tooltip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

/************************************************************************/
/* //����TOOLTIP��Ϊ��ǰѡ����ı��͹���ע�ͣ�                        */
/************************************************************************/
void CMdlSeriesDlg::OnLvnHotTrackList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    int      item=0;
	item=(int)m_List.GetHotItem();
	if (item==-1) {
		return;
	}
	CWnd* pW=GetDlgItem(IDC_LIST1);  
	m_tooltip.UpdateTipText(p_ItemProp[(int)m_List.GetItemData(item)].note,pW,0);
	*pResult = 0;
}

/************************************************************************/
/* ������ӵ�ϵ�����뵽���ݿ��У����ϵ�е���ӹ���                     */
/************************************************************************/
void CMdlSeriesDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ʼ��鹤��
	int      newInsNum=0;
	CString  sql_lang;
	int      itemNum=0;
	int      itemCurrent=0;

    if(!CheckNewSeries()) return;
	newInsNum=GetMdlSeriesMaxNumber()+1;

    CDatabase * m_db;
    m_db=m_InsTableSet.m_pDatabase;
	m_InsValueTableSet.m_pDatabase=m_db;
	m_InsPerfValueTableSet.m_pDatabase=m_db;
	if(m_InsTableSet.IsOpen())           m_InsTableSet.Close();
	if(m_InsValueTableSet.IsOpen())      m_InsValueTableSet.Close();
	if(m_InsPerfValueTableSet.IsOpen())  m_InsPerfValueTableSet.Close();

	if (m_db->CanTransact()) {
		if(!m_InsTableSet.Open(CRecordset::dynaset)) return;
		if(!m_InsValueTableSet.Open(CRecordset::dynaset)) return;
		if(!m_InsPerfValueTableSet.Open(CRecordset::dynaset)) return;

		if(!m_db->BeginTrans())  return;
		//���ϵ����Ϣ
		try{
			m_InsTableSet.AddNew();
			m_InsTableSet.m_PrtNum=mdlNumber;
			m_InsTableSet.m_InsNum=newInsNum;
			m_InsTableSet.m_InsName=newIns.Name;
			m_InsTableSet.m_InsCode=newIns.Code;
			m_InsTableSet.m_InsNote=newIns.Note;
			m_InsTableSet.m_InsCreaterName=newIns.Person;
			m_InsTableSet.m_InsCreateDpt=newIns.Dept;
			m_InsTableSet.Update();
            itemNum=(int)m_List.GetItemCount();
			for(itemCurrent=1;itemCurrent<itemNum;itemCurrent++)
			{
				//�����ǰ��������еĲ�����Ϊ-1
				if(p_ItemProp[(int)m_List.GetItemData(itemCurrent)].para_num==-1 ) break;
				m_InsValueTableSet.AddNew();
				m_InsValueTableSet.m_PrtNum=mdlNumber;
				m_InsValueTableSet.m_InsNum=newInsNum;
				m_InsValueTableSet.m_Num=p_ItemProp[(int)m_List.GetItemData(itemCurrent)].para_num;
				sql_lang.Format("��%d,ֵ%s",itemCurrent,m_List.GetItemText(itemCurrent,m_currentCol));
				AfxMessageBox(sql_lang);
				m_InsValueTableSet.m_Value=m_List.GetItemText(itemCurrent,m_currentCol);
				m_InsValueTableSet.Update();
			}
			itemCurrent+=1;
			for(;itemCurrent<itemNum;itemCurrent++)
			{
				m_InsPerfValueTableSet.AddNew();
				m_InsPerfValueTableSet.m_PrtNum=mdlNumber;
				m_InsPerfValueTableSet.m_InsNum=newInsNum;
				m_InsPerfValueTableSet.m_Num=p_ItemProp[(int)m_List.GetItemData(itemCurrent)].para_num;
				m_InsPerfValueTableSet.m_Value=m_List.GetItemText(itemCurrent,m_currentCol);
				m_InsPerfValueTableSet.Update();
			}
			m_db->CommitTrans();

		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			delete e;
			m_db->Rollback();
			if(m_InsTableSet.IsOpen())           m_InsTableSet.Close();
			if(m_InsValueTableSet.IsOpen())      m_InsValueTableSet.Close();
			if(m_InsPerfValueTableSet.IsOpen())  m_InsPerfValueTableSet.Close();
			return;
		}
		m_InsTableSet.Close();
		m_InsValueTableSet.Close();
		m_InsPerfValueTableSet.Close();

		//���ð�ť
		m_AddButton.EnableWindow(TRUE);
		m_CancelAddButton.EnableWindow(FALSE);
		m_OkAddButton.EnableWindow(FALSE);
		//���ø���Ϊ���ɱ༭
		if(!m_List.IsReadOnly(m_currentCol)) m_List.SetReadOnlyColumns(m_currentCol,true);
		if(m_List.IsCombo(m_currentCol))     m_List.SetComboColumns(m_currentCol,false);
		m_currentCol=-1;
		//����������Ա�
		//������ܲ���
		
	}
}
/************************************************************************/
/* ���µ�ϵ�н��м��                                                   */
/************************************************************************/
bool CMdlSeriesDlg::CheckNewSeries(void)
{
	int     itemNum=0;
	itemNum=(int)m_List.GetItemCount();
	for(int i=1;i<itemNum;i++)
	{
		if(m_List.GetItemText(i,3).IsEmpty()) continue;
		if(m_List.GetItemText(i,m_currentCol).IsEmpty())
		{
			CString  temp;			
			temp.Format("����%s,����Ϊ�գ�",m_List.GetItemText(i,1));
			AfxMessageBox(temp);
			return false;
		}
	}
	return true;
}

/************************************************************************/
/* ��������ӵ�ϵ��                                                     */
/************************************************************************/
void CMdlSeriesDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_AddButton.EnableWindow(TRUE);
	m_OkAddButton.EnableWindow(FALSE);
	m_CancelAddButton.EnableWindow(FALSE);
	if(!m_List.IsReadOnly(m_currentCol)) m_List.SetReadOnlyColumns(m_currentCol,true);
	if(m_List.IsCombo(m_currentCol))     m_List.SetComboColumns(m_currentCol,false);
	m_List.DeleteColumn(m_currentCol);
	m_currentCol=-1;
}
/************************************************************************/
/* ���ģ��ϵ�������ĺ�                                               */
/************************************************************************/
long CMdlSeriesDlg::GetMdlSeriesMaxNumber(void)
{
	long      maxNum=0;
	CString   sql_lang;
	if(m_InsTableSet.IsOpen()) m_InsTableSet.Close();
	sql_lang.Format("SELECT * FROM [dbo].[PartInsTable] WHERE [PrtNum]=%d",mdlNumber);
	m_InsTableSet.Open(AFX_DB_USE_DEFAULT_TYPE,_T(sql_lang),CRecordset::none);
	if (m_InsTableSet.IsBOF()) {
		m_InsTableSet.Close();
		return maxNum;
	}
	m_InsTableSet.MoveFirst();
	while (!m_InsTableSet.IsEOF()) {
		maxNum=maxNum>=m_InsTableSet.m_InsNum?maxNum:m_InsTableSet.m_InsNum;
		m_InsTableSet.MoveNext();
	}
	m_InsTableSet.Close();
	return maxNum;
}
///************************************************************************/
///* ������ӵ�ϵ�����뵽���ݿ��У����ϵ�е���ӹ���                     */
///************************************************************************/
//void CMdlSeriesDlg::OnBnClickedButton1()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	//��ʼ��鹤��
//	int      newInsNum=0;
//	CString  sql_lang;
//	int      itemNum=0;
//	int      itemCurrent=0;
//	//CpartInsValueTableSet *     temp_ins_value_Set;
//	//CpartInsPerfValueTableSet * temp_ins_Perf_Set;
//
//	if(!CheckNewSeries()) return;
//	newInsNum=GetMdlSeriesMaxNumber()+1;    
//	CDatabase * m_db;
//	m_db=m_InsTableSet.m_pDatabase;
//    CpartInsValueTableSet *  temp_ins_value_Set=new CpartInsValueTableSet(m_db);
//    CpartInsPerfValueTableSet * temp_ins_Perf_Set=new CpartInsPerfValueTableSet(m_db);
//
//	if(m_InsTableSet.IsOpen())           m_InsTableSet.Close();
//
//	if (m_db->CanTransact()) {
//		if(!m_InsTableSet.Open(CRecordset::dynaset)) return;
//		if(!temp_ins_value_Set->Open(CRecordset::dynaset)) return;
//		if(!temp_ins_Perf_Set->Open(CRecordset::dynaset)) return;
//
//		if(!m_db->BeginTrans())  return;
//		//���ϵ����Ϣ
//		try{
//			m_InsTableSet.AddNew();
//			m_InsTableSet.m_PrtNum=mdlNumber;
//			m_InsTableSet.m_InsNum=newInsNum;
//			m_InsTableSet.m_InsName=newIns.Name;
//			m_InsTableSet.m_InsCode=newIns.Code;
//			m_InsTableSet.m_InsNote=newIns.Note;
//			m_InsTableSet.m_InsCreaterName=newIns.Person;
//			m_InsTableSet.m_InsCreateDpt=newIns.Dept;
//			m_InsTableSet.Update();
//			itemNum=(int)m_List.GetItemCount();
//			for(itemCurrent=1;itemCurrent<itemNum;itemCurrent++)
//			{
//				//�����ǰ��������еĲ�����Ϊ-1
//				if(p_ItemProp[(int)m_List.GetItemData(itemCurrent)].para_num==-1 ) break;
//				temp_ins_value_Set->AddNew();
//				temp_ins_value_Set->m_PrtNum=mdlNumber;
//				temp_ins_value_Set->m_InsNum=newInsNum;
//				temp_ins_value_Set->m_Num=p_ItemProp[(int)m_List.GetItemData(itemCurrent)].para_num;
//				/*sql_lang.Format("��%d,ֵ%s",itemCurrent,m_List.GetItemText(itemCurrent,m_currentCol));
//				AfxMessageBox(sql_lang);*/
//				temp_ins_value_Set->m_Value=m_List.GetItemText(itemCurrent,m_currentCol);
//				temp_ins_value_Set->Update();
//			}
//			itemCurrent+=1;
//			for(;itemCurrent<itemNum;itemCurrent++)
//			{
//				temp_ins_Perf_Set->AddNew();
//				temp_ins_Perf_Set->m_PrtNum=mdlNumber;
//				temp_ins_Perf_Set->m_InsNum=newInsNum;
//				temp_ins_Perf_Set->m_Num=p_ItemProp[(int)m_List.GetItemData(itemCurrent)].para_num;
//				temp_ins_Perf_Set->m_Value=m_List.GetItemText(itemCurrent,m_currentCol);
//				temp_ins_Perf_Set->Update();
//			}
//			m_db->CommitTrans();
//
//		}
//		catch (CDBException * e) {
//			AfxMessageBox(e->m_strError);
//			delete e;
//			m_db->Rollback();
//			if(m_InsTableSet.IsOpen())           m_InsTableSet.Close();
//			if (temp_ins_Perf_Set->IsOpen())     temp_ins_Perf_Set->Close();
//            if (temp_ins_value_Set->IsOpen())     temp_ins_value_Set->Close();
//		    delete temp_ins_Perf_Set,temp_ins_value_Set;
//			return;
//		}
//		m_InsTableSet.Close();
//		if (temp_ins_Perf_Set->IsOpen())     temp_ins_Perf_Set->Close();
//		if (temp_ins_value_Set->IsOpen())     temp_ins_value_Set->Close();
//        delete temp_ins_Perf_Set,temp_ins_value_Set;
//
//		//���ð�ť
//		m_AddButton.EnableWindow(TRUE);
//		m_CancelAddButton.EnableWindow(FALSE);
//		m_OkAddButton.EnableWindow(FALSE);
//		//���ø���Ϊ���ɱ༭
//		if(!m_List.IsReadOnly(m_currentCol)) m_List.SetReadOnlyColumns(m_currentCol,true);
//		if(m_List.IsCombo(m_currentCol))     m_List.SetComboColumns(m_currentCol,false);
//		m_currentCol=-1;
//		//����������Ա�
//		//������ܲ���
//
//	}
//}

//void CMdlSeriesDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
//{
//	// TODO: Add your message handler code here and/or call default
//
//	//CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
//	if(nIDCtl == IDC_LIST1)//IDC_MY_LISTΪList��ID
//	{
//		lpMeasureItemStruct->itemHeight =60;
//	}
//	
//}
void CMdlSeriesDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDCtl == IDC_LIST1)//IDC_MY_LISTΪList��ID
		{
			lpMeasureItemStruct->itemHeight =10;
	}
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
