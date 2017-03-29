// FtpFile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FtpFile.h"
//#include ".\ftpfile.h"


// CFtpFile

CFtpFile::CFtpFile()
{
}

CFtpFile::~CFtpFile()
{
}


// �ϴ��ļ�

BOOL CFtpFile::UploadFiletoFile(CString strLocalFilePath, CString strServerFilePath, FtpConnectInfo info)
{
	
	CString strFtp=info.strFtp;
    CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	if((strLocalFilePath!="")&&(strServerFilePath!=""))
	{
		//�жϱ����ļ��Ƿ����
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalFilePath);
		if(!a)
		{
			//AfxMessageBox("��Ҫ������ļ������ڻ���·������ȷ��");
		}	
		else
		{
			//��ʼ��FTP����
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			pConnection=NULL;
			//��������
			pSession=new CInternetSession();
			try
			{
				pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
			}
			catch(CInternetException* e)
			{
				e->Delete();
				pConnection=NULL;
			}
			//�����������
			if(pConnection!=NULL)
			{
				BOOL b=pConnection->PutFile(strLocalFilePath,strServerFilePath);
				if(b)
				{
					//AfxMessageBox("�ļ�����ɹ���");
					returnvalue=1;
				}	
				if(pConnection!=NULL)
				{
					pConnection->Close();
					delete pConnection;
				}
				delete pSession;
			}
			else
			{
				//AfxMessageBox("���ӷ�����ʧ�ܣ�");
			}
		}
	}
	else
	{
		//AfxMessageBox("���벻��Ϊ�գ�");
	}
	return returnvalue;
}

// �ϴ��ļ���������ָ��Ŀ¼
BOOL CFtpFile::UploadFiletoDirectory(CString strLocalFilePath, CString strServerDirectoryPath,FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	if((strServerDirectoryPath!="")&&(strLocalFilePath!=""))
	{
		//�жϱ����ļ��Ƿ����
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalFilePath);
		if(!a)
		{
			//AfxMessageBox("��Ҫ������ļ������ڻ���·������ȷ��");
		}
		else
		{
			//��ʼ��FTP����
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			CFtpFileFind* pFileFind;
			pConnection=NULL;
			pFileFind=NULL;
			//��������
			pSession=new CInternetSession();
			try
			{
				pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
			}
			catch(CInternetException* e)
			{
				e->Delete();
				pConnection=NULL;
			}
			//�����������
			if(pConnection!=NULL)
			{
				pFileFind=new CFtpFileFind(pConnection);
				BOOL b=pFileFind->FindFile(strServerDirectoryPath);
				//����ָ����Ŀ¼�Ƿ����
				if(!b)
				{
					//AfxMessageBox("�����ļ���Ŀ¼�����ڣ�");
					pFileFind->Close();
					pFileFind=NULL;
				}
				else
				{
					//����ͬ���ļ�����
					CString strName;
					int numStart=strLocalFilePath.ReverseFind('\\');
					if(numStart!=-1)
						strName=strLocalFilePath.Right(strLocalFilePath.GetLength()-numStart-1).GetString();
					CString strServerFilePath;
					if(strServerDirectoryPath.Right(1)!="/")
						strServerFilePath=strServerDirectoryPath+"/"+strName;
					else
						strServerFilePath=strServerDirectoryPath+strName;
					//AfxMessageBox(strServerFilePath);
					//AfxMessageBox(strName);
					BOOL c=pConnection->PutFile(strLocalFilePath,strServerFilePath);
					if(c)
					{
						//AfxMessageBox("�ļ�����ɹ���");
						returnvalue=1;
					}
				}
				//�Ͽ�FTP����
				if(pFileFind!=NULL)
				{
					pFileFind->Close();
					pFileFind=NULL;
				}
				delete pFileFind;
				if(pConnection!=NULL)
				{
					pConnection->Close();
					delete pConnection;
				}
				delete pSession;
			}
			else
			{
				//AfxMessageBox("���ӷ�����ʧ�ܣ�");
			}
		}
	}
	else
	{
		//AfxMessageBox("���벻��Ϊ�գ�");
	}
	return returnvalue;
}

// �ϴ�����Ŀ¼�����ļ���������ָ��Ŀ¼
BOOL CFtpFile::UploadDirectorytoDirectory(CString strLocalDirectoryPath, CString strServerDirectoryPath, FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	if((strLocalDirectoryPath!="")&&(strServerDirectoryPath!=""))
	{
		//���ұ���Ŀ¼
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(!a)
		{
			AfxMessageBox("��Ҫ�����Ŀ¼�����ڻ���·������ȷ��");
		}
		//���ҷ�������Ŀ¼
		else
		{
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			CFtpFileFind* pFileFind;
			pConnection=NULL;
			pFileFind=NULL;
			//��ʼ��
			pSession= new CInternetSession();
			try
			{
				pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
			}
			catch(CInternetException* e)
			{
				e->Delete();
				pConnection=NULL;
			}
			if(pConnection!=NULL)
			{
				//��ʼ�����
				pFileFind= new CFtpFileFind(pConnection);
				//���ҷ�������Ŀ¼
				pConnection->SetCurrentDirectory("\\");
				BOOL b=pFileFind->FindFile(strServerDirectoryPath);
				if(!b)
				{
					AfxMessageBox("������Ŀ¼�����ڻ���·������");
				}
				else
				{
					//��������Ŀ¼
					CString szDir=strLocalDirectoryPath;
					if(szDir.Right(1)!="/")
						szDir+="/";
					szDir+="*.*";
					b=ff.FindFile(szDir);
					while(b)
					{
						//�ϴ��ļ�����
						b=ff.FindNextFile();
						CString strPath=ff.GetFilePath();
						CString strName=ff.GetFileName();
						if(!ff.IsDirectory()&&!ff.IsDots())
						{
							CString strSaveName;
							if(strServerDirectoryPath.Right(1)!="/")
								strSaveName=strServerDirectoryPath+"/"+strName;
							else
								strSaveName=strServerDirectoryPath+strName;
							pConnection->PutFile(strPath,strSaveName,FALSE);			
						}
						else if(ff.IsDirectory()&&!ff.IsDots())
						{
							CString strSaveName;
							if(strServerDirectoryPath.Right(1)!="/")
								strSaveName=strServerDirectoryPath+"/"+strName;
							else
								strSaveName=strServerDirectoryPath+strName;
							pConnection->CreateDirectory(strSaveName);
							CString strLocalName;
							if(strLocalDirectoryPath.Right(1)!="\\")
								strLocalDirectoryPath+="\\";
							strLocalName=strLocalDirectoryPath+strName;
							//AfxMessageBox(strLocalName);
							//AfxMessageBox(strSaveName);
							UploadDirectorytoDirectory(strLocalName,strSaveName,info);
						}
						returnvalue=1;
					}
				}
				//�ر�����
				if(pFileFind!=NULL)
				{
					pFileFind->Close();
					pFileFind=NULL;
				}
				delete pFileFind;
				if(pConnection!=NULL)
				{
					pConnection->Close();
					delete pConnection;
				}
				delete pSession;
			}
			else
			{
				AfxMessageBox("���ӷ�����ʧ�ܣ�");
			}
		}
	}
	else
	{
		AfxMessageBox("���벻��Ϊ�գ�");
	}
	return returnvalue;
}

// �ϴ�����Ŀ¼���������ļ���������ָ��Ŀ¼
BOOL CFtpFile::UploadDirectorytoDirectory2(CString strLocalDirectoryPath, CString strServerDirectoryPath, FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	if((strServerDirectoryPath!="")&&(strLocalDirectoryPath!=""))
	{
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(a)
		{
			//���ҷ�������Ŀ¼
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			CFtpFileFind* pFileFind;
			pConnection=NULL;
			pFileFind=NULL;
			//��ʼ��
			pSession= new CInternetSession();
			try
			{
				pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
			}
			catch(CInternetException* e)
			{
				e->Delete();
				pConnection=NULL;
			}
			if(pConnection!=NULL)
			{
				//���ҷ�����Ŀ¼�Ƿ����
				pFileFind=new CFtpFileFind(pConnection);
				pConnection->SetCurrentDirectory("/");
				BOOL b=pFileFind->FindFile(strServerDirectoryPath);
				if(b)
				{
					CString theName;
					if(strLocalDirectoryPath.Right(1)!="\\")
					{
						int num=strLocalDirectoryPath.ReverseFind('\\');
						theName=strLocalDirectoryPath.Right(strLocalDirectoryPath.GetLength()-num-1);
					}
					else
					{
						CString temp=strLocalDirectoryPath.Left(strLocalDirectoryPath.GetLength()-1);
						int num=temp.ReverseFind('\\');
						theName=temp.Right(temp.GetLength()-num-1);
					}
					if(strServerDirectoryPath.Right(1)!="/")
					{
						strServerDirectoryPath+="/"+theName;
					}
					else
					{
						strServerDirectoryPath+=theName;
					}
					pConnection->CreateDirectory(strServerDirectoryPath);
					returnvalue=1;
				}
			}
			if(pFileFind!=NULL)
			{
				pFileFind->Close();
				delete pFileFind;
			}
			if(pConnection!=NULL)
			{
				pConnection->Close();
				delete pConnection;
			}
			delete pSession;
		}
	}
	if(returnvalue==1)
	{
		returnvalue=UploadDirectorytoDirectory(strLocalDirectoryPath,strServerDirectoryPath,info);
	}
	return returnvalue;
}

// �����ļ�
BOOL CFtpFile::DownloadFiletoFile(CString strServerFilePath, CString strLocalFilePath,  FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	if((strLocalFilePath!="")&&(strServerFilePath!=""))
	{
		//��ʼ��FTP����
		CInternetSession* pSession;
		CFtpConnection* pConnection;
		CFtpFileFind* pFileFind;
		pConnection=NULL;
		pFileFind=NULL;
		//��������
		pSession=new CInternetSession();
		try
		{
			pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
		}
		catch(CInternetException* e)
		{
			e->Delete();
			pConnection=NULL;
		}
		//�����������
		if(pConnection!=NULL)
		{
			pFileFind=new CFtpFileFind(pConnection);
			BOOL b=pFileFind->FindFile(strServerFilePath);
			//����ָ����Ŀ¼�Ƿ����
			if(!b)
			{
				//AfxMessageBox("��Ҫ���ص��ļ������ڣ�");
				pFileFind->Close();
				pFileFind=NULL;
			}
			else
			{
				BOOL b=pConnection->GetFile(strServerFilePath,strLocalFilePath,FALSE);
				if(b)
				{
					//AfxMessageBox("�ļ����سɹ���");
					returnvalue=1;
				}
			}
			//�Ͽ�FTP����
			if(pFileFind!=NULL)
			{
				pFileFind->Close();
				pFileFind=NULL;
			}
			delete pFileFind;
			if(pConnection!=NULL)
			{
				pConnection->Close();
				delete pConnection;
			}
			delete pSession;
		}
		else
		{
			//AfxMessageBox("���ӷ�����ʧ�ܣ�");
		}
	}
	else
	{
		//AfxMessageBox("���벻��Ϊ�գ�");	
	}
	return returnvalue;
}

// �����ļ�������ָ��Ŀ¼����ͬ���ļ�
BOOL CFtpFile::DownloadFiletoDirectory(CString strServerFilePath, CString strLocalDirectoryPath, FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	if((strServerFilePath!="")&&(strLocalDirectoryPath!=""))
	{
		//�жϱ���Ŀ¼�Ƿ����
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(!a)
		{
			//AfxMessageBox("��Ҫ�����ļ���Ŀ¼�����ڻ���·������");
		}
		//��ʼ��FTP����
		else
		{
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			CFtpFileFind* pFileFind;
			pConnection=NULL;
			pFileFind=NULL;
			//��������
			pSession=new CInternetSession();
			try
			{
				pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
			}
			catch(CInternetException* e)
			{
				e->Delete();
				pConnection=NULL;
			}
			//�����������
			if((pConnection!=NULL)&&(strServerFilePath!=""))
			{
				pFileFind=new CFtpFileFind(pConnection);
				BOOL b=pFileFind->FindFile(strServerFilePath);
				//����ָ�����ļ��Ƿ����
				if(!b)
				{
					//AfxMessageBox("�ļ������ڣ�");
					pFileFind->Close();
					pFileFind=NULL;
				}
				else
				{
					//����ͬ���ļ�����
					CString strName;
					int numStart=strServerFilePath.ReverseFind('/');
					if(numStart!=-1)
						strName=strServerFilePath.Right(strServerFilePath.GetLength()-numStart-1).GetString();
					CString strSaveName;
					if(strLocalDirectoryPath.Right(1)!="\\")
						strSaveName=strLocalDirectoryPath+"\\"+strName;
					else
						strSaveName=strLocalDirectoryPath+strName;
					//AfxMessageBox(strServerFilePath);
					//AfxMessageBox(strName);
					BOOL c=pConnection->GetFile(strServerFilePath,strSaveName);
					if(c)
					{
						//AfxMessageBox("�ļ�����ɹ���");
						returnvalue=1;
					}
				}
				//�Ͽ�FTP����
				if(pFileFind!=NULL)
				{
					pFileFind->Close();
					pFileFind=NULL;
				}
				delete pFileFind;
				if(pConnection!=NULL)
				{
					pConnection->Close();
					delete pConnection;
				}
				delete pSession;
			}
			else
			{
				//AfxMessageBox("���ӷ�����ʧ�ܣ�");
			}
		}
	}
	else
	{
		//AfxMessageBox("���벻��Ϊ�գ�");
	}
	return returnvalue;
}

// ���ط�����ָ��Ŀ¼�����ļ�������ָ��Ŀ¼
BOOL CFtpFile::DownloadDirectorytoDirectory(CString strServerDirectoryPath, CString strLocalDirectoryPath, FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	if((strServerDirectoryPath!="")&&(strLocalDirectoryPath!=""))
	{
		//���ұ���Ŀ¼
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(!a)
		{
			//AfxMessageBox("ָ������Ŀ¼�����ڣ�");
		}
		else
		{
			//���ҷ�������Ŀ¼
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			CFtpFileFind* pFileFind;
			pConnection=NULL;
			pFileFind=NULL;
			//��ʼ��
			pSession= new CInternetSession();
			try
			{
				pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
			}
			catch(CInternetException* e)
			{
				e->Delete();
				pConnection=NULL;
			}
			if((pConnection!=NULL)&&(strServerDirectoryPath!=""))
			{
				pFileFind= new CFtpFileFind(pConnection);
				//����Ŀ¼
				pConnection->SetCurrentDirectory("\\");
				BOOL b=pFileFind->FindFile(strServerDirectoryPath);
				if(!b)
				{
					//AfxMessageBox("ָ������Ŀ¼�����ڣ�");
				}
				//����������Ŀ¼
				else
				{
					CString szDir=strServerDirectoryPath;
					if(szDir.Right(1)!="/")
						szDir+="/";
					szDir+="*.*";
					b=pFileFind->FindFile(szDir);
				}
				while(b)
				{
					b=pFileFind->FindNextFile();
					CString strPath=pFileFind->GetFilePath();
					CString strName=pFileFind->GetFileName();
					if(!pFileFind->IsDirectory()&&!pFileFind->IsDots())
					{
						CString strSaveName;
						if(strLocalDirectoryPath.Right(1)!="\\")
							strLocalDirectoryPath+="\\";
						strSaveName=strLocalDirectoryPath+strName;
						pConnection->GetFile(strPath,strSaveName,FALSE);			
					}
					else if(pFileFind->IsDirectory()&&!pFileFind->IsDots())
					{
						CString strSaveName;
						if(strLocalDirectoryPath.Right(1)!="\\")
							strLocalDirectoryPath+="\\";
						strSaveName=strLocalDirectoryPath+strName;
						::CreateDirectory(strSaveName,NULL);
						CString strFromName;
						if(strServerDirectoryPath.Right(1)!="/")
							strServerDirectoryPath+="/";
						strFromName=strServerDirectoryPath+strName;
						//AfxMessageBox(strFromName);
						DownloadDirectorytoDirectory(strFromName,strSaveName,info);
					}
					returnvalue=1;
				}
				//�ر�����
				if(pFileFind!=NULL)
				{
					pFileFind->Close();
					pFileFind=NULL;
				}
				delete pFileFind;
				if(pConnection!=NULL)
				{
					pConnection->Close();
					delete pConnection;
				}
				delete pSession;	
			}
			else
			{
				//AfxMessageBox("���ӷ�����ʧ�ܣ�");	
			}
		}
	}
	else
	{
		//AfxMessageBox("���벻��Ϊ�գ�");
	}
	return returnvalue;
}

// ���ط�����ָ��Ŀ¼���������ļ�������ָ��Ŀ¼
BOOL CFtpFile::DownloadDirectorytoDirectory2(CString strServerDirectoryPath, CString strLocalDirectoryPath,FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	if((strServerDirectoryPath!="")&&(strLocalDirectoryPath!=""))
	{
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(a)
		{
			//���ҷ�������Ŀ¼
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			CFtpFileFind* pFileFind;
			pConnection=NULL;
			pFileFind=NULL;
			//��ʼ��
			pSession= new CInternetSession();
			try
			{
				pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
			}
			catch(CInternetException* e)
			{
				e->Delete();
				pConnection=NULL;
			}
			if(pConnection!=NULL)
			{
				pFileFind=new CFtpFileFind(pConnection);
				pConnection->SetCurrentDirectory("/");
				BOOL b=pFileFind->FindFile(strServerDirectoryPath);
				if(b)
				{
					CString theName;
					if(strServerDirectoryPath.Right(1)!="/")
					{
						int num=strServerDirectoryPath.ReverseFind('/');
						theName=strServerDirectoryPath.Right(strServerDirectoryPath.GetLength()-num-1);
					}
					else
					{
						CString temp=strServerDirectoryPath.Left(strServerDirectoryPath.GetLength()-1);
						int num=temp.ReverseFind('/');
						theName=temp.Right(temp.GetLength()-num-1);
					}
					if(strLocalDirectoryPath.Right(1)!="\\")
					{
						strLocalDirectoryPath+="\\"+theName;
					}
					else
					{
						strLocalDirectoryPath+=theName;
					}
					::CreateDirectory(strLocalDirectoryPath,NULL);
					returnvalue=1;
				}
			}
			if(pFileFind!=NULL)
			{
				pFileFind->Close();
				pFileFind=NULL;
			}
			delete pFileFind;
			if(pConnection!=NULL)
			{
				pConnection->Close();
				delete pConnection;
			}
			delete pSession;
		}
	}
	if(returnvalue==1)
	{
		returnvalue=DownloadDirectorytoDirectory(strServerDirectoryPath,strLocalDirectoryPath,info);
	}
	return returnvalue;
}

BOOL CFtpFile::CreateDirectory(FtpConnectInfo info, CString strDirName)
{
	CString strFtp=info.strFtp;
	CString strFtpUser=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	//���ҷ�������Ŀ¼
	CInternetSession* pSession;
	CFtpConnection* pConnection;
	CFtpFileFind* pFileFind;
	pConnection=NULL;
	pFileFind=NULL;
	//��ʼ��
	pSession= new CInternetSession();
	try
	{
		pConnection=pSession->GetFtpConnection(strFtp,strFtpUser,strFtpPwd,iFtpPort);
	}
	catch(CInternetException* e)
	{
		e->Delete();
		pConnection=NULL;
	}
	if(pConnection!=NULL)
	{
		//AfxMessageBox("���ӳɹ���");
		BOOL b=pConnection->CreateDirectory(strDirName);
		if(b)
			returnvalue=1;
	}
	else
	{
		AfxMessageBox("�������ݿ�ʧ�ܣ�");
	}
	if(pConnection!=NULL)
	{
		pConnection->Close();
		delete pConnection;
	}
	delete pSession;
	return returnvalue;
}
BOOL CFtpFile::RemoveDirectory(FtpConnectInfo info, CString strDirName)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	//���ҷ�������Ŀ¼
	CInternetSession* pSession;
	CFtpConnection* pConnection;
	CFtpFileFind* pFileFind;
	pConnection=NULL;
	pFileFind=NULL;
	//��ʼ��
	pSession= new CInternetSession();
	try
	{
		pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
	}
	catch(CInternetException* e)
	{
		e->Delete();
		pConnection=NULL;
	}
	if(pConnection!=NULL)
	{
		//AfxMessageBox("���ӳɹ���");
		BOOL b=pConnection->RemoveDirectory(strDirName);
		if(b)
			returnvalue=1;
	}
	else
	{
		AfxMessageBox("�������ݿ�ʧ�ܣ�");
	}
	if(pConnection!=NULL)
	{
		pConnection->Close();
		delete pConnection;
	}
	delete pSession;
	return returnvalue;
}
BOOL CFtpFile::RemoveFile(FtpConnectInfo info, CString strDirName)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	//���ҷ�������Ŀ¼
	CInternetSession* pSession;
	CFtpConnection* pConnection;
	CFtpFileFind* pFileFind;
	pConnection=NULL;
	pFileFind=NULL;
	//��ʼ��
	pSession= new CInternetSession();
	try
	{
		pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
	}
	catch(CInternetException* e)
	{
		e->Delete();
		pConnection=NULL;
	}
	if(pConnection!=NULL)
	{
		//AfxMessageBox("���ӳɹ���");
		BOOL b=pConnection->Remove(strDirName);
		if(b)
			returnvalue=1;
	}
	else
	{
		AfxMessageBox("�������ݿ�ʧ�ܣ�");
	}
	if(pConnection!=NULL)
	{
		pConnection->Close();
		delete pConnection;
	}
	delete pSession;
	return returnvalue;
}
BOOL CFtpFile::GetCurrentDirectory(FtpConnectInfo info, CString * currentdirectory)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	//���ҷ�������Ŀ¼
	CInternetSession* pSession;
	CFtpConnection* pConnection;
	CFtpFileFind* pFileFind;
	pConnection=NULL;
	pFileFind=NULL;
	//��ʼ��
	pSession= new CInternetSession();
	try
	{
		pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
	}
	catch(CInternetException* e)
	{
		e->Delete();
		pConnection=NULL;
	}
	if(pConnection!=NULL)
	{
		//AfxMessageBox("���ӳɹ���");
		BOOL b=pConnection->GetCurrentDirectory((*currentdirectory));
		if(b)
			returnvalue=1;
	}
	else
	{
		AfxMessageBox("�������ݿ�ʧ�ܣ�");
	}
	if(pConnection!=NULL)
	{
		pConnection->Close();
		delete pConnection;
	}
	delete pSession;
	return returnvalue;
}
/************************************************************************/
/* ɾ����������ָ��Ŀ¼�µ���������                                     */
/************************************************************************/
BOOL CFtpFile::DeleteDirAndFile(CString strServerDirectoryPath,/*, CString strLocalDirectoryPath,*/ FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	if(strServerDirectoryPath!="")
	{
			//���ҷ�������Ŀ¼
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			CFtpFileFind* pFileFind;
			pConnection=NULL;
			pFileFind=NULL;
			//��ʼ��
			pSession= new CInternetSession();
			try
			{
				pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
			}
			catch(CInternetException* e)
			{
				e->Delete();
				pConnection=NULL;
			}
			if((pConnection!=NULL)&&(strServerDirectoryPath!=""))
			{
				pFileFind= new CFtpFileFind(pConnection);
				//����Ŀ¼
				pConnection->SetCurrentDirectory("\\");
				BOOL b=pFileFind->FindFile(strServerDirectoryPath);
				if(!b)
				{
					//AfxMessageBox("ָ������Ŀ¼�����ڣ�");
					returnvalue=1;
				}
				//����������Ŀ¼
				else
				{
					CString szDir=strServerDirectoryPath;
					if(szDir.Right(1)!="/")
						szDir+="/";
					szDir+="*.*";
					b=pFileFind->FindFile(szDir);
					while(b)
					{
						b=pFileFind->FindNextFile();
						CString strPath=pFileFind->GetFilePath();
						CString strName=pFileFind->GetFileName();
						if(!pFileFind->IsDirectory()&&!pFileFind->IsDots())
						{						
							pConnection->Remove(strPath);
						}
						else if(pFileFind->IsDirectory()&&!pFileFind->IsDots())
						{
							
							CString strFromName;
							if(strServerDirectoryPath.Right(1)!="/")
								strServerDirectoryPath+="/";
							strFromName=strServerDirectoryPath+strName;
                            DeleteDirAndFile(strFromName,info);
						}
						returnvalue=1;
						
					} 
					pConnection->RemoveDirectory(strServerDirectoryPath);
				}


				//�ر�����
				if(pFileFind!=NULL)
				{
					pFileFind->Close();
					pFileFind=NULL;
				}
				delete pFileFind;
				if(pConnection!=NULL)
				{
					pConnection->Close();
					delete pConnection;
				}
				delete pSession;	
			}
			else
			{
				AfxMessageBox("���ӷ�����ʧ�ܣ�");	
				returnvalue=0;
			}
		
	}
	return returnvalue;
}
/************************************************************************/
/* ���Ե�������������                                                   */
/************************************************************************/
bool CFtpFile::TestConnect(FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	CInternetSession* pSession;
	CFtpConnection* pConnection;
	pConnection=NULL;
	pSession= new CInternetSession();
	try
	{
		pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
	}
	catch(CInternetException* e)
	{
		e->Delete();
		pConnection=NULL;
		delete pSession;
		return false;
	}
	//���ӳɹ�
    if(pConnection!=NULL)
	{
		pConnection->Close();
		delete pConnection;
        delete pSession;
		return true;
	}
	return false;
}

BOOL CFtpFile::DownloadPartFileToDirectory(CString serverFileName, CString serverDirectory, CString LocalDirectory, FtpConnectInfo info)
{
	return 0;
}

BOOL CFtpFile::Rename(CString serverDirectory,CString newName, FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	CInternetSession* pSession;
	CFtpConnection* pConnection;
	pConnection=NULL;
	pSession= new CInternetSession();
	try
	{
		pConnection=pSession->GetFtpConnection(strFtp,strFtpName,strFtpPwd,iFtpPort);
	}
	catch(CInternetException* e)
	{
		e->Delete();
		pConnection=NULL;
		delete pSession;
		return false;
	}
	//���ӳɹ�
	if(pConnection!=NULL)
	{
		if(!pConnection->Rename(serverDirectory,newName))
		{
			pConnection->Close();
			return false;
		}
		pConnection->Close();
		delete pConnection;
		delete pSession;
		return true;
	}
	return false;
}
