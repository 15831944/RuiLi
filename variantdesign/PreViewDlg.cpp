// PreViewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PreViewDlg.h"
#include "MyDatabase.h"
#include "FtpFile.h"
#include "myproe.h"

UINT MyProductModalThreadProc(LPVOID pParam);
UINT MyProductInsThreadProc(LPVOID pParam);

extern CString g_ConnectString;

extern FtpInfo Ftp_struc;
// CPreViewDlg 对话框

IMPLEMENT_DYNAMIC(CPreViewDlg, CDialog)
CPreViewDlg::CPreViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreViewDlg::IDD, pParent)
{
	m_Initflag=0;
	m_TempPath.Format("c:\\windows\\proetemp\\");
	m_ModalFlag=0;
	m_2DFlag=0;
	m_strID="";
	m_EndFlag=0;
}

CPreViewDlg::~CPreViewDlg()
{
}

void CPreViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AUTOVUEXCTRL1, m_AutoVue);
}


BEGIN_MESSAGE_MAP(CPreViewDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPreViewDlg 消息处理程序

BOOL CPreViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTimer(1,100,NULL);

	CRect rc;
	this->GetClientRect(rc);
	m_AutoVue.MoveWindow(rc);
	m_AutoVue.ZoomFit();

    
	this->DeleteFolderAllFile(m_TempPath);
	::CreateDirectory(m_TempPath,0);
	// TODO:  在此添加额外的初始化
   if(m_ModalFlag==1)
   {
	   this->GetPartModal();
   }else if(m_ModalFlag==2)
   {
	   this->GetPartIns();
   }else if(m_ModalFlag==3)
   {
	   this->NGetProductModal();
   }else if(m_ModalFlag==4)
   {
	   this->NGetProductIns();
   }



    m_Initflag=1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CPreViewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);


	if(m_Initflag==1)
	{
		CRect rc;
		this->GetClientRect(rc);
		m_AutoVue.MoveWindow(rc);
		m_AutoVue.ZoomFit();
	}

	ProWindowRepaint(PRO_VALUE_UNUSED);
	// TODO: 在此添加消息处理程序代码
}



int CPreViewDlg::DeleteFolderAllFile(CString folder)
{
	CFileFind find;
	CString   strPathFiles =folder;
	BOOL      bFind;
	CStringArray strDirArray;
	CStringArray strFileArray;


	if ( strPathFiles.Right(1) != "\\" )
		strPathFiles += "\\";
	strPathFiles += "*.*";

	bFind = find.FindFile( strPathFiles );

	while ( bFind )
	{
		bFind = find.FindNextFile();
		if ( find.IsDirectory() && !find.IsDots() )
		{		
			strDirArray.Add( find.GetFilePath() );
		}
		if ( !find.IsDirectory())
			strFileArray.Add( find.GetFilePath());
	}

	for(int i=0;i<strFileArray.GetSize();i++)
		::DeleteFile(strFileArray[i]);
	for(int i=0;i<strDirArray.GetSize();i++)
	{
		DeleteFolderAllFile(strDirArray[i]);
		::RemoveDirectory(strDirArray[i]);
	}
	return 1;
}

int CPreViewDlg::GetPartModal()
{
	if(m_ModalFlag!=1) return 0;
	if(m_strID=="") return 0;
    int status;
    CMyDatabase mydb;
	status=mydb.Open(g_ConnectString);if(status==0){AfxMessageBox("数据库无法连接");return 0;}
	int rows;
	CString sql,str;
	sql.Format("SELECT * FROM NPartManageTable WHERE Number=%s",m_strID);
	status=mydb.Query(sql);
	rows=mydb.getRows();
	if(rows==0) return 0;
	mydb.MoveBegin();
	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;
	SolidFileName=mydb.getString("SolidFileName");
	SolidFilePlace=mydb.getString("SolidFilePlace");
	DrawingFileName=mydb.getString("DrawingFileName");
	DrawingFilePlace=mydb.getString("DrawingFilePlace");
	m_Name=mydb.getString("Name");
	mydb.Close();
	SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
	DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
	m_LocalSolidPath.Format("%s%s",m_TempPath,SolidFileName);
	m_LocalDrawingPath.Format("%s%s",m_TempPath,DrawingFileName);

	if(DrawingFileName==""&&m_2DFlag==1)
	{
		AfxMessageBox("没有二维图形");
		return 0;
	}

	CFtpFile ftp;
	if(m_2DFlag==1)
	{
		if(DrawingFileName!="") 
		status=ftp.DownloadFiletoFile(DrawingFullName,m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	}

    status=ftp.DownloadFiletoFile(SolidFullName,m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	if(status==0) return 0;
    
	if(m_2DFlag==1)
		m_AutoVue.SetSRC(m_LocalDrawingPath);
	else
		m_AutoVue.SetSRC(m_LocalSolidPath);
	m_AutoVue.ZoomFit();

	return 1;
}

int CPreViewDlg::GetPartIns()
{
	if(m_ModalFlag!=2) return 0;
	if(m_strID=="") return 0;
	int status;
	CMyDatabase mydb;
	status=mydb.Open(g_ConnectString);if(status==0){AfxMessageBox("数据库无法连接");return 0;}
	int rows;
	CString sql,str;
	sql.Format("select * from NPartInsInfoTable WHERE PrtNum=%s",m_strID);
	status=mydb.Query(sql);
	rows=mydb.getRows();
	if(rows==0) return 0;
	mydb.MoveBegin();
	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;
	SolidFileName=mydb.getString("SolidFileName");
	SolidFilePlace=mydb.getString("SolidPlace");
	DrawingFileName=mydb.getString("DrawingName");
	DrawingFilePlace=mydb.getString("DrawingPlace");
	m_Name=mydb.getString("Name");
	mydb.Close();

	if(DrawingFileName==""&&m_2DFlag==1)
	{
		AfxMessageBox("没有二维图形");
		return 0;
	}
	SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
	DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
	m_LocalSolidPath.Format("%s%s",m_TempPath,SolidFileName);
	m_LocalDrawingPath.Format("%s%s",m_TempPath,DrawingFileName);

	CFtpFile ftp;
	if(m_2DFlag==1)
	{
		if(DrawingFileName!="") 
		status=ftp.DownloadFiletoFile(DrawingFullName,m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	}

	status=ftp.DownloadFiletoFile(SolidFullName,m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	if(status==0) return 0;



	if(m_2DFlag==1)
		m_AutoVue.SetSRC(m_LocalDrawingPath);
	else
		m_AutoVue.SetSRC(m_LocalSolidPath);
	m_AutoVue.ZoomFit();

	return 1;
}



int CPreViewDlg::GetProductModal()
{
	if(m_ModalFlag!=3) return 0;
	if(m_strID=="") return 0;
	int status;
	CMyDatabase mydb;
	status=mydb.Open(g_ConnectString);if(status==0){AfxMessageBox("数据库无法连接");return 0;}
	int rows;
	CString sql,str;
	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;

	CFtpFile ftp;

	CStringArray PartNumArray,TypeArray;

	sql.Format("SELECT * FROM NAsmBomTable WHERE AsmNum=%s",m_strID);
	status=mydb.Query(sql);
	rows=mydb.getRows();
	if(rows==0) return 0;
	mydb.MoveBegin();
	for(int i=0;i<rows;i++)
	{
		str.Format("%d",mydb.getInt("PartNum"));
        PartNumArray.Add(str);
		TypeArray.Add(mydb.getString("Type"));
		mydb.MoveNext();
	}



    for(int i=0;i<PartNumArray.GetSize();i++)
	{
		if(TypeArray[i]=="prt")
		{
			sql.Format("select * from NPartInsInfoTable WHERE PrtNum=%s",PartNumArray[i]);
			status=mydb.Query(sql);
			rows=mydb.getRows();
			if(rows==0) return 0;
			mydb.MoveBegin();
		//	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;
			SolidFileName=mydb.getString("SolidFileName");
			SolidFilePlace=mydb.getString("SolidPlace");
			DrawingFileName=mydb.getString("DrawingName");
			DrawingFilePlace=mydb.getString("DrawingPlace");
			m_Name=mydb.getString("Name");

			SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
			DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
			m_LocalSolidPath.Format("%s%s",m_TempPath,SolidFileName);
			m_LocalDrawingPath.Format("%s%s",m_TempPath,DrawingFileName);

		//	CFtpFile ftp;
			if(m_2DFlag==1)
			{
				if(DrawingFileName!="") 
					status=ftp.DownloadFiletoFile(DrawingFullName,m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			}

			status=ftp.DownloadFiletoFile(SolidFullName,m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			if(status==0) return 0;
		}
		else if(TypeArray[i]=="asm")
		{
			sql.Format("select * from NProductInsInfoTable WHERE PdtNum=%s",PartNumArray[i]);
			status=mydb.Query(sql);
			rows=mydb.getRows();
			if(rows==0) return 0;
			mydb.MoveBegin();
			//	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;
			SolidFileName=mydb.getString("SolidFileName");
			SolidFilePlace=mydb.getString("SolidPlace");
			DrawingFileName=mydb.getString("DrawingName");
			DrawingFilePlace=mydb.getString("DrawingPlace");
			m_Name=mydb.getString("Name");
			
			SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
			DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
			m_LocalSolidPath.Format("%s%s",m_TempPath,SolidFileName);
			m_LocalDrawingPath.Format("%s%s",m_TempPath,DrawingFileName);

			//	CFtpFile ftp;
			if(m_2DFlag==1)
			{
				if(DrawingFileName!="") 
					status=ftp.DownloadFiletoFile(DrawingFullName,m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			}

			status=ftp.DownloadFiletoFile(SolidFullName,m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			if(status==0) return 0;
		}

	}


	sql.Format("SELECT * FROM NAsmManageTable WHERE Number=%s",m_strID);
	status=mydb.Query(sql);
	rows=mydb.getRows();
	if(rows==0) return 0;
	mydb.MoveBegin();

	SolidFileName=mydb.getString("SolidFileName");
	SolidFilePlace=mydb.getString("SolidFilePlace");
	DrawingFileName=mydb.getString("DrawingFileName");
	DrawingFilePlace=mydb.getString("DrawingFilePlace");//
	m_Name=mydb.getString("Name");


	SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
	DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
	m_LocalSolidPath.Format("%s%s",m_TempPath,SolidFileName);
	m_LocalDrawingPath.Format("%s%s",m_TempPath,DrawingFileName);

	//CFtpFile ftp;
	if(m_2DFlag==1)
	{
		if(DrawingFileName!="") 
			status=ftp.DownloadFiletoFile(DrawingFullName,m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	}

	status=ftp.DownloadFiletoFile(SolidFullName,m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	if(status==0) return 0;

		mydb.Close();

	if(m_2DFlag==1)
		m_AutoVue.SetSRC(m_LocalDrawingPath);
	else
		m_AutoVue.SetSRC(m_LocalSolidPath);
	m_AutoVue.ZoomFit();


   

	return 1;
}


int CPreViewDlg::GetProductIns()
{
	if(m_ModalFlag!=4) return 0;
	if(m_strID=="") return 0;
	int status;
	CMyDatabase mydb;
	status=mydb.Open(g_ConnectString);if(status==0){AfxMessageBox("数据库无法连接");return 0;}
	int rows;
	CString sql,str;
	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;

	CFtpFile ftp;

	CStringArray PartNumArray,TypeArray;

	sql.Format("SELECT * FROM NProductBomTable WHERE PdtNum=%s",m_strID);
	status=mydb.Query(sql);
	rows=mydb.getRows();
	if(rows==0) return 0;
	mydb.MoveBegin();
	for(int i=0;i<rows;i++)
	{
		str.Format("%d",mydb.getInt("PrtNum"));
		PartNumArray.Add(str);
		TypeArray.Add(mydb.getString("Type"));
		mydb.MoveNext();
	}



	for(int i=0;i<PartNumArray.GetSize();i++)
	{
		if(TypeArray[i]=="prt")
		{
			sql.Format("select * from NPartInsInfoTable WHERE PrtNum=%s",PartNumArray[i]);
			status=mydb.Query(sql);
			rows=mydb.getRows();
			if(rows==0) return 0;
			mydb.MoveBegin();
			//	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;
			SolidFileName=mydb.getString("SolidFileName");
			SolidFilePlace=mydb.getString("SolidPlace");
			DrawingFileName=mydb.getString("DrawingName");
			DrawingFilePlace=mydb.getString("DrawingPlace");
			m_Name=mydb.getString("Name");
	
			SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
			DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
			m_LocalSolidPath.Format("%s%s",m_TempPath,SolidFileName);
			m_LocalDrawingPath.Format("%s%s",m_TempPath,DrawingFileName);

			//	CFtpFile ftp;
			if(m_2DFlag==1)
			{
				if(DrawingFileName!="") 
					status=ftp.DownloadFiletoFile(DrawingFullName,m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			}

			status=ftp.DownloadFiletoFile(SolidFullName,m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			if(status==0) return 0;
		}
		else if(TypeArray[i]=="asm")
		{
			sql.Format("select * from NProductInsInfoTable WHERE PdtNum=%s",PartNumArray[i]);
			status=mydb.Query(sql);
			rows=mydb.getRows();
			if(rows==0) return 0;
			mydb.MoveBegin();
			//	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;
			SolidFileName=mydb.getString("SolidFileName");
			SolidFilePlace=mydb.getString("SolidPlace");
			DrawingFileName=mydb.getString("DrawingName");
			DrawingFilePlace=mydb.getString("DrawingPlace");
			m_Name=mydb.getString("Name");
	
			SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
			DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
			m_LocalSolidPath.Format("%s%s",m_TempPath,SolidFileName);
			m_LocalDrawingPath.Format("%s%s",m_TempPath,DrawingFileName);

			//	CFtpFile ftp;
			if(m_2DFlag==1)
			{
				if(DrawingFileName!="") 
					status=ftp.DownloadFiletoFile(DrawingFullName,m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			}

			status=ftp.DownloadFiletoFile(SolidFullName,m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			if(status==0) return 0;
		}

	}


	sql.Format("select * from NProductInsInfoTable WHERE PdtNum=%s",m_strID);
	status=mydb.Query(sql);
	rows=mydb.getRows();
	if(rows==0) return 0;
	mydb.MoveBegin();

	SolidFileName=mydb.getString("SolidFileName");
	SolidFilePlace=mydb.getString("SolidPlace");
	DrawingFileName=mydb.getString("DrawingName");
	DrawingFilePlace=mydb.getString("DrawingPlace");//
	m_Name=mydb.getString("Name");


	SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
	DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
	m_LocalSolidPath.Format("%s%s",m_TempPath,SolidFileName);
	m_LocalDrawingPath.Format("%s%s",m_TempPath,DrawingFileName);

	//CFtpFile ftp;
	if(m_2DFlag==1)
	{
		if(DrawingFileName!="") 
			status=ftp.DownloadFiletoFile(DrawingFullName,m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	}

	status=ftp.DownloadFiletoFile(SolidFullName,m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	if(status==0) return 0;

	mydb.Close();

	if(m_2DFlag==1)
		m_AutoVue.SetSRC(m_LocalDrawingPath);
	else
		m_AutoVue.SetSRC(m_LocalSolidPath);
	m_AutoVue.ZoomFit();




	return 1;
}


int CPreViewDlg::NGetProductModal()
{
	if(m_ModalFlag!=3) return 0;
	if(m_strID=="") return 0;
	m_pDlg=new CProgressDlg;
	m_pDlg->Create(IDD_DIALOG_PROGRESS,NULL);
	m_pDlg->ShowWindow(SW_SHOW);
	m_pDlg->UpdateWindow();
	
	AfxBeginThread(MyProductModalThreadProc,(LPVOID)this);
	return 1;
}

int CPreViewDlg::NGetProductIns()
{
	if(m_ModalFlag!=4) return 0;
	if(m_strID=="") return 0;
	m_pDlg=new CProgressDlg;
	m_pDlg->Create(IDD_DIALOG_PROGRESS,NULL);
	m_pDlg->ShowWindow(SW_SHOW);
	m_pDlg->UpdateWindow();

	AfxBeginThread(MyProductInsThreadProc,(LPVOID)this);
	return 1;
}



UINT MyProductModalThreadProc(LPVOID pParam)
{
	CPreViewDlg * pUtil=(CPreViewDlg *)pParam;
	CProgressDlg * pDlg=pUtil->m_pDlg;

	if(pUtil->m_ModalFlag!=3) return 0;
	if(pUtil->m_strID=="") return 0;
	int status;
	CMyDatabase mydb;
	status=mydb.Open(g_ConnectString);if(status==0){AfxMessageBox("数据库无法连接");return 0;}
	int rows;
	CString sql,str;
	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;

	CFtpFile ftp;

	CStringArray PartNumArray,TypeArray;


	sql.Format("SELECT * FROM NAsmManageTable WHERE Number=%s",pUtil->m_strID);
	status=mydb.Query(sql);
	rows=mydb.getRows();
	if(rows==0) return 0;
	mydb.MoveBegin();

	SolidFileName=mydb.getString("SolidFileName");
	SolidFilePlace=mydb.getString("SolidFilePlace");
	DrawingFileName=mydb.getString("DrawingFileName");
	DrawingFilePlace=mydb.getString("DrawingFilePlace");//
	pUtil->m_Name=mydb.getString("Name");


	SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
	DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
	pUtil->m_LocalSolidPath.Format("%s%s",pUtil->m_TempPath,SolidFileName);
	pUtil->m_LocalDrawingPath.Format("%s%s",pUtil->m_TempPath,DrawingFileName);

	pUtil->m_ProductLocalSolidPath=pUtil->m_LocalSolidPath;
	pUtil->m_ProductLocalDrawingPath=pUtil->m_LocalDrawingPath;

	//CFtpFile ftp;
	if(pUtil->m_2DFlag==1)
	{
		if(DrawingFileName=="") 
		{
			AfxMessageBox("没有二维图形");
				return 0;
		}
		if(DrawingFileName!="") 
			status=ftp.DownloadFiletoFile(DrawingFullName,pUtil->m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	}

	status=ftp.DownloadFiletoFile(SolidFullName,pUtil->m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	if(status==0) return 0;





	sql.Format("SELECT * FROM NAsmBomTable WHERE AsmNum=%s",pUtil->m_strID);
	status=mydb.Query(sql);
	rows=mydb.getRows();
	if(rows==0) return 0;
	mydb.MoveBegin();
	for(int i=0;i<rows;i++)
	{
		str.Format("%d",mydb.getInt("PartNum"));
		PartNumArray.Add(str);
		TypeArray.Add(mydb.getString("Type"));
		mydb.MoveNext();
	}


    int num=(int)PartNumArray.GetSize();
	for(int i=0;i<PartNumArray.GetSize();i++)
	{
		pUtil->m_pDlg->m_Pos=i*10000/num;
		str.Format("%d%%",i*100/num);
		pUtil->m_pDlg->m_Label=str;
		if(TypeArray[i]=="prt")
		{
			sql.Format("select * from NPartInsInfoTable WHERE PrtNum=%s",PartNumArray[i]);
			status=mydb.Query(sql);
			rows=mydb.getRows();
			if(rows==0) return 0;
			mydb.MoveBegin();
			//	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;
			SolidFileName=mydb.getString("SolidFileName");
			SolidFilePlace=mydb.getString("SolidPlace");
			DrawingFileName=mydb.getString("DrawingName");
			DrawingFilePlace=mydb.getString("DrawingPlace");
			pUtil->m_Name=mydb.getString("Name");

			SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
			DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
			pUtil->m_LocalSolidPath.Format("%s%s",pUtil->m_TempPath,SolidFileName);
			pUtil->m_LocalDrawingPath.Format("%s%s",pUtil->m_TempPath,DrawingFileName);

			//	CFtpFile ftp;
			if(pUtil->m_2DFlag==1)
			{
				//if(DrawingFileName!="") 
					//status=ftp.DownloadFiletoFile(DrawingFullName,pUtil->m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			}

			status=ftp.DownloadFiletoFile(SolidFullName,pUtil->m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			if(status==0) return 0;
		}
		else if(TypeArray[i]=="asm")
		{
			sql.Format("select * from NProductInsInfoTable WHERE PdtNum=%s",PartNumArray[i]);
			status=mydb.Query(sql);
			rows=mydb.getRows();
			if(rows==0) return 0;
			mydb.MoveBegin();
			//	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;
			SolidFileName=mydb.getString("SolidFileName");
			SolidFilePlace=mydb.getString("SolidPlace");
			DrawingFileName=mydb.getString("DrawingName");
			DrawingFilePlace=mydb.getString("DrawingPlace");
			pUtil->m_Name=mydb.getString("Name");

			SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
			DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
			pUtil->m_LocalSolidPath.Format("%s%s",pUtil->m_TempPath,SolidFileName);
			pUtil->m_LocalDrawingPath.Format("%s%s",pUtil->m_TempPath,DrawingFileName);

			//	CFtpFile ftp;
			if(pUtil->m_2DFlag==1)
			{
				//if(DrawingFileName!="") 
					//status=ftp.DownloadFiletoFile(DrawingFullName,pUtil->m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			}

			status=ftp.DownloadFiletoFile(SolidFullName,pUtil->m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			if(status==0) return 0;
		}

	}


	mydb.Close();

	pUtil->m_LocalSolidPath=pUtil->m_ProductLocalSolidPath;
	pUtil->m_LocalDrawingPath=pUtil->m_ProductLocalDrawingPath;

	pUtil->m_pDlg->SendMessage(WM_CLOSE);

    pUtil->m_EndFlag=1;
    
	return false;
}






UINT MyProductInsThreadProc(LPVOID pParam)
{
	CPreViewDlg * pUtil=(CPreViewDlg *)pParam;
	CProgressDlg * pDlg=pUtil->m_pDlg;

	if(pUtil->m_ModalFlag!=4) return 0;
	if(pUtil->m_strID=="") return 0;
	int status;
	CMyDatabase mydb;
	status=mydb.Open(g_ConnectString);if(status==0){AfxMessageBox("数据库无法连接");return 0;}
	int rows;
	CString sql,str;
	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;

	CFtpFile ftp;

	CStringArray PartNumArray,TypeArray;


	sql.Format("select * from NProductInsInfoTable WHERE PdtNum=%s",pUtil->m_strID);
	status=mydb.Query(sql);
	rows=mydb.getRows();
	if(rows==0) return 0;
	mydb.MoveBegin();

	SolidFileName=mydb.getString("SolidFileName");
	SolidFilePlace=mydb.getString("SolidPlace");
	DrawingFileName=mydb.getString("DrawingName");
	DrawingFilePlace=mydb.getString("DrawingPlace");//
	pUtil->m_Name=mydb.getString("Name");


	SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
	DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
	pUtil->m_LocalSolidPath.Format("%s%s",pUtil->m_TempPath,SolidFileName);
	pUtil->m_LocalDrawingPath.Format("%s%s",pUtil->m_TempPath,DrawingFileName);

	pUtil->m_ProductLocalSolidPath=pUtil->m_LocalSolidPath;
	pUtil->m_ProductLocalDrawingPath=pUtil->m_LocalDrawingPath;

	//CFtpFile ftp;
	if(pUtil->m_2DFlag==1)
	{
		if(DrawingFileName=="") 
		{
			AfxMessageBox("没有二维图形");
				return 0;
		}
		if(DrawingFileName!="") 
			status=ftp.DownloadFiletoFile(DrawingFullName,pUtil->m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	}

	status=ftp.DownloadFiletoFile(SolidFullName,pUtil->m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	if(status==0) return 0;





	sql.Format("SELECT * FROM NProductBomTable WHERE PdtNum=%s",pUtil->m_strID);
	status=mydb.Query(sql);
	rows=mydb.getRows();
	if(rows==0) return 0;
	mydb.MoveBegin();
	for(int i=0;i<rows;i++)
	{
		str.Format("%d",mydb.getInt("PrtNum"));
		PartNumArray.Add(str);
		TypeArray.Add(mydb.getString("Type"));
		mydb.MoveNext();
	}


	int num=(int)PartNumArray.GetSize();
	for(int i=0;i<PartNumArray.GetSize();i++)
	{
		pUtil->m_pDlg->m_Pos=i*10000/num;
		str.Format("%d%%",i*100/num);
		pUtil->m_pDlg->m_Label=str;
		if(TypeArray[i]=="prt")
		{
			sql.Format("select * from NPartInsInfoTable WHERE PrtNum=%s",PartNumArray[i]);
			status=mydb.Query(sql);
			rows=mydb.getRows();
			if(rows==0) return 0;
			mydb.MoveBegin();
			//	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;
			SolidFileName=mydb.getString("SolidFileName");
			SolidFilePlace=mydb.getString("SolidPlace");
			DrawingFileName=mydb.getString("DrawingName");
			DrawingFilePlace=mydb.getString("DrawingPlace");
			pUtil->m_Name=mydb.getString("Name");

			SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
			DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
			pUtil->m_LocalSolidPath.Format("%s%s",pUtil->m_TempPath,SolidFileName);
			pUtil->m_LocalDrawingPath.Format("%s%s",pUtil->m_TempPath,DrawingFileName);

			//	CFtpFile ftp;
			if(pUtil->m_2DFlag==1)
			{
				//if(DrawingFileName!="") 
					//status=ftp.DownloadFiletoFile(DrawingFullName,pUtil->m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			}

			status=ftp.DownloadFiletoFile(SolidFullName,pUtil->m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			if(status==0) return 0;
		}
		else if(TypeArray[i]=="asm")
		{
			sql.Format("select * from NProductInsInfoTable WHERE PdtNum=%s",PartNumArray[i]);
			status=mydb.Query(sql);
			rows=mydb.getRows();
			if(rows==0) return 0;
			mydb.MoveBegin();
			//	CString SolidFileName,SolidFilePlace,DrawingFileName,DrawingFilePlace,SolidFullName,DrawingFullName;
			SolidFileName=mydb.getString("SolidFileName");
			SolidFilePlace=mydb.getString("SolidPlace");
			DrawingFileName=mydb.getString("DrawingName");
			DrawingFilePlace=mydb.getString("DrawingPlace");
			pUtil->m_Name=mydb.getString("Name");

			SolidFullName.Format("%s/%s.1",SolidFilePlace,SolidFileName);
			DrawingFullName.Format("%s/%s.1",DrawingFilePlace,DrawingFileName);
			pUtil->m_LocalSolidPath.Format("%s%s",pUtil->m_TempPath,SolidFileName);
			pUtil->m_LocalDrawingPath.Format("%s%s",pUtil->m_TempPath,DrawingFileName);

			//	CFtpFile ftp;
			if(pUtil->m_2DFlag==1)
			{
				//if(DrawingFileName!="") 
					//status=ftp.DownloadFiletoFile(DrawingFullName,pUtil->m_LocalDrawingPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			}

			status=ftp.DownloadFiletoFile(SolidFullName,pUtil->m_LocalSolidPath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			if(status==0) return 0;
		}

	}




	mydb.Close();

	pUtil->m_pDlg->SendMessage(WM_CLOSE);

	pUtil->m_LocalSolidPath=pUtil->m_ProductLocalSolidPath;
	pUtil->m_LocalDrawingPath=pUtil->m_ProductLocalDrawingPath;

	pUtil->m_EndFlag=1;

	return false;
}












void CPreViewDlg::OnClose()
{
	ProWindowRepaint(PRO_VALUE_UNUSED);

	CDialog::OnClose();
}











void CPreViewDlg::OnTimer(UINT nIDEvent)
{
	if(m_EndFlag==1)
	{
		if(m_2DFlag==1)
			m_AutoVue.SetSRC(m_LocalDrawingPath);
		else
			m_AutoVue.SetSRC(m_LocalSolidPath);
		m_AutoVue.ZoomFit();
		m_EndFlag=0;
	}

	CDialog::OnTimer(nIDEvent);
}
