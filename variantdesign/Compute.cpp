// Compute.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "Compute.h"
#include "MyDatabase.h"
#include <ctype.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <commctrl.h>
#include "afxtempl.h"
////
#include "ProSolid.h"
#include "ProWindows.h"
#include "ProDimension.h"
#include "ProUtil.h"
#include "ProParameter.h"
#include <ProParamval.h>
#include <ProMdl.h>
#include "ProMessage.h"
////
#include "ProModelitem.h"
#include "ProFeature.h"
#include "ProAsmcomp.h"
#include "ProFeatType.h"
#include <ProAsmcomppath.h>
#include "CommonFuncClass.h"
#include "MyUtil.h"
#include "DetailInfoClass.h"


// CCompute
extern CMyDatabase m_db;
//extern CMyDatabase m_pdm;
void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
ProError SolidFeatGet(ProSolid solid,ProFeature **p_data,BOOL nFlag);
ProError FeatureFilterActionFn(ProFeature *,ProAppData);
ProError FeatureVisitActionFn(ProFeature *,ProError ,ProAppData);
ProError FeatureFilterAction(ProFeature *p_object,ProAppData app_data); 
ProError FeatureDimensionVisitAction(ProDimension *p_object,ProError status,ProAppData app_data) ;
ProError FeatureDimensionFilterAction(ProDimension *p_object,ProAppData app_data) ;

extern CStringArray RelationSet;
CStringArray PostIdArray;
extern CPtrArray p_classarray;
extern CStringArray RelationSetTemp;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CArray<Database,Database> Database_struc;
extern CArray<Database,Database> Database_struc_temp;
extern CArray<JuBing,JuBing> JuBing_struc;

CCompute::CCompute()
{
	if (!m_db.DatabaseState())
	{
		m_db.ReOpen();
	}
}

CCompute::~CCompute()
{
}

// CCompute ��Ա����
/************************************************************************/
/* ������еõ���������ر���                                           */
/************************************************************************/
BOOL CCompute::ObtainFromBase(ProSolid solid,CString partnum)	 //������
{
	CString strSql,strSql1,strSql2;
	ProName w_name;
	CString name;
	Database uu;
	ProError status;
	strSql.Format("select * from partVariantTable where PrtNum=%s",partnum);
	int T=m_db.Query(strSql);
	if(T==-1) 
	{ 
		return FALSE;
	}
	if (T==0)
	{
		return TRUE;
	}
	m_db.MoveBegin();
	int j;
	CString str,str1,str2;
	do {
		uu.Asmsolid=solid;
		str=m_db.getString("Type");
		if (str=="dim") 
		{
			uu.solid=solid;
			uu.type="dim";	
			str1=m_db.getString("Name");
			uu.name=str1;
			str1=m_db.getString("IdNum");
			str1.MakeLower();
			uu.idnum=str1;
			str1=m_db.getString("DimSymbol");
			str1.MakeLower();
			uu.paradimsymbol=str1;
			status=ProMdlNameGet((ProMdl)solid,w_name);
			name=(CString)w_name+".prt";
			str1=m_db.getString("SubType");
			uu.subtype=str1;
			///////
			str1=m_db.getString("Note");
			uu.note=str1;
			////////
			j=m_db.getInt("Number");
			uu.number=j;
			uu.ModelNum=partnum;
			uu.MdlType="prt";
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
				str1=m_db.getString("Name");
				uu.name=str1;
				str1=m_db.getString("SubType");
				uu.subtype=str1;
				str1=m_db.getString("IdNum");
				str1.MakeLower();
				uu.idnum=str1;
				str1=m_db.getString("ParaFeatName");
				str1.MakeLower();
				uu.paradimsymbol=str1;
				status=ProMdlNameGet((ProMdl)solid,w_name);
				name=(CString)w_name+".prt";
				//////
				str1=m_db.getString("Note");
				uu.note=str1;
				////
				j=m_db.getInt("Number");
				uu.number=j;
				uu.ModelNum=partnum;
				uu.MdlType="prt";
				uu.IsOrNot=TRUE;
				uu.IsOk=FALSE;
				Database_struc.Add(uu);
			}
	} while(m_db.MoveNext());
	///��ȡ��ز����ı�
	strSql.Format("select * from [PartRelativeDimParaTable] where [PrtNum]=%s",partnum);
	int Z=m_db.Query(strSql);
	if (Z==-1)
	{
		AfxMessageBox("��ѯ���ݿ�����");
		return FALSE;
	}
	else
		if(Z!=0)
		{
			m_db.MoveBegin();
			do {
				uu.Asmsolid=solid;
				str=m_db.getString("Type");
				if (str=="dim") 
				{
					uu.solid=solid;
					uu.type="dim";	
					str1=m_db.getString("Name");
					uu.name=str1;
					str1=m_db.getString("IdNum");
					str1.MakeLower();
					uu.idnum=str1;
					str1=m_db.getString("DimSymbol");
					str1.MakeLower();
					uu.paradimsymbol=str1;
					status=ProMdlNameGet((ProMdl)solid,w_name);
					name=(CString)w_name+".prt";
					str1=m_db.getString("SubType");
					uu.subtype=str1;
					uu.ModelNum=partnum;
					uu.MdlType="prt";
					uu.IsOrNot=FALSE;
					uu.IsOk=FALSE;
					Database_struc.Add(uu);
				}
				else
					if (str=="para")
					{
						uu.solid=solid;
						uu.type="para";	
						str1=m_db.getString("Name");
						uu.name=str1;
						str1=m_db.getString("SubType");
						str1.MakeLower();
						uu.subtype=str1;
						str1=m_db.getString("IdNum");
						uu.idnum=str1;
						str1=m_db.getString("DimSymbol");
						uu.paradimsymbol=str1;
						status=ProMdlNameGet((ProMdl)solid,w_name);
						name=(CString)w_name+".prt";
						uu.ModelNum=partnum;
						uu.MdlType="prt";
						uu.IsOrNot=FALSE;
						uu.IsOk=FALSE;
						Database_struc.Add(uu);
					}
			} while(m_db.MoveNext());
		}
	return TRUE;
}
/************************************************************************/
/* ����ϵʽ                                                             */
/************************************************************************/
BOOL CCompute::ReadRelation(CString partnum)			  //�����ģ�;��
{
	CString str;
	//����ģ�ͱ���������ݿ�
	CString strSql;
	strSql.Format("select Relation from partRelationSet where PrtNum=%s",partnum);//ǰ������ݿ�Ҫ���;
	int Z=m_db.Query(strSql);
	if (Z==-1)
	{
		AfxMessageBox("��ѯ���ݿ�����");
		return FALSE;
	}
	else
	if(Z!=0)
	{
		m_db.MoveBegin();
		do {
			str=m_db.getString("Relation");
			str.MakeLower();
			str.Remove(' ');
			RelationSet.Add(str);
		} while(m_db.MoveNext());
	}
	return TRUE;
}
/************************************************************************/
/* ���¹�ϵʽ������                                                     */
/************************************************************************/
BOOL CCompute::UpdateAllRelation(CString str,int j)//  solid��ǰģ�ͣ�str=������j�Ǳ����ṹ�еĶ�λ
{																  //  ��Ҫ�޸��Ը��õķ����������
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
			k=UpdateOneRelation(str,i,k,j);  //strҪ�ҵ��ַ��� i��ϵʽ k�ַ����Ķ�λ j�����Ķ�λ
		} 
		continue;
	}
	for (i=0;i<RelationSet.GetSize();i++)
	{	   
		X=IsOnlyOne(i);
		teststring=X;
		if(X=="������")
			continue;
		else
			if(X!="����")
			{
				result=ComputeEquation(RelationSet[i],X);   /*      ���ϵʽ */
				value.Format("%0.2f",result);
				//AfxMessageBox(value);
				h=UpdateDim(X,value);//����ǵ�ǰ�����ģ�;��
				break;
		   }
	}

	if (X!="����"&& InRelation && IsGen)
	{
		UpdateAllRelation(X,h);
	}
	return TRUE;
}
/************************************************************************/
/*        �滻һ�����ʽ�����еĵ���֪��                                */
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
				if (k==0)		   //˵�����Ǹñ��ʽ�ĵ�һ��
				{
					c=RelationSet[i].GetAt(str.GetLength()+k);
					if ((c=='=')|| (c=='*')|| (c=='+') || (c=='-')|| c==('/')|| (c==')') || (c=='('))
					{
						UpdateOneDim(i,k,j,str); 
						teststring=RelationSet[i];
						return k+1;
					}
					else
						return k+1;
				}
				if (k+str.GetLength()==RelationSet[i].GetLength())//˵�������һ��
				{
					c=RelationSet[i].GetAt(k-1);
					if ((c=='=')|| (c=='*')|| (c=='+') || (c=='-')|| c==('/')|| (c==')') || (c=='(')) 
					{
						UpdateOneDim(i,k,j,str); 
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
							UpdateOneDim(i,k,j,str); 
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
/* ���¹�ϵ�еĳߴ�                                                     */
/************************************************************************/
BOOL CCompute::UpdateOneDim(int i,int k,int j,CString str) 
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
/* ���½ṹ�еĳߴ�                                                     */
/************************************************************************/
int CCompute::UpdateDim(CString str,CString value)		 
{
	CString teststrucstring;
	IsGen=true;
	for (int j=0;j<Database_struc.GetSize();j++)
	{ 
		teststrucstring=Database_struc[j].paradimsymbol;///������
		teststrucstring=Database_struc[j].PARADIMSYMBOL;///������
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
/*     �ж��Ƿ���ֻ��һ��δ֪��                                         */
/************************************************************************/
CString CCompute::IsOnlyOne(int i)
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
		str="����";
		return str;
	}
	else
	{
		str="������";
		return str;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
/////�������ɰ���sin(),cos(),log(),pow();
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
		///////////////////��������
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

			 str1=expression.Mid(0,start);///���ǰ���
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
			 AfxMessageBox("���Ų�ƥ��!");
			 return -1;
		 }
		}
		//////////////////
		///////////////////����˳�
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
		/////////////////////////����Ӽ�
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
		AfxMessageBox("����");
		return -1;
	}
	return temp;
}
////////////////////////////////////////////////////////////////////
/**/
/////////////��ⷽ��
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
		AfxMessageBox("���ǵ�ʽ");
		return -1;
	}
	end=expression.Find(x);
	if(end==-1)
	{
		AfxMessageBox("�Ҳ���δ֪��"+x);
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
	/////////////��ʼ����
	str=expression;
	left=str.Left(start);
	right=str.Right(str.GetLength()-start-1);
	///////////����������
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

			///////////�õ�c1
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
			//////�жϺ���
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
			///////////�õ�c1
			str=expression;
			str=str.Mid(min,rIndex-min+1);
			if(fun=='/')
			{
				str="1"+str;/////����1
			}
			c1=CalString(str);		 ////////////////////////////////////////////
			str.Format("%f",c1);
			str.Replace('-','@');
			if(fun=='/')
			{
				str="*"+str;////���ϳ˺�
			}
			expression.Replace(strTemp,str);
			return ComputeEquation(expression,x);
		}
	}
	///////////////////�����������

	///////////////
	//////////////////////////
	/////���������
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
		AfxMessageBox("����");
		return -1;
	}
}

BOOL CCompute::ObtainToleranceFromMdl()
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
	return TRUE;
}
/************************************************************************/
/* ������л�ù�ϵʽ                                                   */
/************************************************************************/
BOOL CCompute::ReadRelationFromAsm(ProSolid solid,CString AsmNum)       //Ӧ���ǲ�Ʒ�ľ��
{
	CString str;
	CString strSql;
	strSql.Format("select * from AsmRelationTable where AsmNum='%s'",AsmNum);
	int Z=m_db.Query(strSql);
	if (Z==-1)
	{
		AfxMessageBox("��ѯ���ݿ�����");
		return FALSE;
	}
	if(Z==0)
	{
		return TRUE;
	}
	m_db.MoveBegin();
	do {
		str=m_db.getString("Relation");
		str.MakeLower();
		str.Remove(' ');
		RelationSet.Add(str);
	} while(m_db.MoveNext());
	ObtainPostIdFromRelation();
	return TRUE;
}
/************************************************************************/
/* Ϊ�����ϵʽ���0��׺                                                */
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
/* �õ���ǰ��Ʒ�����б�������ر���                                     */
/************************************************************************/
BOOL CCompute::ObtainAllVariantAndRelativeVariant(ProSolid Productsolid,CString ProductNum)		  //����ǲ�Ʒ�ľ��
{
	 ProError status;
	 ProMdlType p_type;
	 CCommonFuncClass commonfun;
	 CString partnum;
	 for (int i=0;i<JuBing_struc.GetSize();i++)
	 {
		if (partnum!="-1")   //˵����ģ��
	    {
			status=ProMdlTypeGet((ProMdl)JuBing_struc[i].solid,&p_type);
			if (p_type==PRO_MDL_ASSEMBLY)
			{
				int n_dim=(int)Database_struc.GetSize();
				if(!ObtainAsmFromBase(JuBing_struc[i].solid,partnum))
				{
					return FALSE;	//��ǰ����ľ��
				}
				ModifyPostId(JuBing_struc[i].solid,ProductNum,partnum,n_dim);
			}
			else
				if (p_type==PRO_MDL_PART)
				{
					if(!ObtainFromBase(JuBing_struc[i].solid,partnum))
					{
						return FALSE;
					}
				}
	    }
	 }
	 IsVariantAlreadyHave();
	 return TRUE;
}
/************************************************************************/
/* �ı�������ı����ĺ�׺�;��                                         */
/************************************************************************/
BOOL CCompute::ModifyPostId(ProSolid solid,CString Asmnum,CString partnum,int n_dim) //1.��ǰ���2.��Ʒͼ��3.���ͼ��4.���װ����֮ǰ�ı�������
{
	CString sql,str,str1;
	int PostId,PostIdtemp;
	if (solid==JuBing_struc[0].Productsolid)//˵���������������ñ任��׺������任��׺
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
		int Z=m_db.Query(sql);
		if(Z==-1)
		{
			AfxMessageBox("�޷��õ���׺��");
			return FALSE;
		}
		else
			if(Z!=0)
		{
			m_db.MoveBegin();
			do {
				PostId=m_db.getInt("PartNum");		//�����һ����ȡ��һ��,���ǵ�ǰװ�����ڲ�Ʒ�еı��
				break;
			} while(m_db.MoveNext());
		}
		for (int i=n_dim;i<Database_struc.GetSize();i++)
		{
			int k=Database_struc[i].PARADIMSYMBOL.Find(":");
			str=Database_struc[i].PARADIMSYMBOL;
			str.Delete(0,k+1);
			PostIdtemp=atoi(str);
			PostId=PostId+PostIdtemp;							//������׺ֵ���
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
/* �ı�������ı����ĺ�׺�;������ʱ��                                 */
/************************************************************************/
BOOL CCompute::ModifyPostIdTemp(ProSolid solid,CString Asmnum,CString partnum,int n_dim) //1.��ǰ������2.��Ʒͼ��3.���ͼ��4.���װ����֮ǰ�ı�������
{
	CString sql,str,str1;
	int PostId,PostIdtemp;
	if (solid==JuBing_struc[0].Productsolid)//˵���������������ñ任��׺������任��׺
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
		int Z=m_db.Query(sql);
		if(Z==-1)
		{
			AfxMessageBox("�޷��õ���׺��");
			return FALSE;
		}
		else
			if(Z!=0)
		{
			m_db.MoveBegin();
			do {
				PostId=m_db.getInt("PartNum");		//�����һ����ȡ��һ��,���ǵ�ǰװ�����ڲ�Ʒ�еı��
				break;
			} while(m_db.MoveNext());
		}
		for (int i=n_dim;i<Database_struc_temp.GetSize();i++)
		{
			int k=Database_struc_temp[i].PARADIMSYMBOL.Find(":");
			str=Database_struc_temp[i].PARADIMSYMBOL;
			str.Delete(0,k+1);
			PostIdtemp=atoi(str);
			PostId=PostId+PostIdtemp;							//������׺ֵ���
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
/* �õ����еĺ�׺                                                       */
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
				postID_temp=Relation.Tokenize(":+-*/()=<>{}[]^%",tempCur2);//�õ��ĺ�׺	
				str.Format("%d",postID_temp);
				PostIdArray.Add(str);
				tempCur1=tempCur1+1;
			}
		} while(tempCur1!=-1);
	}
}
/************************************************************************/
/* ��������ӿ�                                                           */
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
	uu.Productsolid=solid;				  //����Ӧ���ڴ򿪵�ʱ���ã�
	uu.solid=solid;
	JuBing_struc.Add(uu);
	//�õ�����������ر���
    if(!ObtainFromBase(solid,partnum))
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
	//�����ݿ��е�����
	sql.Format("SELECT * FROM partInsValueTable WHERE PrtNum=%s AND InsNum=%d",partnum,InsNum);
	int Z=m_db.Query(sql);
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
		m_db.MoveBegin();
		do {
			number=m_db.getInt("Num");
			value=m_db.getString("Value");
			for(int i=0;i<Database_struc.GetSize();i++)
			{
				if (Database_struc[i].number==number && Database_struc[i].IsOrNot)
				{
					  Database_struc[i].value=value;
					  break;
				}
			}
		} while(m_db.MoveNext());
	}
	//�滻��ϵʽ
	for (int j=0;j<Database_struc.GetSize();j++)
	{
		if (Database_struc[j].IsOrNot)
		{
			UpdateAllRelation(Database_struc[j].PARADIMSYMBOL,j);
		}
	}
	//��֤��ϵʽ
	for (int j=0;j<RelationSet.GetSize();j++)
	{
		if(IsOnlyOne(j)!="����")
		{
			AfxMessageBox("�㽨�Ĺ�ϵʽ�����޷�Ϊ����㣡");
			return FALSE;
		}
	}
	//����
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

/************************************************************************/
/* ��������ӿ�                                                           */
/************************************************************************/
BOOL CCompute::AsmInterface(int s)			//Ҫ�����ľ����ģ�ͺţ�ʵ����
{
	CCompute compute;
	CCommonFuncClass commonfun;
	BOOL IsOk=TRUE;
	CString str;
	Database_struc_temp.RemoveAll();
	if(!compute.ObtainAsmFromBaseTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum))
	{
		AfxMessageBox("�޷��õ���ģ�͵Ŀɱ�ߴ�");
		return FALSE;
	}

	//�õ���ϵʽ
	RelationSet.RemoveAll();
	//����˶�����
	RelationSet.Copy(RelationSetTemp);
	for (int j=0;j<Database_struc.GetSize();j++)
	{
		str=Database_struc[j].oldvalue;
		Database_struc[j].value=Database_struc[j].oldvalue;       //����ʧ��ʹ��ԭ��
		Database_struc[j].IsOk=FALSE;
	}
	//�õ�������
	for (int i=0;i<Database_struc_temp.GetSize();i++)
	{
		for (int j=0;j<Database_struc.GetSize();j++)
		{
			str=Database_struc[j].PARADIMSYMBOL;
			str=Database_struc_temp[i].PARADIMSYMBOL;
			if (Database_struc[j].PARADIMSYMBOL==Database_struc_temp[i].PARADIMSYMBOL)
			{
				Database_struc[j].IsOk=TRUE;
			}
		}
	}
	//�õ��������������ֵ
	if (!compute.GetValueFromMdl())
	{
		return FALSE;
	}
	//�����б��������ݼ���
	for (int i=0;i<Database_struc.GetSize();i++)
	{
		if (Database_struc[i].IsOk)
		{
			compute.UpdateAllRelation(Database_struc[i].PARADIMSYMBOL,i);
		}
	}
	//�ж��Ƿ��Ѿ�ȫ����������
	for (int j=0;j<RelationSet.GetSize();j++)
	{
		if(compute.IsOnlyOne(j)!="����")
		{
			IsOk=FALSE;
			break;
		}
	} 
	if (!IsOk)
	{
		return FALSE;
	}
	if(!AsmGeneration(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
	{
		return FALSE;
	}
	return TRUE;
}

/************************************************************************/
/* ���������ͼ�Ż������ı�������ر���                               */
/************************************************************************/
BOOL CCompute::ObtainAsmFromBaseTemp(ProSolid solid,CString AsmNum)//��ǰ�����ͼ��
{
	CString strSql,strSql1,strSql2,teststr;
	Database uu;
	strSql.Format("select * from NAsmVariantTable where AsmNum=%s",AsmNum);
	int Z=m_db.Query(strSql);
	if (Z==-1)
	{
		return FALSE;
	}
	if(Z==0) 
	{ 
		return TRUE;
	}
	m_db.MoveBegin();
	int j,t;
	int p=0;
	CString str,str1,str2,FileName;
	do {
		uu.Asmsolid=solid;
		str=m_db.getString("Type");
		if (str=="dim") 
		{
			uu.type="dim";	
			str1=m_db.getString("Name");
			//str1.MakeLower();
			uu.name=str1;
			str1=m_db.getString("IdNum");
			str1.MakeLower();
			t=str1.Find(':');
			uu.idnum=str1.Left(t);
			str1=m_db.getString("DimSymbol");
			str1.MakeLower();
			uu.PARADIMSYMBOL=str1;
			t=str1.Find(':');
			uu.paradimsymbol=str1.Left(t);
			str1=m_db.getString("SubType");
			uu.subtype=str1;
			///////
			str1=m_db.getString("Note");
			uu.note=str1;
			////////
			j=m_db.getInt("Number");
			uu.number=j;
			uu.ModelNum=AsmNum;
			uu.MdlType="asm";
			uu.IsOrNot=TRUE;
			uu.IsOk=FALSE;
			Database_struc_temp.Add(uu);
			p++;
		}
		else
			if (str=="para")
			{
				uu.type="para";	
				str1=m_db.getString("Name");
				//str1.MakeLower();
				uu.name=str1;
				str1=m_db.getString("SubType");
				uu.subtype=str1;
				str1=m_db.getString("IdNum");
				str1.MakeLower();
				t=str1.Find(':');
				uu.idnum=str1.Left(t);
				str1=m_db.getString("ParaFeatName");
				str1.MakeLower();
				uu.PARADIMSYMBOL=str1;
				t=str1.Find(':');
				uu.paradimsymbol=str1.Left(t);
				//////
				str1=m_db.getString("Note");
				uu.note=str1;
				////
				j=m_db.getInt("Number");
				uu.number=j;
				uu.ModelNum=AsmNum;
				uu.MdlType="asm";
				uu.IsOrNot=TRUE;
				uu.IsOk=FALSE;
				Database_struc_temp.Add(uu);
				p++;
			}
	} while(m_db.MoveNext());
	AddSolidToDatabaseTemp(solid);
	return TRUE;
}
/************************************************************************/
/* �õ���ǰģ�͵ĳߴ���ֵ                                               */
/************************************************************************/
BOOL CCompute::ObtainValueFromMdlTemp(ProSolid solid)	 //����ľ��
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
		if (Database_struc_temp[i].type=="dim")
		{
			str=Database_struc_temp[i].idnum;
			str.Delete(0,1);
			haha.id=atoi(str.GetBuffer());
			haha.type=PRO_DIMENSION;
			haha.owner=((ProMdl)Database_struc_temp[i].solid);
			status=ProDimensionValueGet(&haha,&value);
			if (status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
			str.Format("%0.2f",value);
			Database_struc_temp[i].value=str;
			status=ProDimensionToleranceGet(&haha,&up_limit,&down_limit);
			if (status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
			m_up.Format("%0.3f",up_limit);
			m_down.Format("%0.3f",down_limit);
			Database_struc_temp[i].up=m_up;
			Database_struc_temp[i].down=m_down;
		}
		if (Database_struc_temp[i].type=="para")
		{
			status=ProMdlToModelitem((ProMdl)Database_struc_temp[i].solid,&modelitem);
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
			else
				return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* �����������                                                         */
/************************************************************************/
BOOL CCompute::AsmGeneration(ProSolid solid)//���ǲ�Ʒ�ľ��
{
	ProError status;
	ProDimension haha;
	ProName ParamName;
	ProModelitem modelitem;
	ProMdl mdl;
	CString str,Name;
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
			status=ProDimensionValueSet(&haha,value);
			if (status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
			status=ProDimensionToleranceSet(&haha,atof(Database_struc[j].up.GetBuffer()),atof(Database_struc[j].down.GetBuffer()));
			if (status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
		}
		if (Database_struc[j].type=="para")
		{
			status=ProMdlToModelitem((ProMdl)Database_struc[j].solid,&modelitem);
			if (status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
			str=Database_struc[j].idnum;
			ProStringToWstring(ParamName,str.GetBuffer());
			status=ProParameterInit(&modelitem,ParamName,&param);
			if (status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
			status=ProParameterValueGet(&param,&p_value);
			if (status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
			if (Database_struc[j].subtype=="PRO_PARAM_DOUBLE")
			{
				d_value=atof(Database_struc[j].value);
				status=ProParamvalueSet(&p_value,(void*)&d_value,PRO_PARAM_DOUBLE);
				if (status!=PRO_TK_NO_ERROR)
				{
					return FALSE;
				}
			}
			else
				if (Database_struc[j].subtype=="PRO_PARAM_INTEGER")
				{
					i_value=atoi(Database_struc[j].value);   
					status=ProParamvalueSet(&p_value,(void*)&i_value,PRO_PARAM_INTEGER);
					if (status!=PRO_TK_NO_ERROR)
					{
						return FALSE;
					}
				}
				else
					if (Database_struc[j].subtype=="PRO_PARAM_STRING")
					{
						ProStringToWstring(line,Database_struc[j].value.GetBuffer());
						status=ProParamvalueSet(&p_value,(void*)line,PRO_PARAM_STRING);
						if (status!=PRO_TK_NO_ERROR)
						{
							return FALSE;
						}
					}
					else
						if (Database_struc[j].subtype=="PRO_PARAM_BOOLEAN")
						{
							l_value=atoi(Database_struc[j].value);
							status=ProParamvalueSet(&p_value,(void*)l_value,PRO_PARAM_BOOLEAN);
							if (status!=PRO_TK_NO_ERROR)
							{
								return FALSE;
							}
						}
						else 
							AfxMessageBox("���Ĳ������Ͳ���ȷ!");
			status=ProParameterValueSet (&param,&p_value);
			if (status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
		}
	}
	//ProMdlTypeGet((ProMdl)solid,&p_type);
	//if (p_type==PRO_MDL_PART)
	//{
		status=ProSolidRegenerate(solid,PRO_B_FALSE);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("����ʧ��");
			return FALSE;
		}
		status=ProWindowRepaint(PRO_VALUE_UNUSED);
		status=ProTreetoolRefresh((ProMdl)solid);
		//������άģ��
		//DrawingList.RemoveAll();
		//GetSolidDrawing(solid,&DrawingList);
		//for (int i=0;i<DrawingList.GetSize();i++)
		//{
			//status=ProSolidRegenerate((ProSolid)DrawingList[i],PRO_B_FALSE);
			//if (status!=PRO_TK_NO_ERROR)
			//{
			//	return FALSE;
			//}
		//}
	//}
	//else
	//	if (p_type==PRO_MDL_ASSEMBLY)
	//	{
	//		if (!ObtainJuBing(solid))
	//		{
	//			return FALSE;
	//		}
	//		for (int i=0;i<JuBing_struc.GetSize();i++)
	//		{
	//			status=ProSolidRegenerate(JuBing_struc[i].solid,PRO_B_FALSE);
	//			if (status!=PRO_TK_NO_ERROR)
	//			{                
	//				return FALSE;
	//			}
	//			status=ProWindowRepaint(PRO_VALUE_UNUSED);
	//			status=ProTreetoolRefresh((ProMdl)solid);
	//			DrawingList.RemoveAll();
	//			GetSolidDrawing(solid,&DrawingList);
	//			for (int j=0;j<DrawingList.GetSize();j++)
	//			{
	//				ProSolidRegenerate((ProSolid)DrawingList[j],PRO_B_TRUE);
	//			}
	//		}
	//	}
		ProWindowRepaint(PRO_VALUE_UNUSED);
		ProMdlCurrentGet(&mdl);
		status = ProWindowActivate(PRO_VALUE_UNUSED);
		return TRUE;
}
/************************************************************************/
/* ���ǲ�Ʒģ�͵�ʱ��õ��������²��㲿���ľ�������·��                   */
/************************************************************************/
BOOL CCompute::ObtainJuBing(ProSolid solid)			  //��ǰ�ľ��
{
	ProError status;
	ProFeature *feature;
	ProName w_name;
	ProMdl   mdl;
	ProMdlType p_type;
	CString strSql,InsCode="";
	JuBing uu;
	ProAsmcomppath comppath;
	CCommonFuncClass commonfun;
	uu.Productsolid=solid;
	uu.solid=solid;
	post_id=0;
	uu.number=post_id;
	status=ProMdlNameGet((ProMdl)solid,w_name);
	if(status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	//����ǵ���������򷵻�
	status=ProMdlTypeGet((ProMdl)solid,&p_type);
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	else
	{
		if (p_type==PRO_MDL_PART)
		{
			uu.Type="prt";
			uu.Name=CString(w_name)+".prt";
			JuBing_struc.Add(uu);
			return TRUE;
		}
		else
		{
			uu.Type="asm";
		    uu.Name=CString(w_name)+".asm";
		}
	}
	JuBing_struc.Add(uu);
	post_id++;
	int n,id;
	int *a=new int[1];

	SolidFeatGet(solid,&feature,FALSE);
	ProArraySizeGet((ProArray)feature,&n);
	for (int i=0;i<n;i++)
	{
		id=(feature+i)->id;
		status=ProAsmcompMdlGet(( ProAsmcomp*)(feature+i),&mdl);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("�����Ʒ�Ѿ����ƻ����޷��õ����");
			return FALSE;
		}
		status=ProMdlTypeGet(mdl,&p_type);
		if (p_type==PRO_ASSEMBLY)
		{
			a[0]=id;
			ObtainJuBingFromAsm((ProSolid)mdl,&a,1,0);			 //���һ��������ʾ��һ���ı�ţ�0�ǲ�Ʒ���ı��
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
/* ���ǲ�����ʱ��õ��������²��㲿���ľ�������·��                   */
/************************************************************************/
void CCompute::ObtainJuBingFromAsm(ProSolid solid,int **a,int j,int upnum)		 //�㲿���ľ��
{
	ProError status;
	ProFeature *feature;
	ProMdl   mdl;
	ProMdlType p_type;
	ProAsmcomppath comppath;
	status=ProAsmcomppathInit(JuBing_struc[0].Productsolid,*a,j,&comppath);	   
	ObtainJuBingFromPart(solid,&comppath,upnum);
	SolidFeatGet(solid,&feature,FALSE);
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
/* ���������ʱ��õ���������·��                                     */
/************************************************************************/
void CCompute::ObtainJuBingFromPart(ProSolid solid,ProAsmcomppath *comppath,int upnum)//����ľ��
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
		uu.Type="asm";
	}
	else
		if (p_type==PRO_MDL_PART)
		{
			post="prt";
			uu.Type="prt";
		}
	uu.Name=name+"."+post;
	uu.comppath=*comppath;
	uu.number=post_id;
	uu.upnum=upnum;
	post_id++;
	JuBing_struc.Add(uu);
}

/************************************************************************/
/* �õ���ǰ��Ʒ�����б�������ر���                                     */
/************************************************************************/
BOOL CCompute::GetAllVariantAndRelativeVariant(ProSolid Productsolid,CString ProductNum)		  //����ǲ�Ʒ�ľ��
{
	CString sql;
	CCommonFuncClass commonfun;
	CString PrtNum;
	Database_struc.RemoveAll();
	RelationSet.RemoveAll();
	//�õ�����ж���ı���
	if(!ObtainAsmFromBase(Productsolid,ProductNum))
	{
		return FALSE;	//��ǰ����ľ��
	}
	//���������׺�õ�����ı����͹�ϵ���޸Ĺ�ϵʽ��
	for (int i=0;i<PostIdArray.GetSize();i++)
	{
		sql.Format("SELECT * FROM NAsmBomTable where AsmNum=%s AND ComNum=%s",ProductNum,PostIdArray[i]);
		int Z=m_db.Query(sql);
		if (Z==-1 || Z==0)
		{
			return FALSE;
		}
		else
		{
			m_db.MoveBegin();
			do {
				PrtNum.Format("%d",m_db.getInt("PartMdlNum"));
			} while(m_db.MoveNext());
		}
		//�õ�����ı���
		if (!GetVariantForAsm(PrtNum,PostIdArray[i]))
		{
			return FALSE;
		}
		//�õ���ϵʽ
	    if (!GetRelationForAsm(PrtNum,PostIdArray[i]))
	    {
			return FALSE;
	    }
	}
	//ȥ���ظ��ı���
	IsVariantAlreadyHave();
	//Ϊ����ṹ��Ӿ��
	if (!AddSolidToDatabase(Productsolid))
	{
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* �ж�Database_struc�Ƿ��Ѿ��д�����                                   */
/************************************************************************/
BOOL CCompute::IsVariantAlreadyHave()
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
		IsVariantAlreadyHave();
	}
	else
	{
		return TRUE;
	}
	return TRUE;
}
/************************************************************************/
/*�õ���ǰ���������������                                              */
/************************************************************************/
BOOL CCompute::ObtainAsmFromBase(ProSolid solid,CString AsmNum)//��ǰ�����ͼ�ź;��
{
	CString strSql,strSql1,strSql2,teststr;
	Database uu;
	int t,T;
	strSql.Format("select * from NAsmVariantTable where AsmNum=%s",AsmNum);
	T=m_db.Query(strSql);
	if(T==-1) 
	{ 
		return FALSE;
	}
	if (T==0)
	{
		return TRUE;
	}
	m_db.MoveBegin();
	int j;
	CString str,str1,str2,FileName;
	do {
		str=m_db.getString("Type");
		uu.Asmsolid=solid;
		if (str=="dim") 
		{
			uu.type="dim";	
			str1=m_db.getString("Name");

			uu.name=str1;
			str1=m_db.getString("IdNum");
			str1.MakeLower();
			t=str1.Find(':');
			uu.idnum=str1.Left(t);
			str1=m_db.getString("DimSymbol");
			str1.MakeLower();
			uu.PARADIMSYMBOL=str1;
			t=str1.Find(':');
			uu.paradimsymbol=str1.Left(t);
			str1=m_db.getString("SubType");
			uu.subtype=str1;
			///////
			str1=m_db.getString("Note");
			uu.note=str1;
			////////
			j=m_db.getInt("Number");
			uu.number=j;
			uu.ModelNum=AsmNum;
			uu.MdlType="asm";
			uu.IsOrNot=TRUE;
			uu.IsOk=FALSE;
			Database_struc.Add(uu);	
		}
		else
			if (str=="para")
			{
				uu.type="para";	
				str1=m_db.getString("Name");

				uu.name=str1;
				str1=m_db.getString("SubType");
				uu.subtype=str1;
				str1=m_db.getString("IdNum");
				str1.MakeLower();
				t=str1.Find(':');
				uu.idnum=str1.Left(t);
				str1=m_db.getString("ParaFeatName");
				str1.MakeLower();
				uu.PARADIMSYMBOL=str1;
				t=str1.Find(':');
				uu.paradimsymbol=str1.Left(t);
				//////
				str1=m_db.getString("Note");
				uu.note=str1;
				////
				j=m_db.getInt("Number");
				uu.number=j;
				uu.ModelNum=AsmNum;
				uu.MdlType="asm";
				uu.IsOrNot=TRUE;
				uu.IsOk=FALSE;
				Database_struc.Add(uu);
			}
	} while(m_db.MoveNext());
	//���ݱ����ĵõ���׺
	PostIdArray.RemoveAll();
	ObtainPostIdFromVariant();
	return TRUE;
}
/************************************************************************/
/* �õ����еĺ�׺                                                       */
/************************************************************************/
void CCompute::ObtainPostIdFromVariant()
{
	CCommonFuncClass commonfun;
	CString str;
	int k;
	for (int i=0;i<Database_struc.GetSize();i++)
	{
		str=Database_struc[i].PARADIMSYMBOL;
		k=str.Find(":");
		str.Delete(0,k+1);
		if (str=="0")
		{
			continue;
		}
		else
		    PostIdArray.Add(str);
	}
	commonfun.RemoveNameFromNameArray(&PostIdArray);
}
/************************************************************************/
/*Ϊ�к�׺�ı������ݺ�׺��ӱ���                                        */
/************************************************************************/
BOOL CCompute::AddSolidToDatabase(ProSolid solid)	 //����ľ��
{
	int k;
	CString str,PostID;

    JuBing_struc.RemoveAll();
	if(!ObtainJuBing(solid))
		return FALSE;

	for (int i=0;i<Database_struc.GetSize();i++)
	{
		str=Database_struc[i].PARADIMSYMBOL;
		k=str.Find(":");
		str.Delete(0,k+1);
		for (int j=0;j<JuBing_struc.GetSize();j++)
		{
			if (atoi(str)==JuBing_struc[j].number)
			{
				Database_struc[i].solid=JuBing_struc[j].solid;
			}
		}
	}
	return TRUE;

}
/************************************************************************/
/*Ϊ�к�׺�ı������ݺ�׺��ӱ���                                        */
/************************************************************************/
BOOL CCompute::AddSolidToDatabaseTemp(ProSolid solid)	 //����ľ��
{
	int k;
	CString str,PostID;

	JuBing_struc.RemoveAll();
	if(!ObtainJuBing(solid))
		return FALSE;

	for (int i=0;i<Database_struc_temp.GetSize();i++)
	{
		str=Database_struc_temp[i].PARADIMSYMBOL;
		k=str.Find(":");
		str.Delete(0,k+1);
		for (int j=0;j<JuBing_struc.GetSize();j++)
		{
			if (atoi(str)==JuBing_struc[j].number)
			{
				Database_struc_temp[i].solid=JuBing_struc[j].solid;
			}
		}
	}
	return TRUE;

}
/************************************************************************/
/* ������еõ���������ر���                                           */
/************************************************************************/
BOOL CCompute::GetVariantForPart(ProSolid solid,CString partnum,BOOL temp)	 //������	,3.��ӵ��ĸ��ṹ
{
	CString strSql,strSql1,strSql2;
	Database uu;
	strSql.Format("select * from NPartVariantTable where PrtNum=%s",partnum);
	int T=m_db.Query(strSql);
	if(T==-1) 
	{ 
		return FALSE;
	}
	if (T==0)
	{
		return TRUE;
	}
	m_db.MoveBegin();
	int j;
	CString str,str1,str2;
	do {
		uu.Asmsolid=solid;
		str=m_db.getString("Type");
		if (str=="dim") 
		{
			uu.solid=solid;
			uu.type="dim";	
			str1=m_db.getString("Name");
			uu.name=str1;
			str1=m_db.getString("IdNum");
			str1.MakeLower();
			uu.idnum=str1;
			str1=m_db.getString("DimSymbol");
			str1.MakeLower();
			uu.paradimsymbol=str1;
			uu.PARADIMSYMBOL=str1;
			str1=m_db.getString("SubType");
			uu.subtype=str1;
			///////
			str1=m_db.getString("Note");
			uu.note=str1;
			////////
			j=m_db.getInt("Number");
			uu.number=j;
			uu.ModelNum=partnum;
			uu.MdlType="prt";
			uu.IsOrNot=TRUE;
			uu.IsOk=FALSE;
			uu.up="0.01";
			uu.down="0.01";
			if (temp)
			{
				Database_struc_temp.Add(uu);
			}
			else
				Database_struc.Add(uu);
		}
		else
			if (str=="para")
			{
				uu.solid=solid;
				uu.type="para";	
				str1=m_db.getString("Name");
				uu.name=str1;
				str1=m_db.getString("SubType");
				uu.subtype=str1;
				str1=m_db.getString("IdNum");
				str1.MakeLower();
				uu.idnum=str1;
				str1=m_db.getString("ParaFeatName");
				str1.MakeLower();
				uu.paradimsymbol=str1;
				uu.PARADIMSYMBOL=str1;
				str1=m_db.getString("Note");
				uu.note=str1;
				////
				j=m_db.getInt("Number");
				uu.number=j;
				uu.ModelNum=partnum;
				uu.MdlType="prt";
				uu.IsOrNot=TRUE;
				uu.IsOk=FALSE;
				if (temp)
				{
					Database_struc_temp.Add(uu);
				}
				else
					Database_struc.Add(uu);
			}
	} while(m_db.MoveNext());
	///��ȡ��ز����ı�
	if (temp)
	{
		return TRUE;
	}
	strSql.Format("select * from [NPartRelativeDimParaTable] where [PrtNum]=%s",partnum);
	int Z=m_db.Query(strSql);
	if (Z==-1)
	{
		AfxMessageBox("��ѯ���ݿ�����");
		return FALSE;
	}
	else
		if(Z!=0)
		{
			m_db.MoveBegin();
			do {
				uu.Asmsolid=solid;
				str=m_db.getString("Type");
				if (str=="dim") 
				{
					uu.solid=solid;
					uu.type="dim";	
					str1=m_db.getString("Name");
					uu.name=str1;
					str1=m_db.getString("IdNum");
					str1.MakeLower();
					uu.idnum=str1;
					str1=m_db.getString("DimSymbol");
					str1.MakeLower();
					uu.paradimsymbol=str1;
					uu.PARADIMSYMBOL=str1;
					str1=m_db.getString("SubType");
					uu.subtype=str1;
					uu.ModelNum=partnum;
					uu.MdlType="prt";
					uu.IsOrNot=FALSE;
					uu.IsOk=FALSE;
					Database_struc.Add(uu);
				}
				else
					if (str=="para")
					{
						uu.solid=solid;
						uu.Asmsolid=solid;
						uu.type="para";	
						str1=m_db.getString("Name");
						uu.name=str1;
						str1=m_db.getString("SubType");
						//str1.MakeLower();
						uu.subtype=str1;
						str1=m_db.getString("IdNum");
						uu.idnum=str1;
						str1=m_db.getString("Name");
						str1.MakeLower();
						uu.paradimsymbol=str1;
						uu.PARADIMSYMBOL=str1;
						uu.ModelNum=partnum;
						uu.MdlType="prt";
						uu.IsOrNot=FALSE;
						uu.IsOk=FALSE;
						Database_struc.Add(uu);
					}
			} while(m_db.MoveNext());
		}
		return TRUE;
}
/************************************************************************/
/* �������Ϊ��Ʒ�õ���������ر���                                           */
/************************************************************************/
BOOL CCompute::GetVariantForAsm(CString partnum,CString PostID)	 //1.���ģ��ID,2.��׺
{
	CString strSql;
	Database uu;
	strSql.Format("select * from NPartVariantTable where PrtNum=%s",partnum);
	int T=m_db.Query(strSql);
	if(T==-1) 
	{ 
		return FALSE;
	}
	if (T==0)
	{
		return TRUE;
	}
	m_db.MoveBegin();
	int j;
	CString str,str1,str2;
	do {
		str=m_db.getString("Type");
		if (str=="dim") 
		{
			uu.type="dim";	
			str1=m_db.getString("Name");
			uu.name=str1;
			str1=m_db.getString("IdNum");
			str1.MakeLower();
			uu.idnum=str1;
			str1=m_db.getString("DimSymbol");
			str1.MakeLower();
			uu.paradimsymbol=str1;
			uu.PARADIMSYMBOL=str1+":"+PostID;
			str1=m_db.getString("SubType");
			uu.subtype=str1;
			///////
			str1=m_db.getString("Note");
			uu.note=str1;
			////////
			j=m_db.getInt("Number");
			uu.number=j;
			uu.ModelNum=partnum;
			uu.MdlType="prt";
			uu.IsOrNot=TRUE;
			uu.IsOk=FALSE;
			uu.up="0.01";
			uu.down="0.01";
			Database_struc.Add(uu);
		}
		else
			if (str=="para")
			{
				uu.type="para";	
				str1=m_db.getString("Name");
				uu.name=str1;
				str1=m_db.getString("SubType");
				uu.subtype=str1;
				str1=m_db.getString("IdNum");
				str1.MakeLower();
				uu.idnum=str1;
				str1=m_db.getString("ParaFeatName");
				str1.MakeLower();
				uu.paradimsymbol=str1;
				uu.PARADIMSYMBOL=str1+":"+PostID;
				str1=m_db.getString("Note");
				uu.note=str1;
				////
				j=m_db.getInt("Number");
				uu.number=j;
				uu.ModelNum=partnum;
				uu.MdlType="prt";
				uu.IsOrNot=TRUE;
				uu.IsOk=FALSE;
			    Database_struc.Add(uu);
			}
	} while(m_db.MoveNext());
	///��ȡ��ز����ı�
	strSql.Format("select * from [NPartRelativeDimParaTable] where [PrtNum]=%s",partnum);
	int Z=m_db.Query(strSql);
	if (Z==-1)
	{
		return FALSE;
	}
	else
		if(Z!=0)
		{
			m_db.MoveBegin();
			do {
				str=m_db.getString("Type");
				if (str=="dim") 
				{
					uu.type="dim";	
					str1=m_db.getString("Name");
					uu.name=str1;
					str1=m_db.getString("IdNum");
					str1.MakeLower();
					uu.idnum=str1;
					str1=m_db.getString("DimSymbol");
					str1.MakeLower();
					uu.paradimsymbol=str1;
					uu.PARADIMSYMBOL=str1+":"+PostID;
					str1=m_db.getString("SubType");
					uu.subtype=str1;
					uu.ModelNum=partnum;
					uu.MdlType="prt";
					uu.IsOrNot=FALSE;
					uu.IsOk=FALSE;
					Database_struc.Add(uu);
				}
				else
					if (str=="para")
					{
						uu.type="para";	
						str1=m_db.getString("Name");
						uu.name=str1;
						str1=m_db.getString("SubType");
						str1.MakeLower();
						uu.subtype=str1;
						str1=m_db.getString("IdNum");
						uu.idnum=str1;
						str1=m_db.getString("DimSymbol");
						str1.MakeLower();
						uu.paradimsymbol=str1;
						uu.PARADIMSYMBOL=str1+":"+PostID;
						uu.ModelNum=partnum;
						uu.MdlType="prt";
						uu.IsOrNot=FALSE;
						uu.IsOk=FALSE;
						Database_struc.Add(uu);
					}
			} while(m_db.MoveNext());
		}
		return TRUE;
}
/************************************************************************/
/* ��ģ���еõ�����                                                     */
/************************************************************************/
BOOL CCompute::GetValueFromMdl()	 //������
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
	for (int i=0;i<Database_struc.GetSize();i++)
	{
		if ((!Database_struc[i].IsOk) && Database_struc[i].IsOrNot)
		{
			if (Database_struc[i].type=="dim")
			{
				str=Database_struc[i].idnum;
				str.Delete(0,1);
				haha.id=atoi(str.GetBuffer());
				haha.type=PRO_DIMENSION;
				haha.owner=((ProMdl)Database_struc[i].solid);
				status=ProDimensionValueGet(&haha,&value);
				if (status!=PRO_TK_NO_ERROR)
				{
					return FALSE;
				}

				str.Format("%0.2f",value);
				Database_struc[i].value=str;
				status=ProDimensionToleranceGet(&haha,&up_limit,&down_limit);
				if (status!=PRO_TK_NO_ERROR)
				{
					return FALSE;
				}

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
				else
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* ����ϵʽ                                                             */
/************************************************************************/
BOOL CCompute::GetRelationForPart(CString partnum)			 
{
	CString str;
	//����ģ�ͱ���������ݿ�
	CString strSql;
	strSql.Format("select Relation from NPartRelationSet where PrtNum=%s",partnum);//ǰ������ݿ�Ҫ���;
	int Z=m_db.Query(strSql);
	if (Z==-1)
	{
		AfxMessageBox("��ѯ���ݿ�����");
		return FALSE;
	}
	else
		if(Z!=0)
		{
			m_db.MoveBegin();
			do {
				str=m_db.getString("Relation");
				str.MakeLower();
				str.Remove(' ');
				RelationSet.Add(str);
			} while(m_db.MoveNext());
		}
		return TRUE;
}
/************************************************************************/
/* ����ϵʽ                                                             */
/************************************************************************/
BOOL CCompute::GetRelationForAsm(CString partnum,CString PostID)			 
{
	CString str;
	CMyUtil myutil;
	//����ģ�ͱ���������ݿ�
	CStringArray Relatemp;
	Relatemp.RemoveAll();
	CString strSql;
	strSql.Format("select Relation from NPartRelationSet where PrtNum=%s",partnum);//ǰ������ݿ�Ҫ���;
	int Z=m_db.Query(strSql);
	if (Z==-1)
	{
		return FALSE;
	}
	else
		if(Z!=0)
		{
			m_db.MoveBegin();
			do {
				str=m_db.getString("Relation");
				str.MakeLower();
				str.Remove(' ');
				Relatemp.Add(str);
			} while(m_db.MoveNext());
		}
	//Ϊ����Ĺ�ϵʽ��Ӻ�׺
	myutil.AddPostfix(&Relatemp,PostID);
	RelationSet.Append(Relatemp);
	return TRUE;
}
/************************************************************************/
/* �õ����еĺ�׺                                                       */
/************************************************************************/
void CCompute::ModifyPostIdForRelation(CStringArray *Relatemp,CString PostId)
{


	//////////////////////////////////////////////////////////////////////////////
	//CString Relation,str;
	//for (int i=0;i<Relatemp->GetSize();i++)
	//{
	//	Relation=(*Relatemp)[i];
	//	int cur=0;
	//	int tempCur1=0;
	//	do {
	//		cur=tempCur1;
	//		tempCur1=Relation.Find(':',cur);
	//		CString postID_temp;
	//		if (tempCur1!=-1)
	//		{
	//			int tempCur2=tempCur1;
	//			postID_temp=Relation.Tokenize(":+-*/()=<>{}[]^%",tempCur2);//�õ��ĺ�׺	
	//			str.Format("%d",postID_temp);
	//			tempCur1=tempCur1+1;
	//		}
	//	} while(tempCur1!=-1);
	//}
//	RelationSet.Append();
}
ProError SolidFeatGet(ProSolid solid,ProFeature **p_data,BOOL nFlag)
{
	ProError status;
	if( p_data != NULL )
	{
		status=ProArrayAlloc(0,sizeof(ProFeature),1,(ProArray*)p_data);
		if( status == PRO_TK_NO_ERROR )
		{  //���óߴ������ʺ���
			if(nFlag)
			{
				status=ProSolidFeatVisit (solid,
					(ProFeatureVisitAction) FeatureVisitActionFn,
					(ProFeatureFilterAction)FeatureFilterAction,(ProAppData)&p_data);  
			}
			else
			{
				status=ProSolidFeatVisit (solid,
					(ProFeatureVisitAction) FeatureVisitActionFn,
					(ProFeatureFilterAction)FeatureFilterActionFn,(ProAppData)&p_data);  
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
/*        �������ʶ�������                                              */
/************************************************************************/
ProError FeatureVisitActionFn(ProFeature *p_object,ProError status,ProAppData app_data)
{
	ProArray *p_array;
	p_array=(ProArray*)((ProFeature**)app_data)[0];
	status=ProArrayObjectAdd(p_array,PRO_VALUE_UNUSED,1,p_object);
	return status;
}
/************************************************************************/
/*       �������˺���                                                   */
/************************************************************************/
ProError FeatureFilterActionFn(ProFeature *feature,ProAppData app_data)
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
Function:FeatureFilterAction() ����������ʺ����Ĺ��˺�������
\*=========================================================================*/
ProError FeatureFilterAction(ProFeature *p_object,
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
�Զ��庯����������õ�ǰģ�͵ĳߴ����ָ�룬��������ָ��������
\*================================================================*/
ProError  SgCollectDim(ProSolid solid,			//(In)��ǰģ��
					   ProDimension **p_data)	//(In)�ߴ��������
{
	ProError status;
	if( p_data != NULL )
	{
		status=ProArrayAlloc(0,sizeof(ProDimension),1,(ProArray*)p_data);
		if( status == PRO_TK_NO_ERROR )
		{  //���óߴ������ʺ���
			status=ProSolidDimensionVisit (solid,PRO_B_FALSE,
				(ProDimensionVisitAction)FeatureDimensionVisitAction,
				(ProDimensionFilterAction)FeatureDimensionFilterAction,(ProAppData)&p_data);  
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
/*================================================================*\
FUNCTION: FeatureCollectDim()
�Զ��庯����������õ�ǰ�����ĳߴ����ָ�룬��������ָ��������
\*================================================================*/
ProError  FeatureCollectDim(ProFeature feature,			//(In)��ǰģ��
							ProDimension **p_data)	
{
	ProError status;
	if( p_data != NULL )
	{
		status=ProArrayAlloc(0,sizeof(ProDimension),1,(ProArray*)p_data);
		if( status == PRO_TK_NO_ERROR )
		{  //���óߴ������ʺ���
			ProFeatureDimensionVisit(&feature,(ProDimensionVisitAction)FeatureDimensionVisitAction,
				(ProDimensionFilterAction)FeatureDimensionFilterAction,(ProAppData)&p_data);
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
/*=========================================================================*\
Function:	FeatureDimensionVisitAction() �ߴ������ʺ����Ķ�����������
\*=========================================================================*/
ProError FeatureDimensionVisitAction(ProDimension *p_object,ProError status,
									 ProAppData app_data)  

{   
	ProArray *p_array;
	p_array = (ProArray*)((ProDimension**)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1,p_object );
	return (status);
}
/*=========================================================================*\
Function: FeatureDimensionFilterAction() �ߴ������ʺ����Ĺ��˺�������
\*=========================================================================*/
ProError FeatureDimensionFilterAction(ProDimension *p_object,
									  ProAppData app_data)  
{   
	return (PRO_TK_NO_ERROR);
}
/************************************************************************/
/* �õ���������һ���Ĺ���ͼ���                                       */
/************************************************************************/
void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList)
{
	ProMdl *p_model_array;
	int     p_count=0;
	int         num=0;
	ProSessionMdlList(PRO_MDL_DRAWING,&p_model_array,&p_count);
	for (int i=0;i<p_count;i++)
	{
		ProSolid *solids;
		int       solidnum=0;
		ProDrawingSolidsCollect((ProDrawing)p_model_array[i],&solids);
		ProArraySizeGet((ProArray)solids,&solidnum);
		for (int j=0;j<solidnum;j++)
		{
			if(solids[j]==solid) 
			{
				int Flag=0;
				//�ж��Ƿ��Ѿ����ڣ�һλ������������ܲ�ͬ�����ͬһ������ͼ��
				for (int m=0;m<(int)p_DrawingList->GetCount();m++)
				{
					if(p_DrawingList->GetAt(m)==(ProDrawing)p_model_array[i])
					{
						Flag=1;
						break;
					}
				}
				if(Flag==0) p_DrawingList->Add((ProDrawing)p_model_array[i]);
				break;
			}
		}
	}
	//ProMdlTypeGet(solid,&type);
	//if (type==PRO_MDL_ASSEMBLY)
	//{
	//	SolidFeatGet(solid,&p_feature);
	//	num=(int)feat_list.GetCount();
	//	for (int i=0;i<num;i++) {
	//		ProFeattype    ftype;
	//		ProFeatureTypeGet (&feat_list[i], &ftype);
	//		if (ftype == PRO_FEAT_COMPONENT)
	//		{
	//			ProSolid  mdl;
	//			ProAsmcompMdlGet((ProAsmcomp *)(&feat_list[i]),(ProMdl *)&mdl);
	//			GetSolidDrawing(mdl,p_DrawingList);
	//		}
	//	
	//}
}