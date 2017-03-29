// PdtSaveAsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PdtSaveAsDlg.h"
#include "ProWindows.h"
#include "CommonFuncClass.h"
#include "Compute.h"
#include "DetailInfoClass.h"
#include "afxtempl.h"
#include "MyDatabase.h"
#include "StyleStandDlg.h"
#include "ProductMenberBasicInfoDlg.h"
#include "ProductClass.h"
#include "SelectDlg.h"
#include "UserNameDlg.h"


void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
ProError SolidFeatGet(ProSolid solid,ProFeature **p_data,BOOL nFlag);

extern CArray <Database,Database> Database_struc_temp;
extern CMyDatabase m_db;
extern CArray <JuBing,JuBing> JuBing_struc;
extern CArray <JuBing,JuBing> JuBing_struc_temp;
extern FtpInfo Ftp_struc;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;
// CPdtSaveAsDlg �Ի���

IMPLEMENT_DYNAMIC(CPdtSaveAsDlg, CDialog)
CPdtSaveAsDlg::CPdtSaveAsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPdtSaveAsDlg::IDD, pParent)
	, m_inscode(_T(""))
	, m_insname(_T(""))
	, m_mdlnum(_T(""))
	, m_designer(_T(""))
	, m_note(_T(""))
	, m_place(_T(""))
	, m_drwingplace(_T(""))
	, m_filename(_T(""))
	, m_group(_T(""))
	, m_pdmcode(_T(""))
{
}

CPdtSaveAsDlg::~CPdtSaveAsDlg()
{
}

void CPdtSaveAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_PRODUCT, m_treeproduct);

	DDX_Text(pDX, IDC_EDIT_CODE, m_inscode);
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_insname);
	DDX_Text(pDX, IDC_EDIT_MDLFRM, m_mdlnum);
	DDX_Text(pDX, IDC_DESIGNER, m_designer);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
	DDX_Text(pDX, IDC_EDIT_PLACE, m_place);
	DDX_Text(pDX, IDC_EDIT_DRWPLACE, m_drwingplace);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_filename);
	DDX_Control(pDX, IDC_COMBO_FINISH, m_designstate);
	DDX_Control(pDX, IDC_LIST_PER, m_perf);
	DDX_Text(pDX, IDC_EDIT_GRPFRM, m_group);
	DDX_Text(pDX, IDC_EDIT_CODE2, m_pdmcode);
}


BEGIN_MESSAGE_MAP(CPdtSaveAsDlg, CDialog)
	ON_WM_MOVE()
	//ON_NOTIFY(NM_DBLCLK, IDC_LIST_PART, OnNMDblclkListPart)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDLOCALPLACE, OnBnClickedButtonSolidlocalplace)
	ON_BN_CLICKED(IDC_BUTTON_DRWLOCALPLACE, OnBnClickedButtonDrwlocalplace)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDSERVERPLACE, OnBnClickedButtonSolidserverplace)
	ON_BN_CLICKED(IDC_BUTTON_DWGSERVERPLACE, OnBnClickedButtonDwgserverplace)
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM, OnBnClickedButtonGrpfrm)
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM2, OnBnClickedButtonGrpfrm2)
END_MESSAGE_MAP()


// CPdtSaveAsDlg ��Ϣ�������

void CPdtSaveAsDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: �ڴ������Ϣ����������
}

BOOL CPdtSaveAsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ProError status;
	CProductClass pdtclass;
	CCommonFuncClass commonfun;

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	//ȥ��������û�еĻ�����Ϣ
	///��ʼ��ͼ���б�
	m_ImageList.Create(16, 16, ILC_COLOR8, 0, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
    m_treeproduct.SetImageList(&m_ImageList,TVSIL_NORMAL);

	commonfun.IsBasicInfoAlreadyHave();
	commonfun.RemoveNotInSession();
	//�õ���Ʒ��
	ProSolid solid;
	status=ProMdlCurrentGet((ProMdl*)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�Բ�����û�д�װ��ģ�ͣ�");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	//��Ӳ�Ʒ��
	commonfun.AddProductToTree(solid,&m_treeproduct);
	//����Ҫ������д������Ϣ
	int s;
	CString sql,str;
	int index=m_designstate.AddString("���");
	m_designstate.SetCurSel(index);
	m_designstate.AddString("δ���");
	s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		info = new DetailInfoClass;
		info->SolidID=-1;
		info->solid=solid;
		info->MdlNum="-1";
		info->PDMState="δ�ύ";
		m_inscode=commonfun.ObtainPartMap(solid);
		m_mdlnum="��";
		UpdateData(FALSE);
		p_classarray.Add(info);
		m_perf.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
		m_perf.InsertColumn(0,"���",LVCFMT_LEFT,40,-1);
		m_perf.InsertColumn(1,"���ܲ�����",LVCFMT_LEFT,80,-1);
		m_perf.InsertColumn(2,"���ܲ���",LVCFMT_LEFT,100,-1);
		m_perf.InsertColumn(3,"��ע",LVCFMT_LEFT,100,-1);
		s=commonfun.IsBasicInfoHave(solid);
		if(s==-1)  //�ٴ�ȷ�ϵ�ǰ�������Ϣ
		{
			AfxMessageBox("�޷��õ���ǰ����Ļ�����Ϣ��");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
	}
	else
	{
		m_mdlnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlName;
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
	if(!commonfun.ObtianBasicInfoFromHandle(solid))				   //�жϵ�ǰ������Ļ�����Ϣ�Ƿ񶼴���
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
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
}
/************************************************************************/
/* �����Ʒ��ť                                                         */
/************************************************************************/
void CPdtSaveAsDlg::OnBnClickedOk()
{
	//�жϱ���·���Ƿ�Ϊ��
	CCommonFuncClass commonfun;
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
		AfxMessageBox("��ǰ��Ʒ����ȷ��");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if (m_place=="" || m_drwingplace=="" || m_filename=="" || m_inscode=="")
	{
		AfxMessageBox("�����Ϣ����ȫ������������");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
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
	//�жϵ�ǰ���㲿���Ƿ񶼿����ҵ�
	if (!commonfun.ObtianBasicInfoFromHandle(solid))
	{
		return;
	}
	int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("�Ҳ�������������Ϣ���޷�����");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
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
	//�ж��Ƿ��в����
	if (((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert=="T")
	{
		str="��������Ѿ��������Ƿ���һ�����";
		if(AfxMessageBox(str,MB_YESNO)!=IDYES)
		{
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			return;
		}
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum=="-1")
	{
		//�ж������Ƿ����
		sql.Format("SELECT * FROM NPartInsInfoTable where SolidFileName='%s'",FileName);
		if (commonfun.IsHave(sql))
		{
			str.Format("�ļ�%s�Ѿ����ڣ������",FileName);
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			AfxMessageBox(str);
			return;
		}
		else
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0; 
	}
	else
	{
		//�ж��Ƿ�����ͬ������Ĳ������ж��Ƿ����Ҫ���²���һ���²�Ʒ
		if (commonfun.IsAsmExist(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum)!=-1)
		{
			str.Format("����ͬ�ɱ����������Ѿ����ڣ��Ƿ���Ҫ����µ�");
			if(AfxMessageBox(str,MB_YESNO)!=IDYES)
			{
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				return;
			}
		}
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;          //�²���
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
	((DetailInfoClass *)p_classarray.GetAt(s))->MdlName=m_mdlnum;       //ģ����
	((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=m_inscode;
	((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode=m_pdmcode;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsName=m_insname;      //ʵ����
	((DetailInfoClass *)p_classarray.GetAt(s))->Designer=m_designer;
	((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm=m_group;
	((DetailInfoClass *)p_classarray.GetAt(s))->Note=m_note;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName=FileName;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=m_drwingplace;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=m_place;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwName=DrwName;
	//commonfun.SetParam(s);
	if (!commonfun.SetParam(s))
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
		AfxMessageBox("���������ʧ��");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	//���뻹û�б�����㲿��
	if(!pdtclass.InsertProductMenberForProduct(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
	{
		AfxMessageBox("���������ʧ��");
		commonfun.TheWorkAferSave(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if (IsOneOfProduct)        //��һ����Ʒ��һ����
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;
		//���²�����ͬʱ���²�Ʒ
		if (!pdtclass.SaveBothPartAndAsm(s,"asm",TRUE))
		{
			commonfun.TheWorkAferSave(ProductSolid,FALSE);
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			AfxMessageBox("����ʧ�ܣ�");
			return;
		}
		else
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
            ((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
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
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="����";
			//����������ж����ϴ����ݿ⻹�Ǹ������ݿ�
			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE,TRUE))
			{
				commonfun.TheWorkAferSave(ProductSolid,FALSE);
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				AfxMessageBox("����ʧ�ܣ�");
				return;
			}
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				commonfun.TheWorkAferSave(ProductSolid,TRUE);
				AfxMessageBox("����ɹ�");
			}
		}
		else
		{   
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="������";
			//����֮ǰҪ���޸Ĺ�������Ȳ���
			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,TRUE,IsLocal))
			{
				commonfun.TheWorkAferSave(ProductSolid,FALSE);
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				AfxMessageBox("����ʧ�ܣ�");
				return;
			}
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				commonfun.TheWorkAferSave(ProductSolid,TRUE);
				AfxMessageBox("����ɹ�");
			}
		}
	}
	OnOK();
}
void CPdtSaveAsDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
/************************************************************************/
/* ѡ����ά���ص�Ŀ¼                                                   */
/************************************************************************/
void CPdtSaveAsDlg::OnBnClickedButtonSolidlocalplace()
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
void CPdtSaveAsDlg::OnBnClickedButtonDrwlocalplace()
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
/************************************************************************/
/* ѡ�����������ά��Ŀ¼                                               */
/************************************************************************/
void CPdtSaveAsDlg::OnBnClickedButtonSolidserverplace()
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
/* ѡ��������Ϲ���ͼ��Ŀ¼                                             */
/************************************************************************/
void CPdtSaveAsDlg::OnBnClickedButtonDwgserverplace()
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

void CPdtSaveAsDlg::OnBnClickedButtonGrpfrm()
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

void CPdtSaveAsDlg::OnBnClickedButtonGrpfrm2()
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
