#pragma once




// CFtpFile 命令目标

class CFtpFile : public CObject
{
public:
	CFtpFile();
	virtual ~CFtpFile();
	// 上传文件
	BOOL UploadFiletoFile(CString strLocalFilePath, CString strServerFilePath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// 上传文件到服务器指定目录
	BOOL UploadFiletoDirectory(CString strLocalFilePath, CString strServerDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// 上传本地目录所有文件到服务器指定目录
	BOOL UploadDirectorytoDirectory(CString strLocalDirectoryPath, CString strServerDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// 上传本地目录及其所有文件到服务器指定目录
	BOOL UploadDirectorytoDirectory2(CString strLocalDirectoryPath, CString strServerDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// 下载文件
	BOOL DownloadFiletoFile(CString strServerFilePath, CString strLocalFilePath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// 下载文件到本地指定目录创建同名文件
	BOOL DownloadFiletoDirectory(CString strServerFilePath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// 下载服务器指定目录所有文件到本地指定目录
	BOOL DownloadDirectorytoDirectory(CString strServerDirectoryPath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// 下载服务器指定目录及其所有文件到本地指定目录
	BOOL DownloadDirectorytoDirectory2(CString strServerDirectoryPath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// 查找本地是否已经有这个目录
	//BOOL FineDirectory(CString LocalPath);
	//删除文件夹极其内部文件
	BOOL RemoveDireAndFile(CString localdir);
	//删除服务器上保存失败的目录
	BOOL DeleteDirAndFile(CString strServerDirectoryPath,CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);

	CString FindFile(CString str,CString str1,CString str2);

	BOOL FindFile(CString FileName,CString strServerDirectoryPath, CString BackupPath,CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);

    BOOL RemoveFile(CString strServerFilePath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);//	删除单个文件

	//下载一组文件到本地目录
	BOOL DownloadFilestoDirectory(CStringArray * strServerFilePath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
};