// NewAlterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "NewAlterDlg.h"
#include "MyDatabase.h"
#include "Compute.h"
#include "afxtempl.h"
#include "CommonFuncClass.h"
#include "ProMdl.h"
#include "ProWindows.h"
#include "ProUtil.h"
#include "ProSolid.h"
#include "FtpFile.h"
#include "DetailInfoClass.h"
#include "PrtClass.h"
#include "NoPass.h"

#include "QuerySeries.h"
int thePrtModleNum;
CStringArray strValueArray;
CStringArray strInputValueArray;
CArray<int,int> IntNumArray;
CArray<int,int> IntSelNumArray;

// CNewAlterDlg �Ի���
extern CPtrArray p_classarray;
extern CMyDatabase m_db;
extern CStringArray RelationSet;
extern FtpInfo Ftp_struc;
extern CArray<Database,Database> Database_struc;
extern CArray<Database,Database> Database_struc_temp;
extern CArray<ProDrawing,ProDrawing>  DrawingList;


void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);

IMPLEMENT_DYNAMIC(CNewAlterDlg, CDialog)
CNewAlterDlg::CNewAlterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewAlterDlg::IDD, pParent)
	,p_Picture(NULL)
{
}

CNewAlterDlg::~CNewAlterDlg()
{
	if (p_Picture!=NULL)
	{
		p_Picture->Release();
	}
}

void CNewAlterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_pic);
	DDX_Control(pDX, IDC_LIST_DIM, m_variantdim);
}


BEGIN_MESSAGE_MAP(CNewAlterDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_PRE, OnBnClickedButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnBnClickedButtonNext)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_QuerySeries, OnQuerySeries)
END_MESSAGE_MAP()


// CNewAlterDlg ��Ϣ�������

void CNewAlterDlg::PreInitDialog()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::PreInitDialog();
}

BOOL CNewAlterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CCommonFuncClass commonfun;
	//ȥ��������û�еĻ�����Ϣ
	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	//��ʾͼƬ
	ListView_SetExtendedListViewStyle(m_variantdim.m_hWnd, LVS_EX_GRIDLINES/*|LVS_EX_FULLROWSELECT*/);
	m_variantdim.InsertColumn(0,"�ߴ���",LVCFMT_LEFT,60,-1);
	m_variantdim.InsertColumn(1,"�ߴ�ֵ",LVCFMT_LEFT,60,-1);
	m_variantdim.InsertColumn(2,"��ƫ��",LVCFMT_LEFT,50,-1);
	m_variantdim.InsertColumn(3,"��ƫ��",LVCFMT_LEFT,50,-1);
	m_variantdim.InsertColumn(4,"ע��",LVCFMT_LEFT,80,-1);
	m_variantdim.SetReadOnlyColumns(0,false);
	GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);

	//��ѯ���ݿ�
	CCompute compute;
	CString strSql;
	
	Database_struc_temp.RemoveAll();
	if(!compute.GetVariantForPart(((DetailInfoClass *)p_classarray.GetAt(s))->solid,MdlNum,TRUE))
	{
		AfxMessageBox("�޷��õ���ģ�͵Ŀɱ�ߴ�");
		return FALSE;
	}
	if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid))		 //Ҳ��Ҫ����ƫ����Ϣ
	{
		return FALSE;
	}
	ShowPic(atol(MdlNum),1);
	int t=0;
	//��ʾ�����ߴ�
	for(int i=0;i<Database_struc_temp.GetSize();i++)	
	{
		int indexNo=m_variantdim.InsertItem(LVIF_TEXT|LVIF_PARAM,t,Database_struc_temp[i].name,
			0,0,0,Database_struc_temp[i].number);
		m_variantdim.SetItemText(indexNo,1,Database_struc_temp[i].value);
		m_variantdim.SetItemText(indexNo,2,Database_struc_temp[i].up);	 //��ƫ��
		m_variantdim.SetItemText(indexNo,3,Database_struc_temp[i].down);	 //��ƫ��
		m_variantdim.SetItemText(indexNo,4,Database_struc_temp[i].note);
		t++;
	}
	return TRUE;  
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ����ͼƬ���ݿ�                                                       */
/************************************************************************/

BOOL CNewAlterDlg::ShowPic(int num,int Number)
{
	BYTE *pBuffer=NULL;
	CRect rect;
	m_pic.GetClientRect(&rect);
	CString strSql;
	int nSize=0;
	if (p_Picture!=NULL)
	{
		p_Picture->Release();
		p_Picture=NULL;
	}
	if (!m_db.DatabaseState())
	{
		m_db.ReOpen();
	}
	strSql.Format("SELECT * FROM NPartPic WHERE PrtNum=%d AND Number=%d",num,Number);
	int Z=m_db.Query(strSql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		m_pic.ClientToScreen(&rect);
		this->ScreenToClient(&rect);
		RedrawWindow(&rect,NULL,RDW_INVALIDATE | RDW_ERASE);
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			nSize=m_db.getBinary("File",&pBuffer);
			picNum=m_db.getInt("Number");
			break;
		} while(m_db.MoveNext());
	}
	DBOpenPic(pBuffer,nSize,&p_Picture);
	OnPaint();
	strSql.Format("SELECT * FROM NPartPic WHERE PrtNum=%d AND Number=%d",num,picNum+1);
	if (m_db.Query(strSql)==0)
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE); 
	}
	else
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
	strSql.Format("SELECT * FROM NPartPic WHERE PrtNum=%d AND Number=%d",num,picNum-1);
	if (m_db.Query(strSql)==0)
	{
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE); 
	}
	else
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(TRUE);
	delete [] pBuffer;
	return TRUE;
}
/************************************************************************/
/* �Ѷ����Ƶ�����ת��ΪͼƬ                                             */
/************************************************************************/
BOOL CNewAlterDlg::DBOpenPic(BYTE *pBuffer,int nSize,IPicture ** pic)
{			
	HGLOBAL hGlobal=::GlobalAlloc(GMEM_MOVEABLE,nSize);
	LPSTR buffer=(LPSTR)GlobalLock(hGlobal);
	memcpy(buffer,pBuffer,nSize);

	IStream *  pstm =NULL; // ��Ҫһ������stream��			
	CreateStreamOnHGlobal(buffer, TRUE, &pstm);
	if (!SUCCEEDED(OleLoadPicture(pstm,nSize,TRUE,IID_IPicture,(LPVOID*)pic)))
	{
		AfxMessageBox("��ͼƬ");
		return FALSE;
	}
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal)	;
	pstm->Release();
	return TRUE;
}

void CNewAlterDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ������Ϣ����������
	CRect rect;
	if (p_Picture!=NULL)
	{
		CSize m_size;

		m_pic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		p_Picture->get_Width(&m_size.cx);
		p_Picture->get_Height(&m_size.cy);
		m_pic.MoveWindow(&rect,TRUE);
		p_Picture->Render(*GetDC(),rect.left,rect.bottom,rect.Width(),-rect.Height(),0,0,m_size.cx,m_size.cy,NULL);
	}
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CNewAlterDlg::OnBnClickedButtonPre()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowPic(atol(MdlNum),picNum-1);
}

void CNewAlterDlg::OnBnClickedButtonNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowPic(atol(MdlNum),picNum+1);
}
/************************************************************************/
/* ���������ı���                                                     */
/************************************************************************/
void CNewAlterDlg::OnBnClickedOk()
{
	if(ExamTheDatas())
	{
		MessageBox("������֤��ͨ����","��ʾ��Ϣ");

		
		CCommonFuncClass commonfun;
		CPrtClass prtclass;
		CCompute compute;
		CFtpFile ftp;
		CString Name,SolidPath,DrwPath,temppath;
		ProName w_name;
		ProError status;
		ProType type;
		int w_id;
		//�õ��б�������
		int nnn=m_variantdim.GetItemCount();
		for (int i=0;i<m_variantdim.GetItemCount();i++)
		{
			Database_struc_temp[i].value=m_variantdim.GetItemText(i,1);
			Database_struc_temp[i].up=m_variantdim.GetItemText(i,2);
			Database_struc_temp[i].down=m_variantdim.GetItemText(i,3);
		}
		RelationSet.RemoveAll();
		if(!compute.GetRelationForPart(MdlNum))
		{
			AfxMessageBox("��ϵʽ��ȡ����");
			return;
		}
		Database_struc.RemoveAll();
		compute.GetVariantForPart(((DetailInfoClass *)p_classarray.GetAt(s))->solid,MdlNum,FALSE);
		//��ʱ�Ľṹ������ʽ�Ľṹ
		for (int i=0;i<Database_struc_temp.GetSize();i++)
		{
			for (int j=0;j<Database_struc.GetSize();j++)
			{
				if (Database_struc[j].PARADIMSYMBOL==Database_struc_temp[i].PARADIMSYMBOL)
				{
					Database_struc[j].value=Database_struc_temp[i].value;
					Database_struc[j].up=Database_struc_temp[i].up;
					Database_struc[j].down=Database_struc_temp[i].down;
					Database_struc[j].IsOk=TRUE; 
				}
			}
		}
		//����ģ��
		for (int j=0;j<Database_struc.GetSize();j++)
		{
			if (Database_struc[j].IsOrNot)
			{
				compute.UpdateAllRelation(Database_struc[j].PARADIMSYMBOL,j);
			}
		}
		//�ж��Ƿ��Ѿ�ȫ����������
		for (int j=0;j<RelationSet.GetSize();j++)
		{
			if(compute.IsOnlyOne(j)!="����")
			{
				AfxMessageBox("�㽨�Ĺ�ϵʽ�����޷�Ϊ����㣡");
				return;
			}
		}
		compute.AsmGeneration(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
		//��ʾģ��
		status = ProMdlTypeGet((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid,(ProMdlType*)&type);
		status= ProMdlNameGet((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid, w_name);
		status= ProObjectwindowCreate(w_name, type, &w_id);
		if(!status== PRO_TK_NO_ERROR)
		{
			return;
		}
		ProWindowRepaint(PRO_VALUE_UNUSED);
		//commonfun.SetParam(s);
		((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="F";
		status = ProMdlDisplay((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid);
		status = ProWindowActivate(w_id);
		//commonfun.AddMdlInfo(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
		OnOK();
		
	}
	else
	{
		CNoPass m_dlg;
		if(m_dlg.DoModal()==IDOK)
		{
			//MessageBox("���ɣ�","��ʾ��Ϣ");
			
			CCommonFuncClass commonfun;
			CPrtClass prtclass;
			CCompute compute;
			CFtpFile ftp;
			CString Name,SolidPath,DrwPath,temppath;
			ProName w_name;
			ProError status;
			ProType type;
			int w_id;
			//�õ��б�������
			for (int i=0;i<m_variantdim.GetItemCount();i++)
			{
				Database_struc_temp[i].value=m_variantdim.GetItemText(i,1);
				Database_struc_temp[i].up=m_variantdim.GetItemText(i,2);
				Database_struc_temp[i].down=m_variantdim.GetItemText(i,3);
			}
			RelationSet.RemoveAll();
			if(!compute.GetRelationForPart(MdlNum))
			{
				AfxMessageBox("��ϵʽ��ȡ����");
				return;
			}
			Database_struc.RemoveAll();
			compute.GetVariantForPart(((DetailInfoClass *)p_classarray.GetAt(s))->solid,MdlNum,FALSE);
			//��ʱ�Ľṹ������ʽ�Ľṹ
			for (int i=0;i<Database_struc_temp.GetSize();i++)
			{
				for (int j=0;j<Database_struc.GetSize();j++)
				{
					if (Database_struc[j].PARADIMSYMBOL==Database_struc_temp[i].PARADIMSYMBOL)
					{
						Database_struc[j].value=Database_struc_temp[i].value;
						Database_struc[j].up=Database_struc_temp[i].up;
						Database_struc[j].down=Database_struc_temp[i].down;
						Database_struc[j].IsOk=TRUE; 
					}
				}
			}
			//����ģ��
			for (int j=0;j<Database_struc.GetSize();j++)
			{
				if (Database_struc[j].IsOrNot)
				{
					compute.UpdateAllRelation(Database_struc[j].PARADIMSYMBOL,j);
				}
			}
			//�ж��Ƿ��Ѿ�ȫ����������
			for (int j=0;j<RelationSet.GetSize();j++)
			{
				if(compute.IsOnlyOne(j)!="����")
				{
					AfxMessageBox("�㽨�Ĺ�ϵʽ�����޷�Ϊ����㣡");
					return;
				}
			}
			compute.AsmGeneration(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
			//��ʾģ��
			status = ProMdlTypeGet((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid,(ProMdlType*)&type);
			status= ProMdlNameGet((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid, w_name);
			status= ProObjectwindowCreate(w_name, type, &w_id);
			if(!status== PRO_TK_NO_ERROR)
			{
				return;
			}
			ProWindowRepaint(PRO_VALUE_UNUSED);
			//commonfun.SetParam(s);
			((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="F";
			status = ProMdlDisplay((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid);
			status = ProWindowActivate(w_id);
			//commonfun.AddMdlInfo(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
			OnOK();
			
		}
		
	}

	
}

void CNewAlterDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: �ڴ������Ϣ����������
}

void CNewAlterDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ProName w_name;
	ProError status;
	ProType type;
	int w_id;
	CPrtClass prtclass;
	CCommonFuncClass commonfun;
	status = ProMdlTypeGet((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid,(ProMdlType*)&type);
	status= ProMdlNameGet((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid, w_name);
	status= ProObjectwindowCreate(w_name, type, &w_id);
	if(!status== PRO_TK_NO_ERROR)
	{
		return;
	}
	ProWindowRepaint(PRO_VALUE_UNUSED);
	if (!commonfun.SetParam(s)) 
	{
		AfxMessageBox("���ò�������");
		return;
	}
	//if (!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
	//{
	//	AfxMessageBox("���治�ɹ�");
	//	return;
	//}
	((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="F";
	status = ProMdlDisplay((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid);
	status = ProWindowActivate(w_id);
	OnCancel();
}
//�鿴ϵ�л�
void CNewAlterDlg::OnQuerySeries()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	thePrtModleNum=atol(MdlNum);
	CQuerySeries m_dlg;
	m_dlg.DoModal();
}
//��������
BOOL CNewAlterDlg::ExamTheDatas(void)
{
	thePrtModleNum=atol(MdlNum);
	//get the intput data
	CString str;
	strInputValueArray.RemoveAll();
	int nItemCount=0;
	nItemCount=m_variantdim.GetItemCount();
	if(nItemCount==0)
		return TRUE;
	for(int i=0;i<nItemCount;i++)
	{
		str=m_variantdim.GetItemText(i,1);
		strInputValueArray.Add(str);
	}
	int nn=strInputValueArray.GetCount();

	//get all the instance value
	//get the prtnum first
	CString strSql;
	strSql.Format("select DISTINCT PrtNum from NPartInsVariantTable where MdlNum=%d",thePrtModleNum);
	if(m_db.Query(strSql)!=0)
	{
		int num;
		m_db.MoveBegin();
		IntNumArray.RemoveAll();
		do{
			num=m_db.getInt("PrtNum");
			IntNumArray.Add(num);
		}while(m_db.MoveNext());
	}

	//get the str value
	strValueArray.RemoveAll();
	for(int i=0;i<IntNumArray.GetCount();i++)
	{
		int num=0;
		num=IntNumArray.GetAt(i);
		strSql.Format("select * from NPartInsVariantTable where PrtNum=%d",num);
		if(m_db.Query(strSql)!=0)
		{
			m_db.MoveBegin();
			do {
				str=m_db.getString("Value");
				strValueArray.Add(str);
			} while(m_db.MoveNext());
		}
	}

	//compare the values
	//compute the standard value
	double theStandardValue=0.0;
	for(int i=0;i<strInputValueArray.GetCount();i++)
	{
		theStandardValue+=abs(atof(strInputValueArray.GetAt(i)))*0.1;
	}

	//compute the result
	int nCount=0;
	double theResult=0;
	IntSelNumArray.RemoveAll();
	nCount=strValueArray.GetCount()/strInputValueArray.GetCount();
	for(int i=0;i<nCount;i++)
	{
		theResult=0;
		for(int j=0;j<strInputValueArray.GetCount();j++)
		{
			theResult+=abs(atof(strValueArray.GetAt(i*strInputValueArray.GetCount()+j))-atof(strInputValueArray.GetAt(j)));
		}
		if(theResult<=theStandardValue)
			IntSelNumArray.Add(IntNumArray.GetAt(i));
	}
	if(IntSelNumArray.GetCount()>0)
        return false;
	return TRUE;
}
