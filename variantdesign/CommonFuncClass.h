#pragma once
#include "ProSolid.h"
#include "afxtempl.h"



// CCommonFuncClass ����Ŀ��

class CCommonFuncClass : public CObject
{
public:
	CCommonFuncClass();
	virtual ~CCommonFuncClass();
public:
	 
	BOOL GetAllAsmContainCurrentMdl(ProSolid ProductSolid,ProSolid CurrentSolid,CArray <ProSolid,ProSolid> *asmarray);
	ProSolid ObtainTheTopSolid(ProSolid solid);           //���ݵ�ǰ����õ����Ĳ�Ʒ���
	void AddPartSolid(ProSolid solid,CArray <ProSolid,ProSolid> *solidarray);	 //���뵱ǰװ������²��㲿��
	//BOOL AddMdlInfo(ProSolid ProductsSolid);
	BOOL IsHaveCurrentSolid(ProSolid Asm,ProSolid Currentsolid);			 //�ж���װ�����Ƿ��а�����ǰ�����
	BOOL RemoveTheSubAam(CArray <ProMdl,ProMdl> *asmarray);					 //ȥ����װ����
	BOOL RemoveTheSubAamNotContainCurrent(CArray <ProMdl,ProMdl> *asmarray,ProSolid solid);   //ȥ����������ǰģ�͵�װ����
	
	BOOL RemoveNotDrw(CString DrawingPath);	//ȥ��������Ĳ���
	BOOL RemoveDireAndFile(CString localdir);
	BOOL IsInSession(ProSolid solid,CArray <ProMdl,ProMdl> *p_promdl); //����Ƿ����ڴ���
	BOOL GetAlreadyInTheSession(CArray <ProSolid,ProSolid> *p_arraylist);
	

	BOOL RemoveTempFile(CString localpath);

	void ChangeJuBingToJuBingTemp();

	BOOL RemoveNameFromNameArray(CStringArray *NameArray);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL SaveFile(ProSolid solid,CString SolidPlace,CString DrwPlace,CString SolidName);
	BOOL SaveProductFile(ProSolid solid,CString SolidPlace,CString DrwPlace);
	BOOL DeleteSpecialFile(CString LocalPlace, CString Name);
	CString FindFile(CString FilePath);

	BOOL IsDrwFileExist(CString DrwFile,CString DrwPlace);
	BOOL IsFileExist(CString SolidPlace,CString DrwPlace,CString SolidName,CString DrwName);		 //�жϱ�������ļ��Ƿ����
	BOOL IsSolidIDAlreadyHave();

	BOOL InsertNewPart(int s,BOOL NeedTrans);              //�������
	BOOL InsertDatabase(int s);
	BOOL SavePartToTheServerforPart(int s);
	BOOL SavePartToTheServerforProduct(int s);

	BOOL UpLoadFile(int s);				 //�ϴ��ļ�
	BOOL RollBackFile(int s); //ɾ���ļ�

	int IsPartExist(ProSolid solid,CString PartNum);			  //�ж�������ʵ���Ƿ����
	int IsAsmExist(ProSolid solid,CString MdlNum);
	BOOL GetDimInfo(CString AsmNum,CString strSql,int variantnum,BOOL *IsHave);

	BOOL UpdateNewPart(int s,BOOL NeedTrans);
	//BOOL WriteBasicInfo(ProSolid solid,CString SolidPlace,CString DrwPlace);
	BOOL InsertVariantAndPerf(int s,int PrtNum);
	BOOL UpdateVariantAndPerf(int s);

	int IsBasicInfoHave(CString InsCode);
	int IsBasicInfoHave(ProSolid solid);
	BOOL IsHandleAlreadyHave();				 //ȥ���ظ��ľ���ṹ
	BOOL IsNameNeedAlter(ProSolid solid,CString Name);

	BOOL ObtianHandleFromName(int s);	   //������������ֵõ����
	BOOL ObtianBasicInfoFromHandle(ProSolid solid);	   //��Ʒ�ľ��
	BOOL IsTheSavePlaceTheSameIfReplace(BOOL IsLocal);   //�����滻ʱ���жϱ���λ�õĺϷ���

	BOOL IsBasicInfoAlreadyHave();	        //��������Ϣ�Ƿ����ظ���
	BOOL IsHave(CString sql);				//�ж�ͼ�ţ��ļ�����Ψһ��
	BOOL IsInfoOk(int s,CString InsCode,CString SolidName,CString DrwName);   //���ݲ�ͬ�ı��淽ʽ�ж�ͼ�ţ��ļ����ĺϷ���
	void RemoveNotInSession();              //ȥ�������ڴ��еĻ�����Ϣ

	int IsPrtInsCodeAndPrtNumOk(CString InsCode,int SolidID);
	BOOL IsPathTheSame(ProSolid solid,CString Path,CString Name);	//�ж��µ�ַ�;͵�ַ�Ƿ�һ����
	BOOL IsAuditPass(int SolidID,BOOL IsPart);				        //�ж�����Ƿ�ͨ��
	BOOL SaveStyle(ProSolid solid,int s,CString MdlNum);			  //�ж�����ı��淽ʽ

	BOOL UsrMdlBackUp(ProMdl mdl,ProMdldata * data);
	BOOL BackUpFile(int s);   //��Ʒ���

	BOOL RemoveMemory(ProSolid Productsolid);					    //ȥ���ڴ溯��
	BOOL IsAlreadyInTheSession(CString Name,CArray <ProSolid,ProSolid> *p_arraylist);		    //���������ж��Ƿ���ڸ�ģ��

	BOOL AddProductToTree(ProSolid solid,CTreeCtrl *m_tree);
	HTREEITEM AddConponentToTree(HTREEITEM hParent,ProMdl mdl,CTreeCtrl *m_tree);
	void AddAssemblyToTree(HTREEITEM hParent,ProSolid solid,CTreeCtrl *m_tree);
	BOOL ObtainPartModelClass(CTreeCtrl *m_tree);					   //����ķ���
	BOOL ObtainProductModelClass(CTreeCtrl *m_tree);				   //��Ʒ�ķ���
    
	BOOL TheWorkBeforeSave(ProSolid ProductSolid);                     //����ǰ��׼������
	BOOL TheWorkAferSave(ProSolid ProductSolid,BOOL Success);          //�����Ĺ���
	BOOL SetParam(int s);   
	CString ObtainPartMap(ProSolid solid);                             //�õ���ǰ��ͼ��
	BOOL SetCodeInfo(ProSolid solid) ;                                 //����ͼ����Ϣ
	BOOL StandardizeDocumentName(CString OldSolidName,CString OldDrwName,CString *NewSolidName,CString *NewDrwName);      //��׼���ļ���
	BOOL SetProductPara(ProSolid ProductsSolid);                       //���ò�Ʒ��ͼ����Ϣ
public:
	CString PartMap;
	CString InsName;
	CString LocalPath;
	CImageList imageList;
};



//BOOL CCommonFuncClass::AddProductInfo(CString AsmNum)
//{
//	CString sql,PartMap="";
//	int PartNum,Num,Z;
//	sql.Format("SELECT * from AsmMakeupTable WHERE AsmNum=%s",AsmNum);
//	Z=m_db.Query(sql);
//	if (Z==-1)
//	{
//		return FALSE;
//	}
//	else
//		if (Z!=0)
//		{
//			m_db.MoveBegin();
//			do {
//				PartNum=m_db.getInt("PartMdlNum");
//				Num=m_db.getInt("PartNum");
//				for (int i=0;i<JuBing_struc.GetSize();i++)
//				{
//					if(Num==JuBing_struc[i].number)
//					{
//						AddMdlInfo(JuBing_struc[i].solid,PartNum,PartMap);
//						break;
//					} 
//				}
//			} while(m_db.MoveNext());
//		}
//		return TRUE;
//}
//void CCommonFuncClass::AddMdlInfo(ProSolid solid,int PartNum,CString PartCode)
//{
//	CString str;
//	str.Format("%d",PartNum);
//	ProLine line;
//	ProModelitem modelitem;
//	ProParameter param;
//	ProParamvalue p_value;
//	ProError status;
//	ProName w_name;
//	status=ProMdlToModelitem((ProMdl)solid,&modelitem);
//	ProStringToWstring(w_name,"����ģ��");
//	status=ProParameterInit(&modelitem,w_name,&param);
//	if (status==PRO_TK_E_NOT_FOUND)
//	{
//		//�½�����
//		p_value.type=PRO_PARAM_STRING;
//		ProStringToWstring(p_value.value.s_val,str.GetBuffer());
//		ProParameterCreate(&modelitem,w_name,&p_value,&param);
//	}
//	if (status==PRO_TK_NO_ERROR)
//	{
//		ProStringToWstring(line,str.GetBuffer());
//		status=ProParamvalueSet(&p_value,(void*)line,PRO_PARAM_STRING);
//		status=ProParameterValueSet (&param,&p_value);
//	}
//	//�½�ͼ��
//	ProStringToWstring(w_name,"ͼ��");
//	status=ProParameterInit(&modelitem,w_name,&param);
//	if (status==PRO_TK_E_NOT_FOUND)
//	{
//		//�½�ͼ�Ų���
//		p_value.type=PRO_PARAM_STRING;
//		ProStringToWstring(p_value.value.s_val,PartCode.GetBuffer());	   //Ӧ���ǵ�ǰ��ʵ��
//		ProParameterCreate(&modelitem,w_name,&p_value,&param);
//	}
//	if (status==PRO_TK_NO_ERROR)
//	{
//		ProStringToWstring(line,PartCode.GetBuffer());
//		status=ProParamvalueSet(&p_value,(void*)line,PRO_PARAM_STRING);
//		status=ProParameterValueSet (&param,&p_value);
//	}
//}
///************************************************************************/
///* �õ�ģ�ͺ���Ϣ                                                         */
///************************************************************************/
//CString CCommonFuncClass::ObtainModelNum(ProSolid solid)
//{
//	ProError status;
//	ProModelitem modelitem;
//	ProCharName cname;
//	ProName w_name;
//	ProParameter param;
//	ProParamvalue p_value;
//	ProLine wstr;
//	CString partnum;
//	////�õ�ģ�ͺ�
//	status=ProMdlToModelitem((ProMdl)solid,&modelitem);
//	sprintf(cname,"����ģ��");
//	ProStringToWstring(w_name,cname);
//	status=ProParameterInit(&modelitem,w_name,&param);
//	if (status!=PRO_TK_NO_ERROR)
//	{
//		partnum=="-2";					 //���������û��ģ�ͱ���򷵻�-2
//		return partnum;
//	}
//	status=ProParameterValueGet(&param,&p_value);
//	if (status!=PRO_TK_NO_ERROR)
//	{
//		partnum=="-2";
//		return partnum;
//	}
//	swprintf(wstr,L"%s",p_value.value.s_val);
//	partnum=CString(wstr);
//	return partnum;
//}
///************************************************************************/
///* �õ�ͼ����Ϣ                                                         */
///************************************************************************/
//CString CCommonFuncClass::ObtainPartMap(ProSolid solid)
//{
//	ProError status;
//	ProModelitem modelitem;
//	ProCharName cname;
//	ProName w_name;
//	ProParameter param;
//	ProParamvalue p_value;
//	ProLine wstr;
//	CString partmap;
//	////�õ�ͼ��
//	status=ProMdlToModelitem((ProMdl)solid,&modelitem);
//	sprintf(cname,"ͼ��");
//	ProStringToWstring(w_name,cname);
//	status=ProParameterInit(&modelitem,w_name,&param);
//	if (status!=PRO_TK_NO_ERROR)
//	{
//		partmap=="-1";
//		return partmap;
//	}
//	status=ProParameterValueGet(&param,&p_value);
//	if (status!=PRO_TK_NO_ERROR)
//	{
//		partmap=="-1";
//		return partmap;
//	}
//	swprintf(wstr,L"%s",p_value.value.s_val);
//	partmap=CString(wstr);
//	return partmap;
//}
