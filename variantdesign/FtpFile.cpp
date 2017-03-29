// FtpFile.cpp : 实现文件
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


// 上传文件

BOOL CFtpFile::UploadFiletoFile(CString strLocalFilePath, CString strServerFilePath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=0;
	if((strLocalFilePath!="")&&(strServerFilePath!=""))
	{
		//判断本地文件是否存在
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalFilePath);
		if(!a)
		{
			//AfxMessageBox("需要保存的文件不存在或者路径不正确！");
		}	
		else
		{
			//初始化FTP连接
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			pConnection=NULL;
			//建立连接
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
			//建立连接完成
			if(pConnection!=NULL)
			{
				BOOL b=pConnection->PutFile(strLocalFilePath,strServerFilePath);
				if(b)
				{
					//AfxMessageBox("文件保存成功！");
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
				//AfxMessageBox("连接服务器失败！");
			}
		}
	}
	else
	{
		//AfxMessageBox("输入不能为空！");
	}
	return returnvalue;
}

// 上传文件到服务器指定目录
BOOL CFtpFile::UploadFiletoDirectory(CString strLocalFilePath, CString strServerDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=0;
	if((strServerDirectoryPath!="")&&(strLocalFilePath!=""))
	{
		//判断本地文件是否存在
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalFilePath);
		if(!a)
		{
			//AfxMessageBox("需要保存的文件不存在或者路径不正确！");
			returnvalue=TRUE;
		}
		else
		{
			//初始化FTP连接
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			CFtpFileFind* pFileFind;
			pConnection=NULL;
			pFileFind=NULL;
			//建立连接
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
			//建立连接完成
			if(pConnection!=NULL)
			{
				pFileFind=new CFtpFileFind(pConnection);
				BOOL b=pFileFind->FindFile(strServerDirectoryPath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
				//查找指定的目录是否存在
				if(!b)
				{
					//AfxMessageBox("保存文件的目录不存在！");
					pConnection->CreateDirectory(strServerDirectoryPath);
					//pFileFind->Close();
					//pFileFind=NULL;
				}
				//else
				//{
					//保存同名文件操作
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
						//AfxMessageBox("文件保存成功！");
						returnvalue=1;
					}
				//}
				//断开FTP连接
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
				//AfxMessageBox("连接服务器失败！");
			}
		}
	}
	else
	{
		//AfxMessageBox("输入不能为空！");
	}
	return returnvalue;
}
/************************************************************************/
/* 判断服务器上这个文件是否存在1.文件名.prt/.asm/.drw/,2.服务器路径,3.备份的地址，如果存在则下载备份     */
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
			//初始化FTP连接
		CInternetSession* pSession;
		CFtpConnection* pConnection;
		CFtpFileFind* pFileFind;
		pConnection=NULL;
		pFileFind=NULL;
			//建立连接
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
			//建立连接完成
		if(pConnection!=NULL)
		{
			pFileFind=new CFtpFileFind(pConnection);
			BOOL b=pFileFind->FindFile(strServerFilePath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
			//查找指定的目录是否存在
			if(!b)
			{
				returnvalue=TRUE;
			}
			else
			{
				//CString str;
				//str.Format("服务器上%s这个文件已经存在，是否替换！",FileName);
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
			//断开FTP连接
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
/* 删除单个文件                                                             */
/************************************************************************/
BOOL CFtpFile::RemoveFile(CString strServerFilePath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=FALSE;
	if(strServerFilePath!="")
	{
		//初始化FTP连接
		CInternetSession* pSession;
		CFtpConnection* pConnection;
		CFtpFileFind* pFileFind;
		pConnection=NULL;
		pFileFind=NULL;
		//建立连接
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
		//建立连接完成
		if(pConnection!=NULL)
		{
			pFileFind=new CFtpFileFind(pConnection);
			BOOL b=pFileFind->FindFile(strServerFilePath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
			//查找指定的目录是否存在
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
			//断开FTP连接
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
// 上传本地目录所有文件到服务器指定目录
BOOL CFtpFile::UploadDirectorytoDirectory (CString strLocalDirectoryPath, CString strServerDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=0;
	if((strLocalDirectoryPath!="")&&(strServerDirectoryPath!=""))
	{
		//查找本地目录
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(!a)
		{
			AfxMessageBox("需要保存的目录不存在或者路径不正确！");
		}
		//查找服务器端目录
		else
		{
			CInternetSession *pSession;
			CFtpConnection* pConnection;
			CFtpFileFind* pFileFind;
			pConnection=NULL;
			pFileFind=NULL;
			//初始化
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
				//初始化完成
				pFileFind= new CFtpFileFind(pConnection);
				//CFtpFileFind pFileFind(pConnection);

				//查找服务器端目录
				pConnection->SetCurrentDirectory("\\");
				BOOL b=pFileFind->FindFile(strServerDirectoryPath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
				if(!b)
				{
					pConnection->CreateDirectory(strServerDirectoryPath);
				}
				//else
				//{
					//遍历本地目录
					CString szDir=strLocalDirectoryPath;
					if(szDir.Right(1)!="/")
						szDir+="/";
					szDir+="*.*";
					b=ff.FindFile(szDir);
					while(b)
					{
						//上传文件处理
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
				//关闭连接
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
				AfxMessageBox("连接服务器失败！");
			}
		}
	}
	else
	{
		AfxMessageBox("输入不能为空！");
	}
	return returnvalue;
}
// 上传本地目录及其所有文件到服务器指定目录
BOOL CFtpFile::UploadDirectorytoDirectory2(CString strLocalDirectoryPath, CString strServerDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=0;
	if((strServerDirectoryPath!="")&&(strLocalDirectoryPath!=""))
	{
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(a)
		{
			//查找服务器端目录
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			CFtpFileFind* pFileFind;
			pConnection=NULL;
			pFileFind=NULL;
			//初始化
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
				//查找服务器目录是否存在
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

// 下载文件
BOOL CFtpFile::DownloadFiletoFile(CString strServerFilePath, CString strLocalFilePath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=0;
	if((strLocalFilePath!="")&&(strServerFilePath!=""))
	{
		//初始化FTP连接
		CInternetSession* pSession;
		CFtpConnection* pConnection;
		CFtpFileFind* pFileFind;
		pConnection=NULL;
		pFileFind=NULL;
		//建立连接
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
		//建立连接完成
		if(pConnection!=NULL)
		{
			pFileFind=new CFtpFileFind(pConnection);
			strServerFilePath.MakeLower();
			BOOL b=pFileFind->FindFile(strServerFilePath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
			//查找指定的目录是否存在
			if(!b)
			{
				//AfxMessageBox("需要下载的文件不存在！");
				pFileFind->Close();
				pFileFind=NULL;
			}
			else
			{
				BOOL b=pConnection->GetFile(strServerFilePath,strLocalFilePath,FALSE);
				if(b)
				{
					//AfxMessageBox("文件下载成功！");
					returnvalue=1;
				}
			}
			//断开FTP连接
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
			//AfxMessageBox("连接服务器失败！");
		}
	}
	else
	{
		//AfxMessageBox("输入不能为空！");	
	}
	return returnvalue;
}

// 下载文件到本地指定目录创建同名文件
BOOL CFtpFile::DownloadFiletoDirectory(CString strServerFilePath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	//strServerFilePath.Replace(" ","#");
	BOOL returnvalue=FALSE;
	if((strServerFilePath!="")&&(strLocalDirectoryPath!=""))
	{
		//判断本地目录是否存在
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
		//初始化FTP连接
		//else
		//{
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			CFtpFileFind* pFileFind;
			pConnection=NULL;
			pFileFind=NULL;
			//建立连接
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
			//建立连接完成
			if((pConnection!=NULL)&&(strServerFilePath!=""))
			{
				pFileFind=new CFtpFileFind(pConnection);
				strServerFilePath.MakeLower();
				BOOL b=pFileFind->FindFile(strServerFilePath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
				//查找指定的文件是否存在
				if(!b)
				{
					//AfxMessageBox("文件不存在！");
					pFileFind->Close();
					pFileFind=NULL;
				}
				else
				{
					//保存同名文件操作
					CString strName;
					int numStart=strServerFilePath.ReverseFind('/');
					if(numStart!=-1)
						strName=strServerFilePath.Right(strServerFilePath.GetLength()-numStart-1).GetString();
					CString strSaveName;
					//应该判断要下载的文件是否已经存在,如果已存在,则删除;
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
						//AfxMessageBox("文件保存成功！");
						returnvalue=TRUE;
					}
				}
				//断开FTP连接
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
				//AfxMessageBox("连接服务器失败！");
			}
	}

	else
	{
		//AfxMessageBox("输入不能为空！");
		return FALSE;
	}
	return returnvalue;
}

// 下载服务器指定目录所有文件到本地指定目录
BOOL CFtpFile::DownloadDirectorytoDirectory(CString strServerDirectoryPath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=FALSE;
	if((strServerDirectoryPath!="")&&(strLocalDirectoryPath!=""))
	{
		//查找本地目录
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(!a)
		{
			//AfxMessageBox("指定保存目录不存在！");
			return FALSE;
		}
		else
		{
			//查找服务器端目录
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			CFtpFileFind* pFileFind;
			pConnection=NULL;
			pFileFind=NULL;
			//初始化
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

				//查找目录
				pConnection->SetCurrentDirectory("\\");
				strServerDirectoryPath.MakeLower();
				BOOL b=pFileFind->FindFile(strServerDirectoryPath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
				if(!b)
				{
					//AfxMessageBox("指定下载目录不存在！");
					return FALSE;
				}
				//遍历服务器目录
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
						//查找本地是否已经有这个文件夹,如果有,则删除
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
				//关闭连接
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
				//AfxMessageBox("连接服务器失败！");
				return FALSE;
			}
		}
	}
	else
	{
		//AfxMessageBox("输入不能为空！");
		return FALSE;
	}
	return returnvalue;
}

// 下载服务器指定目录及其所有文件到本地指定目录
BOOL CFtpFile::DownloadDirectorytoDirectory2(CString strServerDirectoryPath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=FALSE;
	if((strServerDirectoryPath!="")&&(strLocalDirectoryPath!=""))
	{
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(a)
		{
			//查找服务器端目录
			CInternetSession* pSession;
			CFtpConnection* pConnection;
			CFtpFileFind* pFileFind;
			pConnection=NULL;
			pFileFind=NULL;
			//初始化
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
//下载一组文件到本地目录
BOOL CFtpFile::DownloadFilestoDirectory(CStringArray * strServerFilePath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{

	CString str,str1;
	BOOL    returnvalue=0;
	TCHAR  * lpError=NULL;
	BOOL   IsHave=TRUE;
	if((!strServerFilePath->IsEmpty())&&(strLocalDirectoryPath!=""))
	{
		//判断本地目录是否存在
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(!a)
		{
			if (strLocalDirectoryPath.Right(1)!=":")
			{
				if(::CreateDirectory(strLocalDirectoryPath,NULL)==0)
					return FALSE;
			}
			//str.Format("本地目录%s并不存在,无法下载到本地目录",strLocalDirectoryPath.GetBuffer());
			//AfxMessageBox(str);
			//return FALSE;
		}
		//初始化FTP连接
		//else
		//{
		CInternetSession* pSession;
		CFtpConnection* pConnection;
		CFtpFileFind* pFileFind;
		pConnection=NULL;
		pFileFind=NULL;
		//建立连接
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
		//建立连接完成
		if((pConnection!=NULL)&&(strServerFilePath->GetCount()>0))
		{
			pFileFind=new CFtpFileFind(pConnection);
			//首先查找一下文件是否都存在
			for (int i=0;i<strServerFilePath->GetCount();i++)
			{
				str1=strServerFilePath->GetAt(i);
				BOOL b=pFileFind->FindFile(str1.MakeLower(),INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
				if (!b)
				{
					IsHave=FALSE;
					str.Format("%s在服务器中无法找到文件%s\n",str,strServerFilePath->GetAt(i));
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
			//断开连接
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
/* localdir格式:g:\abc\bcd                                              */
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
		if(ff.IsDirectory() && !ff.IsDots())       //目录是文件夹
		{
			//如果是一个子目录，用递归继续往深一层找
			CString strPath = ff.GetFilePath();    //得到路径，做为递归调用的开始			         	
			RemoveDireAndFile(strPath);            //递归调用
		}
		else if(!ff.IsDirectory() && !ff.IsDots()) //到达最低层的文件
		{
			//显示当前访问的文件
			CString strPath="";			
			strPath = ff.GetFilePath();
			DeleteFile(strPath);
		}
	}	
	ff.Close();                                   //关闭
	RemoveDirectory(_T(localdir+"\\"));	
	return 1;
}
/************************************************************************/
/* 删除保存失败的目录                                                   */
/************************************************************************/
BOOL CFtpFile::DeleteDirAndFile(CString strServerDirectoryPath,CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort)
{
	BOOL returnvalue=0;
	if(strServerDirectoryPath!="")
	{
		//查找服务器端目录
		CInternetSession* pSession;
		CFtpConnection* pConnection;
		CFtpFileFind* pFileFind;
		pConnection=NULL;
		pFileFind=NULL;
		//初始化
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
			//查找目录
			pConnection->SetCurrentDirectory("\\");
			BOOL b=pFileFind->FindFile(strServerDirectoryPath,INTERNET_FLAG_SECURE|INTERNET_FLAG_RELOAD);
			if(!b)
			{
				returnvalue=1;
			}
			//遍历服务器目录
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
			//关闭连接
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
			AfxMessageBox("连接服务器失败！");	
			returnvalue=0;
		}

	}
	return returnvalue;
}
/************************************************************************/
/* 根据下载到的目录找文件                                               */
/************************************************************************/
CString CFtpFile::FindFile(CString str,CString str1,CString str2)   //1.本地目录地址，2.服务器目录地址 3.零件名
{
	CFileFind finder;
	CString strname,strpath;
	int i;
	if(str1.Right(1)=="/")
	{
		//去掉/
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


