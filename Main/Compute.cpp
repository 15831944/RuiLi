// Compute.cpp : 实现文件
//

#include "stdafx.h"
//#include "variantdesign.h"
#include "Compute.h"
#include "MyDatabase.h"
#include <ctype.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <commctrl.h>
#include "afxtempl.h"
//#define M_PI 3.1415
//#define M_E  2.61
////
//#include "ProSolid.h"
//#include "ProWindows.h"
//#include "ProDimension.h"
//#include "ProUtil.h"
//#include "ProParameter.h"
//#include <ProParamval.h>
//#include <ProMdl.h>
//#include "ProMessage.h"
////
//#include "ProModelitem.h"
//#include "ProFeature.h"
//#include "ProAsmcomp.h"
//#include "ProFeatType.h"
//#include <ProAsmcomppath.h>
//#include "CommonFuncClass.h"


// CCompute
extern CMyDatabase * m_db;
//extern CMyDatabase m_pdm;
//void	 GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
ProError SolidFeatGetMe(ProSolid solid,ProFeature **p_data,BOOL nFlag);
ProError MeFeatureFilterActionFn(ProFeature *,ProAppData);
ProError MeFeatureVisitActionFn(ProFeature *,ProError ,ProAppData);
ProError MeFeatureFilterAction(ProFeature *p_object,ProAppData app_data); 
//ProError FeatureDimensionVisitAction(ProDimension *p_object,ProError status,ProAppData app_data) ;
//ProError FeatureDimensionFilterAction(ProDimension *p_object,ProAppData app_data) ;

/*extern */CStringArray RelationSet;
CStringArray PostIdArray;
/*extern */CArray<ProDrawing,ProDrawing>  DrawingList;
/*extern */CArray<Database,Database> Database_struc;
/*extern */CArray<Database,Database> Database_struc_temp;
//句柄结构
typedef struct JuBing
{
	ProSolid Productsolid;			   //产品句柄
	ProSolid solid;					   //零部件句柄
	ProAsmcomppath comppath;		   //零件的路径
	CString  Name;					   //零件的名字，包括后缀
	int number;		
	int upnum;      //后缀编号
}JuBing;
//CArray<JuBing,JuBing> JuBing_struc;
/*extern */CArray<JuBing,JuBing> JuBing_struc;

CCompute::CCompute()
{
	//if (!m_db->DatabaseState())
	//{
	//	m_db->ReOpen();
	//}
}

CCompute::~CCompute()
{
}

// CCompute 成员函数
/************************************************************************/
/* 从零件中得到变量和相关变量                                           */
/************************************************************************/
BOOL CCompute::ObtianFromBase(ProSolid solid,CString partnum)	 //零件句柄
{
	CString strSql,strSql1,strSql2;
	ProName w_name;
	CString name;
	Database uu;
	ProError status;
	strSql.Format("select * from partVariantTable where PrtNum=%s",partnum);
	int T=m_db->Query(strSql);
	if(T==-1) 
	{ 
		return FALSE;
	}
	if (T==0)
	{
		return TRUE;
	}
	m_db->MoveBegin();
	int j;
	CString str,str1,str2;
	do {
		str=m_db->getString("Type");
		if (str=="dim") 
		{
			uu.solid=solid;
			uu.type="dim";	
			str1=m_db->getString("Name");
			uu.name=str1;
			str1=m_db->getString("IdNum");
			str1.MakeLower();
			uu.idnum=str1;
			str1=m_db->getString("DimSymbol");
			str1.MakeLower();
			uu.paradimsymbol=str1;
			status=ProMdlNameGet((ProMdl)solid,w_name);
			name=(CString)w_name+".prt";
			ObtianPostIdFromName(JuBing_struc[0].Productsolid,name,&uu);
			//uu.PARADIMSYMBOL=uu.paradimsymbol+":"+name;
			str1=m_db->getString("SubType");
			uu.subtype=str1;
			///////
			str1=m_db->getString("Note");
			uu.note=str1;
			////////
			j=m_db->getInt("Number");
			uu.number=j;
			uu.IsOrNot=TRUE;
			uu.IsOk=FALSE;
			uu.up="0.01";
			uu.down="0.01";
			Database_struc.Add(uu);
		}
		else
			if (str=="para")
			{
				uu.solid=solid;
				uu.type="para";	
				str1=m_db->getString("Name");
				//str1.MakeLower();
				uu.name=str1;
				str1=m_db->getString("SubType");
				uu.subtype=str1;
				str1=m_db->getString("IdNum");
				str1.MakeLower();
				uu.idnum=str1;
				str1=m_db->getString("ParaFeatName");
				str1.MakeLower();
				uu.paradimsymbol=str1;
				status=ProMdlNameGet((ProMdl)solid,w_name);
				name=(CString)w_name+".prt";
				ObtianPostIdFromName(JuBing_struc[0].Productsolid,name,&uu);
				//////
				str1=m_db->getString("Note");
				uu.note=str1;
				////
				j=m_db->getInt("Number");
				uu.number=j;
				uu.IsOrNot=TRUE;
				uu.IsOk=FALSE;
				Database_struc.Add(uu);
			}
	} while(m_db->MoveNext());
	///读取相关参数的表
	strSql.Format("select * from [PartRelativeDimParaTable] where [PrtNum]=%s",partnum);
	int Z=m_db->Query(strSql);
	if (Z==-1)
	{
		AfxMessageBox("查询数据库有误");
		return FALSE;
	}
	else
		if(Z!=0)
		{
			m_db->MoveBegin();
			do {
				str=m_db->getString("Type");
				if (str=="dim") 
				{
					uu.solid=solid;
					uu.type="dim";	
					str1=m_db->getString("Name");
					uu.name=str1;
					str1=m_db->getString("IdNum");
					str1.MakeLower();
					uu.idnum=str1;
					str1=m_db->getString("DimSymbol");
					str1.MakeLower();
					uu.paradimsymbol=str1;
					status=ProMdlNameGet((ProMdl)solid,w_name);
					name=(CString)w_name+".prt";
					ObtianPostIdFromName(JuBing_struc[0].Productsolid,name,&uu);
					str1=m_db->getString("SubType");
					uu.subtype=str1;
					uu.IsOrNot=FALSE;
					uu.IsOk=FALSE;
					Database_struc.Add(uu);
				}
				else
					if (str=="para")
					{
						uu.solid=solid;
						uu.type="para";	
						str1=m_db->getString("Name");
						uu.name=str1;
						str1=m_db->getString("SubType");
						str1.MakeLower();
						uu.subtype=str1;
						str1=m_db->getString("IdNum");
						str1.MakeLower();
						uu.idnum=str1;
						str1=m_db->getString("DimSymbol");
						str1.MakeLower();
						uu.paradimsymbol=str1;
						status=ProMdlNameGet((ProMdl)solid,w_name);
						name=(CString)w_name+".prt";
						ObtianPostIdFromName(JuBing_struc[0].Productsolid,name,&uu);
						uu.IsOrNot=FALSE;
						uu.IsOk=FALSE;
						Database_struc.Add(uu);
					}
			} while(m_db->MoveNext());
		}
	return TRUE;
}
/************************************************************************/
/* 读取关系式  ,要把这个删掉                                            */
/************************************************************************/
BOOL CCompute::ReadRelation(ProSolid solid)			  //零件的模型句柄
{
	ProError status;
	ProModelitem modelitem;
	ProCharName cname;
	ProName w_name;
	ProParameter param;
	ProParamvalue p_value;
	ProLine wstr;
	CString partnum;
	CString str;
	////得到图号
	status=ProMdlToModelitem((ProMdl)solid,&modelitem);
	sprintf(cname,"所属模型");
	ProStringToWstring(w_name,cname);
	status=ProParameterInit(&modelitem,w_name,&param);
	ProParameterValueGet(&param,&p_value);
	swprintf(wstr,L"%s",p_value.value.s_val);
	partnum=CString(wstr);
	//根据模型编码查找数据库
	CString strSql;
	strSql.Format("select Relation from partRelationSet where PrtNum=%s",partnum);//前面的数据库要清空;
	int Z=m_db->Query(strSql);
	if (Z==-1)
	{
		return FALSE;
	}
	else
	if(Z!=0)
	{
		m_db->MoveBegin();
		do {
			str=m_db->getString("Relation");
			str.MakeLower();
			str.Remove(' ');
			RelationSet.Add(str);
		} while(m_db->MoveNext());
	}
	return TRUE;
}
/************************************************************************/
/* 读关系式                                                             */
/************************************************************************/
BOOL CCompute::ReadRelation(/*ProSolid solid,*/CString partnum)			  //零件的模型句柄
{
	CString str;
	//根据模型编码查找数据库
	CString strSql;
	strSql.Format("select Relation from partRelationSet where PrtNum=%s",partnum);//前面的数据库要清空;
	int Z=m_db->Query(strSql);
	if (Z==-1)
	{
		AfxMessageBox("查询数据库有误");
		return FALSE;
	}
	else
	if(Z!=0)
	{
		m_db->MoveBegin();
		do {
			str=m_db->getString("Relation");
			str.MakeLower();
			str.Remove(' ');
			RelationSet.Add(str);
		} while(m_db->MoveNext());
	}
	return TRUE;
}
/************************************************************************/
/* 更新关系式的数据                                                     */
/************************************************************************/
BOOL CCompute::UpdateAllRelation(CString str,int j)//  solid当前模型，str=变量，j是变量结构中的定位
{																  //  需要修改以更好的符合组件变型
	int i;
	int k=0;
	int h;
	CString value,X;
	double result;
	CString teststring;
	teststring=str;
	InRelation=false;
	IsGen=false;
			for (i=0;i<RelationSet.GetSize();i++)
			{
				int k=0;
				teststring=RelationSet[i];
				while (k!=-1) 
				{
					k=UpdateOneRelation(str,i,k,j);  //str要找的字符串 i关系式 k字符串的定位 j变量的定位
				} 
				continue;
			}
			for (i=0;i<RelationSet.GetSize();i++)
			{	   
				X=IsOnlyOne(i);
				teststring=X;
				if(X=="不能求")
					continue;
				else
					if(X!="结束")
					{
						result=ComputeEquation(RelationSet[i],X);   /*      求关系式 */
						value.Format("%0.2f",result);
						//AfxMessageBox(value);
						h=UpdateDim(X,value);//这个是当前的零件模型句柄
						break;
					}
			}
	if (X!="结束"&& InRelation && IsGen)
	{
		UpdateAllRelation(/*Database_struc[j].solid,*/X,h);
	}
	return TRUE;
}
/************************************************************************/
/*        替换一个表达式中所有的的已知量                                */
/************************************************************************/
int CCompute::UpdateOneRelation(CString str,int i,int h,int j)
{
	char c,d;
	CString X,teststring;
	CString value;
	teststring=str;
	///////////////////////////////////////////////////////////
			int k=RelationSet[i].Find(str,h);
			if (k==-1)
			{
				return k;
			}
			else
			{
				if (k==0)		   //说明其是该表达式的第一个
				{
					c=RelationSet[i].GetAt(str.GetLength()+k);
					if ((c=='=')|| (c=='*')|| (c=='+') || (c=='-')|| c==('/')|| (c==')') || (c=='('))
					{
						UpdateOneDim(/*solid,*/i,k,j,str); 
						teststring=RelationSet[i];
						return k+1;
					}
					else
						return k+1;
				}
				if (k+str.GetLength()==RelationSet[i].GetLength())//说明是最后一个
				{
					c=RelationSet[i].GetAt(k-1);
					if ((c=='=')|| (c=='*')|| (c=='+') || (c=='-')|| c==('/')|| (c==')') || (c=='(')) 
					{
						UpdateOneDim(/*solid,*/i,k,j,str); 
						teststring=RelationSet[i];
						return k+1;
					}
					else
					{
						return k+1;
					}	
				}
				else
				{
					c=RelationSet[i].GetAt(str.GetLength()+k);
					d=RelationSet[i].GetAt(k-1);
					if ((c=='=')|| (c=='*')|| (c=='+') || (c=='-')|| c==('/')|| (c==')') || (c=='('))
					{  
						if ((d=='=')|| (d=='*')|| (d=='+') || (d=='-')|| d==('/')|| (d==')') || (d=='(')) 
						{
							teststring=Database_struc[j].value;
							UpdateOneDim(/*solid,*/i,k,j,str); 
							teststring=RelationSet[i];
							return k+1;
						}
					}
				}
				return k+1;
			}
	return -1;
	///////////////////////////////////////////////
}
/************************************************************************/
/* 更新关系中的尺寸                                                     */
/************************************************************************/
BOOL CCompute::UpdateOneDim(/*ProSolid solid,*/int i,int k,int j,CString str) 
{
	CString tempstr,tempq,temph;
	////////////////////////////////////

			tempstr=RelationSet[i];
			InRelation=true;
			if (k==0)
			{
				tempstr.Delete(0,str.GetLength());
				temph=tempstr;
				RelationSet[i]=Database_struc[j].value+temph;
				return TRUE;
			}
			if (k+str.GetLength()==RelationSet[i].GetLength())
			{
				tempstr=RelationSet[i];
				tempq=tempstr.Left(k);
				RelationSet[i]=tempq+Database_struc[j].value;
				return TRUE;
			}
			else
			{
				tempstr=RelationSet[i];
				tempq=tempstr.Left(k);
				tempstr=RelationSet[i];
				tempstr.Delete(0,k+str.GetLength());
				temph=tempstr;
				RelationSet[i]=tempq+Database_struc[j].value+temph;
				return TRUE;
			}
    return TRUE;
	////////////////////////////////////
}
/************************************************************************/
/* 更新结构中的尺寸                                                     */
/************************************************************************/
int CCompute::UpdateDim(CString str,CString value)		 
{
	CString teststrucstring;
	IsGen=true;
	for (int j=0;j<Database_struc.GetSize();j++)
	{ 
		teststrucstring=Database_struc[j].paradimsymbol;///测试用
		teststrucstring=Database_struc[j].PARADIMSYMBOL;///测试用
		if (Database_struc[j].type=="dim")
		{
			if ((Database_struc[j].PARADIMSYMBOL==str)/* || (Database_struc[j].paradimsymbol==str)) && Database_struc[j].solid==solid*/)
			{
				teststrucstring=Database_struc[j].paradimsymbol;
				teststrucstring=Database_struc[j].PARADIMSYMBOL;
				Database_struc[j].value=value;
				Database_struc[j].IsOk=TRUE;
				teststrucstring=Database_struc[j].value;
				break;
			}

		}
		if (Database_struc[j].type=="para")
		{
			if ((Database_struc[j].PARADIMSYMBOL==str) /*|| (Database_struc[j].paradimsymbol==str)) && Database_struc[j].solid==solid*/)
			{
				teststrucstring=Database_struc[j].idnum;
				Database_struc[j].value=value;
				Database_struc[j].IsOk=TRUE;
				teststrucstring=Database_struc[j].value;
				break;
			}
		}
	}
	return j;
}
/************************************************************************/
/*     判断是否是只有一个未知数                                         */
/************************************************************************/
CString CCompute::IsOnlyOne(/*ProSolid solid,*/int i)
{
	/////////////////////////
	CString resToken,str,teststring;
	int curPos= 0;
	int j=0;
	resToken=RelationSet[i].Tokenize("()=+-*/",curPos);
	while (resToken !="")
	{

		if(iswdigit(resToken.GetAt(0))==0 && resToken!="sin" && resToken!="cos")
		{
			str=resToken;
			j++;
		}
		resToken=RelationSet[i].Tokenize("=+-*/()",curPos);
		teststring=RelationSet[i];
	}
	if (j==1) 
	{
		return str;
	}
	if (j==0)
	{
		str="结束";
		return str;
	}
	else
	{
		str="不能求";
		return str;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
/////本函数可包括sin(),cos(),log(),pow();
double CCompute::CalString(CString expression)
{
	int start,end;
	int t;
	CString str;
	CString str1,str2,str3;
	double temp=-1;
	double d1,d2;
	char c;
	try
	{
		///////////////////计算括号
		start=expression.Find('(');
		if(start!=-1)
		{
			end=expression.Find(')',start);
			if(end!=-1)
		 {
			 str=expression.Mid(0,end);
			 start=str.ReverseFind('(');

			 str=expression.Mid(start+1,end-start+1-2);
			 temp=CalString(str);
			 str.Format("%f",temp);
			 str.Replace('-','@');
			 str2=expression.Mid(start,end-start+1);

			 str1=expression.Mid(0,start);///获得前半段
			 int a1,a2;
			 a1=str1.ReverseFind('+');
			 a2=str1.ReverseFind('-');
			 if(a2>a1) a1=a2;
			 a2=str1.ReverseFind('*');
			 if(a2>a1) a1=a2;
			 a2=str1.ReverseFind('/');
			 if(a2>a1) a1=a2;
			 a2=str1.ReverseFind('(');
			 if(a2>a1) a1=a2;
			 str3=str1.Mid(a1+1);
			 if(str3=="")
				{
				}
				if(str3=="sin")
				{
					temp=sin(temp);
					str.Format("%f",temp);
					str.Replace('-','@');
					str2=str3+str2;
				}
				if(str3=="cos")
				{
					temp=cos(temp);
					str.Format("%f",temp);
					str.Replace('-','@');
					str2=str3+str2;
				}
				if(str3=="log")
				{
					temp=log(temp);
					str.Format("%f",temp);
					str.Replace('-','@');
					str2=str3+str2;
				}

				expression.Replace(str2,str);
				return CalString(expression);
		 }
			else
		 {
			 AfxMessageBox("括号不匹配!");
			 return -1;
		 }
		}
		//////////////////
		///////////////////计算乘除
		start=expression.FindOneOf("*/");
		if(start!=-1)
		{
			t=start;
			str1=expression.Tokenize("+-*/",t);
			str2=expression.Mid(0,start);/////
			t=str2.ReverseFind('+');
			end=str2.ReverseFind('-');
			if(t>end) end=t;
			str3=str2.Mid(end+1);
			c=expression.GetAt(start);
			switch(c) {
	case '*':
		str2=str3+"*"+str1;
		//str3.Replace('@','-');
		//d1=atof(str3);
		d1=CalString(str3);
		d2=CalString(str1);
		temp=d1*d2;
		str.Format("%f",temp);
		str.Replace('-','@');
		expression.Replace(str2,str);
		return CalString(expression);
		break;
	case '/':
		str2=str3+"/"+str1;
		d1=CalString(str3);
		d2=CalString(str1);
		temp=d1/d2;
		str.Format("%f",temp);
		str.Replace('-','@');
		expression.Replace(str2,str);
		return CalString(expression);
		break;
	default:break;
			}
		}

		///////////////////////
		/////////////////////////计算加减
		start=expression.FindOneOf("+-");
		if(start!=-1)
		{
			t=start;
			str1=expression.Tokenize("+-",t);
			str2=expression.Mid(0,start);/////
			t=str2.ReverseFind('+');
			end=str2.ReverseFind('-');
			if(t>end) end=t;
			str3=str2.Mid(end+1);
			c=expression.GetAt(start);
			switch(c) {
	case '+':
		str2=str3+"+"+str1;

		d1=CalString(str3);
		d2=CalString(str1);
		temp=d1+d2;
		str.Format("%f",temp);
		str.Replace('-','@');

		expression.Replace(str2,str);
		return CalString(expression);
		break;
	case '-':
		str2=str3+"-"+str1;

		d1=CalString(str3);
		d2=CalString(str1);
		temp=d1-d2;
		str.Format("%f",temp);
		str.Replace('-','@');

		expression.Replace(str2,str);
		return CalString(expression);
		break;
	default:
		break;
			}
		}
		//////////////////////////////////////
		///////////
		if(expression=="PI")
		{
			return M_PI;
		}
		if(expression=="E")
		{
			return M_E; 
		}
		expression.Replace('@','-');
		temp=atof(expression);
	}

	catch (...) {
		AfxMessageBox("出错");
		return -1;
	}
	return temp;
}
////////////////////////////////////////////////////////////////////
/**/
/////////////求解方程
double CCompute::ComputeEquation(CString expression,CString x)
{
	int start,end;
	CString left,right;
	CString str;
	str="("+x+")";
	expression.Replace(x,str);
	double dleft,dright;
	double c1,c2;
	double result;
	start=expression.Find('=');
	if(start==-1)
	{
		AfxMessageBox("不是等式");
		return -1;
	}
	end=expression.Find(x);
	if(end==-1)
	{
		AfxMessageBox("找不到未知数"+x);
		return -1;
	}
	if(end>start)
	{
		str=expression;
		left=str.Left(start);
		right=str.Right(str.GetLength()-start-1);
		str=left+"-("+right+")=0";
		return ComputeEquation(str,x);
	}
	/////////////开始计算
	str=expression;
	left=str.Left(start);
	right=str.Right(str.GetLength()-start-1);
	///////////解除函数情况
	int min=1000;
	int nIndex;
	CStringArray  label;
	label.Add("sin");
	label.Add("cos");
	label.Add("log");
	label.Add("/");
	str=expression.Left(end);
	for(int i=0;i<label.GetSize();i++)
	{
		nIndex=str.Find(label.GetAt(i));
		if((min>nIndex)&&nIndex!=-1) 
			min=nIndex;
	}
	if(min!=1000)
	{
		str=expression; 
		int n=0,rIndex;
		nIndex=min;
		rIndex=min; 
		int j;
		for(j=min;j<str.GetLength();j++)
		{
			char ch=str.GetAt(j);
			if(ch=='(')
			{
				n++;
			}
			else if(ch==')')
			{
				n--;
				if(n==0)
					break;
			}
		}
		rIndex=j;
		CString fun,sub;
		if(rIndex>end)
		{

			str=expression;
			nIndex=min;
			fun=str.Tokenize("(",nIndex);

			///////////得到c1
			sub=str.Mid(min,rIndex-min+1);
			if(fun=="/")
			{
				str.Replace(sub,"*"+sub);
			}
			c1=ComputeEquation(str,sub);
			///////////
			str=expression;

			nIndex=min+fun.GetLength()+1;
			rIndex=rIndex-min-1-fun.GetLength();
			sub=str.Mid(nIndex,rIndex);
			//////判断函数
			if(fun=="/")
			{
				c1=1/c1;
				sub.Format("%s=%f",sub,c1);
				return ComputeEquation(sub,x);
			}

			if(fun=="sin")
			{
				c1=asin(c1);
				sub.Format("%s=%f",sub,c1);
				return ComputeEquation(sub,x);
			}

			if(fun=="cos")
			{
				c1=acos(c1);
				sub.Format("%s=%f",sub,c1);
				return ComputeEquation(sub,x);
			}

			if(fun=="log")
			{
				c1=exp(c1);
				sub.Format("%s=%f",sub,c1);
				return ComputeEquation(sub,x);
			}
			// /////////////////////////////
		}
		else
		{
			CString strTemp=str;
			str=expression;
			nIndex=min;
			fun=str.Tokenize("(",nIndex);
			///////////得到c1
			str=expression;
			str=str.Mid(min,rIndex-min+1);
			if(fun=='/')
			{
				str="1"+str;/////加上1
			}
			c1=CalString(str);		 ////////////////////////////////////////////
			str.Format("%f",c1);
			str.Replace('-','@');
			if(fun=='/')
			{
				str="*"+str;////加上乘号
			}
			expression.Replace(strTemp,str);
			return ComputeEquation(expression,x);
		}
	}
	///////////////////函数计算完毕

	///////////////
	//////////////////////////
	/////分两种情况
	if(end>start)
	{
		dleft=CalString(left);
		str=right;
		str.Replace(x,"1");
		c1=CalString(str);
		str=right;
		str.Replace(x,"2");
		c2=CalString(str);
		result=(dleft-(2*c1-c2))/(c2-c1);
		return result;
	}
	else if(end<start)
	{
		dright=CalString(right);
		str=left;
		str.Replace(x,"1");
		c1=CalString(str);
		str=left;
		str.Replace(x,"2");
		c2=CalString(str);
		result=(dright-(2*c1-c2))/(c2-c1);
		return result;
	}
	else
	{
		AfxMessageBox("出错");
		return -1;
	}
}
/************************************************************************/
/* 当是产品模型的时候得到部件的下层零部件的句柄和零件路径                   */
/************************************************************************/
BOOL CCompute::ObtainJuBing(ProSolid solid)			  //当前的句柄
{
	ProError status;
	ProFeature *feature;
	ProMdl   mdl;
	ProMdlType p_type;
	CString strSql,InsCode="";
	JuBing uu;
	ProAsmcomppath comppath;
	//CCommonFuncClass commonfun;
	uu.Productsolid=solid;
	uu.solid=solid;
	post_id=0;
	uu.number=post_id;
	JuBing_struc.Add(uu);
	post_id++;
	int n,id;
	int *a=new int[1];

	SolidFeatGetMe(solid,&feature,FALSE);
	ProArraySizeGet((ProArray)feature,&n);
	for (int i=0;i<n;i++)
	{
		id=(feature+i)->id;
		status=ProAsmcompMdlGet(( ProAsmcomp*)(feature+i),&mdl);
		status=ProMdlTypeGet(mdl,&p_type);
		if (p_type==PRO_ASSEMBLY)
		{
			a[0]=id;
			ObtainJuBingFromAsm((ProSolid)mdl,&a,1,0);			 //最后一个参数表示上一级的编号，0是产品级的编号
		}
		else
			if (p_type==PRO_PART)
			{
				a[0]=id;
				status=ProAsmcomppathInit(JuBing_struc[0].Productsolid,a,1,&comppath);
				ObtainJuBingFromPart((ProSolid)mdl,&comppath,0);
			}
	}			
	delete [] a;
	return TRUE;
}
/************************************************************************/
/* 当是部件的时候得到部件的下层零部件的句柄和零件路径                   */
/************************************************************************/
void CCompute::ObtainJuBingFromAsm(ProSolid solid,int **a,int j,int upnum)		 //零部件的句柄
{
	ProError status;
	ProFeature *feature;
	ProMdl   mdl;
	ProMdlType p_type;
	ProAsmcomppath comppath;
	status=ProAsmcomppathInit(JuBing_struc[0].Productsolid,*a,j,&comppath);	   
	ObtainJuBingFromPart(solid,&comppath,upnum);
	SolidFeatGetMe(solid,&feature,FALSE);
	int n,id;
	ProArraySizeGet((ProArray)feature,&n);
	int k=(int)JuBing_struc.GetSize()-1;
	for (int i=0;i<n;i++)
	{
		status=ProAsmcompMdlGet(( ProAsmcomp*)(feature+i),&mdl);
		status=ProMdlTypeGet(mdl,&p_type);
		//
		id=(feature+i)->id;
		int *table=new int[j+1];
		for(int t=0;t<j;t++)
		{
			table[t]=*(*a+t);
		}
		table[j]=id;
		status=ProAsmcomppathInit(JuBing_struc[0].Productsolid,table,j+1,&comppath);
		//
		if (p_type==PRO_ASSEMBLY)
		{
			ObtainJuBingFromAsm((ProSolid)mdl,&table,j+1,JuBing_struc[k].number);
		}
		else
			if (p_type==PRO_PART)
			{
				ObtainJuBingFromPart((ProSolid)mdl,&comppath,JuBing_struc[k].number);
			}
			delete [] table;
	}
}
/************************************************************************/
/* 当是零件的时候得到句柄和零件路径                                     */
/************************************************************************/
void CCompute::ObtainJuBingFromPart(ProSolid solid,ProAsmcomppath *comppath,int upnum)//零件的句柄
{
	JuBing uu;
	ProError status;
	ProName  wname;
	ProMdlType p_type;
	CString name;
	CString post;
	uu.Productsolid=JuBing_struc[0].Productsolid;
	uu.solid=solid;
	status=ProMdlNameGet((ProMdl)solid,wname);
	name=CString(wname);
	status=ProMdlTypeGet((ProMdl)solid,&p_type);
    if (p_type==PRO_MDL_ASSEMBLY)
    {
		post="asm";
    }
	else
		if (p_type==PRO_MDL_PART)
		{
			post="prt";
		}
	uu.Name=name+"."+post;
	uu.comppath=*comppath;
	uu.number=post_id;
	uu.upnum=upnum;
	post_id++;
	JuBing_struc.Add(uu);
}
/************************************************************************/
/* 从模型中得到数据                                                     */
/************************************************************************/
void CCompute::ObtianValueFromMdl(ProSolid solid,int i)	 //零件句柄
{
	double up_limit,down_limit;
	CString m_up,m_down;
	ProError status;
	ProModelitem modelitem;
	ProParameter param;
	ProParamvalue p_value;
	ProLine wstr;
	ProDimension haha;
	ProName ParamName;
	CString str;
	double value;
		if ((!Database_struc[i].IsOk) && Database_struc[i].IsOrNot)
		{
				if (Database_struc[i].type=="dim")
				{
					str=Database_struc[i].idnum;
					str.Delete(0,1);
					haha.id=atoi(str.GetBuffer());
					haha.type=PRO_DIMENSION;
					haha.owner=((ProMdl)solid);
					status=ProDimensionValueGet(&haha,&value);
					str.Format("%0.2f",value);
					Database_struc[i].value=str;
					status=ProDimensionToleranceGet(&haha,&up_limit,&down_limit);
					m_up.Format("%0.3f",up_limit);
					m_down.Format("%0.3f",down_limit);
					Database_struc[i].up=m_up;
					Database_struc[i].down=m_down;
					Database_struc[i].IsOk=TRUE;
				}
				if (Database_struc[i].type=="para")
				{
					status=ProMdlToModelitem((ProMdl)Database_struc[i].solid,&modelitem);
					str=Database_struc[i].idnum;
					ProStringToWstring(ParamName,str.GetBuffer());
					status=ProParameterInit(&modelitem,ParamName,&param);
					if (status==PRO_TK_NO_ERROR)
					{
						ProParameterValueGet(&param,&p_value);
						if (Database_struc[i].subtype=="PRO_PARAM_DOUBLE")
						{
							str.Format("%0.2f",p_value.value.d_val);

						}
						if (Database_struc[i].subtype=="PRO_PARAM_INTEGER")
						{
							str.Format("%d",p_value.value.i_val);

						}
						if (Database_struc[i].subtype=="PRO_PARAM_STRING")
						{
							swprintf(wstr,L"%s",p_value.value.s_val);
							ProWstringToString(str.GetBuffer(),wstr);
						}
						if (Database_struc[i].subtype=="PRO_PARAM_BOOLEAN")
						{
							str.Format("%l",p_value.value.l_val);
						}
						Database_struc[i].value=str;
						Database_struc[i].up="";
						Database_struc[i].down="";
						Database_struc[i].IsOk=TRUE;
					}
				}
		}
}
void CCompute::ObtianToleranceFromMdl()
{
	double up_limit,down_limit;
	CString m_up,m_down;
	ProDimension haha;
	CString str;
	double value;
	for (int i=0;i<Database_struc.GetSize();i++)
	{
		if (Database_struc[i].type=="dim")
		{
			str=Database_struc[i].idnum;
			str.Delete(0,1);
			haha.id=atoi(str.GetBuffer());
			haha.type=PRO_DIMENSION;
			haha.owner=((ProMdl)Database_struc[i].solid);
			ProDimensionValueGet(&haha,&value);
			str.Format("%0.2f",value);
			Database_struc[i].value=str;
			ProDimensionToleranceGet(&haha,&up_limit,&down_limit);
			m_up.Format("%0.3f",up_limit);
			m_down.Format("%0.3f",down_limit);
			Database_struc[i].up=m_up;
			Database_struc[i].down=m_down;
			Database_struc[i].IsOk=FALSE;
		}
	}
}
/************************************************************************/
/* 根据组件的图号获得组件的变量，相关变量                               */
/************************************************************************/
int CCompute::ObtianAsmFromBaseTemp(ProSolid solid,CString AsmNum)//当前组件的图号
{
	CString strSql,strSql1,strSql2,teststr;
	Database uu;
	strSql.Format("select * from AsmVariantTable where AsmNum=%s",AsmNum);
	int Z=m_db->Query(strSql);
	if (Z==-1)
	{
		AfxMessageBox("查询数据库有误");
		return -1;
	}
	if(Z==0) 
	{ 
		return 0;
	}
	m_db->MoveBegin();
	int j,t;
	int p=0;
	CString str,str1,str2,FileName;
	do {
		str=m_db->getString("Type");
		if (str=="dim") 
		{
				uu.type="dim";	
				str1=m_db->getString("Name");
				//str1.MakeLower();
				uu.name=str1;
				str1=m_db->getString("IdNum");
				str1.MakeLower();
				t=str1.Find(':');
				uu.idnum=str1.Left(t);
				str1=m_db->getString("DimSymbol");
				str1.MakeLower();
				uu.PARADIMSYMBOL=str1;
				t=str1.Find(':');
				uu.paradimsymbol=str1.Left(t);
				str1=m_db->getString("SubType");
				uu.subtype=str1;
				///////
				str1=m_db->getString("Note");
				uu.note=str1;
				////////
				j=m_db->getInt("Number");
				uu.number=j;
				uu.IsOrNot=TRUE;
				uu.IsOk=FALSE;
			    Database_struc_temp.Add(uu);
				p++;
		}
		else
			if (str=="para")
			{
					uu.type="para";	
					str1=m_db->getString("Name");
					//str1.MakeLower();
					uu.name=str1;
					str1=m_db->getString("SubType");
					uu.subtype=str1;
					str1=m_db->getString("IdNum");
					str1.MakeLower();
					uu.idnum=str1;
					str1=m_db->getString("ParaFeatName");
					str1.MakeLower();
					uu.PARADIMSYMBOL=str1;
					t=str1.Find(':');
					uu.paradimsymbol=str1.Left(t);
					//////
					str1=m_db->getString("Note");
					uu.note=str1;
					////
					j=m_db->getInt("Number");
					uu.number=j;
					uu.IsOrNot=TRUE;
					uu.IsOk=FALSE;
					Database_struc_temp.Add(uu);
					p++;
			}
	} while(m_db->MoveNext());
	return p;
}
/************************************************************************/
/* 从组件中获得关系式                                                   */
/************************************************************************/
BOOL CCompute::ReadRelationFromAsm(ProSolid solid,CString AsmNum)       //应该是产品的句柄
{
	CString str;
	CString strSql;
	strSql.Format("select * from AsmRelationTable where AsmNum='%s'",AsmNum);
	int Z=m_db->Query(strSql);
	if (Z==-1)
	{
		AfxMessageBox("查询数据库有误");
		return FALSE;
	}
	if(Z==0)
	{
		return TRUE;
	}
	m_db->MoveBegin();
	do {
		str=m_db->getString("Relation");
		str.MakeLower();
		str.Remove(' ');
		RelationSet.Add(str);
	} while(m_db->MoveNext());
	ObtainPostIdFromRelation();
	return TRUE;
}
///************************************************************************/
///* 获得零件的句柄，在更新结构中用到                                     */
///************************************************************************/
//ProSolid CCompute::ObtainJuBingFromPart(CString variant,int t)	 //t是：的位置
//{
//	CString str,name;
//	ProSolid solid;
//	str=variant;
//	str.Delete(0,t+1);
//	name=str;
//	for (int i=0;i<JuBing_struc.GetSize();i++)
//	{
//		if (JuBing_struc[i].Name==name)
//		{
//		   solid=JuBing_struc[i].solid;
//		   break;
//		}
//	}
//	return solid;
//}
/************************************************************************/
/* 组件重生函数                                                         */
/************************************************************************/
BOOL CCompute::AsmGeneration(ProSolid solid)//这是产品的句柄
{
	ProError status;
	ProDimension haha;
	ProName ParamName;
	ProModelitem modelitem;
	ProMdlType p_type;
	ProMdl mdl;
	CString str;
	ProLine line;
	ProParameter param;
	ProParamvalue p_value;
	int i_value;
	double d_value;
	double value;
	short  l_value;

	for (int j=0;j<Database_struc.GetSize();j++)
	{
		if (Database_struc[j].type=="dim")
		{
			str=Database_struc[j].idnum;
			str.Delete(0,1);
			haha.id=atoi(str.GetBuffer());
			haha.type=PRO_DIMENSION;
			haha.owner=Database_struc[j].solid;
			value=atof(Database_struc[j].value.GetBuffer());
			ProDimensionValueSet(&haha,atof(Database_struc[j].value.GetBuffer()));
			status=ProDimensionToleranceSet(&haha,atof(Database_struc[j].up.GetBuffer()),atof(Database_struc[j].down.GetBuffer()));
		}
		if (Database_struc[j].type=="para")
		{
			//solid=Database_struc[j].solid;
			status=ProMdlToModelitem((ProMdl)Database_struc[j].solid,&modelitem);
			str=Database_struc[j].name;
			ProStringToWstring(ParamName,str.GetBuffer());
			status=ProParameterInit(&modelitem,ParamName,&param);
			status=ProParameterValueGet(&param,&p_value);
			if (Database_struc[j].subtype=="PRO_PARAM_DOUBLE")
			{
				d_value=atof(Database_struc[j].value);
				status=ProParamvalueSet(&p_value,(void*)&d_value,PRO_PARAM_DOUBLE);
			}
			else
				if (Database_struc[j].subtype=="PRO_PARAM_INTEGER")
				{
					i_value=atoi(Database_struc[j].value);   
					status=ProParamvalueSet(&p_value,(void*)&i_value,PRO_PARAM_INTEGER);
				}
				else
					if (Database_struc[j].subtype=="PRO_PARAM_STRING")
					{
						ProStringToWstring(line,Database_struc[j].value.GetBuffer());
						status=ProParamvalueSet(&p_value,(void*)line,PRO_PARAM_STRING);
					}
					else
						if (Database_struc[j].subtype=="PRO_PARAM_BOOLEAN")
						{
							l_value=atoi(Database_struc[j].value);
							status=ProParamvalueSet(&p_value,(void*)l_value,PRO_PARAM_BOOLEAN);
						}
						else 
							AfxMessageBox("您的参数类型不正确!");
			status=ProParameterValueSet (&param,&p_value);
		}
	}
	ProMdlTypeGet((ProMdl)solid,&p_type);
	if (p_type==PRO_MDL_PART)
	{
		 ProSolidRegenerate(solid,PRO_B_TRUE);
		 //重生二维模型
		 DrawingList.RemoveAll();
		 GetSolidDrawing(solid,&DrawingList);
		 for (int i=0;i<DrawingList.GetSize();i++)
		 {
			 ProSolidRegenerate((ProSolid)DrawingList[i],PRO_B_TRUE);
		 }
	}
	else
		if (p_type==PRO_MDL_ASSEMBLY)
		{
			for (int i=0;i<JuBing_struc.GetSize();i++)
			{
				ProSolidRegenerate(JuBing_struc[i].solid,PRO_B_TRUE);
				DrawingList.RemoveAll();
				GetSolidDrawing(solid,&DrawingList);
				for (int j=0;j<DrawingList.GetSize();j++)
				{
					ProSolidRegenerate((ProSolid)DrawingList[j],PRO_B_TRUE);
				}
			}
		}
	ProWindowRepaint(PRO_VALUE_UNUSED);
	ProMdlCurrentGet(&mdl);
	status = ProWindowActivate(PRO_VALUE_UNUSED);
	return TRUE;

}
/************************************************************************/
/* 把以零件编号为后缀的变量转换成以零件名为后缀的变量 ,不要了           */
/************************************************************************/
BOOL CCompute::ObtianNameFromPostId(CString partnum,Database *uu)		  //前者是组件的图号
{
	int k;
	CString sql,str,teststr,strtemp;
		k=uu->PARADIMSYMBOL.Find(':');
		if (k!=-1 && iswdigit(uu->PARADIMSYMBOL.GetAt(k+1))!=0)
		{
			strtemp=uu->PARADIMSYMBOL;
			strtemp.Delete(0,k+1);
			str=strtemp;
			sql.Format("SELECT * FROM AsmMakeupTable WHERE AsmNum=%s AND PartNum=%s",partnum,str);
			int Z=m_db->Query(sql);
			if (Z==-1)
			{
				AfxMessageBox("查询数据库有误");
				return FALSE;
			}
			if(Z==0)
			{
				return TRUE;
			}
			else
			{
				m_db->MoveBegin();
				do {
					str=m_db->getString("FileName");
				} while(m_db->MoveNext());
				for (int j=0;j<JuBing_struc.GetSize();j++)
				{
					teststr=JuBing_struc[j].Name;
					if (str==JuBing_struc[j].Name)	  
					{
						uu->solid=JuBing_struc[j].solid;
						uu->PARADIMSYMBOL=uu->paradimsymbol+":"+str;
						break;
					}
				}
			}
		}
		else
		{
			AfxMessageBox("无法句柄！");
			return FALSE;
		}
    return TRUE;
}
///************************************************************************/
///* 是否和当前的模型属于同一个产品的 ,可以不要了                         */
///************************************************************************/
//BOOL CCompute::IsTheSameProduct(ProSolid Productsolid,ProSolid solid)
//{
//	ProSolid Asmsolid;
//	for (int j=0;j<JuBing_struc.GetSize();j++)
//	{
//		if (solid==JuBing_struc[j].solid)
//		{
//			Asmsolid=JuBing_struc[j].Productsolid;
//			if (Asmsolid==Productsolid)
//			{
//				return TRUE;
//			}
//			else
//			{
//				return FALSE;
//			}
//			break;
//		}
//	}
//	return FALSE;
//}
/************************************************************************/
/* 为零件关系式添加0后缀                                                */
/************************************************************************/
CString CCompute::AlterRelationFromPost(CString Relation)
{
	int curPos=0;
	CString str,teststring;
	CString resToken=Relation.Tokenize("()=+-*/",curPos);
	while (resToken !="")
	{
		if(isdigit(resToken.GetAt(0))==0 && resToken!="sin" && resToken!="cos")
		{
			str=resToken;
			str.Format(":%d",0);
			Relation.Insert(curPos-1,str);
			curPos=curPos+2;
		}
		resToken=Relation.Tokenize("=+-*/()",curPos);
		teststring=Relation;
	}
	return Relation;
}
/************************************************************************/
/* 得到当前产品的所有变量和相关变量                                     */
/************************************************************************/
BOOL CCompute::ObtainAllVariantAndRelativeVariant(ProSolid Productsolid,CString ProductNum)		  //这个是产品的句柄
{
	 ProError status;
	 ProMdlType p_type;
	 //CCommonFuncClass commonfun;
	 CString partnum;
	 for (int i=0;i<JuBing_struc.GetSize();i++)
	 {
		partnum=ObtainModelNum(JuBing_struc[i].solid);
		if (partnum!="-1")   //说明是模型
	    {
			status=ProMdlTypeGet((ProMdl)JuBing_struc[i].solid,&p_type);
			if (p_type==PRO_MDL_ASSEMBLY)
			{
				int n_dim=(int)Database_struc.GetSize();
				if(ObtianAsmFromBase(JuBing_struc[i].solid,partnum)==-1)
				{
					return FALSE;	//当前组件的句柄
				}
				ModifyPostId(JuBing_struc[i].solid,ProductNum,partnum,n_dim);
			}
			else
				if (p_type==PRO_MDL_PART)
				{
					ObtianFromBase(JuBing_struc[i].solid,partnum);
				}
	    }
	 }
	 IsAlreadyHave();
	 return TRUE;
}
/************************************************************************/
/* 去掉产品中重复的句柄，以防重复保存（只在入库的时候用）               */
/************************************************************************/
BOOL CCompute::IsJuBingAlreadyHave()
{
	BOOL IsOk=TRUE;  
	for (int i=0;i<JuBing_struc.GetSize();i++)
	{
		for (int j=i+1;j<JuBing_struc.GetSize();j++)
		{
			if(JuBing_struc[j].solid==JuBing_struc[i].solid)
			{
				IsOk=FALSE;
				JuBing_struc.RemoveAt(i);
				break;
			}
		}
		if (!IsOk)
		{
			break;
		}  
	}
	if (!IsOk)
	{
		IsJuBingAlreadyHave();
	}
	else
	{
		return TRUE;
	}
	return TRUE;
}
/************************************************************************/
/* 判断Database_struc是否已经有存在了                                   */
/************************************************************************/
BOOL CCompute::IsAlreadyHave()
{
	BOOL IsOk=TRUE;  
	for (int i=0;i<Database_struc.GetSize();i++)
	  {
		  for (int j=i+1;j<Database_struc.GetSize();j++)
		  {
			  if(Database_struc[j].PARADIMSYMBOL==Database_struc[i].PARADIMSYMBOL)
			  {
				  IsOk=FALSE;
				  Database_struc.RemoveAt(j);
				  break;
			  }
		  }
		  if (!IsOk)
		  {
			  break;
		  }  
	  }
	  if (!IsOk)
	  {
		  IsAlreadyHave();
	  }
	  else
	  {
		  return TRUE;
	  }
	  return TRUE;
}
/************************************************************************/
/* 改变子组件的变量的后缀和句柄                                         */
/************************************************************************/
BOOL CCompute::ModifyPostId(ProSolid solid,CString Asmnum,CString partnum,int n_dim) //1.当前句柄2.产品图号3.组件图号4.这个装配体之前的变量个数
{
	CString sql,str,str1;
	int PostId,PostIdtemp;
	if (solid==JuBing_struc[0].Productsolid)//说明是最顶层组件，不用变换后缀，否则变换后缀
	{
		for (int i=0;i<Database_struc.GetSize();i++)
		{
			int k=Database_struc[i].PARADIMSYMBOL.Find(":");
			str=Database_struc[i].PARADIMSYMBOL;
			str.Delete(0,k+1);
			int PostId=atoi(str);
			for (int j=0;j<JuBing_struc.GetSize();j++)
			{
				if (PostId==JuBing_struc[j].number)
				{
					Database_struc[i].solid=JuBing_struc[j].solid;
					break;
				}
			}
		}
	}
	else
	{
		sql.Format("SELECT * FROM AsmMakeupTable WHERE AsmNum=%s AND PartMdlNum=%s",Asmnum,partnum);
		int Z=m_db->Query(sql);
		if(Z==-1)
		{
			AfxMessageBox("无法得到后缀！");
			return FALSE;
		}
		else
			if(Z!=0)
		{
			m_db->MoveBegin();
			do {
				PostId=m_db->getInt("PartNum");		//如果有一样的取第一个,这是当前装配体在产品中的编号
				break;
			} while(m_db->MoveNext());
		}
		for (int i=n_dim;i<Database_struc.GetSize();i++)
		{
			int k=Database_struc[i].PARADIMSYMBOL.Find(":");
			str=Database_struc[i].PARADIMSYMBOL;
			str.Delete(0,k+1);
			PostIdtemp=atoi(str);
			PostId=PostId+PostIdtemp;							//两个后缀值相加
			str1.Format("%d",PostId);
			str=Database_struc[i].PARADIMSYMBOL.Left(k);
			Database_struc[i].PARADIMSYMBOL=str+":"+str1;
			for (int j=0;j<JuBing_struc.GetSize();j++)
			{
				if (PostId==JuBing_struc[j].number)
				{
					Database_struc[i].solid=JuBing_struc[j].solid;
					break;
				}
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 改变子组件的变量的后缀和句柄（临时）                                 */
/************************************************************************/
BOOL CCompute::ModifyPostIdTemp(ProSolid solid,CString Asmnum,CString partnum,int n_dim) //1.当前组件句柄2.产品图号3.组件图号4.这个装配体之前的变量个数
{
	CString sql,str,str1;
	int PostId,PostIdtemp;
	if (solid==JuBing_struc[0].Productsolid)//说明是最顶层组件，不用变换后缀，否则变换后缀
	{
		for (int i=n_dim;i<Database_struc_temp.GetSize();i++)
		{
			int k=Database_struc_temp[i].PARADIMSYMBOL.Find(":");
			str=Database_struc_temp[i].PARADIMSYMBOL;
			str.Delete(0,k+1);
			int PostId=atoi(str);
			for (int j=0;j<JuBing_struc.GetSize();j++)
			{
				if (PostId==JuBing_struc[j].number)
				{
					Database_struc_temp[i].solid=JuBing_struc[j].solid;
					break;
				}
			}
		}
	}
	else
	{
		sql.Format("SELECT * FROM AsmMakeupTable WHERE AsmNum=%s AND PartMdlNum=%s",Asmnum,partnum);
		int Z=m_db->Query(sql);
		if(Z==-1)
		{
			AfxMessageBox("无法得到后缀！");
			return FALSE;
		}
		else
			if(Z!=0)
		{
			m_db->MoveBegin();
			do {
				PostId=m_db->getInt("PartNum");		//如果有一样的取第一个,这是当前装配体在产品中的编号
				break;
			} while(m_db->MoveNext());
		}
		for (int i=n_dim;i<Database_struc_temp.GetSize();i++)
		{
			int k=Database_struc_temp[i].PARADIMSYMBOL.Find(":");
			str=Database_struc_temp[i].PARADIMSYMBOL;
			str.Delete(0,k+1);
			PostIdtemp=atoi(str);
			PostId=PostId+PostIdtemp;							//两个后缀值相加
			str1.Format("%d",PostId);
			str=Database_struc_temp[i].PARADIMSYMBOL.Left(k);
			Database_struc[i].PARADIMSYMBOL=str+":"+str1;
			for (int j=0;j<JuBing_struc.GetSize();j++)
			{
				if (PostId==JuBing_struc[j].number)
				{
					Database_struc_temp[i].solid=JuBing_struc[j].solid;
					break;
				}
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 得到所有的后缀                                                       */
/************************************************************************/
void CCompute::ObtainPostIdFromRelation()
{
	CString Relation,str;
	PostIdArray.RemoveAll();
	for (int i=0;i<RelationSet.GetSize();i++)
	{
		Relation=RelationSet[i];
		int cur=0;
		int tempCur1=0;
		CString sql,str;
		do {
			cur=tempCur1;
			tempCur1=Relation.Find(':',cur);
			CString postID_temp;
			if (tempCur1!=-1)
			{
				int tempCur2=tempCur1;
				postID_temp=Relation.Tokenize(":+-*/()=<>{}[]^%",tempCur2);//得到的后缀	
				str.Format("%d",postID_temp);
				PostIdArray.Add(str);
				tempCur1=tempCur1+1;
			}
		} while(tempCur1!=-1);
	}
}
/************************************************************************/
/* 根据零件的名字得到后缀                                               */
/************************************************************************/
void CCompute::ObtianPostIdFromName(ProSolid solid,CString name,Database *uu)//产品句柄
{
	CString str;
	ProMdlType p_type;
	ObtainPostIdFromRelation();
	ProMdlTypeGet(solid,&p_type);
	if (p_type==PRO_MDL_PART)
	{
		str.Format("%d",0);  
		uu->PARADIMSYMBOL=uu->paradimsymbol+":"+str;
	}
	else
		if (p_type=PRO_MDL_ASSEMBLY)
		{
			for (int i=0;i<JuBing_struc.GetSize();i++)
			{
				JuBing_struc[i].Name.MakeLower();
				name.MakeLower();
				if (JuBing_struc[i].Name==name)
				{
					str.Format("%d",JuBing_struc[i].number);
					for(int j=0;j<PostIdArray.GetSize();j++)		   //PostIdArray是什么时候得到的
					{
						if (str==PostIdArray[j])
						{
							break;
						}
					}
				}
				uu->PARADIMSYMBOL=uu->paradimsymbol+":"+str;
			}
		}
}
/************************************************************************/
/* 得到当前零件的主变量函数,临时值,在没有打开模型的时候用               */
/************************************************************************/
BOOL CCompute::ObtianPartFromBaseTemp(CString partnum)		 //当前句柄
{
	CString strSql,strSql1,strSql2;
	CString name;
	Database uu;
	Database_struc_temp.RemoveAll();
	strSql.Format("select * from partVariantTable where PrtNum=%s",partnum);
	int Z=m_db->Query(strSql);
	if(Z==-1) 
	{ 
		return FALSE;
	}
	if(Z==0) 
	{ 
		return TRUE;
	}
	m_db->MoveBegin();
	int j;
	int t=0;
	CString str,str1,str2;
	do {
		str=m_db->getString("Type");
		if (str=="dim") 
		{
			uu.type="dim";	
			str1=m_db->getString("Name");
			uu.name=str1;
			str1=m_db->getString("IdNum");
			str1.MakeLower();
			uu.idnum=str1;
			str1=m_db->getString("DimSymbol");
			str1.MakeLower();
			
			uu.paradimsymbol=str1;
			uu.PARADIMSYMBOL=str1+":0";

			str1=m_db->getString("SubType");
			uu.subtype=str1;
			///////
			str1=m_db->getString("Note");
			uu.note=str1;
			////////
			j=m_db->getInt("Number");
			uu.number=j;
			uu.IsOrNot=TRUE;
			uu.IsOk=FALSE;
			Database_struc_temp.Add(uu);
			t++;
		}
		else
			if (str=="para")
			{
				uu.type="para";	
				str1=m_db->getString("Name");
				uu.name=str1;
				str1=m_db->getString("SubType");
				uu.subtype=str1;
				str1=m_db->getString("IdNum");
				str1.MakeLower();
				uu.idnum=str1;
				str1=m_db->getString("ParaFeatName");
				str1.MakeLower();
				uu.paradimsymbol=str1;
				uu.PARADIMSYMBOL=str1+":0";
				//////
				str1=m_db->getString("Note");
				uu.note=str1;
				////
				j=m_db->getInt("Number");
				uu.number=j;
				uu.IsOrNot=TRUE;
				uu.IsOk=FALSE;
				Database_struc_temp.Add(uu);
				t++;
			}
	} while(m_db->MoveNext());
	return TRUE;
}
/************************************************************************/
/* 得到当前产品的主变量函数,临时值,在没有打开模型的时候用               */
/************************************************************************/
BOOL CCompute::ObtianAsmFromBaseTemp(CString AsmNum)		 //当前句柄
{
	CString strSql,strSql1,strSql2,teststr;
	Database uu;
	strSql.Format("select * from AsmVariantTable where AsmNum=%s",AsmNum);
	int Z=m_db->Query(strSql);
	if(Z==-1) 
	{ 
		return FALSE;
	}
	if(Z==0) 
	{ 
		return TRUE;
	}
	m_db->MoveBegin();
	int j,t;
	CString str,str1,str2,FileName;
	do {
		str=m_db->getString("Type");
		if (str=="dim") 
		{
			uu.type="dim";	
			str1=m_db->getString("Name");
			//str1.MakeLower();
			uu.name=str1;
			str1=m_db->getString("IdNum");
			str1.MakeLower();
			t=str1.Find(':');
			uu.idnum=str1.Left(t);
			str1=m_db->getString("DimSymbol");
			str1.MakeLower();
			uu.PARADIMSYMBOL=str1;
			t=str1.Find(':');
			uu.paradimsymbol=str1.Left(t);
			str1=m_db->getString("SubType");
			uu.subtype=str1;
			///////
			str1=m_db->getString("Note");
			uu.note=str1;
			////////
			j=m_db->getInt("Number");
			uu.number=j;
			uu.IsOrNot=TRUE;
			uu.IsOk=FALSE;
			Database_struc_temp.Add(uu);
		}
		else
			if (str=="para")
			{
				uu.type="para";	
				str1=m_db->getString("Name");
				//str1.MakeLower();
				uu.name=str1;
				str1=m_db->getString("SubType");
				uu.subtype=str1;
				str1=m_db->getString("IdNum");
				str1.MakeLower();
				uu.idnum=str1;
				str1=m_db->getString("ParaFeatName");
				str1.MakeLower();
				uu.PARADIMSYMBOL=str1;
				t=str1.Find(':');
				uu.paradimsymbol=str1.Left(t);
				//////
				str1=m_db->getString("Note");
				uu.note=str1;
				////
				j=m_db->getInt("Number");
				uu.number=j;
				uu.IsOrNot=TRUE;
				uu.IsOk=FALSE;
				Database_struc_temp.Add(uu);
			}
	} while(m_db->MoveNext());
	return TRUE;
}
/************************************************************************/
/* 得到当前零件的主变量函数,临时值                                      */
/************************************************************************/

int CCompute::ObtianFromBaseTemp(ProSolid solid,CString partnum)		 //当前句柄
{
	CString strSql,strSql1,strSql2;
	ProName w_name;
	CString name;
	Database uu;
	ProError status;
	Database_struc_temp.RemoveAll();
	strSql.Format("select * from partVariantTable where PrtNum=%s",partnum);
	int Z=m_db->Query(strSql);
	if(Z==-1) 
	{ 
		return FALSE;
	}
	if(Z==0) 
	{ 
		return TRUE;
	}
	m_db->MoveBegin();
	int j;
	int t=0;
	CString str,str1,str2;
	do {
		str=m_db->getString("Type");
		if (str=="dim") 
		{
			uu.solid=solid;
			uu.type="dim";	
			str1=m_db->getString("Name");
			uu.name=str1;
			str1=m_db->getString("IdNum");
			str1.MakeLower();
			uu.idnum=str1;
			str1=m_db->getString("DimSymbol");
			str1.MakeLower();
			uu.paradimsymbol=str1;

			status=ProMdlNameGet((ProMdl)solid,w_name);
			name=(CString)w_name+".prt";
			ObtianPostIdFromName(JuBing_struc[0].Productsolid,name,&uu);

			str1=m_db->getString("SubType");
			uu.subtype=str1;
			///////
			str1=m_db->getString("Note");
			uu.note=str1;
			////////
			j=m_db->getInt("Number");
			uu.number=j;
			uu.IsOrNot=TRUE;
			uu.IsOk=FALSE;
			Database_struc_temp.Add(uu);
			t++;
		}
		else
			if (str=="para")
			{
				uu.solid=solid;
				uu.type="para";	
				str1=m_db->getString("Name");
				//str1.MakeLower();
				uu.name=str1;
				str1=m_db->getString("SubType");
				uu.subtype=str1;
				str1=m_db->getString("IdNum");
				str1.MakeLower();
				uu.idnum=str1;
				str1=m_db->getString("ParaFeatName");
				str1.MakeLower();
				uu.paradimsymbol=str1;
				status=ProMdlNameGet((ProMdl)solid,w_name);
				name=(CString)w_name+".prt";
				ObtianPostIdFromName(JuBing_struc[0].Productsolid,name,&uu);
				//////
				str1=m_db->getString("Note");
				uu.note=str1;
				////
				j=m_db->getInt("Number");
				uu.number=j;
				uu.IsOrNot=TRUE;
				uu.IsOk=FALSE;
				Database_struc_temp.Add(uu);
			    t++;
			}
	} while(m_db->MoveNext());
	return t;
}
/************************************************************************/
/*得到当前组件的主变量函数                                       */
/************************************************************************/
BOOL CCompute::ObtianAsmFromBase(ProSolid solid,CString AsmNum)//当前组件的图号和句柄
{
	CString strSql,strSql1,strSql2,teststr;;
	Database uu;
	int t,T;
	strSql.Format("select * from AsmVariantTable where AsmNum=%s",AsmNum);
	T=m_db->Query(strSql);
	if(T==-1) 
	{ 
		AfxMessageBox("对不起，没有你要的数据！");
		return FALSE;
	}
	if (T==0)
	{
		return TRUE;
	}
	m_db->MoveBegin();
	int j;
	CString str,str1,str2,FileName;
	do {
		str=m_db->getString("Type");
		if (str=="dim") 
		{
			uu.type="dim";	
			str1=m_db->getString("Name");
			//str1.MakeLower();
			uu.name=str1;
			str1=m_db->getString("IdNum");
			str1.MakeLower();
			t=str1.Find(':');
			uu.idnum=str1.Left(t);
			str1=m_db->getString("DimSymbol");
			str1.MakeLower();
			uu.PARADIMSYMBOL=str1;
			t=str1.Find(':');
			uu.paradimsymbol=str1.Left(t);
			str1=m_db->getString("SubType");
			uu.subtype=str1;
			///////
			str1=m_db->getString("Note");
			uu.note=str1;
			////////
			j=m_db->getInt("Number");
			uu.number=j;
			uu.IsOrNot=TRUE;
			uu.IsOk=FALSE;
			Database_struc.Add(uu);	
		}
		else
			if (str=="para")
			{
				uu.type="para";	
				str1=m_db->getString("Name");
				//str1.MakeLower();
				uu.name=str1;
				str1=m_db->getString("SubType");
				uu.subtype=str1;
				str1=m_db->getString("IdNum");
				str1.MakeLower();
				uu.idnum=str1;
				str1=m_db->getString("ParaFeatName");
				str1.MakeLower();
				uu.PARADIMSYMBOL=str1;
				t=str1.Find(':');
				uu.paradimsymbol=str1.Left(t);
				//////
				str1=m_db->getString("Note");
				uu.note=str1;
				////
				j=m_db->getInt("Number");
				uu.number=j;
				uu.IsOrNot=TRUE;
				uu.IsOk=FALSE;
				Database_struc.Add(uu);
			}
	} while(m_db->MoveNext());
	///读取相关参数的表
	strSql.Format("select * from [AsmRelativeDimParaTable] where [ProductMdlNum]=%s",AsmNum);
	int Z=m_db->Query(strSql);
	if(Z==-1) 
	{ 
		return FALSE;
	}
	else
	if(Z!=0)
	{
		m_db->MoveBegin();
		do {
			str=m_db->getString("Type");
			if (str=="dim") 
			{
				uu.type="dim";	
				str1=m_db->getString("Name");
				str1.MakeLower();
				uu.name=str1;
				str1=m_db->getString("IdNum");
				str1.MakeLower();
				t=str1.Find(':');
				uu.idnum=str1.Left(t);
				str1=m_db->getString("DimSymbol");
				str1.MakeLower();
				uu.PARADIMSYMBOL=str1;
				t=str1.Find(':');
				uu.paradimsymbol=str1.Left(t);
				str1=m_db->getString("SubType");
				uu.subtype=str1;
				uu.IsOrNot=FALSE;
				uu.IsOk=FALSE;
				Database_struc.Add(uu);
			}
			else
			{
				if (str=="para")
				{
					uu.type="para";	
					str1=m_db->getString("Name");
					str1.MakeLower();
					uu.name=str1;
					str1=m_db->getString("SubType");
					str1.MakeLower();
					uu.subtype=str1;
					str1=m_db->getString("IdNum");
					str1.MakeLower();
					uu.idnum=str1;
					str1=m_db->getString("ParaFeatName");
					str1.MakeLower();
					uu.PARADIMSYMBOL=str1;
					t=str1.Find(':');
					uu.paradimsymbol=str1.Left(t);
					uu.IsOrNot=FALSE;
					uu.IsOk=FALSE;
					Database_struc.Add(uu);
				}
			}
		} while(m_db->MoveNext());
	}
	return TRUE;
}
/************************************************************************/
/* 得到当前模型的尺寸数值                                               */
/************************************************************************/
void CCompute::ObtianValueFromMdlTemp(ProSolid solid)	 //组件的句柄
{
	ProError status;
	double up_limit,down_limit;
	CString m_up,m_down;
	ProModelitem modelitem;
	ProParameter param;
	ProParamvalue p_value;
	ProLine wstr;
	ProDimension haha;
	ProName ParamName;
	CString str;
	double value;
	for (int i=0;i<Database_struc_temp.GetSize();i++)
	{
		if (Database_struc_temp[i].IsOrNot)
		{
			if (Database_struc_temp[i].type=="dim")
			{
				str=Database_struc_temp[i].idnum;
				str.Delete(0,1);
				haha.id=atoi(str.GetBuffer());
				haha.type=PRO_DIMENSION;
				haha.owner=((ProMdl)Database_struc_temp[i].solid);
				status=ProDimensionValueGet(&haha,&value);
				str.Format("%0.2f",value);
				Database_struc_temp[i].value=str;
				status=ProDimensionToleranceGet(&haha,&up_limit,&down_limit);
				m_up.Format("%0.3f",up_limit);
				m_down.Format("%0.3f",down_limit);
				Database_struc_temp[i].up=m_up;
				Database_struc_temp[i].down=m_down;
			}
			if (Database_struc_temp[i].type=="para")
			{
				status=ProMdlToModelitem((ProMdl)solid,&modelitem);
				str=Database_struc_temp[i].idnum;
				ProStringToWstring(ParamName,str.GetBuffer());
				status=ProParameterInit(&modelitem,ParamName,&param);
				if (status==PRO_TK_NO_ERROR)
				{
					ProParameterValueGet(&param,&p_value);
					if (Database_struc_temp[i].subtype=="PRO_PARAM_DOUBLE")
					{
						str.Format("%0.2f",p_value.value.d_val);

					}
					if (Database_struc_temp[i].subtype=="PRO_PARAM_INTEGER")
					{
						str.Format("%d",p_value.value.i_val);

					}
					if (Database_struc_temp[i].subtype=="PRO_PARAM_STRING")
					{
						swprintf(wstr,L"%s",p_value.value.s_val);
						ProWstringToString(str.GetBuffer(),wstr);
					}
					if (Database_struc_temp[i].subtype=="PRO_PARAM_BOOLEAN")
					{
						str.Format("%l",p_value.value.l_val);
					}
					Database_struc_temp[i].value=str;
					Database_struc_temp[i].up="";
					Database_struc_temp[i].down="";
				}
			}
		}
	}
}
/************************************************************************/
/* 零件重生接口                                                           */
/************************************************************************/
BOOL CCompute::Interface(ProSolid solid,CString partnum,int InsNum)
{
	CString sql;
	int number;
	CString value;
	ProSolid ProductSolid;
	Database_struc.RemoveAll();
	RelationSet.RemoveAll();
	JuBing_struc.RemoveAll();
	JuBing uu;
	uu.Productsolid=solid;				  //我想应该在打开的时候用；
	uu.solid=solid;
	JuBing_struc.Add(uu);
	//得到主变量和相关变量
    if(!ObtianFromBase(solid,partnum))
	{
		return FALSE;
	}
	if(!ReadRelation(partnum))
	{
		 return FALSE;
	}	
	for (int i=0;i<RelationSet.GetSize();i++)
	{
		RelationSet[i]=AlterRelationFromPost(RelationSet[i]);
	}
	//读数据库中的数据
	sql.Format("SELECT * FROM partInsValueTable WHERE PrtNum=%s AND InsNum=%d",partnum,InsNum);
	int Z=m_db->Query(sql);
	if(Z==-1) 
	{ 
		return FALSE;
	}
	if(Z==0) 
	{ 
		return TRUE;
	}
	else
	{
		m_db->MoveBegin();
		do {
			number=m_db->getInt("Num");
			value=m_db->getString("Value");
			for(int i=0;i<Database_struc.GetSize();i++)
			{
				if (Database_struc[i].number==number && Database_struc[i].IsOrNot)
				{
					  Database_struc[i].value=value;
					  break;
				}
			}
		} while(m_db->MoveNext());
	}
	//替换关系式
	for (int j=0;j<Database_struc.GetSize();j++)
	{
		if (Database_struc[j].IsOrNot)
		{
			UpdateAllRelation(Database_struc[j].PARADIMSYMBOL,j);
		}
	}
	//验证关系式
	for (int j=0;j<RelationSet.GetSize();j++)
	{
		if(IsOnlyOne(j)!="结束")
		{
			AfxMessageBox("你建的关系式有误，无法为你计算！");
			return FALSE;
		}
	}
	//重生
	for (int i=0;JuBing_struc.GetSize();i++)
	{
		if (solid==JuBing_struc[i].solid)
		{
			ProductSolid=JuBing_struc[i].Productsolid;
			break;
		}
	}
	AsmGeneration(ProductSolid);
	return TRUE;
}
CString CCompute::ObtainModelNum(ProSolid solid)
{
	ProError status;
	ProModelitem modelitem;
	ProCharName cname;
	ProName w_name;
	ProParameter param;
	ProParamvalue p_value;
	ProLine wstr;
	CString partnum;
	////得到模型号
	status=ProMdlToModelitem((ProMdl)solid,&modelitem);
	sprintf(cname,"所属模型");
	ProStringToWstring(w_name,cname);
	status=ProParameterInit(&modelitem,w_name,&param);
	if (status!=PRO_TK_NO_ERROR)
	{
		partnum=="-2";					 //如果参数中没有模型编号则返回-2
		return partnum;
	}
	status=ProParameterValueGet(&param,&p_value);
	if (status!=PRO_TK_NO_ERROR)
	{
		partnum=="-2";
		return partnum;
	}
	swprintf(wstr,L"%s",p_value.value.s_val);
	partnum=CString(wstr);
	return partnum;
}
/************************************************************************/
/* 组件重生接口                                                           */
/************************************************************************/
BOOL CCompute::AsmInterface(ProSolid solid,CString ProductNum,int InsNum)			//要重生的句柄，模型号，实例号
{
	CString sql;
	int number;
	CString value;
	Database_struc.RemoveAll();					//清空尺寸结构信息
	Database_struc_temp.RemoveAll();            //清空临时尺寸结构信息
	RelationSet.RemoveAll();				    //清空关系信息
	JuBing_struc.RemoveAll();
	//得到主变量和相关变量
	if(!ObtainJuBing(solid))   
	{
		return FALSE;
	}
	IsJuBingAlreadyHave();
	//读入关系
	if(!ReadRelationFromAsm(solid,ProductNum))
	{
		AfxMessageBox("没有该模型的关系式！");
		return FALSE;
	}
	//读入所有可变信息的数据
	if(!ObtainAllVariantAndRelativeVariant(solid,ProductNum))
	{
		AfxMessageBox("无法得到可变量");
		return FALSE;
	}
	//读产品可变量
	if(!ObtianAsmFromBaseTemp(solid,ProductNum))	   //要读产品的数据
	{
		AfxMessageBox("无法显示当前模型的可变参数！");
		return FALSE;
	}
	//ObtianValueFromMdlTemp(solid);
	//数据库中的数据
	sql.Format("SELECT * FROM AsmInsVariantTable WHERE AsmNum=%s AND InsNum=%d",ProductNum,InsNum);
	int Z=m_db->Query(sql);
	if (Z==-1)
	{
		AfxMessageBox("数据库出错");
		return FALSE;
	}
	else
		if (Z!=0)
	{
		m_db->MoveBegin();
		do {
			number=m_db->getInt("Num");
			value=m_db->getString("Value");
			for(int i=0;i<Database_struc_temp.GetSize();i++)
			{
				if (Database_struc_temp[i].number==number)
				{
					Database_struc_temp[i].value=value;
					break;
				}
			}
		} while(m_db->MoveNext());
	}
	//把临时变量里的值赋给正式的变量
	for (int i=0;i<Database_struc_temp.GetSize();i++)
	{
		for (int j=0;j<Database_struc.GetSize();j++)
		{ 
			if (Database_struc[j].PARADIMSYMBOL==Database_struc_temp[i].PARADIMSYMBOL) 
			{
				Database_struc[j].value=Database_struc_temp[i].value;
				Database_struc[j].up="0.2";
				Database_struc[j].down="0.1";
				Database_struc[j].IsOk=TRUE;
				break;
			}
		}
	}
	//替换关系式
	for (int j=0;j<Database_struc.GetSize();j++)
	{
		if (Database_struc[j].IsOrNot)
		{
			UpdateAllRelation(Database_struc[j].PARADIMSYMBOL,j);
		}
	}
	//再获得零件中没有计算出来的，但是主变量的值
	for (int i=0;i<Database_struc.GetSize();i++)
	{
		if (Database_struc[j].IsOrNot && (!Database_struc[j].IsOk))
		{
			ObtianValueFromMdl(Database_struc[i].solid,i);
		}
	}
	//根据以上获得的值算出其余的值
	for (int j=0;j<Database_struc.GetSize();j++)
	{
		if (Database_struc[j].IsOrNot && Database_struc[j].IsOk)
		{
			UpdateAllRelation(Database_struc[j].PARADIMSYMBOL,j);
		}
	}
	//验证关系式
	for (int j=0;j<RelationSet.GetSize();j++)
	{
		if(IsOnlyOne(j)!="结束")
		{
			AfxMessageBox("你建的关系式有误，无法为你计算！");
			return FALSE;
		}
	}
	//重生
	AsmGeneration(solid);
	return TRUE;
}
ProError SolidFeatGetMe(ProSolid solid,ProFeature **p_data,BOOL nFlag)
{
	ProError status;
	if( p_data != NULL )
	{
		status=ProArrayAlloc(0,sizeof(ProFeature),1,(ProArray*)p_data);
		if( status == PRO_TK_NO_ERROR )
		{  //调用尺寸对象访问函数
			if(nFlag)
			{
				status=ProSolidFeatVisit (solid,
					(ProFeatureVisitAction) MeFeatureVisitActionFn,
					(ProFeatureFilterAction)MeFeatureFilterAction,(ProAppData)&p_data);  
			}
			else
			{
				status=ProSolidFeatVisit (solid,
					(ProFeatureVisitAction) MeFeatureVisitActionFn,
					(ProFeatureFilterAction)MeFeatureFilterActionFn,(ProAppData)&p_data);  
			}
		}
		else
		{
			ProArrayFree( (ProArray*)p_data );
			*p_data = NULL;
		}
	}
	else
		status = PRO_TK_BAD_INPUTS;
	return (status);
}
/************************************************************************/
/*        特征访问动作函数                                              */
/************************************************************************/
ProError MeFeatureVisitActionFn(ProFeature *p_object,ProError status,ProAppData app_data)
{
	ProArray *p_array;
	p_array=(ProArray*)((ProFeature**)app_data)[0];
	status=ProArrayObjectAdd(p_array,PRO_VALUE_UNUSED,1,p_object);
	return status;
}
/************************************************************************/
/*       特征过滤函数                                                   */
/************************************************************************/
ProError MeFeatureFilterActionFn(ProFeature *feature,ProAppData app_data)
{
	ProBoolean isVisible;
	ProFeatureVisibilityGet(feature,&isVisible);
	ProError status;
	ProFeattype ftype;
	/*----------------------------------------------------------------*\
	Get the feature type
	\*----------------------------------------------------------------*/
	status = ProFeatureTypeGet (feature, &ftype);
	if (ftype == PRO_FEAT_COMPONENT && isVisible)
		return (PRO_TK_NO_ERROR);
	return (PRO_TK_CONTINUE);
	//status = ProFeatureTypeGet (feature,&ftype);
	//if (ftype==PRO_FEAT_COMPONENT)
	//	return (PRO_TK_NO_ERROR);
	//else
	//	return (PRO_TK_CONTINUE);
}
/*=========================================================================*\
Function:FeatureFilterAction() 特征对象访问函数的过滤函数定义
\*=========================================================================*/
ProError MeFeatureFilterAction(ProFeature *p_object,
							 ProAppData app_data)  
{   
	ProBoolean isVisible;
	ProFeatureVisibilityGet(p_object,&isVisible);
	if (!isVisible)
		return (PRO_TK_CONTINUE);
	else
		return (PRO_TK_NO_ERROR);
}
/*================================================================*\
FUNCTION: SgCollectDim() 
自定义函数，用来获得当前模型的尺寸对象指针，并保存在指针数组中
\*================================================================*/
//ProError  SgCollectDim(ProSolid solid,			//(In)当前模型
//					   ProDimension **p_data)	//(In)尺寸对象数组
//{
//	ProError status;
//	if( p_data != NULL )
//	{
//		status=ProArrayAlloc(0,sizeof(ProDimension),1,(ProArray*)p_data);
//		if( status == PRO_TK_NO_ERROR )
//		{  //调用尺寸对象访问函数
//			status=ProSolidDimensionVisit (solid,PRO_B_FALSE,
//				(ProDimensionVisitAction)FeatureDimensionVisitAction,
//				(ProDimensionFilterAction)FeatureDimensionFilterAction,(ProAppData)&p_data);  
//		}
//		else
//		{
//			ProArrayFree( (ProArray*)p_data );
//			*p_data = NULL;
//		}
//
//	}
//	else
//		status = PRO_TK_BAD_INPUTS;
//	return (status);
//
//}
/*================================================================*\
FUNCTION: FeatureCollectDim()
自定义函数，用来获得当前特征的尺寸对象指针，并保存在指针数组中
\*================================================================*/
//ProError  FeatureCollectDim(ProFeature feature,			//(In)当前模型
//							ProDimension **p_data)	
//{
//	ProError status;
//	if( p_data != NULL )
//	{
//		status=ProArrayAlloc(0,sizeof(ProDimension),1,(ProArray*)p_data);
//		if( status == PRO_TK_NO_ERROR )
//		{  //调用尺寸对象访问函数
//			ProFeatureDimensionVisit(&feature,(ProDimensionVisitAction)FeatureDimensionVisitAction,
//				(ProDimensionFilterAction)FeatureDimensionFilterAction,(ProAppData)&p_data);
//		}
//		else
//		{
//			ProArrayFree( (ProArray*)p_data );
//			*p_data = NULL;
//		}
//
//	}
//	else
//		status = PRO_TK_BAD_INPUTS;
//	return (status);
//
//}
/*=========================================================================*\
Function:	FeatureDimensionVisitAction() 尺寸对象访问函数的动作函数定义
\*=========================================================================*/
/*ProError FeatureDimensionVisitAction(ProDimension *p_object,ProError status,
									 ProAppData app_data)  

{   
	ProArray *p_array;
	p_array = (ProArray*)((ProDimension**)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1,p_object );
	return (status);
}*/
/*=========================================================================*\
Function: FeatureDimensionFilterAction() 尺寸对象访问函数的过滤函数定义
\*=========================================================================*/
//ProError FeatureDimensionFilterAction(ProDimension *p_object,
//									  ProAppData app_data)  
//{   
//	return (PRO_TK_NO_ERROR);
//}