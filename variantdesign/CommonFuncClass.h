#pragma once
#include "ProSolid.h"
#include "afxtempl.h"



// CCommonFuncClass 命令目标

class CCommonFuncClass : public CObject
{
public:
	CCommonFuncClass();
	virtual ~CCommonFuncClass();
public:
	 
	BOOL GetAllAsmContainCurrentMdl(ProSolid ProductSolid,ProSolid CurrentSolid,CArray <ProSolid,ProSolid> *asmarray);
	ProSolid ObtainTheTopSolid(ProSolid solid);           //根据当前句柄得到最顶层的产品句柄
	void AddPartSolid(ProSolid solid,CArray <ProSolid,ProSolid> *solidarray);	 //加入当前装配体的下层零部件
	//BOOL AddMdlInfo(ProSolid ProductsSolid);
	BOOL IsHaveCurrentSolid(ProSolid Asm,ProSolid Currentsolid);			 //判断子装配体是否有包含当前的零件
	BOOL RemoveTheSubAam(CArray <ProMdl,ProMdl> *asmarray);					 //去掉子装配体
	BOOL RemoveTheSubAamNotContainCurrent(CArray <ProMdl,ProMdl> *asmarray,ProSolid solid);   //去掉不包含当前模型的装配体
	
	BOOL RemoveNotDrw(CString DrawingPath);	//去掉非零件的部分
	BOOL RemoveDireAndFile(CString localdir);
	BOOL IsInSession(ProSolid solid,CArray <ProMdl,ProMdl> *p_promdl); //句柄是否在内存中
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
	BOOL IsFileExist(CString SolidPlace,CString DrwPlace,CString SolidName,CString DrwName);		 //判断本地这个文件是否存在
	BOOL IsSolidIDAlreadyHave();

	BOOL InsertNewPart(int s,BOOL NeedTrans);              //新增零件
	BOOL InsertDatabase(int s);
	BOOL SavePartToTheServerforPart(int s);
	BOOL SavePartToTheServerforProduct(int s);

	BOOL UpLoadFile(int s);				 //上传文件
	BOOL RollBackFile(int s); //删除文件

	int IsPartExist(ProSolid solid,CString PartNum);			  //判断这个零件实例是否存在
	int IsAsmExist(ProSolid solid,CString MdlNum);
	BOOL GetDimInfo(CString AsmNum,CString strSql,int variantnum,BOOL *IsHave);

	BOOL UpdateNewPart(int s,BOOL NeedTrans);
	//BOOL WriteBasicInfo(ProSolid solid,CString SolidPlace,CString DrwPlace);
	BOOL InsertVariantAndPerf(int s,int PrtNum);
	BOOL UpdateVariantAndPerf(int s);

	int IsBasicInfoHave(CString InsCode);
	int IsBasicInfoHave(ProSolid solid);
	BOOL IsHandleAlreadyHave();				 //去掉重复的句柄结构
	BOOL IsNameNeedAlter(ProSolid solid,CString Name);

	BOOL ObtianHandleFromName(int s);	   //根据零件的名字得到句柄
	BOOL ObtianBasicInfoFromHandle(ProSolid solid);	   //产品的句柄
	BOOL IsTheSavePlaceTheSameIfReplace(BOOL IsLocal);   //当有替换时，判断保存位置的合法性

	BOOL IsBasicInfoAlreadyHave();	        //检查基本信息是否有重复的
	BOOL IsHave(CString sql);				//判断图号，文件名的唯一性
	BOOL IsInfoOk(int s,CString InsCode,CString SolidName,CString DrwName);   //根据不同的保存方式判断图号，文件名的合法性
	void RemoveNotInSession();              //去掉不在内存中的基本信息

	int IsPrtInsCodeAndPrtNumOk(CString InsCode,int SolidID);
	BOOL IsPathTheSame(ProSolid solid,CString Path,CString Name);	//判断新地址和就地址是否一样；
	BOOL IsAuditPass(int SolidID,BOOL IsPart);				        //判断审核是否通过
	BOOL SaveStyle(ProSolid solid,int s,CString MdlNum);			  //判断零件的保存方式

	BOOL UsrMdlBackUp(ProMdl mdl,ProMdldata * data);
	BOOL BackUpFile(int s);   //产品句柄

	BOOL RemoveMemory(ProSolid Productsolid);					    //去处内存函数
	BOOL IsAlreadyInTheSession(CString Name,CArray <ProSolid,ProSolid> *p_arraylist);		    //根据名字判断是否存在该模型

	BOOL AddProductToTree(ProSolid solid,CTreeCtrl *m_tree);
	HTREEITEM AddConponentToTree(HTREEITEM hParent,ProMdl mdl,CTreeCtrl *m_tree);
	void AddAssemblyToTree(HTREEITEM hParent,ProSolid solid,CTreeCtrl *m_tree);
	BOOL ObtainPartModelClass(CTreeCtrl *m_tree);					   //零件的分类
	BOOL ObtainProductModelClass(CTreeCtrl *m_tree);				   //产品的分类
    
	BOOL TheWorkBeforeSave(ProSolid ProductSolid);                     //保存前的准备工作
	BOOL TheWorkAferSave(ProSolid ProductSolid,BOOL Success);          //保存后的工作
	BOOL SetParam(int s);   
	CString ObtainPartMap(ProSolid solid);                             //得到当前的图号
	BOOL SetCodeInfo(ProSolid solid) ;                                 //设置图号信息
	BOOL StandardizeDocumentName(CString OldSolidName,CString OldDrwName,CString *NewSolidName,CString *NewDrwName);      //标准化文件名
	BOOL SetProductPara(ProSolid ProductsSolid);                       //设置产品等图号信息
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
//	ProStringToWstring(w_name,"所属模型");
//	status=ProParameterInit(&modelitem,w_name,&param);
//	if (status==PRO_TK_E_NOT_FOUND)
//	{
//		//新建参数
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
//	//新建图号
//	ProStringToWstring(w_name,"图号");
//	status=ProParameterInit(&modelitem,w_name,&param);
//	if (status==PRO_TK_E_NOT_FOUND)
//	{
//		//新建图号参数
//		p_value.type=PRO_PARAM_STRING;
//		ProStringToWstring(p_value.value.s_val,PartCode.GetBuffer());	   //应该是当前的实例
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
///* 得到模型号信息                                                         */
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
//	////得到模型号
//	status=ProMdlToModelitem((ProMdl)solid,&modelitem);
//	sprintf(cname,"所属模型");
//	ProStringToWstring(w_name,cname);
//	status=ProParameterInit(&modelitem,w_name,&param);
//	if (status!=PRO_TK_NO_ERROR)
//	{
//		partnum=="-2";					 //如果参数中没有模型编号则返回-2
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
///* 得到图号信息                                                         */
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
//	////得到图号
//	status=ProMdlToModelitem((ProMdl)solid,&modelitem);
//	sprintf(cname,"图号");
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
