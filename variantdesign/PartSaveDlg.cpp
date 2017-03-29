// PartSaveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PartSaveDlg.h"
#include "ProWindows.h"
#include "Compute.h"
#include "ProSolid.h"
#include "ProUtil.h"
#include "afxtempl.h"
#include "MyDatabase.h"
#include <stdlib.h>
#include <stdio.h>
#include "FtpFile.h"
#include "ProFeature.h"
#include "ProDrawing.h"
#include "CommonFuncClass.h"
#include "UpdateDlg.h"
#include "DetailInfoClass.h"
#include "SelectDlg.h"
#include "StyleStandDlg.h"
#include "PrtClass.h"
#include "ProductClass.h"
#include "MaterialDlg.h"
#include "UnitDlg.h"
#include "UserNameDlg.h"


void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
ProError SolidFeatGet(ProSolid solid,ProFeature **p_data);
extern CArray <Database,Database> Database_struc_temp;
extern CMyDatabase m_db;
extern CArray <JuBing,JuBing> JuBing_struc;
extern FtpInfo Ftp_struc;
CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;
// CPartSaveDlg �Ի���

IMPLEMENT_DYNAMIC(CPartSaveDlg, CDialog)
CPartSaveDlg::CPartSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartSaveDlg::IDD, pParent)
	, m_inscode(_T(""))
	, m_insname(_T(""))
	, m_mdlnum(_T(""))
	, m_designer(_T(""))
	, m_group(_T(""))
	, m_material(_T(""))
	, m_unit(_T(""))
	, m_note(_T(""))
	, m_place(_T(""))
	, m_drwingplace(_T(""))
	, m_filename(_T(""))
	, m_pdmcode(_T(""))
{
	BeUsed=FALSE;
}

CPartSaveDlg::~CPartSaveDlg()
{
}

void CPartSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CODE, m_inscode);
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_insname);
	DDX_Text(pDX, IDC_EDIT_MDLFRM, m_mdlnum);
	DDX_Text(pDX, IDC_DESIGNER, m_designer);
	DDX_Text(pDX, IDC_EDIT_GRPFRM, m_group);
	DDX_Text(pDX, IDC_EDIT_MATERIAL, m_material);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_unit);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
	DDX_Text(pDX, IDC_EDIT_PLACE, m_place);
	DDX_Text(pDX, IDC_EDIT_DRWPLACE, m_drwingplace);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_filename);
	DDX_Control(pDX, IDC_COMBO_SAVESTYLE, m_designstate);
	DDX_Control(pDX, IDC_LIST_PERFORMNACE, m_perf);
	DDX_Text(pDX, IDC_EDIT_PDMCode, m_pdmcode);
}


BEGIN_MESSAGE_MAP(CPartSaveDlg, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM, OnBnClickedButtonGrpfrm)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDLOCAL, OnBnClickedButtonSolidlocal)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDSERVER, OnBnClickedButtonSolidserver)
	ON_BN_CLICKED(IDC_BUTTON_DRWLOCAL, OnBnClickedButtonDrwlocal)
	ON_BN_CLICKED(IDC_BUTTON_DRWSERVER, OnBnClickedButtonDrwserver)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON9, OnBnClickedButton9)
END_MESSAGE_MAP()


// CPartSaveDlg ��Ϣ�������

BOOL CPartSaveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CCommonFuncClass commonfun;
	commonfun.IsBasicInfoAlreadyHave();
	//ȥ��������û�еĻ�����Ϣ
	commonfun.RemoveNotInSession();
	CProductClass pdtclass;

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	ProError status;
	ProMdlType p_type;
	int s;
	CString sql,str;
	BOOL IsHave=FALSE;
	status=ProMdlCurrentGet((ProMdl *)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ǰģ�Ͳ���ȷ��");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	status=ProMdlTypeGet((ProMdl)solid,&p_type);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ǰģ�Ͳ���ȷ��");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	else
		if (p_type!=PRO_MDL_PART)
		{
			AfxMessageBox("��ǰ�������ģ�ͣ����ܱ��棡");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
		else
		{	
			//ȥ�����ڵ�ǰ�����еĽṹ��
			int index=m_designstate.AddString("���");
			m_designstate.SetCurSel(index);
			m_designstate.AddString("δ���");
			s=commonfun.IsBasicInfoHave(solid);
			if (s==-1)
			{
				AfxMessageBox("�������������������Ϊ��ť");
				this->SendMessage(WM_CLOSE,0,0);
				return FALSE;
			}
			else
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidID==-1)
				{
					AfxMessageBox("������������������Ϊ��ť");
					this->SendMessage(WM_CLOSE,0,0);
					return FALSE;
				}
				m_mdlnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlName;
				m_inscode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
				m_pdmcode=((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode;
				m_insname=((DetailInfoClass *)p_classarray.GetAt(s))->InsName;
				m_designer=((DetailInfoClass *)p_classarray.GetAt(s))->Designer;
				m_group=((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm;
				m_material=((DetailInfoClass *)p_classarray.GetAt(s))->Material;
				m_unit=((DetailInfoClass *)p_classarray.GetAt(s))->Unit;
				m_note=((DetailInfoClass *)p_classarray.GetAt(s))->Note;
				m_filename=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
				m_drwingplace=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
				m_place=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
				//��ʾ���ܲ���
				if(!pdtclass.ShowPerf(s,"prt",&m_perf))
				{
					AfxMessageBox("�޷��õ����ܲ���");
					this->SendMessage(WM_CLOSE,0,0);
					return FALSE;
				}
			}
		}
	//�ж��Ƿ��ǵ������
    ProductSolid=commonfun.ObtainTheTopSolid(solid);
	if(ProductSolid!=NULL)
	{
		if (ProductSolid==solid)
		{
			IsOneOfProduct=FALSE;//�ǵ������
		}
		else
			IsOneOfProduct=TRUE;
	}
	else
		AfxMessageBox("����˶���ص��Ĳ�Ʒ����رն���ģ�");
	if (IsOneOfProduct)
	{
		int Pos=commonfun.IsBasicInfoHave(ProductSolid);
		if (Pos==-1)
		{
			AfxMessageBox("�Ҳ����������Ĳ�Ʒ����Ϣ���޷�����");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
		else
			if(((DetailInfoClass *)p_classarray.GetAt(Pos))->SolidID==-1)
			{
				AfxMessageBox("��û�б����Ʒ�����ȱ����Ʒ");
				this->SendMessage(WM_CLOSE,0,0);
				return FALSE;
			}
	}
	//�ж��������Ƿ񱻶����Ʒ����
	if (!pdtclass.IsUsedByProduct(s,&BeUsed,"prt"))
	{
		AfxMessageBox("�޷��õ������Ϣ");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	UpdateData(FALSE);
	return TRUE;  
}

void CPartSaveDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: �ڴ������Ϣ����������
}
/************************************************************************/
/* �����ļ�                                                             */
/************************************************************************/
void CPartSaveDlg::OnBnClickedOk()
{
	CCommonFuncClass commonfun;
	CPrtClass prtclass;
	CProductClass pdtclass;
	CString  Path,FileName,DrwName,str,sql;
	ProSolid solid;
	ProError status;
	BOOL IsLocal=FALSE;
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	status=ProMdlCurrentGet((ProMdl *)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ǰ�������ȷ��");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("�Ҳ�������������Ϣ���޷�����,�����������������Ϊ��ť");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if (m_place=="" || m_drwingplace=="" || m_filename=="" || m_inscode=="" || m_group=="" )
	{
		AfxMessageBox("�����Ϣ����ȫ������������");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	int k;
	if (m_place.Right(1)=="\\")
	{
		k=m_place.ReverseFind('\\');
		if (k!=-1)
		{
			m_place=m_place.Left(k);
		}
	}
	if (m_drwingplace.Right(1)=="\\")
	{
		k=m_drwingplace.ReverseFind('\\');
		if (k!=-1)
		{
			m_drwingplace=m_drwingplace.Left(k);
		}
	}
	k=m_place.Find(":");
	if (k==-1)		   //˵���Ƿ������ϵ�
	{
		if (k=m_drwingplace.Find(":"))
		{
			if (k!=-1)
			{
				AfxMessageBox("��άģ�ͺ���άģ�ͱ����λ�ò�һ�£�");
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				return;
			}
			IsLocal=FALSE;
		}
	}
	else			 //�ڱ���
	{
		if (k=m_drwingplace.Find(":"))
		{
			if (k==-1)
			{
				AfxMessageBox("��άģ�ͺ���άģ�ͱ����λ�ò�һ�£�");		//����ʹ����һ���ڷ������ϣ�һ���ڱ���
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				return;
			}
			IsLocal=TRUE;
		}
	}
	//�ж�ͼ���Ƿ���ȷ
	k=m_filename.Find(".prt");
	if (k==-1)			 //˵��û�к�׺
	{
		FileName=m_filename+".prt";
		DrwName=m_filename+".drw";
	}
	else
	{
		FileName=m_filename.Left(k)+".prt";
		DrwName=m_filename.Left(k)+".drw";
	}
	//�Ƿ�Ӧ�����²���
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0 && (!((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved))
	{
		AfxMessageBox("���������µģ��������Ϊ��ť");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	//�ж��Ƿ��ǵ������
	if (!IsOneOfProduct)
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;
	}
	//���������õĻ��ж�������û�б���
	if (BeUsed)
	{
		if (FileName.MakeLower()!=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName.MakeLower())
		{
			AfxMessageBox("����������Ĳ�Ʒ���ã������������");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			return;
		}
	}
	//�жϵ�ǰ�ĵ��������Ƿ����
	sql.Format("SELECT * FROM NPartInsInfoTable where PrtNum=%d",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
	if (commonfun.IsHave(sql))
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;          //����һ�����
		//�ж��Ƿ��Ѿ���˹�
		BOOL IsAudit;
		if (!prtclass.IsAlreadyAudit(((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,&IsAudit))
		{
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			return;
		}
		else
			if (IsAudit)
			{
				AfxMessageBox("�������Ѿ�ͨ�����,�������޸�");
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				return;
			}
	}
	else
	{
		AfxMessageBox("�������������������Ϊ��ť");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	//���ݲ�ͬ�ı��淽ʽ�ж�ͼ�ţ��ļ����ĺϷ���
	if(!commonfun.IsInfoOk(s,m_inscode,FileName,DrwName))
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if (!commonfun.TheWorkBeforeSave(ProductSolid))
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	//�޸��ļ���
	if (!pdtclass.NewRename(solid,FileName,&DrwName))
	{
		AfxMessageBox("������ʧ�ܣ�");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	//��ԭ����Ŀ¼��ɾɵ�Ŀ¼,������ʧ�ܵ�ʱ����
	((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFileName=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
	((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwName=((DetailInfoClass *)p_classarray.GetAt(s))->DrwName;
	((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
	((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
	//��������
	((DetailInfoClass *)p_classarray.GetAt(s))->MdlName=m_mdlnum;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=m_inscode;
	((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode=m_pdmcode;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsName=m_insname;
	((DetailInfoClass *)p_classarray.GetAt(s))->Designer=m_designer;
	((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm=m_group;
	((DetailInfoClass *)p_classarray.GetAt(s))->Material=m_material;
	((DetailInfoClass *)p_classarray.GetAt(s))->Unit=m_unit;
	((DetailInfoClass *)p_classarray.GetAt(s))->Note=m_note;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName=FileName;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=m_drwingplace;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=m_place;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwName=DrwName;
	//���ö�άͼ��ͼ�ź�����
	if (!commonfun.SetParam(s)) 
	{
		AfxMessageBox("����ͼ�Ŵ���");
		return;
	}
	//if (!commonfun.SetCodeInfo(((DetailInfoClass *)p_classarray.GetAt(s))->solid))   //���ò�����ͼ��
	//{
	//	AfxMessageBox("����ͼ�Ŵ���");
	//	return;
	//}
	//�����ܲ���
	(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).RemoveAll();
	//�������ܲ�����
	for(int i=0;i<m_perf.GetItemCount();i++)
	{
		str=m_perf.GetItemText(i,2);
		(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).Add(str);
	}
	int index=m_designstate.GetCurSel();
	m_designstate.GetLBText(index,((DetailInfoClass *)p_classarray.GetAt(s))->DesignState);
	if (((DetailInfoClass *)p_classarray.GetAt(s))->DesignState=="δ���")
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime="";
	}
	else
	{
		CTime time=CTime::GetCurrentTime();
		((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime=time.Format("%Y-%m-%d %H:%M:%S");
	}
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="���δͨ��";
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime="";
	((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="δ�ύ";
	if(IsOneOfProduct)
	{
		if (!pdtclass.SaveBothPartAndAsm(s,"prt",FALSE))
		{
			commonfun.TheWorkAferSave(ProductSolid,FALSE);
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			AfxMessageBox("����ʧ�ܣ�");
			return;
		}
		else
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			commonfun.TheWorkAferSave(ProductSolid,TRUE);
			AfxMessageBox("����ɹ�");
		}
	}
	//��ʼ����
	else
	{
		if (IsLocal)			   //�ڱ��أ������ڱ���
		{
			//if(!commonfun.SaveFile(solid,m_place,m_drwingplace,m_filename))
			//{
			//	AfxMessageBox("����ʧ�ܣ�");
			//	GetDlgItem(IDOK)->EnableWindow(TRUE);
			//	return;
			//}
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="����";
			//����������ж����ϴ����ݿ⻹�Ǹ������ݿ�
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)		//�������ݿ�
			{
				if (!prtclass.UpdatePart(s,FALSE))
				{
					commonfun.TheWorkAferSave(ProductSolid,FALSE);
					GetDlgItem(IDOK)->EnableWindow(TRUE);
					AfxMessageBox("����ʧ��");
					return;
				}
				else
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
					GetDlgItem(IDOK)->EnableWindow(TRUE);
					commonfun.TheWorkAferSave(ProductSolid,TRUE);
					AfxMessageBox("����ɹ�");
				}
			}
		}
		else
		{   
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="������";
			if (!commonfun.SavePartToTheServerforPart(s))
			{
				commonfun.TheWorkAferSave(ProductSolid,FALSE);
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				AfxMessageBox("����ʧ�ܣ�");
				return;
			}//�ϴ���������
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				commonfun.TheWorkAferSave(ProductSolid,TRUE);
				AfxMessageBox("����ɹ�!");
			}
		}
	}
	   OnOK();
}


void CPartSaveDlg::OnBnClickedButtonGrpfrm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CSelectDlg selectdlg;
	selectdlg.Type="prt";
	int s=-1;
	for(s=0;s<p_classarray.GetSize();s++)
	{
		if (((DetailInfoClass *)p_classarray.GetAt(s))->solid==solid)
		{
			break;
		}
	}
	if (s==-1)
	{
		AfxMessageBox("�޷��õ�����������Ϣ");
		return;
	}
	if (IDOK==selectdlg.DoModal())
	{
		m_group=selectdlg.Name;
		((DetailInfoClass *)p_classarray.GetAt(s))->GroupNum=selectdlg.num;
		UpdateData(FALSE);
	}
}

void CPartSaveDlg::OnBnClickedButtonSolidlocal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString pathName;
	CString path;
	CString FileName;
	char szFilters[]=
		"PartFiles (*.prt)|*.prt*|Asm Files (*.asm)|*.asm|All Files (*.*)|*.*||";

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg (FALSE, "����Ի���", "*.prt",
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if( fileDlg.DoModal ()==IDOK )
	{
		pathName=fileDlg.GetPathName();
		int k=pathName.ReverseFind('\\');
		m_place=pathName.Left(k);
		m_filename= fileDlg.GetFileTitle();
	}
	UpdateData(FALSE);
}

void CPartSaveDlg::OnBnClickedButtonSolidserver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CStyleStandDlg severdirdlg;
	if(IDOK==severdirdlg.DoModal())
	{
		m_place=severdirdlg.path;
	}
	UpdateData(FALSE);
}

void CPartSaveDlg::OnBnClickedButtonDrwlocal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString pathName;
	CString path;
	CString FileName;
	char szFilters[]=
		"Drwing Files (*.drw)|*.drw.*|All Files (*.*)|*.*||";

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg (FALSE, "����Ի���", "*.drw",
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if( fileDlg.DoModal ()==IDOK )
	{
		pathName=fileDlg.GetPathName();
		int k=pathName.ReverseFind('\\');
		m_drwingplace=pathName.Left(k);
	}
	UpdateData(FALSE);
}

void CPartSaveDlg::OnBnClickedButtonDrwserver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CStyleStandDlg severdirdlg;
	if(IDOK==severdirdlg.DoModal())
	{
		m_drwingplace=severdirdlg.path;
	}
	UpdateData(FALSE);
}
/************************************************************************/
/* �ж�����Ƿ񱻲�Ʒ����                                               */
/************************************************************************/
//BOOL CPartSaveDlg::IsPartUsedByProduct(int s)
//{
//	CString sql;
//	CCommonFuncClass commonfun;
//	ProductSolid=commonfun.ObtainTheTopSolid(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
//	if (ProductSolid==NULL)
//	{
//        return FALSE;
//	}
//	else
//		if (ProductSolid==((DetailInfoClass *)p_classarray.GetAt(s))->solid)		 //˵�� �ǵ������
//		{
//			sql.Format("SELECT * FROM NProductBOMTable WHERE PrtNum=%d",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
//			int Z=m_db.Query(sql);
//			if (Z==-1)
//			{
//				return FALSE;
//			}
//			if (Z==0)
//			{
//				BeUsed=FALSE;
//				return TRUE;
//			}
//			else
//				BeUsed=TRUE;
//		}
//		else
//		{
//			sql.Format("SELECT * FROM NProductBOMTable WHERE PrtNum=%d",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
//			int Z=m_db.Query(sql);
//			if (Z==-1)
//			{
//				return FALSE;
//			}
//			if (Z<=1)
//			{
//				BeUsed=FALSE;
//				return TRUE;
//			}
//			else
//				BeUsed=TRUE;
//		}
//	return TRUE;
//}

void CPartSaveDlg::OnBnClickedButton5()
{
	UpdateData(TRUE);
	CMaterialDlg dlg;
	dlg.m_ID=m_material;
	if(dlg.DoModal()==IDOK)
	{
		m_material=dlg.m_ID;
		UpdateData(FALSE);
	}
}

void CPartSaveDlg::OnBnClickedButton6()
{
	UpdateData(TRUE);
	CUnitDlg dlg;
	dlg.m_ID=m_unit;
	if(dlg.DoModal()==IDOK)
	{
		m_unit=dlg.m_ID;
		UpdateData(FALSE);
	}
}

void CPartSaveDlg::OnBnClickedButton9()
{
	UpdateData(TRUE);
	CUserNameDlg dlg;
	dlg.m_UserName=m_designer;
	if(dlg.DoModal()==IDOK)
	{
		m_designer=dlg.m_UserName;
		UpdateData(FALSE);
	}
}
