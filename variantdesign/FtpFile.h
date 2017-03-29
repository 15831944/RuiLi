#pragma once




// CFtpFile ����Ŀ��

class CFtpFile : public CObject
{
public:
	CFtpFile();
	virtual ~CFtpFile();
	// �ϴ��ļ�
	BOOL UploadFiletoFile(CString strLocalFilePath, CString strServerFilePath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// �ϴ��ļ���������ָ��Ŀ¼
	BOOL UploadFiletoDirectory(CString strLocalFilePath, CString strServerDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// �ϴ�����Ŀ¼�����ļ���������ָ��Ŀ¼
	BOOL UploadDirectorytoDirectory(CString strLocalDirectoryPath, CString strServerDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// �ϴ�����Ŀ¼���������ļ���������ָ��Ŀ¼
	BOOL UploadDirectorytoDirectory2(CString strLocalDirectoryPath, CString strServerDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// �����ļ�
	BOOL DownloadFiletoFile(CString strServerFilePath, CString strLocalFilePath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// �����ļ�������ָ��Ŀ¼����ͬ���ļ�
	BOOL DownloadFiletoDirectory(CString strServerFilePath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// ���ط�����ָ��Ŀ¼�����ļ�������ָ��Ŀ¼
	BOOL DownloadDirectorytoDirectory(CString strServerDirectoryPath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// ���ط�����ָ��Ŀ¼���������ļ�������ָ��Ŀ¼
	BOOL DownloadDirectorytoDirectory2(CString strServerDirectoryPath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
	// ���ұ����Ƿ��Ѿ������Ŀ¼
	//BOOL FineDirectory(CString LocalPath);
	//ɾ���ļ��м����ڲ��ļ�
	BOOL RemoveDireAndFile(CString localdir);
	//ɾ���������ϱ���ʧ�ܵ�Ŀ¼
	BOOL DeleteDirAndFile(CString strServerDirectoryPath,CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);

	CString FindFile(CString str,CString str1,CString str2);

	BOOL FindFile(CString FileName,CString strServerDirectoryPath, CString BackupPath,CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);

    BOOL RemoveFile(CString strServerFilePath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);//	ɾ�������ļ�

	//����һ���ļ�������Ŀ¼
	BOOL DownloadFilestoDirectory(CStringArray * strServerFilePath, CString strLocalDirectoryPath, CString strFtp, CString strFtpName, CString strFtpPwd, int iFtpPort);
};