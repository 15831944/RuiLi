// ProductSaveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "afxtempl.h"
#include "variantdesign.h"
#include "ProductSaveDlg.h"
#include "ProMdl.h"
#include "ProSolid.h"
#include "ProUtil.h"
#include "ProWindows.h"
#include "ProductMenberBasicInfoDlg.h"
#include "CommonFuncClass.h"
#include "Compute.h"
#include "MyDatabase.h"
#include "FtpFile.h"
#include "UpdateDlg.h"
#include "DetailInfoClass.h"
#include "ProductClass.h"
#include "StyleStandDlg.h"
#include "SelectDlg.h"
#include "UserNameDlg.h"

// CProductSaveDlg �Ի���
void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
ProError SolidFeatGet(ProSolid solid,ProFeature **p_data,BOOL nFlag);

extern CArray <Database,Database> Database_struc_temp;
extern CMyDatabase m_db;
extern CArray <JuBing,JuBing> JuBing_struc;
extern CArray <JuBing,JuBing> JuBing_struc_temp;
extern FtpInfo Ftp_struc;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;
extern CStringArray NumArray;

IMPLEMENT_DYNAMIC(CProductSaveDlg, CDialog)
CProductSaveDlg::CProductSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProductSaveDlg::IDD, pParent)
	, m_inscode(_T(""))
	, m_insname(_T(""))
	, m_designer(_T(""))
	, m_mdlnum(_T(""))
	, m_group(_T(""))
	, m_note(_T(""))
	, m_place(_T(""))
	, m_drwingplace(_T(""))
	, m_filename(_T(""))
	, m_pdmcode(_T(""))
{
}

CProductSaveDlg::~CProductSaveDlg()
{
}

void CProductSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_ASMMODEL, m_asmmodeltree);
	DDX_Text(pDX, IDC_EDIT_CODE, m_inscode);
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_insname);
	DDX_Text(pDX, IDC_DESIGNER, m_designer);
	DDX_Text(pDX, IDC_EDIT_MDLFRM, m_mdlnum);
	DDX_Text(pDX, IDC_EDIT_GRPFRM, m_group);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
	DDX_Text(pDX, IDC_EDIT_PLACE, m_place);
	DDX_Text(pDX, IDC_EDIT_DRWPLACE, m_drwingplace);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_filename);
	DDX_Control(pDX, IDC_COMBO_FINISH, m_designstate);
	DDX_Control(pDX, IDC_LIST_PER, m_perf);
	DDX_Text(pDX, IDC_EDIT_CODE2, m_pdmcode);
}


BEGIN_MESSAGE_MAP(CProductSaveDlg, CDialog)
	ON_WM_MOVE()
	ON_NOTIFY(NM_CLICK, IDC_TREE_ASMMODEL, OnNMClickTreeAsmmodel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM, OnBnClickedButtonGrpfrm)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDLOCALPLACE, OnBnClickedButtonSolidlocalplace)
	ON_BN_CLICKED(IDC_BUTTON_DRWLOCALPLACE, OnBnClickedButtonDrwlocalplace)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDSERVERPLACE, OnBnClickedButtonSolidserverplace)
	ON_BN_CLICKED(IDC_BUTTON_DWGSERVERPLACE, OnBnClickedButtonDwgserverplace)
	ON_BN_CLICKED(IDC_BUTTON_de, OnBnClickedButtonde)
END_MESSAGE_MAP()


// CProductSaveDlg ��Ϣ�������

BOOL CProductSaveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CCommonFuncClass commonfun;
	ProError status;
	ProSolid solid;
	CProductClass pdtclass;
	//ȥ�������ڴ�Ļ�����Ϣ
	if(p_classarray.GetSize()>1)
	{
		//ȥ��������û�еĻ�����Ϣ
		commonfun.IsBasicInfoAlreadyHave();
		commonfun.RemoveNotInSession();
	}

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	m_ImageList.Create(16, 16, ILC_COLOR8, 0, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_asmmodeltree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	m_db.Close();
	if(m_db.ReOpen()==0)
	this->SendMessage(WM_CLOSE,0,0);

	status=ProMdlCurrentGet((ProMdl*)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�Բ�����û�д�װ��ģ�ͣ�");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	ProductSolid=commonfun.ObtainTheTopSolid(solid);
	if (ProductSolid==NULL)
	{
		AfxMessageBox("�������ͬʱ��������ͬ�Ĳ�Ʒʹ�ã���ر�һ����������ڴ�");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	commonfun.AddProductToTree(solid,&m_asmmodeltree);
	if(!commonfun.ObtianBasicInfoFromHandle(solid))				   //�жϵ�ǰ������Ļ�����Ϣ�Ƿ񶼴���
	{
		AfxMessageBox("�޷��õ��������Ʒ����ϸ��Ϣ���޷����棬������²�Ʒ���������");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	//����Ҫ������д������Ϣ
	int s;
	CString sql,str;
	//ȥ�����ڵ�ǰ�����еĽṹ��
	int index=m_designstate.AddString("���");
	m_designstate.SetCurSel(index);
	m_designstate.AddString("δ���");
	s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	else
	{
		if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidID==-1)
		{
			AfxMessageBox("��������û�б��������ʹ�����Ϊ��ť��");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
		m_mdlnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlName;
		m_inscode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
		m_pdmcode=((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode;
		m_insname=((DetailInfoClass *)p_classarray.GetAt(s))->InsName;
		m_designer=((DetailInfoClass *)p_classarray.GetAt(s))->Designer;
		m_group=((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm;
		m_note=((DetailInfoClass *)p_classarray.GetAt(s))->Note;
		m_filename=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
		m_drwingplace=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
		m_place=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
		//��ʾ���ܲ���
		if(!pdtclass.ShowPerf(s,"asm",&m_perf))
		{
			AfxMessageBox("�޷��õ����ܲ���");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
	}
	if (commonfun.ObtainPartMap(solid)!="")                  //���������û��ͼ�ţ�����ʾ�ڴ��е�ͼ�ţ�������ļ��Ĳ����л��
	{
		m_inscode=commonfun.ObtainPartMap(solid);
	}
	else
	{
		m_inscode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
	}
	if (ProductSolid!=NULL)
	{
		if (ProductSolid==solid)
		{
			IsOneOfProduct=FALSE;//�ǵ������
		}
		else
			IsOneOfProduct=TRUE;
	}
	else
		AfxMessageBox("��������㲿���ص��Ķ����Ʒ����ص�һ��");
	//�ǲ��������жϲ�Ʒ���Ƿ��Ѿ�����
	if (IsOneOfProduct)
	{
		int Pos=commonfun.IsBasicInfoHave(ProductSolid);
		if (Pos==-1)
		{
			AfxMessageBox("�Ҳ�����������Ĳ�Ʒ����Ϣ���޷�����");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
		else
			if (((DetailInfoClass *)p_classarray.GetAt(Pos))->SolidID==-1)
			{
				AfxMessageBox("��û�б����Ʒ�����ȱ����Ʒ");
				this->SendMessage(WM_CLOSE,0,0);
				return FALSE;
			}
	}
	//�ж��������Ƿ񱻶����Ʒ����
	if (!pdtclass.IsUsedByProduct(s,&BeUsed,"asm"))
	{
		AfxMessageBox("�޷��õ������Ϣ");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	UpdateData(FALSE);
	return TRUE;  
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CProductSaveDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRepaint(PRO_VALUE_UNUSED);
}

void CProductSaveDlg::OnNMClickTreeAsmmodel(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CProductSaveDlg::OnBnClickedButtonSave()
{
	//�жϱ���·���Ƿ�Ϊ��
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CString  Path,FileName,DrwName,str,sql;
	ProSolid solid;
	ProError status;
	BOOL IsLocal=FALSE;
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	status=ProMdlCurrentGet((ProMdl *)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ǰ��Ʒ����ȷ��");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	if (m_place=="" || m_drwingplace=="" || m_filename=="" || m_inscode=="" || m_pdmcode=="")
	{
		AfxMessageBox("�����Ϣ����ȫ������������");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	int k=m_place.Find(":");
	if (k==-1)		   //˵���Ƿ������ϵ�
	{
		if (k=m_drwingplace.Find(":"))
		{
			if (k!=-1)
			{
				AfxMessageBox("��άģ�ͺ���άģ�ͱ����λ�ò�һ�£�");
				GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
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
				GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
				return;
			}
			IsLocal=TRUE;
		}
	}
	if (!commonfun.ObtianBasicInfoFromHandle(solid))
	{
		return;
	}
	int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("�Ҳ�������������Ϣ���޷�����");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	//�ж�ͼ���Ƿ���ȷ
	m_filename.MakeLower();
	if (m_filename.Right(4)==".asm")
	{
		k=m_filename.ReverseFind('.');
		if (k!=-1)
		{
			FileName=m_filename.Left(k)+".asm";
			DrwName=m_filename.Left(k)+".drw";
		}
	}
	else// (k==-1)			 //˵��û�к�׺
	{
		FileName=m_filename+".asm";
		DrwName=m_filename+".drw";
	}
	//�ж��Ƿ�Ӧ���ò���
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0 && (!((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved) && ((DetailInfoClass *)p_classarray.GetAt(s))->solid!=ProductSolid)
	{
		AfxMessageBox("���������û�У�Ӧ�����Ϊ�˵���");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
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
			AfxMessageBox("����������Ʒ���ã������������");
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
			return;
		}
	} 
	//�ж���������Ƿ��в����
	if (ProductSolid!=solid)
	{
		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert=="F")
		{
            AfxMessageBox("���������û���²�������޷����棬���ȵ����Ϊ�˵���");
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
			return;
		}
	}
	//�жϵ�ǰ�ĵ��������Ƿ����
	sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%d",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
	if (commonfun.IsHave(sql))
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;          //����һ�����
		//�ж��Ƿ��Ѿ���˹�
		BOOL IsAudit;
		if (!pdtclass.IsAlreadyAudit(((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,&IsAudit))
		{
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
			return;
		}
		else
			if (IsAudit)
			{
				AfxMessageBox("��������Ѿ�ͨ�����,�������޸�");
				GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
				return;
			}
	}
	else
	{
		AfxMessageBox("�������������������Ϊ��ť");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	//}
	//���ݲ�ͬ�ı��淽ʽ�ж�ͼ�ţ��ļ����ĺϷ���
	if(!commonfun.IsInfoOk(s,m_inscode,FileName,DrwName))
	{
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	if (!commonfun.TheWorkBeforeSave(ProductSolid))
	{
		AfxMessageBox("����ǰ׼������ʧ��");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	//�޸��ļ���
	if (!pdtclass.NewRename(solid,FileName,&DrwName))
	{
		AfxMessageBox("������ʧ�ܣ�");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
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
	((DetailInfoClass *)p_classarray.GetAt(s))->Note=m_note;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName=FileName;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=m_drwingplace;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=m_place;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwName=DrwName;
	//commonfun.SetParam(s);
	if (!commonfun.SetParam(s))   //���ò�����ͼ��
	{
		AfxMessageBox("����ͼ�Ŵ���");
		return;
	}
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
	//�ж���Щ�Ѿ�����
	if(!pdtclass.FindProductMenberForProductWhichIsModify(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE))
	{
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	//���뻹û�б�����㲿��
	if(!pdtclass.InsertProductMenberForProduct(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
	{
		AfxMessageBox("���������ʧ��");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	if(IsOneOfProduct)
	{
		if (!pdtclass.SaveBothPartAndAsm(s,"asm",FALSE))
		{
			commonfun.TheWorkAferSave(ProductSolid,FALSE);
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
			AfxMessageBox("����ʧ�ܣ�");
			return;
		}
		else
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
			commonfun.TheWorkAferSave(ProductSolid,TRUE);
			AfxMessageBox("����ɹ�");
		}
	}
	//��ʼ����
	else
	{
		if (IsLocal)			   //�ڱ��أ������ڱ���
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="����";
			//����������ж����ϴ����ݿ⻹�Ǹ������ݿ�
			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE,TRUE))
			{
				commonfun.TheWorkAferSave(ProductSolid,FALSE);
				GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
				AfxMessageBox("����ʧ�ܣ�");
				return;
			}
		}
		else
		{   
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="������";
			//����֮ǰҪ���޸Ĺ�������Ȳ���
			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,TRUE,IsLocal))
			{
				commonfun.TheWorkAferSave(ProductSolid,FALSE);
				GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
				AfxMessageBox("����ʧ�ܣ�");
				return;
			}
		}
		((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
		commonfun.TheWorkAferSave(ProductSolid,TRUE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		AfxMessageBox("����ɹ�");
	}
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
	OnOK();
}
/************************************************************************/
/* �޸�ͼ�Ű�ť��ͬʱ���ͼ�ŵ�Ψһ��                                   */
/************************************************************************/
void CProductSaveDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
void CProductSaveDlg::OnBnClickedCancel()
{
	OnCancel();
}
void CProductSaveDlg::OnClose()
{
	CDialog::OnClose();
}
/************************************************************************/
/* ѡ���Ʒ���ఴť                                                     */
/************************************************************************/
void CProductSaveDlg::OnBnClickedButtonGrpfrm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
	CSelectDlg selectdlg;
	selectdlg.Type="asm";
	int s=-1;
	for(s=0;s<p_classarray.GetSize();s++)
	{
		if (((DetailInfoClass *)p_classarray.GetAt(s))->solid==ProductSolid)
		{
			break;
		}
	}
	if (s==-1)
	{
		AfxMessageBox("�޷��õ������Ʒ����Ϣ");
		return;
	}
	if (IDOK==selectdlg.DoModal())
	{
		m_group=selectdlg.Name;
		((DetailInfoClass *)p_classarray.GetAt(s))->GroupNum=selectdlg.num;
		UpdateData(FALSE);
	}
}
/************************************************************************/
/* ѡ����ά���ص�Ŀ¼                                                   */
/************************************************************************/
void CProductSaveDlg::OnBnClickedButtonSolidlocalplace()
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
/************************************************************************/
/* ѡ�񹤳�ͼ���ص�Ŀ¼                                                 */
/************************************************************************/
void CProductSaveDlg::OnBnClickedButtonDrwlocalplace()
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
		//	m_drwname= fileDlg.GetFileTitle();
	}
	UpdateData(FALSE);
}
/************************************************************************/
/* ѡ����ά��������Ŀ¼                                                     */
/************************************************************************/
void CProductSaveDlg::OnBnClickedButtonSolidserverplace()
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
/************************************************************************/
/* ѡ�񹤳�ͼ�ķ�������Ŀ¼                                             */
/************************************************************************/
void CProductSaveDlg::OnBnClickedButtonDwgserverplace()
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
void CProductSaveDlg::OnBnClickedButtonde()
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
