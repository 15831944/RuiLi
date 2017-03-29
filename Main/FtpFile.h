#pragma once



// CFtpFile 命令目标

class CFtpFile : public CObject
{
public:
	CFtpFile();
	virtual ~CFtpFile();
	// 上传文件
	BOOL UploadFiletoFile(CString strLocalFilePath, CString strServerFilePath, FtpConnectInfo info);
	// 上传文件到服务器指定目录
	BOOL UploadFiletoDirectory(CString strLocalFilePath, CString strServerDirectoryPath, FtpConnectInfo info);
	// 上传本地目录所有文件到服务器指定目录
	BOOL UploadDirectorytoDirectory(CString strLocalDirectoryPath, CString strServerDirectoryPath, FtpConnectInfo info);
	// 上传本地目录及其所有文件到服务器指定目录
	BOOL UploadDirectorytoDirectory2(CString strLocalDirectoryPath, CString strServerDirectoryPath, FtpConnectInfo info);
	// 下载文件
	BOOL DownloadFiletoFile(CString strServerFilePath, CString strLocalFilePath,FtpConnectInfo info);
	// 下载文件到本地指定目录创建同名文件
	BOOL DownloadFiletoDirectory(CString strServerFilePath, CString strLocalDirectoryPath, FtpConnectInfo info);
	// 下载一组文件到本地目录，创建同名文件
	BOOL DownloadFilestoDirectory(CStringArray * strServerFilePath, CString strLocalDirectoryPath, FtpConnectInfo info,CString & strError);
	// 下载服务器指定目录所有文件到本地指定目录
	BOOL DownloadDirectorytoDirectory(CString strServerDirectoryPath, CString strLocalDirectoryPath, FtpConnectInfo info);
	// 下载服务器指定目录及其所有文件到本地指定目录
	BOOL DownloadDirectorytoDirectory2(CString strServerDirectoryPath, CString strLocalDirectoryPath,FtpConnectInfo info);
	BOOL CreateDirectory(FtpConnectInfo info, CString strDirName);
	BOOL RemoveDirectory(FtpConnectInfo info, CString strDirName);
	BOOL RemoveFile(FtpConnectInfo info, CString strDirName);
	BOOL GetCurrentDirectory(FtpConnectInfo info, CString  * currentdirectory);
	BOOL DeleteDirAndFile(CString strServerDirectoryPath/*, CString strLocalDirectoryPath,*/  ,FtpConnectInfo info);
	bool TestConnect(FtpConnectInfo info);
	BOOL DownloadPartFileToDirectory(CString serverFileName, CString serverDirectory, CString LocalDirectory, FtpConnectInfo info);
	BOOL Rename(CString serverDirectory,CString newName, FtpConnectInfo info);
	BOOL CreateDirectorys(FtpConnectInfo info,CStringArray & strDirAry);
	void ShowErrorMessage(CInternetException* e);
};