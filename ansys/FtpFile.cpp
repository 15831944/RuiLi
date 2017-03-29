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

BOOL CFtpFile::UploadFiletoFile(CString strLocalFilePath, CString strServerFilePath, FtpConnectInfo info)
{
	
	CString strFtp=info.strFtp;
    CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
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
BOOL CFtpFile::UploadFiletoDirectory(CString strLocalFilePath, CString strServerDirectoryPath,FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	if((strServerDirectoryPath!="")&&(strLocalFilePath!=""))
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
				BOOL b=pFileFind->FindFile(strServerDirectoryPath);
				//查找指定的目录是否存在
				if(!b)
				{
					//AfxMessageBox("保存文件的目录不存在！");
					pFileFind->Close();
					pFileFind=NULL;
				}
				else
				{
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
	}
	else
	{
		//AfxMessageBox("输入不能为空！");
	}
	return returnvalue;
}

// 上传本地目录所有文件到服务器指定目录
BOOL CFtpFile::UploadDirectorytoDirectory(CString strLocalDirectoryPath, CString strServerDirectoryPath, FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
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
				//初始化完成
				pFileFind= new CFtpFileFind(pConnection);
				//查找服务器端目录
				pConnection->SetCurrentDirectory("\\");
				BOOL b=pFileFind->FindFile(strServerDirectoryPath);
				if(!b)
				{
					AfxMessageBox("服务器目录不存在或者路径错误！");
				}
				else
				{
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
							//AfxMessageBox(strLocalName);
							//AfxMessageBox(strSaveName);
							UploadDirectorytoDirectory(strLocalName,strSaveName,info);
						}
						returnvalue=1;
					}
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

// 下载文件
BOOL CFtpFile::DownloadFiletoFile(CString strServerFilePath, CString strLocalFilePath,  FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
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
			BOOL b=pFileFind->FindFile(strServerFilePath);
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
BOOL CFtpFile::DownloadFiletoDirectory(CString strServerFilePath, CString strLocalDirectoryPath, FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	if((strServerFilePath!="")&&(strLocalDirectoryPath!=""))
	{
		//判断本地目录是否存在
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(!a)
		{
			//AfxMessageBox("需要保存文件的目录不存在或者路径错误！");
		}
		//初始化FTP连接
		else
		{
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
				BOOL b=pFileFind->FindFile(strServerFilePath);
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
					if(strLocalDirectoryPath.Right(1)!="\\")
						strSaveName=strLocalDirectoryPath+"\\"+strName;
					else
						strSaveName=strLocalDirectoryPath+strName;
					//AfxMessageBox(strServerFilePath);
					//AfxMessageBox(strName);
					BOOL c=pConnection->GetFile(strServerFilePath,strSaveName);
					if(c)
					{
						//AfxMessageBox("文件保存成功！");
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
	}
	else
	{
		//AfxMessageBox("输入不能为空！");
	}
	return returnvalue;
}

// 下载服务器指定目录所有文件到本地指定目录
BOOL CFtpFile::DownloadDirectorytoDirectory(CString strServerDirectoryPath, CString strLocalDirectoryPath, FtpConnectInfo info)
{
	CString strFtp=info.strFtp;
	CString strFtpName=info.strFtpUser;
	CString strFtpPwd=info.strFtpPwd;
	int iFtpPort=info.iFtpPort;
	BOOL returnvalue=0;
	if((strServerDirectoryPath!="")&&(strLocalDirectoryPath!=""))
	{
		//查找本地目录
		CFileFind ff;
		BOOL a=ff.FindFile(strLocalDirectoryPath);
		if(!a)
		{
			//AfxMessageBox("指定保存目录不存在！");
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
				BOOL b=pFileFind->FindFile(strServerDirectoryPath);
				if(!b)
				{
					//AfxMessageBox("指定下载目录不存在！");
				}
				//遍历服务器目录
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
			}
		}
	}
	else
	{
		//AfxMessageBox("输入不能为空！");
	}
	return returnvalue;
}

// 下载服务器指定目录及其所有文件到本地指定目录
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
		pConnection=pSession->GetFtpConnection(strFtp,strFtpUser,strFtpPwd,iFtpPort);
	}
	catch(CInternetException* e)
	{
		e->Delete();
		pConnection=NULL;
	}
	if(pConnection!=NULL)
	{
		//AfxMessageBox("连接成功！");
		BOOL b=pConnection->CreateDirectory(strDirName);
		if(b)
			returnvalue=1;
	}
	else
	{
		AfxMessageBox("连接数据库失败！");
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
		//AfxMessageBox("连接成功！");
		BOOL b=pConnection->RemoveDirectory(strDirName);
		if(b)
			returnvalue=1;
	}
	else
	{
		AfxMessageBox("连接数据库失败！");
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
		//AfxMessageBox("连接成功！");
		BOOL b=pConnection->Remove(strDirName);
		if(b)
			returnvalue=1;
	}
	else
	{
		AfxMessageBox("连接数据库失败！");
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
		//AfxMessageBox("连接成功！");
		BOOL b=pConnection->GetCurrentDirectory((*currentdirectory));
		if(b)
			returnvalue=1;
	}
	else
	{
		AfxMessageBox("连接数据库失败！");
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
/* 删除服务器上指定目录下的所有内容                                     */
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
				BOOL b=pFileFind->FindFile(strServerDirectoryPath);
				if(!b)
				{
					//AfxMessageBox("指定下载目录不存在！");
					returnvalue=1;
				}
				//遍历服务器目录
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
/* 测试到服务器的连接                                                   */
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
	//连接成功
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
	//连接成功
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
