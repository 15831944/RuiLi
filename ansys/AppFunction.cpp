#include "stdafx.h"
#include "myproe.h"
#include "_Application.h"
#include "_Workbook.h"
#include "_Worksheet.h"
#include "Shape.h"
#include "Shapes.h"
#include "Workbooks.h"
#include "Worksheets.h"
#include "Range.h"

#include "MyDatabase.h"

typedef struct
{
	CString index;
	CString code;//ͼ��
	CString pdmcode;//pdmͼ��
	CString name;//����
	CString qty;//����
	CString material;//����
	CString picname;//ͼƬ����
	CString picpath;//ͼƬ·��
	CString note;//��ע
	//����� 2007��09��13
	CString gg;//���
	CString jmlc;//��������
}AsmDrawingInfo;
//װ��ģ����Ϣ
typedef struct 
{
	CString asmcode;//װ��ͼͼ��
	CString asmname;//װ��ͼ����
	CString pdmcode;//װ��ͼpdmͼ��
	CString sheji;//�����Ա
	CString shenhe;//�����Ա
	CString shenpi;//������Ա
	CString companyname;//��˾����
}AsmInfo;


//����ȫ�ֵı���
CArray<AsmDrawingInfo,AsmDrawingInfo> GLS_DrwInfo;
AsmDrawingInfo drwinfo;
AsmInfo GL_AsmInfo;
CString gl_vdsDBLinkInfo;
CMyDatabase gl_db;
extern _RecordsetPtr	m_pRecordset;
extern _ConnectionPtr  m_pConnection;

BOOL GetParam(ProModelitem * theOwner,CString strName,CString * strValue);

BOOL GetDBInfo(CString & vdLinkInfo)
{
	HKEY      hKey;
	struct    HKEY__*ReRootKey;
	TCHAR     *ReSubKey;//*ReExitProKey,*RegSubKeyToCreate;
	TCHAR     *ReValueName_DB_NAME,*ReValueName_DB_USER,*ReValueName_DB_PWD;
	TCHAR     *ReValueName_SHUJU;
	char      content_dsn_user[256],content_dsn_pwd[256],content_dsn_name[256];
	char      content_shujuyuan[256];
	DWORD     dwType;		//�����ȡ�������� REG_DWORD
	ReRootKey=HKEY_CURRENT_USER;              //ע�����������
	ReSubKey="Software\\PTC\\VariantDesign";  //����ע���ֵ�ĵ�ַ
	ReValueName_DB_USER="dsn_user";
	ReValueName_DB_PWD="dsn_pwd";
	ReValueName_DB_NAME="dsn";
	ReValueName_SHUJU="SHUJUYUAN";
	vdLinkInfo.Empty();
	DWORD dwLength5,dwLength6,dwLength7,dwLength8;
	try
	{
		if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_READ,&hKey)==ERROR_SUCCESS)
		{
			if(RegQueryValueEx(hKey,ReValueName_DB_USER,NULL,&dwType,(unsigned char *)content_dsn_user,&dwLength5)!=ERROR_SUCCESS) content_dsn_user[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_DB_NAME,NULL,&dwType,(unsigned char *)content_dsn_name,&dwLength6)!=ERROR_SUCCESS) content_dsn_name[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_DB_PWD,NULL,&dwType,(unsigned char *)content_dsn_pwd,&dwLength7)!=ERROR_SUCCESS) content_dsn_pwd[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_SHUJU,NULL,&dwType,(unsigned char *)content_shujuyuan,&dwLength8)!=ERROR_SUCCESS) content_shujuyuan[0]='\0';

			RegCloseKey(hKey);
			vdLinkInfo=_T("DSN="+CString(content_shujuyuan)+";UID="+CString(content_dsn_user)+";PWD="+CString(content_dsn_pwd)+";DATABASE=RuiLiMdlDb;");
		}	 
		else
		{
			HKEY hk;
			DWORD disposition;   
			if (RegCreateKeyEx(ReRootKey,ReSubKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hk,&disposition))
			{
				AfxMessageBox("ע���ע��ʧ��,������ֹ�����");
				return false;;
			}
			RegCloseKey(hk);
		}

	}
	catch (...) {
		AfxMessageBox("��ע����ȡ����ʧ��");
		return false;
	}
	return true;
}

void OutputMainfunction()
{
	//��鹤��ģʽ
	ProError status;
	ProMode  mode;
	ProMdl curMdl;
	ProMdldata curData;
	CString strCode,strName;
	CString str;
	//ProName curName;
	ProCharName curCode;
	ProModelitem curModelitem;
	//ProParameter para;
	//ProParamvalue value;
	status=ProModeCurrentGet(&mode);
	if(status==PRO_TK_NO_ERROR)
	{
		//if(mode==PRO_MODE_DRAWING)
		//{
			//ProMdl curMdl;
			//status=ProMdlCurrentGet(&curMdl);
			//ProMdldata curData;
			//status=ProMdlDataGet(curMdl,&curData);
			//ProCharName str;
			//ProWstringToString(str,curData.name);
			//AfxMessageBox(str);

			//
			//ProModelitem curModelitem;
			//ProMdlToModelitem(curMdl,&curModelitem);
			//
			//ProParameter para;
			//ProName name;
			//CString strname="rl_name";
			//ProStringToWstring(name,strname.GetBuffer());
			//ProParameterInit(&curModelitem,name,&para);

			//ProParamvalue value;
			//CString strValue="";
			//ProParameterValueGet(&para,&value);
			//strValue=CString(para.id);
			//switch(value.type) 
			//{
			//case PRO_PARAM_DOUBLE:
			//	strValue.Format("%0.2f",value.value.d_val);
			//	break;
			//case PRO_PARAM_STRING:
			//	strValue=CString(value.value.s_val);
			//	break;
			//case PRO_PARAM_INTEGER:
			//	strValue.Format("%d",value.value.i_val);
			//	break;
			//case PRO_PARAM_BOOLEAN:
			//	strValue.Format("%d",value.value.l_val);
			//	break;
			//case PRO_PARAM_NOTE_ID:
			//	break;
			//case PRO_PARAM_VOID:
			//	break;
			//default:
			//	break;
			//}
			//AfxMessageBox(strValue);
		//}

		if(mode==PRO_MODE_DRAWING)
		{
			status=ProMdlCurrentGet(&curMdl);
			if(status!=PRO_TK_NO_ERROR)
			{
				AfxMessageBox("��ȡװ��ģ��ʧ�ܣ�������άװ��ģ���Ƿ���ڣ�");
				return;
			}
			//��ȡװ��ͼ��
			ProSolid curSolid;
			ProDrawingCurrentsolidGet((ProDrawing)curMdl,&curSolid);
			//�õ���άͼ����άģ��
			status=ProMdlDataGet(curSolid,&curData);
			ProWstringToString(curCode,curData.name);
			strCode=curCode;
			GL_AsmInfo.asmcode=strCode;//��ȡͼ�ţ�ͼ�ż����ļ���
			//��ȡװ������
			status=ProMdlToModelitem(curSolid,&curModelitem);
			//----------------------�����ǰ��ȡ�����ĳ����------���ڻ�ȡװ�����
			CString strtemp;
			strtemp="";
			GetParam(&curModelitem,"CZCTH",&strtemp);//��ȡpdmͼ��
			GL_AsmInfo.pdmcode=strtemp;
			strtemp="";
			GetParam(&curModelitem,"CNAME",&strtemp);//��ȡ����
			GL_AsmInfo.asmname=strtemp;
			strtemp="";
			GetParam(&curModelitem,"SJR",&strtemp);//��ȡ�����Ա
			GL_AsmInfo.sheji=strtemp;
			strtemp="";
			GetParam(&curModelitem,"CSHR",&strtemp);//��ȡ���
			GL_AsmInfo.shenhe=strtemp;
			strtemp="";
			GetParam(&curModelitem,"CSPR",&strtemp);//��ȡ����
			GL_AsmInfo.shenpi=strtemp;
			strtemp="";
			GetParam(&curModelitem,"CGSMC",&strtemp);//��ȡ��˾����
			GL_AsmInfo.companyname=strtemp;
			strtemp="";
			
			/*******��ȡ���Ƶ�ԭ�г���****2007��9�����ط��޶�
			str="rl_name";
			ProStringToWstring(curName,str.GetBuffer());
			ProParameterInit(&curModelitem,curName,&para);
			status=ProParameterValueGet(&para,&value);
			if(status!=PRO_TK_NO_ERROR)
			{
				AfxMessageBox("��û�н���װ��ģ�����Ʋ���rl_name�������޷���ȡ��Ʒ���ƣ�");
				GL_AsmInfo.asmname="";
			}
			if(status==PRO_TK_NO_ERROR)
			{
				switch(value.type) 
				{
				case PRO_PARAM_DOUBLE:
					strName.Format("%0.2f",value.value.d_val);
					break;
				case PRO_PARAM_STRING:
					strName=CString(value.value.s_val);
					break;
				case PRO_PARAM_INTEGER:
					strName.Format("%d",value.value.i_val);
					break;
				case PRO_PARAM_BOOLEAN:
					strName.Format("%d",value.value.l_val);
					break;
				case PRO_PARAM_NOTE_ID:
					break;
				case PRO_PARAM_VOID:
					break;
				default:
					break;
				}
				GL_AsmInfo.asmname=strName;
			}
			****/
			//AfxMessageBox(strCode);
			//AfxMessageBox(strName);

			/**************************/
			/*���³������ڻ�ȡ��ϸ��*/
			/**************************/
			int theNum=-1;
			int ntable=0;
			int nRows=0;
			int nColumns=0;
			ProWstring *lines;
			ProCharName charname;
			ProDrawing curDrawing;
			ProDwgtable * curTables;

			GLS_DrwInfo.RemoveAll();
			curDrawing=(ProDrawing)curMdl;
			status=ProDrawingTablesCollect(curDrawing,&curTables);
			if(status==PRO_TK_NO_ERROR)
			{
				status=ProArraySizeGet((ProArray)curTables,&ntable);
				if(status==PRO_TK_NO_ERROR)
				{
					for(int i=0;i<ntable;i++)
					{
						status=ProDwgtableCelltextGet(&curTables[i],1,1,PRODWGTABLE_NORMAL,&lines);
						if(status==PRO_TK_NO_ERROR)
						{
							ProWstringToString(charname,*lines);
							str=charname;
							if(str.Find("��")!=-1)
							{
								theNum=i;
								break;
							}
						}
					}
					if(theNum!=-1)
					{
						ProDwgtableRowsCount(&curTables[theNum],&nRows);
						ProDwgtableColumnsCount(&curTables[theNum],&nColumns);
						//if(nColumns!=9)
						//{
						//	AfxMessageBox("��ʹ�õ���ϸ����׼���޷����ɲ�Ʒ���Ŀ¼��");
						//	return;
						//}
						if(nRows>=3)
						{
							//for(int j=1;j<nRows;j++)
							//{
							//	for(int k=0;k<nColumns;k++)
							//	{
							//		status=ProDwgtableCelltextGet(&curTables[i],k,j,PRODWGTABLE_NORMAL,&lines);
							//		if(status==PRO_TK_NO_ERROR)
							//		{
							//			ProWstringToString(charname,*lines);
							//			str=charname;
							//			AfxMessageBox(str);
							//		}
							//	}
							//}
							for(int j=3;j<(nRows+1);j++)
							{
								status=ProDwgtableCelltextGet(&curTables[i],1,j,PRODWGTABLE_NORMAL,&lines);
								if(status==PRO_TK_NO_ERROR)
								{
									ProWstringToString(charname,*lines);
									str=charname;
									drwinfo.index=str;
									str="";
								}
								else
									drwinfo.index="";
								
								status=ProDwgtableCelltextGet(&curTables[i],2,j,PRODWGTABLE_NORMAL,&lines);
								//��ȡ�ڶ���������Ϊ������
								if(status==PRO_TK_NO_ERROR)
								{
									ProWstringToString(charname,*lines);
									str=charname;
									drwinfo.code=str;
									str="";
								}
								else
									drwinfo.code="";	
								/***pdmͼ�Ų�������ϸ��������ʾ�����������������
								status=ProDwgtableCelltextGet(&curTables[i],3,j,PRODWGTABLE_NORMAL,&lines);
								if(status==PRO_TK_NO_ERROR)
								{
									ProWstringToString(charname,*lines);
									str=charname;
									drwinfo.pdmcode=str;
								}
								else
									drwinfo.pdmcode="";
								*****/
								status=ProDwgtableCelltextGet(&curTables[i],4,j,PRODWGTABLE_NORMAL,&lines);
								if(status==PRO_TK_NO_ERROR)
								{
									ProWstringToString(charname,*lines);
									str=charname;
									drwinfo.name=str;
									str="";
								}
								else
									drwinfo.name="";
								
								status=ProDwgtableCelltextGet(&curTables[i],5,j,PRODWGTABLE_NORMAL,&lines);
								if(status==PRO_TK_NO_ERROR)
								{
									ProWstringToString(charname,*lines);
									str=charname;
									drwinfo.qty=str;
									str="";
								}
								else
									drwinfo.qty="";
								
								status=ProDwgtableCelltextGet(&curTables[i],6,j,PRODWGTABLE_NORMAL,&lines);
								if(status==PRO_TK_NO_ERROR)
								{
									ProWstringToString(charname,*lines);
									str=charname;
									drwinfo.material=str;
									str="";
								}
								else
									drwinfo.material="";
								
								status=ProDwgtableCelltextGet(&curTables[i],9,j,PRODWGTABLE_NORMAL,&lines);
								if(status==PRO_TK_NO_ERROR)
								{
									ProWstringToString(charname,*lines);
									str=charname;
									drwinfo.note=str;
									str="";
								}
								else
									drwinfo.note="";
								
								GLS_DrwInfo.Add(drwinfo);
							}							
						}
					//��ø���ϸ����Ŀ�Ĳ�����Ϣ
						char* cname;
						cname=new char[255];
						ProFamilyName listname;
						ProMdl listmdl;
						for(int i=0;i<GLS_DrwInfo.GetCount();i++)
						{
							strcpy(cname,(LPCTSTR)(GLS_DrwInfo.GetAt(i).code));
							//GLS_DrwInfo.GetAt(i).code;//code��CString����
							ProStringToWstring(listname,cname);
							status = ProMdlInit(listname, PRO_MDL_PART, &listmdl);
							if( status == PRO_TK_NO_ERROR )
							{
								ProModelitem pmodel;
								ProMdlToModelitem(listmdl,&pmodel);
								//���pdmͼ�š���񡢼�������
								CString str;
								str="";
								GetParam(&pmodel,"CLJTH",&str);//��ȡpdmͼ��
								GLS_DrwInfo.GetAt(i).pdmcode=str;
								str="";
								GetParam(&pmodel,"CGG",&str);//��ȡ���
								GLS_DrwInfo.GetAt(i).gg=str;
								str="";
								GetParam(&pmodel,"CLC",&str);//��ȡ��������
								GLS_DrwInfo.GetAt(i).jmlc=str;
								str="";
							}
							status = ProMdlInit(listname, PRO_MDL_ASSEMBLY, (ProMdl*)listmdl);
							if( status == PRO_TK_NO_ERROR )
							{
								ProModelitem pmodel;
								ProMdlToModelitem(listmdl,&pmodel);
								//���pdmͼ�š���񡢼�������
								CString str;
								str="";
								GetParam(&pmodel,"CZCTH",&str);//��ȡpdmͼ��
								GLS_DrwInfo.GetAt(i).pdmcode=str;
								str="";
								GetParam(&pmodel,"CGG",&str);//��ȡ���
								GLS_DrwInfo.GetAt(i).gg=str;
								str="";
								GetParam(&pmodel,"CLC",&str);//��ȡ��������
								GLS_DrwInfo.GetAt(i).jmlc=str;
								str="";
							}
						}
						//strcpy(cname,(LPCTSTR)(curCode));
						////GLS_DrwInfo.GetAt(i).code;//code��CString����
						//ProStringToWstring(listname,cname);
						//
						//status = ProMdlRetrieve(listname, PRO_MDL_ASSEMBLY, (ProMdl*)listmdl);
					}
					else
					{
						AfxMessageBox("û�п�����ϸ����Ϣ��");
					}
					
					//for(int i=0;i<ntable;i++)
					//{
					//	ProDwgtableRowsCount(&curTables[i],&nRows);
					//	ProDwgtableColumnsCount(&curTables[i],&nColumns);
					//	for(int j=0;j<nRows;j++)
					//		for(int k=0;k<nColumns;k++)
					//		{
					//			status=ProDwgtableCelltextGet(&curTables[i],k,j,PRODWGTABLE_NORMAL,&lines);
					//			if(status==PRO_TK_NO_ERROR)
					//			{
					//				ProWstringToString(charname,*lines);
					//				str=charname;
					//				AfxMessageBox(str);
					//				int nLength=0;
					//				ProWstringLengthGet(*lines,&nLength);
					//				nLength;
					//			}
					//		}
					//}
				}
				
			}

			/*****************************/
			/*���³�����ͼƬ��Ϣ*/
			/*****************************/
			int ndtlNum=0;
			ndtlNum=GLS_DrwInfo.GetCount();
			if(ndtlNum!=0)
			{
				for(int i=0;i<ndtlNum;i++)
				{
					//ͼƬ����
					CString strInsCode="";
					CString strSql;
					int theMdlNum=-1;
					strInsCode=GLS_DrwInfo[i].code;
					if(strInsCode!="")
					{
						//��ȡģ�ͱ��
						strSql.Format("select * from NPartInsInfoTable where InsCode='%s'",strInsCode);
						if(gl_db.Query(strSql)!=0)
						{
							int nR=gl_db.getRows();
							if(nR!=0)
							{
								theMdlNum=gl_db.getInt("MdlNum");
							}
						}
						//����ģ�ͱ�Ų�ѡ
						if(theMdlNum!=-1)
						{
							strSql.Format("select * from NPartPic where PrtNum=%d",theMdlNum);
							//if(gl_db.Query(strSql)!=0)
							//{
							//	int nR=gl_db.getRows();
							//	if(nR!=0)
							//	{
							//		gl_db.MoveBegin();
							//		//���¶�ȡͼƬ

							//	}
							//}
							//������ado��ȡͼƬ��Ϣ
							m_pRecordset.CreateInstance("ADODB.Recordset");
							HRESULT hr = m_pRecordset->Open(_variant_t(strSql),_variant_t((IDispatch *)m_pConnection,true),adOpenDynamic,adLockPessimistic,adCmdText);
							if(SUCCEEDED(hr))
							{
								//if(m_pRecordset->adoEOF||m_pRecordset->BOF) 
								//{
								//	//MessageBox("adoEOF");
								//	//return;
								//}
								//else
								//{
								//	//Create File
								//	HANDLE hFile; 
								//	hFile = CreateFile("D:\\11.bmp",           // create MYFILE.TXT 
								//		GENERIC_WRITE,                // open for writing 
								//		0,                            // do not share 
								//		NULL,                         // no security 
								//		CREATE_ALWAYS,                // overwrite existing 
								//		FILE_ATTRIBUTE_NORMAL |       // normal file 
								//		FILE_FLAG_OVERLAPPED,         // asynchronous I/O 
								//		NULL);                        // no attr. template 

								//	if (hFile == INVALID_HANDLE_VALUE) 
								//	{ 
								//		MessageBox("Could not open file.");  // process error 
								//	}
								//	else
								//	{
								//		CFile cfile(hFile);
								//		//Data
								//		m_pRecordset->MoveFirst();
								//		_variant_t value;
								//		do{
								//			value = m_pRecordset->GetCollect(_variant_t("Name"));
								//			if(value.vt==VT_NULL)
								//				MessageBox("Null");
								//			CString str=CString(value.bstrVal);
								//			AfxMessageBox(str);	

								//			long lDataSize = m_pRecordset->GetFields()->GetItem("Content")->ActualSize;///�õ����ݵĳ���
								//			if(lDataSize > 0)
								//			{
								//				//��ʵ�ֶ�ȡ
								//				_StreamPtr  pStm;
								//				pStm.CreateInstance("ADODB.Stream");
								//				variant_t varOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);
								//				pStm->PutType(adTypeBinary);
								//				pStm->Open( varOptional,  
								//					adModeUnknown, adOpenStreamUnspecified, _bstr_t(), _bstr_t());
								//				pStm->Write(_variant_t(m_pRecordset->GetFields()->GetItem("Content")->Value));
								//				pStm->SaveToFile("d:\\publogo.bmp", adSaveCreateOverWrite);
								//			}
								//			m_pRecordset->MoveNext();
								//		}
								//		//while(SUCCEEDED(m_pRecordset->MoveNext()));
								//		while(!m_pRecordset->adoEOF);
								//	}
								//	/////////
								//}

								//�����ͼƬ�Ͷ�ȡͼƬ
								m_pRecordset->MoveFirst();
								_variant_t value;
								value = m_pRecordset->GetCollect(_variant_t("Name"));
								CString strNameValue=CString(value.bstrVal);
								CString strNameValue2="";
								//����Ŀ¼���ļ�
								CFileFind ff;
								BOOL b=FALSE;
								b=ff.FindFile("C:\\TempDir");
								BOOL bCreated=false;
								while(b){
									if(bCreated)
										break;
									b=ff.FindNextFile();
									if(!ff.IsDirectory())
									{
										CreateDirectory("C:\\TempDir",NULL);
										bCreated=TRUE;
									}
									else
										bCreated=TRUE;
								}
								if(!bCreated)
									CreateDirectory("C:\\TempDir",NULL);

								strNameValue=GLS_DrwInfo[i].code+".jpg";
								strNameValue2=GLS_DrwInfo[i].code+"t.jpg";
								GLS_DrwInfo[i].picname=strNameValue;

								CString strPathValue="C:\\TempDir\\"+strNameValue;
								CString strPathValue2="C:\\TempDir\\"+strNameValue2;
								GLS_DrwInfo[i].picpath=strPathValue;
								HANDLE hFile; 
								hFile = CreateFile(strPathValue2, GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,NULL); 
								//hFile = CreateFile(strPathValue, GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,NULL); 
								if (hFile == INVALID_HANDLE_VALUE) 
								{ 
									AfxMessageBox("Could not open file.");
								}
								else
								{
									long lDataSize = m_pRecordset->GetFields()->GetItem("File")->ActualSize;
									if(lDataSize > 0)
									{
										//��ʵ�ֶ�ȡ
										_StreamPtr  pStm;
										pStm.CreateInstance("ADODB.Stream");
										variant_t varOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);
										pStm->PutType(adTypeBinary);
										pStm->Open( varOptional,  
											adModeUnknown, adOpenStreamUnspecified, _bstr_t(), _bstr_t());
										pStm->Write(_variant_t(m_pRecordset->GetFields()->GetItem("File")->Value));
										pStm->SaveToFile(_bstr_t(strPathValue), adSaveCreateOverWrite);
										//pStm->SaveToFile("D:\\44.jpg", adSaveCreateOverWrite);
										//pStm->Release();
									}
									CloseHandle(hFile);
								}								
							}
						}
					}
					else
					{
						GLS_DrwInfo[i].picname="";
						GLS_DrwInfo[i].picpath="";
					}
				}
			}
		}
		else
		{
			AfxMessageBox("��ת��װ�乤��ͼ���棬Ȼ�����ɲ�Ʒ���Ŀ¼���EXCEL�ĵ���");
		}
	}
}
void OutPutExcel()
{
	int nRows=0;
	nRows=GLS_DrwInfo.GetCount();
	if(nRows!=0)
	{
		//CString str;
		//str.Format("%d",nRows);
		//AfxMessageBox(str);
		//for(int i=0;i<nRows;i++)
		//{
		//	AfxMessageBox(GLS_DrwInfo[i].code);
		//}
		_Application ExcelApp; 
		Workbooks wbsMyBooks; 
		_Workbook wbMyBook; 
		Worksheets wssMysheets; 
		_Worksheet wsMysheet; 
		Range rgMyRge; 
		Shapes myShapes;
		if (!ExcelApp.CreateDispatch("Excel.Application",NULL)) 
		{ 
			AfxMessageBox("����Excel����ʧ��!"); 
			exit(1); 
		} 
		ExcelApp.put_Visible(FALSE);
		//����ģ���ļ��������ĵ� 
		char path[MAX_PATH];
		GetSystemDirectory(path,MAX_PATH);
		CString strPath = path;
		strPath += "\\standard";
		wbsMyBooks.AttachDispatch(ExcelApp.get_Workbooks(),true); 
		wbMyBook.AttachDispatch(wbsMyBooks.Add(_variant_t(strPath)));
		//�õ�Worksheets 
		wssMysheets.AttachDispatch(wbMyBook.get_Worksheets(),true);
		//�õ�sheet1 
		wsMysheet.AttachDispatch(wssMysheets.get_Item(_variant_t("���Ŀ¼���1ҳ")),true);
		//���ģ�����
		int nSheets=0;
		if(nRows%8==0)
			nSheets=nRows/8;
		else
			nSheets=nRows/8+1;
		//���ģ��
		for(int i=0;i<nSheets-1;i++){
			wsMysheet.Copy(vtMissing,_variant_t(wsMysheet));
		}
		CString str1;
		wsMysheet.AttachDispatch(wssMysheets.get_Item(_variant_t("���Ŀ¼���1ҳ")),true);
		//str1 = "��1ҳ";
		//wsMysheet.put_Name(str1);
		for(i=0;i<wssMysheets.get_Count()-3;i++){
			wsMysheet = wsMysheet.get_Next();
			str1.Format("���Ŀ¼���%dҳ",i+2);
			wsMysheet.put_Name(str1);
		}
		//���ݼ�¼����õ�ǰ������
		for(int j=0;j<nSheets;j++)
		{
			str1.Format("���Ŀ¼���%dҳ",j+1);
			wsMysheet.AttachDispatch(wssMysheets.get_Item(_variant_t(str1)),true);
			myShapes.AttachDispatch(wsMysheet.get_Shapes(),TRUE);
			//AfxMessageBox(str1);
			//�õ�ȫ��Cells����ʱ,rgMyRge��cells�ļ��� 
			rgMyRge.AttachDispatch(wsMysheet.get_Cells(),true);
			//����ӱ�ͷ��Ϣ
			CString strTitleInfo="";
			strTitleInfo="��Ʒ����:"+ GL_AsmInfo.asmname;
			rgMyRge.put_Item(_variant_t((long)2),_variant_t((long)1),_variant_t(strTitleInfo));
			strTitleInfo="��Ʒ����:"+ GL_AsmInfo.pdmcode;
			rgMyRge.put_Item(_variant_t((long)2),_variant_t((long)6),_variant_t(strTitleInfo));
			strTitleInfo.Format("��%dҳ  ��%dҳ",nSheets,j+1);
			rgMyRge.put_Item(_variant_t((long)2),_variant_t((long)9),_variant_t(strTitleInfo));
			for(int k=0;(k<8)&&((j*8+k)<nRows);k++)
			{
				//AfxMessageBox(GLS_DrwInfo[j*6+k].index);
				//�������
				rgMyRge.put_Item(_variant_t((long)(4+2*k)),_variant_t((long)1),_variant_t(GLS_DrwInfo[j*8+k].index));
				
				//AfxMessageBox(GLS_DrwInfo[j*6+k].code);
				//�������ͼ��
				rgMyRge.put_Item(_variant_t((long)(4+2*k)),_variant_t((long)2),_variant_t(GLS_DrwInfo[j*8+k].code));
				
				//AfxMessageBox(GLS_DrwInfo[j*6+k].pdmcode);
				//�������pdmͼ��
				rgMyRge.put_Item(_variant_t((long)(4+2*k)),_variant_t((long)3),_variant_t(GLS_DrwInfo[j*8+k].pdmcode)); 
				
				//AfxMessageBox(GLS_DrwInfo[j*6+k].name);
				//�����������
				rgMyRge.put_Item(_variant_t((long)(4+2*k)),_variant_t((long)4),_variant_t(GLS_DrwInfo[j*8+k].name)); 
				
				//AfxMessageBox(GLS_DrwInfo[j*6+k].qty);
				//�����������
				rgMyRge.put_Item(_variant_t((long)(4+2*k)),_variant_t((long)5),_variant_t(GLS_DrwInfo[j*8+k].qty)); 
				
				//AfxMessageBox(GLS_DrwInfo[j*6+k].material);
				//�����������
				rgMyRge.put_Item(_variant_t((long)(4+2*k)),_variant_t((long)6),_variant_t(GLS_DrwInfo[j*8+k].material));
				
				//����ͼƬ
				//AfxMessageBox(GLS_DrwInfo[j*6+k].picpath);
				if(GLS_DrwInfo[j*8+k].picpath!="")
				{
					//AfxMessageBox(GLS_DrwInfo[j*8+k].picpath);
					myShapes.AddPicture(GLS_DrwInfo[j*8+k].picpath,FALSE,TRUE,430,80+k*45,45,45);
				}
				//rgMyRge.put_Item(_variant_t((long)(4+k*2)),_variant_t((long)6),_variant_t(GLS_DrwInfo[j*6+k].picpath)); 
				//����������
				rgMyRge.put_Item(_variant_t((long)(4+2*k)),_variant_t((long)8),_variant_t(GLS_DrwInfo[j*8+k].gg));
				//���������������
				rgMyRge.put_Item(_variant_t((long)(4+2*k)),_variant_t((long)9),_variant_t(GLS_DrwInfo[j*8+k].jmlc));
				//AfxMessageBox(GLS_DrwInfo[j*6+k].note);
				//���������ע
				rgMyRge.put_Item(_variant_t((long)(4+2*k)),_variant_t((long)10),_variant_t(GLS_DrwInfo[j*8+k].note)); 
			}

			//-------------------------------��д��Ʒͼ����-------------------------------
			wsMysheet.AttachDispatch(wssMysheets.get_Item(_variant_t("��Ʒͼ����")),true);
			myShapes.AttachDispatch(wsMysheet.get_Shapes(),TRUE);
			//�õ�ȫ��Cells����ʱ,rgMyRge��cells�ļ��� 
			rgMyRge.AttachDispatch(wsMysheet.get_Cells(),true);
			//�����Ϣ
			rgMyRge.put_Item(_variant_t((long)3),_variant_t((long)1),_variant_t(GL_AsmInfo.asmname));
			rgMyRge.put_Item(_variant_t((long)8),_variant_t((long)5),_variant_t(GL_AsmInfo.asmcode));
			rgMyRge.put_Item(_variant_t((long)11),_variant_t((long)5),_variant_t(GL_AsmInfo.pdmcode));
			rgMyRge.put_Item(_variant_t((long)44),_variant_t((long)1),_variant_t(GL_AsmInfo.companyname));
			//-------------------------------���Ŀ¼�����-------------------------------
			wsMysheet.AttachDispatch(wssMysheets.get_Item(_variant_t("���Ŀ¼�����")),true);
			myShapes.AttachDispatch(wsMysheet.get_Shapes(),TRUE);
			//�õ�ȫ��Cells����ʱ,rgMyRge��cells�ļ��� 
			rgMyRge.AttachDispatch(wsMysheet.get_Cells(),true);
			//�����Ϣ
			rgMyRge.put_Item(_variant_t((long)8),_variant_t((long)7),_variant_t(GL_AsmInfo.asmcode));
			rgMyRge.put_Item(_variant_t((long)10),_variant_t((long)7),_variant_t(GL_AsmInfo.pdmcode));
			rgMyRge.put_Item(_variant_t((long)14),_variant_t((long)5),_variant_t(GL_AsmInfo.asmname));
			rgMyRge.put_Item(_variant_t((long)18),_variant_t((long)7),_variant_t(GL_AsmInfo.sheji));
			rgMyRge.put_Item(_variant_t((long)20),_variant_t((long)7),_variant_t(GL_AsmInfo.shenhe));
			rgMyRge.put_Item(_variant_t((long)22),_variant_t((long)7),_variant_t(GL_AsmInfo.shenpi));
			rgMyRge.put_Item(_variant_t((long)26),_variant_t((long)2),_variant_t(GL_AsmInfo.companyname));
		}

		try
		{
			//����񱣴�
			CString strSavepath="";
			CString strFilter="Excel Files (*.xls)|*.xls||";
			str1.Format("%s���Ŀ¼��",GL_AsmInfo.asmcode);
			CFileDialog fd(FALSE,"xls",str1,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,strFilter,NULL);
			if(fd.DoModal()==IDOK)
			{
				strSavepath=fd.GetPathName();
				ExcelApp.put_Visible(true); 
				wsMysheet.SaveAs(strSavepath,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing);
			}
		}
		catch (...)
		{
			//AfxMessageBox("ͬ���ļ��ѱ���!");
		}
		
		//��ӡԤ��
		//wbMyBook.PrintPreview(_variant_t(false)); 
		//�ͷŶ��� 
		rgMyRge.ReleaseDispatch(); 
		wsMysheet.ReleaseDispatch(); 
		wssMysheets.ReleaseDispatch(); 
		wbMyBook.ReleaseDispatch(); 
		wbsMyBooks.ReleaseDispatch(); 
		ExcelApp.ReleaseDispatch();
		//ExcelApp.Quit();

		//����ʱ�ļ�ɾ��
		CFileFind ffDel;
		BOOL bDel=FALSE;
		CString sDel="";
		bDel=ffDel.FindFile("C:\\TempDir\\*.*");
		while(bDel)
		{
			bDel=ffDel.FindNextFile();
			sDel=ffDel.GetFilePath();
			DeleteFile(sDel);
		}
	}
}
//��ò���ֵ
BOOL GetParam(ProModelitem * theOwner,CString strName,CString * strValue)
{
	ProError status;
	CString str="";
	ProName name;
	ProParameter param;
	ProParamvalue value;
	ProStringToWstring(name,strName.GetBuffer());
	status = ProParameterInit (theOwner,name,&param);
	if(status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	ProParameterValueGet(&param,&value);
	str=CString(param.id);
	switch(value.type) {
		case PRO_PARAM_DOUBLE:
			str.Format("%0.2f",value.value.d_val);
			break;
		case PRO_PARAM_STRING:
			str=CString(value.value.s_val);
			break;
		case PRO_PARAM_INTEGER:
			str.Format("%d",value.value.i_val);
			break;
		case PRO_PARAM_BOOLEAN:
			str.Format("%d",value.value.l_val);
			break;
		case PRO_PARAM_NOTE_ID:
			break;
		case PRO_PARAM_VOID:
			break;
		default:
			break;
	}
	*strValue=str;
	return TRUE;
}