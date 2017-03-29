#pragma once



// CFtpFile ����Ŀ��

class CFtpFile : public CObject
{
public:
	CFtpFile();
	virtual ~CFtpFile();
	// �ϴ��ļ�
	BOOL UploadFiletoFile(CString strLocalFilePath, CString strServerFilePath, FtpConnectInfo info);
	// �ϴ��ļ���������ָ��Ŀ¼
	BOOL UploadFiletoDirectory(CString strLocalFilePath, CString strServerDirectoryPath, FtpConnectInfo info);
	// �ϴ�����Ŀ¼�����ļ���������ָ��Ŀ¼
	BOOL UploadDirectorytoDirectory(CString strLocalDirectoryPath, CString strServerDirectoryPath, FtpConnectInfo info);
	// �ϴ�����Ŀ¼���������ļ���������ָ��Ŀ¼
	BOOL UploadDirectorytoDirectory2(CString strLocalDirectoryPath, CString strServerDirectoryPath, FtpConnectInfo info);
	// �����ļ�
	BOOL DownloadFiletoFile(CString strServerFilePath, CString strLocalFilePath,FtpConnectInfo info);
	// �����ļ�������ָ��Ŀ¼����ͬ���ļ�
	BOOL DownloadFiletoDirectory(CString strServerFilePath, CString strLocalDirectoryPath, FtpConnectInfo info);
	// ����һ���ļ�������Ŀ¼������ͬ���ļ�
	BOOL DownloadFilestoDirectory(CStringArray * strServerFilePath, CString strLocalDirectoryPath, FtpConnectInfo info,CString & strError);
	// ���ط�����ָ��Ŀ¼�����ļ�������ָ��Ŀ¼
	BOOL DownloadDirectorytoDirectory(CString strServerDirectoryPath, CString strLocalDirectoryPath, FtpConnectInfo info);
	// ���ط�����ָ��Ŀ¼���������ļ�������ָ��Ŀ¼
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