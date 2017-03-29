// ProductMenberBasicInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "ProductMenberBasicInfoDlg.h"
#include "afxtempl.h"
#include "CommonFuncClass.h"
#include "DetailDlg.h"
#include "Compute.h"
#include "MyDatabase.h"
#include "ProWindows.h"
#include "StyleStandDlg.h"
#include "ProductClass.h"
#include "PrtClass.h"
#include "MaterialDlg.h"
#include "UnitDlg.h"
#include "UserNameDlg.h"


void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
extern CArray <Database,Database> Database_struc_temp;
extern CMyDatabase m_db;
extern FtpInfo Ftp_struc;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;
// CProductMenberBasicInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CProductMenberBasicInfoDlg, CDialog)
CProductMenberBasicInfoDlg::CProductMenberBasicInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProductMenberBasicInfoDlg::IDD, pParent)
	, m_inscode(_T(""))
	, m_insname(_T(""))
	, m_mdlnum(_T(""))
	, m_designer(_T(""))
	, m_note(_T(""))
	, m_group(_T(""))
	, m_material(_T(""))
	, m_unit(_T(""))
	, m_filename(_T(""))
	,s(-1)
	, m_place(_T(""))
	, m_drwplace(_T(""))
{
	IsSelect=FALSE;
	Num="";
}

CProductMenberBasicInfoDlg::~CProductMenberBasicInfoDlg()
{
}

void CProductMenberBasicInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_LIST_PERFORMANCE, m_performance);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_product);
	DDX_Text(pDX, IDC_EDIT_CODE, m_inscode);
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_insname);
	DDX_Text(pDX, IDC_EDIT_MDLFRM, m_mdlnum);
	DDX_Text(pDX, IDC_DESIGNER, m_designer);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
	DDX_Text(pDX, IDC_EDIT_GRPFRM, m_group);
	DDX_Text(pDX, IDC_EDIT_MATERIAL, m_material);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_unit);
	DDX_Text(pDX, IDC_EDIT_SOLID, m_filename);
	DDX_Control(pDX, IDC_COMBO_STATE, m_designstate);
	DDX_Text(pDX, IDC_EDIT_SOLIDPLACE, m_place);
	DDX_Text(pDX, IDC_EDIT_DRWPLACE, m_drwplace);
}


BEGIN_MESSAGE_MAP(CProductMenberBasicInfoDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_SEL, OnBnClickedButtonSel)
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM, OnBnClickedButtonGrpfrm)
	ON_BN_CLICKED(IDC_BUTTON_MATERIAL, OnBnClickedButtonMaterial)
	ON_BN_CLICKED(IDC_BUTTON_UNIT, OnBnClickedButtonUnit)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDLOCAL, OnBnClickedButtonSolidlocal)
	ON_BN_CLICKED(IDC_BUTTON_DRWLOCAL, OnBnClickedButtonDrwlocal)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDSERVER, OnBnClickedButtonSolidserver)
	ON_BN_CLICKED(IDC_BUTTON_DRWSERVER, OnBnClickedButtonDrwserver)
	ON_BN_CLICKED(IDC_BUTTON_de, OnBnClickedButtonde)
END_MESSAGE_MAP()


// CProductMenberBasicInfoDlg ��Ϣ�������

void CProductMenberBasicInfoDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CProductMenberBasicInfoDlg::OnBnClickedOk()
{
	//OnOK();
}

BOOL CProductMenberBasicInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CCompute compute;
	CString str,ProductNum,sql;
	int index=m_designstate.AddString("���");
	m_designstate.SetCurSel(index);
	m_designstate.AddString("δ���");
	ProductSolid=commonfun.ObtainTheTopSolid(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
	if (ProductSolid==NULL)
	{
		AfxMessageBox("�޷��õ���Ʒ�ľ��");
		return FALSE;
	}
	//�����Ѿ�������㲿�������ܸ��������Ը�������Ϣ
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==3)
	{
		;//���㲿���ļ򵥸��£����ܸ�����
	}
	else
	{
		if (ProductSolid !=((DetailInfoClass *)p_classarray.GetAt(s))->solid)	  //˵�����ǲ�Ʒ
		{
			//�ж��Ƿ����������
			//if (!IsNew)		//����Ǵ��½���Ʒ�Ի�������ģ���Ϊ�Ѿ��жϹ��ˣ�������Ҫ���ж��Ƿ�����������������
			//{
			//	if (!pdtclass.AffirmIsOld(Type,s))						 //˵�����������
			//	{
			//		return FALSE;
			//	}
			//}
			//else
			//{
			//	str.Format("�ļ�%s�Ѿ����ı�,�������������Ϣ",((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName);
			//	AfxMessageBox(str);
			//}
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1)
			{
				m_designer=((DetailInfoClass *)p_classarray.GetAt(s))->Designer;
				m_insname=((DetailInfoClass *)p_classarray.GetAt(s))->InsName;
				m_inscode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
				m_drwplace=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
				m_place=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
				m_filename=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
			}
		}
		else		 //�ǲ�Ʒ����
		{
			if (IsInsert)				//˵���Ǵ����Ϊ�Ի�������
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;
			}
				else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;   //˵���Ǵӱ���Ի�������	
			}
		}
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==3)
	{
		m_designer=((DetailInfoClass *)p_classarray.GetAt(s))->Designer;
		m_insname=((DetailInfoClass *)p_classarray.GetAt(s))->InsName;
		m_inscode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
		m_drwplace=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
		m_place=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
		m_filename=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
		GetDlgItem(IDC_EDIT_SOLID)->EnableWindow(FALSE);
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1)
	{
		GetDlgItem(IDC_EDIT_CODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INSNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DRWPLACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SOLID)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SOLIDPLACE)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_DRWLOCAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DRWSERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SOLIDLOCAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SOLIDSERVER)->EnableWindow(FALSE);
	}
	m_mdlnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum;
	m_group=((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm;
	m_material=((DetailInfoClass *)p_classarray.GetAt(s))->Material;
	m_unit=((DetailInfoClass *)p_classarray.GetAt(s))->Unit;
	m_note=((DetailInfoClass *)p_classarray.GetAt(s))->Note;

    //��ʾ���ܲ�����
	if (!pdtclass.ShowPerf(s,Type,&m_product))
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1)
	{
		AfxMessageBox("������/����Ѿ����ڣ���Ҫ���޸ģ���ֱ�ӱ���");
	}
	UpdateData(FALSE);
	return TRUE;
}

void CProductMenberBasicInfoDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: �ڴ������Ϣ����������
}
void CProductMenberBasicInfoDlg::OnBnClickedButtonSel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CString  Path,FileName,DrwName,str;
	BOOL IsLocal;
	UpdateData(TRUE);
	if (m_filename=="" || m_inscode=="" || m_insname=="" || m_place=="" || m_drwplace=="")
	{
		AfxMessageBox("�����Ϣ����ȫ������������");
		return;
	}
	int k=m_place.Find(":");
	if (k==-1)		   //˵���Ƿ������ϵ�
	{
		if (k=m_drwplace.Find(":"))
		{
			if (k!=-1)
			{
				AfxMessageBox("��άģ�ͺ���άģ�ͱ����λ�ò�һ�£�");
				return;
			}
			IsLocal=FALSE;
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="������";
		}
	}
	else			 //�ڱ���
	{
		if (k=m_drwplace.Find(":"))
		{
			if (k==-1)
			{
				AfxMessageBox("��άģ�ͺ���άģ�ͱ����λ�ò�һ�£�");		//����ʹ����һ���ڷ������ϣ�һ���ڱ���
				return;
			}
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="����";
			IsLocal=TRUE;
		}
	}
	k=m_filename.Find(".");
	if (k==-1)			 //˵��û�к�׺
	{
		if (Type=="prt")
		{
		   FileName=m_filename+".prt";
		}
		else
		{
			FileName=m_filename+".asm";
		}
		DrwName=m_filename+".drw";
	}
	else
	{
		if (Type=="prt")
		{
			FileName=m_filename.Left(k)+".prt";
		}
		else
		{
			FileName=m_filename.Left(k)+".asm";
		}
		DrwName=m_filename.Left(k)+".drw";
	}
	//���ݲ�ͬ�ı��淽ʽ�ж�ͼ�ţ��ļ����ĺϷ���
	if(!commonfun.IsInfoOk(s,m_inscode,FileName,DrwName))
	{
		return;
	}
	//�޸��ļ���  
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1)
	{
		
		if (!pdtclass.NewRename(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,&(((DetailInfoClass *)p_classarray.GetAt(s))->DrwName)))
		{
			AfxMessageBox("������ʧ�ܣ�");
			return;
		}
	}
	else
	{
		if (!pdtclass.NewRename(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FileName,&DrwName))
		{
			AfxMessageBox("������ʧ�ܣ�");
			return;
		}
	}
	//��ԭ����Ŀ¼��ɾɵ�Ŀ¼,������ʧ�ܵ�ʱ����
	((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFileName=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
	((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwName=((DetailInfoClass *)p_classarray.GetAt(s))->DrwName;
	//��������
	((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum=m_mdlnum;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=m_inscode;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsName=m_insname;
	((DetailInfoClass *)p_classarray.GetAt(s))->Designer=m_designer;
	((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm=m_group;
	((DetailInfoClass *)p_classarray.GetAt(s))->Material=m_material;
	((DetailInfoClass *)p_classarray.GetAt(s))->Unit=m_unit;
	((DetailInfoClass *)p_classarray.GetAt(s))->Note=m_note;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName=FileName;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwName=DrwName;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=m_place;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=m_drwplace;
	//�����ܲ���
	(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).RemoveAll();
	//�������ܲ�����
	for(int i=0;i<m_product.GetItemCount();i++)
	{
		str=m_product.GetItemText(i,2);
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

	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==3)
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;               //Ϊ�˳����ͨ���ԣ��ȸ�Ϊ1
		//if (!pdtclass.UpdateProductMenberToDatabase(s,Type))
		//{
		//	AfxMessageBox("�޸�ʧ��");
		//	((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=3;
		//}//������Ϣ
		//else
		//{
		//	AfxMessageBox("�޸ĳɹ���");
		//   ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=3;
		//}
	}
	else
	{
		//����ǲ�Ʒ
		if((((DetailInfoClass *)p_classarray.GetAt(s))->solid==ProductSolid))
		{
			//�����Ʒ,�������ݿ�
			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,!(IsLocal),IsLocal))
			{
				AfxMessageBox("д���ݿ�ʧ�ܣ�");
				return;
			}
			else
			{
				AfxMessageBox("����ɹ�");
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
			}
		}
		else
		{
			//���ݲ�ͬ�ķ�ʽ�������/���
			if (IsLocal)
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="����";
			}
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="������";
			}
			//�ж��Ƿ������Բ�Ʒ�½���
			if (IsNew)
			{
				//���²�������������
				if (!InsertNewProductMenberToDatabase(s,Type))
				{
					return;
				}
			}
			else
			{
				if(!pdtclass.UpdateProductMenber(ProductSolid,s))
				{
					AfxMessageBox("����ʧ��");
					return;
				}
				else
				{
					AfxMessageBox("����ɹ�");
				}
			}
		}
	}

	IsSelect=TRUE;
	OnOK();
}
/************************************************************************/
/* ѡ����ఴť                                                         */
/************************************************************************/
void CProductMenberBasicInfoDlg::OnBnClickedButtonGrpfrm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}


/************************************************************************/
/* ѡ����ϰ�ť                                                       */
/************************************************************************/
void CProductMenberBasicInfoDlg::OnBnClickedButtonMaterial()
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
/************************************************************************/
/* ѡ��λ��ť                                                                     */
/************************************************************************/
void CProductMenberBasicInfoDlg::OnBnClickedButtonUnit()
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

void CProductMenberBasicInfoDlg::OnBnClickedButtonSolidlocal()
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
		if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
		{
			m_filename= fileDlg.GetFileTitle();
		}
	}
	UpdateData(FALSE);
}

void CProductMenberBasicInfoDlg::OnBnClickedButtonDrwlocal()
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
		m_drwplace=pathName.Left(k);
		//m_drwname= fileDlg.GetFileTitle();
	}
	UpdateData(FALSE);
}

void CProductMenberBasicInfoDlg::OnBnClickedButtonSolidserver()
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

void CProductMenberBasicInfoDlg::OnBnClickedButtonDrwserver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CStyleStandDlg severdirdlg;
	if(IDOK==severdirdlg.DoModal())
	{
		m_drwplace=severdirdlg.path;
	}
	UpdateData(FALSE);
}
/************************************************************************/
/* ���½���Ʒʱ���������������߲�������ʱ                             */
/************************************************************************/
BOOL CProductMenberBasicInfoDlg::InsertNewProductMenberToDatabase(int s,CString Type)
{
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CPrtClass prtclass;
	BOOL IsLocal;
	if(Type=="prt")
	{
			//���ݲ�ͬ�ı���λ�ý��в�ͬ�Ĳ���
		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="����")
		{
			IsLocal=TRUE;
			if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
			{
				AfxMessageBox("�޷����棡");
				return FALSE;
			}
			if (!prtclass.InsertPart(s,FALSE))
			{
				AfxMessageBox("����ʧ��");
				return FALSE;
			}
			else
			{
				AfxMessageBox("����ɹ�");
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
			}
		}
		else
		{
			if (!commonfun.SavePartToTheServerforPart(s))
			{
				AfxMessageBox("����ʧ�ܣ�");
				return FALSE;
			}//�ϴ���������
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
				AfxMessageBox("����ɹ�");
			}
		}
	}
	else
	{
		//�����Ʒ,�������ݿ�
		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="����")
		{
			IsLocal=TRUE;
			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE,IsLocal))
			{
				AfxMessageBox("д���ݿ�ʧ�ܣ�");
				return FALSE;
			}
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
				AfxMessageBox("����ɹ�");
			}
		}
		else
		{
			IsLocal=FALSE;
			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,TRUE,IsLocal))
			{
				AfxMessageBox("д���ݿ�ʧ�ܣ�");
				return FALSE;
			}
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
				AfxMessageBox("����ɹ�");
			}
			    
		}
		
	}
	return TRUE;
}
void CProductMenberBasicInfoDlg::OnBnClickedButtonde()
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
