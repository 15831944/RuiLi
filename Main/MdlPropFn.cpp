#include "stdafx.h"
#include "resource.h"
#include "PartInsTableSet.h"

/************************************************************************/
/* 检查一个模型的基本信息是否已经定义,如果没有则定义                    */
/************************************************************************/
void  IsMdlInfoDefAndDef(ProSolid mdl)
{
	int           num=0;
	int           indexNo=-1;  //定义的编号
	UsrPartPorp   temp;
	ProMdldata    data;
	ProMdlType    type;
	CString       csType;       //文件的扩展符 .prt .asm
	for(int i=0;i<(int)p_UsrPartPorp.GetCount();i++)
	{
		//通过判断名称来判断模型是否还存在
		if(IsMdlDeleted(p_UsrPartPorp[i].owner))
		{                                       //对于以前的进行清理，清理之前要把组件元件变型范围进行修改
			for (int j=0;j<p_AsmCompItemList.GetCount();j++)
			{
				if(p_AsmCompItemList[j].propNum>i)
				{
					p_AsmCompItemList[j].propNum--;
				}
			}
			p_UsrPartPorp.RemoveAt(i);
			p_UsrPartPorp.FreeExtra();
			i=i-1;
			continue;
		}
		//判断模型的工程是否被删除
		if (p_UsrPartPorp[i].bHaveDraw && IsMdlDeleted(p_UsrPartPorp[i].hDraw))
		{
			p_UsrPartPorp[i].bHaveDraw=false;
			p_UsrPartPorp[i].hDraw=NULL;
		}	
		//判断图片是否还存在,如果不存在则为空
		if (!PathFileExists(p_UsrPartPorp[i].pic_path))
			p_UsrPartPorp[i].pic_path="";
		//已经定义，获得序号,返回
		if(p_UsrPartPorp[i].owner==mdl)
			return;
	}
	//对于没有定义的进行定义
	if (indexNo==-1) {
		ProMdlDataGet(mdl,&data);
		ProMdlTypeGet(mdl,&type);
		if (type==PRO_MDL_PART) {

			csType=_T(".prt");
		}
		else if (type==PRO_MDL_ASSEMBLY) {
			csType=_T(".asm");
		}
		temp.owner    = mdl;
		temp.Name=CString(data.name);
		temp.Number   = -1;

		temp.TypeName =_T("");
		temp.TypeNum  = -1;

		temp.MdlNum   =-1;//没有模型
		temp.MdlType  =-1;//没有定义类型
		temp.MdlName  =_T("无模型");

		temp.Code     ="";		
		temp.bHaveDraw     = false;
		temp.hDraw         = NULL;
		temp.Person        ="";
		temp.Material      ="";
		temp.Note          ="";
		temp.pic_path      ="";
		temp.IsAsmComp     =false;  //默认不是装配件的部件
		temp.IsAudited     =false;  //默认为没有通过审核
		temp.ResourceType  =0;      //默认来源是本地
		p_UsrPartPorp.Add(temp);
	}
}
UsrPartPorp * SetMdlInfo(ProMdl mdl)
{
	IsMdlInfoDefAndDef((ProSolid)mdl);
	for(int i=0;i<(int)p_UsrPartPorp.GetCount();i++)
	{
		if(p_UsrPartPorp[i].owner==mdl)
			return &p_UsrPartPorp[i];
	}
	return NULL;
}
UsrPartPorp  GetMdlInfo(ProMdl mdl)
{
	UsrPartPorp temp;
	temp.owner=mdl;
	IsMdlInfoDefAndDef((ProSolid)mdl);
	for(int i=0;i<(int)p_UsrPartPorp.GetCount();i++)
	{
		if(p_UsrPartPorp[i].owner==mdl)
			return p_UsrPartPorp[i];
	}
	return temp;
}
UsrPartPorp * GetMdlPInfo(ProMdl mdl)
{
	IsMdlInfoDefAndDef((ProSolid)mdl);
	for(int i=0;i<(int)p_UsrPartPorp.GetCount();i++)
	{
		if(p_UsrPartPorp[i].owner==mdl)
			return &p_UsrPartPorp[i];
	}
	return NULL;
}
/************************************************************************/
/* 获得制定模型的属性                                                   */
/************************************************************************/
void  GetMdlProp(ProSolid solid,int * indexNo)
{
	int   num=0;
	IsMdlInfoDefAndDef(solid);
	num=(int)p_UsrPartPorp.GetCount();
	for (int i=0;i<num;i++) {
		if(p_UsrPartPorp[i].owner!=(ProMdl)solid) continue;
		(*indexNo)=i;
		return;
	}
}
/************************************************************************/
/* 根据模型文件名来获得模型的属性的编号和模型的句柄                     */
/************************************************************************/
BOOL GetMdlPropWithFileName(CString filename,ProSolid * solid,int * indexNo)
{
	int      num=0;
	CString  tempName;
	num=(int)p_UsrPartPorp.GetCount();
	for (int i=0;i<num;i++)
	{
		tempName=GetMdlFileName(p_UsrPartPorp[i].owner);
		if (tempName.MakeLower()==filename.MakeLower())
		{
			(*solid)=(ProSolid)p_UsrPartPorp[i].owner;
			(*indexNo)=i;
			return TRUE;
		}
	}
	return FALSE;
}
/************************************************************************/
/* 获得模型类型                                                         */
/************************************************************************/
ProMdlType GetMdlType(ProMdl mdl)
{
	ProMdlType mdl_type;
	ProMdlTypeGet(mdl,&mdl_type);
	return mdl_type;
}
/************************************************************************/
/* 根据模型属性数组的号来的装配元件的取值范围的好                       */
/************************************************************************/
BOOL ConvertUsrPropNumToCompInsItemNum(int propNum,int * item)
{
	for (int i=0;i<(int)p_AsmCompItemList.GetCount();i++)
	{
		if (p_AsmCompItemList[i].propNum==propNum)
		{
			(*item)=i;
			return TRUE;
		}
	}
	(*item)=-1;
	return FALSE;
}
/************************************************************************/
/* 获得模型的实例信息，在给组件元件确定变型范围时用到                   */
/************************************************************************/
BOOL GetMdlInsInfo(CDatabase * p_db,int Type,long MdlNum,long InsNum,CompInsItem * item)
{
	CPartInsTableSet m_Set(p_db);
	if(item==NULL) return FALSE;
	try{
		switch(Type) {
	case ASM_MDL_PART:
		m_Set.m_strFilter.Format("[PrtNum]=%d AND [InsNum]=%d",MdlNum,InsNum);
		m_Set.Open();
		if(m_Set.IsBOF()) return FALSE;
		m_Set.MoveFirst();
		item->insNum=InsNum;
		item->insName=m_Set.m_InsName;
		item->insCode=m_Set.m_InsCode;
		item->insNote=m_Set.m_InsNote;
		return TRUE;
		break;
	case ASM_MDL_ASSEMBLY:
		break;
	default:
		break;
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		
	}
	return FALSE;
}
BOOL SetMdlInfo(ProSolid mdl,UsrPartPorp newPorp)
{
	int indexNo=-1;
	if (newPorp.owner==NULL)
	{
		newPorp.owner=mdl;
	}
	if (!IsMdlDeleted(mdl))
	{
		GetMdlProp(mdl,&indexNo);
		p_UsrPartPorp.RemoveAt(indexNo);
		p_UsrPartPorp.InsertAt(indexNo,newPorp,1);
		return TRUE;
	}	
	return FALSE;
}
