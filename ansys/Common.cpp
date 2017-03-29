#include "stdafx.h"
#include "myproe.h"

int globalSaveAs(CString path, ProMdl * pSolid);
int globalSaveAs(CString path,CString name, ProMdl *pSolid);
int globalLoadModel(CString path,ProMdl * pSolid);
int globalShowNewWindow(ProMdl * pSolid);



//显示模型
int globalShowNewWindow(ProMdl * pSolid)
{
	ProError status;

	int window_id ;
	ProName object_name ;
	ProType  object_type ;
	ProMdlNameGet(*pSolid,object_name);
	ProMdlTypeGet(*pSolid,(ProMdlType *)&object_type);
	ProObjectwindowCreate(object_name,object_type,&window_id);
	ProMdlWindowGet(*pSolid,&window_id);
	status=ProMdlDisplay(*pSolid);
	if(status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("显示失败");
		return 0;
	}
	status=ProWindowActivate(window_id);
	if(status==PRO_TK_NO_ERROR)
	{
		//AfxMessageBox("显示成功");
		return 1;
	}
	return 0;
}

int globalLoadModel(CString path,ProMdl * pSolid)
{
	ProPath fullpath;
	ProError status;
	ProStringToWstring(fullpath,path.GetBuffer());
	ProMdlType type=PRO_MDL_UNUSED ;
	status=ProMdlLoad(fullpath,type,PRO_B_FALSE,(ProMdl*)pSolid);
	if(status==PRO_TK_NO_ERROR)
	{
		//AfxMessageBox("加载成功");
		return 1;
	}
	else
	{
		AfxMessageBox("加载失败");
		return 0;
	}
}


int globalSaveAs(CString path,CString name, ProMdl *pSolid)
{

	ProError status;
	int index=name.Find(".");
	if(index!=-1)
	{
		name=name.Left(index);
	}
	ProMdldata  model_info;
	ProName newName,oldName;
	status=ProMdlNameGet(*pSolid,oldName);
	ProStringToWstring(newName,name.GetBuffer());
	status=ProMdlRename(*pSolid,newName);
	ProStringToWstring(model_info.path,path.GetBuffer());
	status=ProMdlBackup((ProMdl)*pSolid,&model_info);
	if(status==PRO_TK_NO_ERROR)
	{
		status=ProMdlRename(*pSolid,oldName);
		AfxMessageBox(name+"保存成功!");
		return 1;
	}
	return 0;
}

int globalSaveAs(CString path, ProMdl * pSolid)
{
	CString filepath,filename,totalPath;
	totalPath=path;
	int index=-1;
	index=totalPath.ReverseFind('\\');//
	filename=totalPath.Right(totalPath.GetLength()-index-1);
	filepath=totalPath.Left(index+1);
	return  globalSaveAs(filepath,filename,pSolid);

}