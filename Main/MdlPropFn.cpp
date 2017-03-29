#include "stdafx.h"
#include "resource.h"
#include "PartInsTableSet.h"

/************************************************************************/
/* ���һ��ģ�͵Ļ�����Ϣ�Ƿ��Ѿ�����,���û������                    */
/************************************************************************/
void  IsMdlInfoDefAndDef(ProSolid mdl)
{
	int           num=0;
	int           indexNo=-1;  //����ı��
	UsrPartPorp   temp;
	ProMdldata    data;
	ProMdlType    type;
	CString       csType;       //�ļ�����չ�� .prt .asm
	for(int i=0;i<(int)p_UsrPartPorp.GetCount();i++)
	{
		//ͨ���ж��������ж�ģ���Ƿ񻹴���
		if(IsMdlDeleted(p_UsrPartPorp[i].owner))
		{                                       //������ǰ�Ľ�����������֮ǰҪ�����Ԫ�����ͷ�Χ�����޸�
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
		//�ж�ģ�͵Ĺ����Ƿ�ɾ��
		if (p_UsrPartPorp[i].bHaveDraw && IsMdlDeleted(p_UsrPartPorp[i].hDraw))
		{
			p_UsrPartPorp[i].bHaveDraw=false;
			p_UsrPartPorp[i].hDraw=NULL;
		}	
		//�ж�ͼƬ�Ƿ񻹴���,�����������Ϊ��
		if (!PathFileExists(p_UsrPartPorp[i].pic_path))
			p_UsrPartPorp[i].pic_path="";
		//�Ѿ����壬������,����
		if(p_UsrPartPorp[i].owner==mdl)
			return;
	}
	//����û�ж���Ľ��ж���
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

		temp.MdlNum   =-1;//û��ģ��
		temp.MdlType  =-1;//û�ж�������
		temp.MdlName  =_T("��ģ��");

		temp.Code     ="";		
		temp.bHaveDraw     = false;
		temp.hDraw         = NULL;
		temp.Person        ="";
		temp.Material      ="";
		temp.Note          ="";
		temp.pic_path      ="";
		temp.IsAsmComp     =false;  //Ĭ�ϲ���װ����Ĳ���
		temp.IsAudited     =false;  //Ĭ��Ϊû��ͨ�����
		temp.ResourceType  =0;      //Ĭ����Դ�Ǳ���
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
/* ����ƶ�ģ�͵�����                                                   */
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
/* ����ģ���ļ��������ģ�͵����Եı�ź�ģ�͵ľ��                     */
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
/* ���ģ������                                                         */
/************************************************************************/
ProMdlType GetMdlType(ProMdl mdl)
{
	ProMdlType mdl_type;
	ProMdlTypeGet(mdl,&mdl_type);
	return mdl_type;
}
/************************************************************************/
/* ����ģ����������ĺ�����װ��Ԫ����ȡֵ��Χ�ĺ�                       */
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
/* ���ģ�͵�ʵ����Ϣ���ڸ����Ԫ��ȷ�����ͷ�Χʱ�õ�                   */
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
