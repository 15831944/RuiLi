// ProductDelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "ProductDelDlg.h"
#include "MyDatabase.h"
#include "FtpFile.h"
#include "CommonFuncClass.h"
#include "ProWindows.h"
#include "ProUtil.h"

extern FtpInfo Ftp_struc;
extern CMyDatabase m_db;
// CProductDelDlg �Ի���

IMPLEMENT_DYNAMIC(CProductDelDlg, CDialog)
CProductDelDlg::CProductDelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProductDelDlg::IDD, pParent)
	, m_inscode(_T(""))
	, m_all(0)
{
}

CProductDelDlg::~CProductDelDlg()
{
}

void CProductDelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_product);
	DDX_Text(pDX, IDC_EDIT_INSCODE, m_inscode);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_type);
	DDX_Check(pDX, IDC_CHECK_ALL, m_all);
}


BEGIN_MESSAGE_MAP(CProductDelDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnBnClickedButtonQuery)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnCbnSelchangeComboType)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CProductDelDlg ��Ϣ�������

BOOL CProductDelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString sql;
	int indexno=m_type.AddString("���");
	m_type.SetCurSel(indexno);
	m_type.AddString("��Ʒ");
	GetDlgItem(IDC_CHECK_ALL)->EnableWindow(FALSE);

	m_product.SetExtendedStyle(m_product.GetExtendedStyle()|LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_CHECKBOXES);
	m_product.InsertColumn(0,"���",LVCFMT_LEFT,40,-1);
	m_product.InsertColumn(1,"����",LVCFMT_LEFT,60,-1);
	m_product.InsertColumn(2,"ͼ��",LVCFMT_LEFT,120,-1);
	m_product.InsertColumn(3,"ģ����",LVCFMT_LEFT,60,-1);
	m_product.InsertColumn(4,"�����",LVCFMT_LEFT,60,-1);
	m_product.InsertColumn(5,"���״̬",LVCFMT_LEFT,60,-1);
	m_product.InsertColumn(6,"���״̬",LVCFMT_LEFT,60,-1);
	return TRUE;  
}
void CProductDelDlg::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_product.DeleteAllItems();
	CString str,sql;
	int nIndex = m_type.GetCurSel();
	m_type.GetLBText(nIndex,str);
	if (str=="���")
	{
		sql.Format("SELECT * FROM NPartInsInfoTable WHERE InsCode LIKE '%%%s%%'",m_inscode);	
	}
	else
		if(str=="��Ʒ")
		{
			sql.Format("SELECT * FROM NProductInsInfoTable WHERE InsCode LIKE '%%%s%%' ",m_inscode);
		}
		else
			return;
	if (!QueryDatabase(sql,str))
	{
		return;
	}
}
/************************************************************************/
/* �������ݿ�                                                           */
/************************************************************************/
BOOL CProductDelDlg::QueryDatabase(CString sql,CString Type)
{
	CString str;
	CStringArray MdlNumArray;
	int Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		return TRUE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			str.Format("%d",i+1);
			m_product.InsertItem(i,str);
			str=m_db.getString("Name");
			m_product.SetItemText(i,1,str);
			str=m_db.getString("InsCode");
			m_product.SetItemText(i,2,str);
			str.Format("%d",m_db.getInt("MdlNum"));
			if (Type=="���")
			{
				m_product.SetItemData(i,m_db.getInt("PrtNum"));
			}
			if (Type=="��Ʒ")
			{
				 m_product.SetItemData(i,m_db.getInt("PdtNum"));
			}
			MdlNumArray.Add(str);
			str=m_db.getString("Designer");
			m_product.SetItemText(i,4,str);
			str=m_db.getString("FinishState");
			m_product.SetItemText(i,5,str);
			str=m_db.getString("AuditState");
			m_product.SetItemText(i,6,str);
			i++;
		} while(m_db.MoveNext());
		for (int j=0;j<MdlNumArray.GetSize();j++)
		{
			if (MdlNumArray[j]=="-1")
			{
				str="��";
				m_product.SetItemText(j,3,str);
			}
			else
			{
				if (Type=="���")
				{
					sql.Format("SELECT * FROM NPartManageTable where Number=%s",MdlNumArray[j]);
				}
				else
					sql.Format("SELECT * FROM NAsmManageTable where Number=%s",MdlNumArray[j]);
				Z=m_db.Query(sql);
				if (Z==-1 || Z==0)
				{
					continue;
				}
				else
				{
					str=m_db.getString("Name");
					m_product.SetItemText(j,3,str);
				}
			}
		}
	}
	return TRUE;
}
void CProductDelDlg::OnBnClickedOk()
{
	BOOL IsHave=FALSE;
	int SolidID;
	CString sql,InsCode,str;
	CStringArray InsCodeArray;
	int nIndex = m_type.GetCurSel();
	m_type.GetLBText(nIndex,str);
	UpdateData(TRUE);
	for (int i=0;i<m_product.GetItemCount();i++)
	{
		if (m_product.GetCheck(i)==TRUE)
		{
			IsHave=TRUE;
			SolidID=(int)m_product.GetItemData(i);
			if (str=="���")
			{
				BOOL BeUsed;
				if(!IsUsedByProduct(SolidID,&BeUsed,"prt"))
				{
					return;
				}
				else
				{
					if (BeUsed)
					{
						AfxMessageBox("����������Ʒ��ʹ�ã�����ɾ��");
						return;
					}
				}
				if(!DeletePrtIns(SolidID))
				{
					AfxMessageBox("ɾ�����ɹ�");
					return;
				}
				else
				{
					AfxMessageBox("ɾ���ɹ�");
					//return;
				}
			}
			if (str=="��Ʒ")
			{
				BOOL BeUsed;
				if(!IsUsedByProduct(SolidID,&BeUsed,"asm"))
				{
					return;
				}
				else
				{
					if (BeUsed)
					{
						AfxMessageBox("�����������Ʒ��ʹ�ã�����ɾ��");
						return;
					}
				}
				if(!DeleteProductIns(SolidID,TRUE))
				{
					AfxMessageBox("ɾ�����ɹ�");
					return;
				}
				else
				{
					AfxMessageBox("ɾ���ɹ�");
					//return;
				}
			}
			break;
		}
	}
	if (IsHave)
	{
		if (str=="���")
		{
			sql.Format("SELECT * FROM NPartInsInfoTable WHERE InsCode LIKE '%%%s%%'",m_inscode);	
		}
		else
			if(str=="��Ʒ")
			{
				sql.Format("SELECT * FROM NProductInsInfoTable WHERE InsCode LIKE '%%%s%%' ",m_inscode);
			}
		m_product.DeleteAllItems();
		QueryDatabase(sql,str);
	}
	if (!IsHave)
	{
		AfxMessageBox("��ѡ����Ҫɾ����ʵ����");
		return;
	}
}
/************************************************************************/
/* ɾ���������Ĳ�Ʒʵ��                                                 */
/************************************************************************/
BOOL CProductDelDlg::DeleteProductIns(int SolidID,BOOL IsTheTop)	//�Ƿ��Ƕ������
{
	CString str,sql,InsCode;
	CFtpFile ftp;
	CStringArray SolidIDArray,TypeArray;
	CCommonFuncClass commonfun;
	CString SolidFilePlace,DrwFilePlace,SolidFileName,DrwFileName,IsLocal,strSolidFilePlace,strDrwFilePlace,strSolidPath,strDrwPath;

	BOOL BeUsed;
	if(!IsUsedByProduct(SolidID,&BeUsed,"asm"))	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
	{
		return FALSE;
	}
	else
	{
		if (BeUsed)
		{
			return TRUE;
		}
	}
	TCHAR TempPath[MAX_PATH];
	GetTempPath(MAX_PATH,TempPath);

	sql.Format("SELECT * FROM NProductInsInfoTable WHERE PdtNum=%d",SolidID);
	int Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		return TRUE;
	}
	else
	{
		m_db.MoveEnd();
		SolidFilePlace=m_db.getString("SolidPlace");
		DrwFilePlace=m_db.getString("DrawingPlace");
		SolidFileName=m_db.getString("SolidFileName");
		DrwFileName=m_db.getString("DrawingName");
		IsLocal=m_db.getString("LocalState");
		InsCode=m_db.getString("InsCode");
		if (IsLocal=="����")
		{
		    strSolidFilePlace=SolidFilePlace+"\\"+SolidFileName;
			strDrwFilePlace=DrwFilePlace+"\\"+DrwFileName;
		}
		if(IsLocal=="������")
		{
			strSolidFilePlace=CString(TempPath)+InsCode+"Solid";
			ftp.RemoveDireAndFile(strSolidFilePlace);
			strDrwFilePlace=CString(TempPath)+InsCode+"Drw";
			ftp.RemoveDireAndFile(strDrwFilePlace);
			//strSolidPath=strSolidFilePlace+"/"+SolidFileName+".1";
			//strDrwPath=DrwFilePlace+"/"+DrwFileName+".1";
		}
	}
	if (m_all && IsTheTop)
	{
		sql.Format("SELECT * FROM NProductBOMTable WHERE PdtNum=%d",SolidID);
		Z=m_db.Query(sql);
		if (Z==0 || Z==-1)
		{
			return FALSE;
		}
		else
		{
			m_db.MoveBegin();
			do {
				str.Format("%d",m_db.getInt("PrtNum"));
				SolidIDArray.Add(str);
				str=m_db.getString("Type");
				TypeArray.Add(str);
			} while(m_db.MoveNext());
		}
	}
	sql.Format("DELETE FROM NProductInsInfoTable WHERE PdtNum=%d",SolidID);
	//�ж��Ƿ񱻱�Ĳ�Ʒ��,��Ҫ�õ���������㲿�����
	try
	{
		m_db.m_pConnection->Close();
		m_db.ReOpen();
		m_db.m_pConnection->BeginTrans();
		if(m_db.Execute(sql)==0)
		{
			m_db.m_pConnection->RollbackTrans();
			return FALSE;
		}
		if (IsLocal=="����")
		{
			commonfun.IsFileExist(SolidFilePlace,DrwFilePlace,SolidFileName,DrwFileName);
		}
		if (IsLocal=="������")
		{
			ftp.FindFile(SolidFileName,SolidFilePlace,strSolidFilePlace,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			ftp.FindFile(DrwFileName,DrwFilePlace,strDrwFilePlace,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
		}
	}
	catch(_com_error e)
	{
		m_db.m_pConnection->RollbackTrans();
		return FALSE;
	}
	m_db.m_pConnection->CommitTrans();
	for (int i=0;i<SolidIDArray.GetSize();i++)
	{
	    if (TypeArray[i]=="prt")
	    {
			DeletePrtIns(atol(SolidIDArray[i]));
	    }
		if (TypeArray[i]=="asm")
		{
			DeleteProductIns(atol(SolidIDArray[i]),FALSE);
		}
	}
	return TRUE;
}
/************************************************************************/
/* ɾ���������Ĳ�Ʒʵ��                                                 */
/************************************************************************/
BOOL CProductDelDlg::DeletePrtIns(int SolidID)
{
	CString str,sql,InsCode;
	CFtpFile ftp;
	CStringArray SolidIDArray,TypeArray;
	CCommonFuncClass commonfun;
	CString SolidFilePlace,DrwFilePlace,SolidFileName,DrwFileName,IsLocal,strSolidFilePlace,strDrwFilePlace,strSolidPath,strDrwPath;

	BOOL BeUsed;
	if(!IsUsedByProduct(SolidID,&BeUsed,"prt"))
	{
		return FALSE;
	}
	else
	{
		if (BeUsed)
		{
			return TRUE;
		}
	}
	TCHAR TempPath[MAX_PATH];
	GetTempPath(MAX_PATH,TempPath);
    sql.Format("SELECT * FROM NPartInsInfoTable WHERE PrtNum=%d",SolidID);
	int Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		return TRUE;
	}
	else
	{
		m_db.MoveEnd();
		SolidFilePlace=m_db.getString("SolidPlace");
		DrwFilePlace=m_db.getString("DrawingPlace");
		SolidFileName=m_db.getString("SolidFileName");
		DrwFileName=m_db.getString("DrawingName");
		IsLocal=m_db.getString("LocalState");
		InsCode=m_db.getString("InsCode");
		if (IsLocal=="����")
		{
			strSolidFilePlace=SolidFilePlace+"\\"+SolidFileName;
			strDrwFilePlace=DrwFilePlace+"\\"+DrwFileName;
		}
		if(IsLocal=="������")
		{
			strSolidFilePlace=CString(TempPath)+InsCode+"Solid";
			ftp.RemoveDireAndFile(strSolidFilePlace);
			strDrwFilePlace=CString(TempPath)+InsCode+"Drw";
			ftp.RemoveDireAndFile(strDrwFilePlace);
			//strSolidPath=strSolidFilePlace+"/"+SolidFileName+".1";
			//strDrwPath=DrwFilePlace+"/"+DrwFileName+".1";
		}
	}
	sql.Format("DELETE FROM NPartInsInfoTable WHERE PrtNum=%d",SolidID);
	try
	{
		m_db.m_pConnection->Close();
		m_db.ReOpen();
		m_db.m_pConnection->BeginTrans();
		if(m_db.Execute(sql)==0)
		{
			m_db.m_pConnection->RollbackTrans();
			return FALSE;
		}
		if (IsLocal=="����")
		{
			commonfun.IsFileExist(SolidFilePlace,DrwFilePlace,SolidFileName,DrwFileName);
		}
		if (IsLocal=="������")
		{
			ftp.FindFile(SolidFileName,SolidFilePlace,strSolidFilePlace,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			ftp.FindFile(DrwFileName,DrwFilePlace,strDrwFilePlace,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
		}
	}
	catch(_com_error e)
	{
		m_db.m_pConnection->RollbackTrans();
		return FALSE;
	}
	m_db.m_pConnection->CommitTrans();
	return TRUE;
}

void CProductDelDlg::OnCbnSelchangeComboType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_product.DeleteAllItems();
	CString str;
	int nIndex = m_type.GetCurSel();
	m_type.GetLBText(nIndex,str);
	if (str=="���")
	{
		m_all=0;
		GetDlgItem(IDC_CHECK_ALL)->EnableWindow(FALSE);
	}
	if (str=="��Ʒ")
	{
	    GetDlgItem(IDC_CHECK_ALL)->EnableWindow(TRUE);
	}
	UpdateData(TRUE);
}
/************************************************************************/
/* �ж�����Ƿ񱻲�Ʒ����                                               */
/************************************************************************/
BOOL CProductDelDlg::IsUsedByProduct(int SolidID,BOOL *BeUsed,CString Type)
{
	CString sql,str;
	CCommonFuncClass commonfun;
	sql.Format("SELECT * FROM NProductBOMTable WHERE PrtNum=%d AND Type='%s'",SolidID,Type);
	int Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		*BeUsed=FALSE;
		return TRUE;
	}
	else
		*BeUsed=TRUE;
	return TRUE;
}

void CProductDelDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: �ڴ������Ϣ����������
}
