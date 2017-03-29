bool UsrGetDBAndFtpInfo(CString & strFtp,CString & strFtpUser,CString & strFtpPwd,int & iFtpPort,CString & vdLinkInfo,CString & pdmLinkInfo)
{
	HKEY      hKey;
	struct    HKEY__*ReRootKey;
	TCHAR     *ReSubKey;//*ReExitProKey,*RegSubKeyToCreate;
	TCHAR     *ReValueName_address,*ReValueName_user,*ReValueName_pwd,*ReValueName_port;
	TCHAR     *ReValueName_DB_NAME,*ReValueName_DB_USER,*ReValueName_DB_PWD;
	TCHAR     *ReValueName_SHUJU;
	char      content_add[256],content_user[256],content_pwd[256];
	char      content_dsn_user[256],content_dsn_pwd[256],content_dsn_name[256];
	char      content_shujuyuan[256];
	int       content_port[256];
	DWORD     dwType;		//定义读取数据类型 REG_DWORD
	ReRootKey=HKEY_CURRENT_USER;              //注册表主键名称
	ReSubKey="Software\\PTC\\VariantDesign";  //欲打开注册表值的地址
	ReValueName_address="ftp_address";               //欲设置值的名称
	ReValueName_user="ftp_user";
	ReValueName_pwd="ftp_pwd";
	ReValueName_port="ftp_port";
	ReValueName_DB_USER="dsn_user";
	ReValueName_DB_PWD="dsn_pwd";
	ReValueName_DB_NAME="dsn";
	ReValueName_SHUJU="SHUJUYUAN";
	strFtp.Empty();
	strFtpPwd.Empty();
	strFtpUser.Empty();
	iFtpPort=0;
	vdLinkInfo.Empty();
	pdmLinkInfo.Empty();
	DWORD dwLength1,dwLength2,dwLength3,dwLength4,dwLength5,dwLength6,dwLength7,dwLength8;
	try
	{
		if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_READ,&hKey)==ERROR_SUCCESS)
		{
			if(RegQueryValueEx(hKey,ReValueName_address,NULL,&dwType,(unsigned char *)content_add,&dwLength1)!=ERROR_SUCCESS) content_add[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_user,NULL,&dwType,(unsigned char *)content_user,&dwLength2)!=ERROR_SUCCESS) content_user[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_pwd,NULL,&dwType,(unsigned char *)content_pwd,&dwLength3)!=ERROR_SUCCESS) content_pwd[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_port,NULL,&dwType,(unsigned char *)content_port,&dwLength4)!=ERROR_SUCCESS) content_port[0]=0;
			if(RegQueryValueEx(hKey,ReValueName_DB_USER,NULL,&dwType,(unsigned char *)content_dsn_user,&dwLength5)!=ERROR_SUCCESS) content_dsn_user[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_DB_NAME,NULL,&dwType,(unsigned char *)content_dsn_name,&dwLength6)!=ERROR_SUCCESS) content_dsn_name[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_DB_PWD,NULL,&dwType,(unsigned char *)content_dsn_pwd,&dwLength7)!=ERROR_SUCCESS) content_dsn_pwd[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_SHUJU,NULL,&dwType,(unsigned char *)content_shujuyuan,&dwLength8)!=ERROR_SUCCESS) content_shujuyuan[0]='\0';
			RegCloseKey(hKey);
			strFtp=CString(content_add);
			strFtpUser=CString(content_user);         
			strFtpPwd=CString(content_pwd);
			iFtpPort=content_port[0];
			vdLinkInfo=_T("DSN="+CString(content_shujuyuan)+";UID="+CString(content_dsn_user)+";PWD="+CString(content_dsn_pwd)+";DATABASE=RuiLiMdlDb;");
			pdmLinkInfo=_T("DSN=xtpdm"+";UID="+CString(content_dsn_user)+";PWD="+CString(content_dsn_pwd)+";");
		}
		else
		{
			HKEY hk;
			DWORD disposition;   
			if (RegCreateKeyEx(ReRootKey,ReSubKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hk,&disposition))
			{
				AfxMessageBox("注册表注册失败,请进行手工设置");
				return false;;
			}
			RegCloseKey(hk);
		}

	}
	catch (...) {
		AfxMessageBox("从注册表读取数据失败");
		return false;
	}
	return true;
}
