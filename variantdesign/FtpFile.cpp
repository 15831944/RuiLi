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

BOOL CFtpFile::UploadFiletoFile(CString strLocalFilePath, CString strServerFilePath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
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
BOOL CFtpFile::UploadFiletoDirectory(CString strLocalFilePath, CString strServerDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=0;
	if((strServerDirectoryPath!="")&&(strLocalFilePath!=""))
	{
		//�жϱ����ļ��Ƿ����
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalFilePath);
		if(!a)
		{
			//AfxMessageBox("��Ҫ������ļ������ڻ���·������ȷ��");
			returnvalue=TRUE;
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
				BOOL b=pFileFind->FindFile(strServerDirectoryPath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
				//����ָ����Ŀ¼�Ƿ����
				if(!b)
				{
					//AfxMessageBox("�����ļ���Ŀ¼�����ڣ�");
					pConnection->CreateDirectory(strServerDirectoryPath);
					//pFileFind->Close();
					//pFileFind=NULL;
				}
				//else
				//{
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
				//}
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
/************************************************************************/
/* �жϷ�����������ļ��Ƿ����1.�ļ���.prt/.asm/.drw/,2.������·��,3.���ݵĵ�ַ��������������ر���     */
/************************************************************************/
BOOL CFtpFile::FindFile(CString FileName,CString strServerDirectoryPath, CString BackupPath,CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=FALSE;
	CString	 strServerFilePath;
	if(strServerDirectoryPath!="")
	{
		FileName=FileName+".1";
		if(strServerDirectoryPath.Right(1)!="/")
			strServerFilePath=strServerDirectoryPath+"/"+FileName;
		else
			strServerFilePath=strServerDirectoryPath+FileName;
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
			BOOL b=pFileFind->FindFile(strServerFilePath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
			//����ָ����Ŀ¼�Ƿ����
			if(!b)
			{
				returnvalue=TRUE;
			}
			else
			{
				//CString str;
				//str.Format("��������%s����ļ��Ѿ����ڣ��Ƿ��滻��",FileName);
				//if(AfxMessageBox(str,MB_YESNO)==IDYES)
				//{
					RemoveDireAndFile(BackupPath);
					if(!DownloadFiletoDirectory(strServerFilePath,BackupPath,strFtp,strFtpName,strFtpPwd,iFtpPort))
					{
						returnvalue=FALSE;
					}
					if(pConnection->Remove(strServerFilePath))
					{
						returnvalue=TRUE;
					}
					else
						returnvalue=FALSE;
				//}
				//else
					//returnvalue=FALSE;
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
	}
	else
	{
		returnvalue=FALSE;
	}
	return returnvalue;
}
/************************************************************************/
/* ɾ�������ļ�                                                             */
/************************************************************************/
BOOL CFtpFile::RemoveFile(CString strServerFilePath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=FALSE;
	if(strServerFilePath!="")
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
			BOOL b=pFileFind->FindFile(strServerFilePath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
			//����ָ����Ŀ¼�Ƿ����
			if(!b)
			{
				returnvalue=TRUE;
			}
			else
			{
				if(pConnection->Remove(strServerFilePath))
				{
					returnvalue=TRUE;
				}
				else
				    returnvalue=FALSE;
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
	}
	else
	{
		returnvalue=FALSE;
	}
	return returnvalue;
}
// �ϴ�����Ŀ¼�����ļ���������ָ��Ŀ¼
BOOL CFtpFile::UploadDirectorytoDirectory (CString strLocalDirectoryPath, CString strServerDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
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
			CInternetSession *pSession;
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
				//CFtpFileFind pFileFind(pConnection);

				//���ҷ�������Ŀ¼
				pConnection->SetCurrentDirectory("\\");
				BOOL b=pFileFind->FindFile(strServerDirectoryPath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
				if(!b)
				{
					pConnection->CreateDirectory(strServerDirectoryPath);
				}
				//else
				//{
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
							UploadDirectorytoDirectory(strLocalName,strSaveName,strFtp,strFtpName,strFtpPwd,iFtpPort);
						}
						returnvalue=1;
					}
				//}
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
BOOL CFtpFile::UploadDirectorytoDirectory2(CString strLocalDirectoryPath, CString strServerDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
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
				BOOL b=pFileFind->FindFile(strServerDirectoryPath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
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
		returnvalue=UploadDirectorytoDirectory(strLocalDirectoryPath,strServerDirectoryPath,strFtp,strFtpName,strFtpPwd,iFtpPort);
	}
	return returnvalue;
}

// �����ļ�
BOOL CFtpFile::DownloadFiletoFile(CString strServerFilePath, CString strLocalFilePath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
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
			strServerFilePath.MakeLower();
			BOOL b=pFileFind->FindFile(strServerFilePath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
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
BOOL CFtpFile::DownloadFiletoDirectory(CString strServerFilePath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	//strServerFilePath.Replace(" ","#");
	BOOL returnvalue=FALSE;
	if((strServerFilePath!="")&&(strLocalDirectoryPath!=""))
	{
		//�жϱ���Ŀ¼�Ƿ����
		CFileFind ff;
		CFile file;
		BOOL c;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(!a)
		{
			if (strLocalDirectoryPath.Right(1)!=":")
			{
				if(::CreateDirectory(strLocalDirectoryPath,NULL)==0)
					return FALSE;
			}
			
		}
		//��ʼ��FTP����
		//else
		//{
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
				strServerFilePath.MakeLower();
				BOOL b=pFileFind->FindFile(strServerFilePath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
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
					//Ӧ���ж�Ҫ���ص��ļ��Ƿ��Ѿ�����,����Ѵ���,��ɾ��;
					if(strLocalDirectoryPath.Right(1)!="\\")
						strSaveName=strLocalDirectoryPath+"\\"+strName;
					else
						strSaveName=strLocalDirectoryPath+strName;
					if (ff.FindFile(strSaveName))
					{
						file.Remove(strSaveName);
					}
					c=pConnection->GetFile(strServerFilePath,strSaveName);
					//AfxMessageBox(strServerFilePath);
					//AfxMessageBox(strName);
					if(c)
					{
						//AfxMessageBox("�ļ�����ɹ���");
						returnvalue=TRUE;
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
		return FALSE;
	}
	return returnvalue;
}

// ���ط�����ָ��Ŀ¼�����ļ�������ָ��Ŀ¼
BOOL CFtpFile::DownloadDirectorytoDirectory(CString strServerDirectoryPath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=FALSE;
	if((strServerDirectoryPath!="")&&(strLocalDirectoryPath!=""))
	{
		//���ұ���Ŀ¼
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(!a)
		{
			//AfxMessageBox("ָ������Ŀ¼�����ڣ�");
			return FALSE;
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
				strServerDirectoryPath.MakeLower();
				BOOL b=pFileFind->FindFile(strServerDirectoryPath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
				if(!b)
				{
					//AfxMessageBox("ָ������Ŀ¼�����ڣ�");
					return FALSE;
				}
				//����������Ŀ¼
				else
				{
					CString szDir=strServerDirectoryPath;
					if(szDir.Right(1)!="/")
						szDir+="/";
					szDir+="*.*";
					b=pFileFind->FindFile(szDir,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
				}
				while(b)
				{
					b=pFileFind->FindNextFile();
					CString strPath=pFileFind->GetFilePath();
					CString strName=pFileFind->GetFileName();
					if(!pFileFind->IsDirectory() && !pFileFind->IsDots())
					{
						CString strSaveName;
						if(strLocalDirectoryPath.Right(1)!="\\")
							strLocalDirectoryPath+="\\";
						strSaveName=strLocalDirectoryPath+strName;
						pConnection->GetFile(strPath,strSaveName,FALSE);			
					}
					else 
						if(pFileFind->IsDirectory() && !pFileFind->IsDots())
					{
						CString strSaveName;
						if(strLocalDirectoryPath.Right(1)!="\\")
							strLocalDirectoryPath+="\\";
						strSaveName=strLocalDirectoryPath+strName;
						//���ұ����Ƿ��Ѿ�������ļ���,�����,��ɾ��
						if (ff.FindFile(strSaveName))
						{
							RemoveDireAndFile(strSaveName);
						}
							::CreateDirectory(strSaveName,NULL);
							CString strFromName;
							if(strServerDirectoryPath.Right(1)!="/")
								strServerDirectoryPath+="/";
							strFromName=strServerDirectoryPath+strName;
							//AfxMessageBox(strFromName);
							DownloadDirectorytoDirectory(strFromName,strSaveName,strFtp,strFtpName,strFtpPwd,iFtpPort);
						
					}
					returnvalue=TRUE;
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
				return FALSE;
			}
		}
	}
	else
	{
		//AfxMessageBox("���벻��Ϊ�գ�");
		return FALSE;
	}
	return returnvalue;
}

// ���ط�����ָ��Ŀ¼���������ļ�������ָ��Ŀ¼
BOOL CFtpFile::DownloadDirectorytoDirectory2(CString strServerDirectoryPath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=FALSE;
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
				strServerDirectoryPath.MakeLower();
				BOOL b=pFileFind->FindFile(strServerDirectoryPath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
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
					if (ff.FindFile(strLocalDirectoryPath))
					{
						 RemoveDireAndFile(strLocalDirectoryPath);
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
		returnvalue=DownloadDirectorytoDirectory(strServerDirectoryPath,strLocalDirectoryPath,strFtp,strFtpName,strFtpPwd,iFtpPort);
	}
	return returnvalue;
}
//����һ���ļ�������Ŀ¼
BOOL CFtpFile::DownloadFilestoDirectory(CStringArray * strServerFilePath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{

	CString str,str1;
	BOOL    returnvalue=0;
	TCHAR  * lpError=NULL;
	BOOL   IsHave=TRUE;
	if((!strServerFilePath->IsEmpty())&&(strLocalDirectoryPath!=""))
	{
		//�жϱ���Ŀ¼�Ƿ����
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(!a)
		{
			if (strLocalDirectoryPath.Right(1)!=":")
			{
				if(::CreateDirectory(strLocalDirectoryPath,NULL)==0)
					return FALSE;
			}
			//str.Format("����Ŀ¼%s��������,�޷����ص�����Ŀ¼",strLocalDirectoryPath.GetBuffer());
			//AfxMessageBox(str);
			//return FALSE;
		}
		//��ʼ��FTP����
		//else
		//{
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
			//e->GetErrorMessage(lpError,100,NULL);
			//strError=CString(lpError);
			//e->Delete();
			delete pSession;
			pConnection=NULL;
			return FALSE;
		}
		//�����������
		if((pConnection!=NULL)&&(strServerFilePath->GetCount()>0))
		{
			pFileFind=new CFtpFileFind(pConnection);
			//���Ȳ���һ���ļ��Ƿ񶼴���
			for (int i=0;i<strServerFilePath->GetCount();i++)
			{
				str1=strServerFilePath->GetAt(i);
				BOOL b=pFileFind->FindFile(str1.MakeLower(),INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
				if (!b)
				{
					IsHave=FALSE;
					str.Format("%s�ڷ��������޷��ҵ��ļ�%s\n",str,strServerFilePath->GetAt(i));
					AfxMessageBox(str);
					return FALSE;
				}
			}
			if (!IsHave)
			{
				pFileFind->Close();
				pFileFind=NULL;
				delete pFileFind;
				pConnection->Close();
				delete pConnection;
				delete pSession;
				return FALSE;
			}
			for (int i=0;i<strServerFilePath->GetCount();i++)
			{
				str1=strServerFilePath->GetAt(i);
				BOOL b=pFileFind->FindFile(str1.MakeLower(),INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
				//BOOL b=pFileFind->FindFile(strServerFilePath->GetAt(i),INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
				if (b)
				{
					CString   strName;
					int       numStart=strServerFilePath->GetAt(i).ReverseFind('/');
					if(numStart!=-1)
						strName=strServerFilePath->GetAt(i).Right(strServerFilePath->GetAt(i).GetLength()-numStart-1).GetString();
					CString   strSaveName;
					if(strLocalDirectoryPath.Right(1)!="\\")
						strSaveName=strLocalDirectoryPath+"\\"+strName;
					else
						strSaveName=strLocalDirectoryPath+strName;
					str1=strServerFilePath->GetAt(i);
					//BOOL c=pConnection->GetFile(strServerFilePath->GetAt(i),strSaveName,FALSE);
					BOOL c=pConnection->GetFile(str1.MakeLower(),strSaveName,FALSE);
				}
			}
			//�Ͽ�����
			pFileFind->Close();
			pFileFind=NULL;
			delete pFileFind;
			pConnection->Close();
			delete pConnection;
			delete pSession;
		}
		//}
	}
	return TRUE;
}
/************************************************************************/
/* localdir��ʽ:g:\abc\bcd                                              */
/************************************************************************/
BOOL CFtpFile::RemoveDireAndFile(CString localdir)
{
	CFileFind ff;
	CString szDir = localdir;
	if(szDir.Right(1) != "\\")
		szDir += "\\";
	szDir += "*.*";

	BOOL res = ff.FindFile(szDir);
	while(res)
	{
		res = ff.FindNextFile();
		if(ff.IsDirectory() && !ff.IsDots())       //Ŀ¼���ļ���
		{
			//�����һ����Ŀ¼���õݹ��������һ����
			CString strPath = ff.GetFilePath();    //�õ�·������Ϊ�ݹ���õĿ�ʼ			         	
			RemoveDireAndFile(strPath);            //�ݹ����
		}
		else if(!ff.IsDirectory() && !ff.IsDots()) //������Ͳ���ļ�
		{
			//��ʾ��ǰ���ʵ��ļ�
			CString strPath="";			
			strPath = ff.GetFilePath();
			DeleteFile(strPath);
		}
	}	
	ff.Close();                                   //�ر�
	RemoveDirectory(_T(localdir+"\\"));	
	return 1;
}
/************************************************************************/
/* ɾ������ʧ�ܵ�Ŀ¼                                                   */
/************************************************************************/
BOOL CFtpFile::DeleteDirAndFile(CString strServerDirectoryPath,CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
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
			BOOL b=pFileFind->FindFile(strServerDirectoryPath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
			if(!b)
			{
				returnvalue=1;
			}
			//����������Ŀ¼
			else
			{
				CString szDir=strServerDirectoryPath;
				if(szDir.Right(1)!="/")
					szDir+="/";
				szDir+="*.*";
				b=pFileFind->FindFile(szDir,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
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
						DeleteDirAndFile(strFromName,strFtp,strFtpName,strFtpPwd,iFtpPort);
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
/* �������ص���Ŀ¼���ļ�                                               */
/************************************************************************/
CString CFtpFile::FindFile(CString str,CString str1,CString str2)   //1.����Ŀ¼��ַ��2.������Ŀ¼��ַ 3.�����
{
	CFileFind finder;
	CString strname,strpath;
	int i;
	if(str1.Right(1)=="/")
	{
		//ȥ��/
		i=str1.ReverseFind('/');
		strname=str1.Left(i);
		str1=strname;
	}
	i=str1.ReverseFind('/');
	str1.Delete(0,i+1);
	strname=str1;
	str=str+"\\"+strname;
	str+= _T("\\*.*");
	BOOL bWorking = finder.FindFile(str);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		strpath=finder.GetFilePath();
		strpath.MakeLower();
		str2.MakeLower();
		int k=strpath.Find(str2,0);
		if (k==-1)
		{
			continue;
		}
		else
		{
			finder.Close();
			return strpath;
		}

		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())
		{
			/*CString str = finder.GetFilePath();
			Recurse(str);*/
			continue;
		}
	}
	return strpath;
	finder.Close();
}


