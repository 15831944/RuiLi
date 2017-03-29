// MyUtil.cpp : 实现文件
//
#pragma  once
#include "stdafx.h"
#include "MyUtil.h"
#define _USE_MATH_DEFINES
#include <math.h>


// CMyUtil

CMyUtil::CMyUtil()
{
	
}

CMyUtil::~CMyUtil()
{
	
}


// CMyUtil 成员函数




CString CMyUtil::ReplaceString(CString str,CString str1,CString str2)
{
	CString ss;
	ss=str1+":::";
	str.Replace(str1,ss);
	str.Replace(str2,str1);
	str.Replace(ss,str2);
	return str;
}

  BOOL CMyUtil::IsDigit(int c)
  {
	  if(c>='0'&&c<='9')
		  return TRUE;
	  else
		  return FALSE;
  }

BOOL CMyUtil::IsDouble(CString str)
{
	CString left,right;
	int c;
	int i;
	int nIndex=str.Find(".");
   if(nIndex!=-1)
   {
	   right=str.Mid(nIndex+1);
	   left=str.Left(nIndex);
   }
   else
   {
		right="12";
		left=str;
   }
   c=left.GetAt(0);
   if(left.GetLength()==1)
   {
	   if(!IsDigit(c)) return FALSE;
   }
   else
   {
	   if(!IsDigit(c)||c=='0') return FALSE;
	   for( i=1;i<left.GetLength();i++)
	   {
		   c=left.GetAt(i);
		   if(!IsDigit(c)) return FALSE;
	   }
   }

   for(i=0;i<right.GetLength();i++)
   {
		 c=right.GetAt(i);	  
	   if(!IsDigit(c)) return FALSE;
   }
	return TRUE;
}

BOOL CMyUtil::IsFunction(CString str)
{
	CStringArray funArray;
	funArray.Add("sin");
	funArray.Add("cos");
	for(int i=0;i<funArray.GetSize();i++)
	{
		if(str==funArray[i])
			return TRUE;
	}
	return FALSE;
}

/************************************************************************/
/* 检验表达式 
	首先找到括号,调用自己处理括号里的表达式,判断是否是函数
	将括号替换成某一实数.重新处理表达式.
	从头处理,首先看是否实数.
	不是实数,在输入Array中,找到匹配项,并将此项加入输出Array中;
	否则返回出错.
	将此项连同运算符号去掉,重新处理表达式.

*/
/************************************************************************/
BOOL CMyUtil::CheckExpression(CString Expression,CStringArray * importDim,CStringArray * exportDim)
{
	int rIndex,min;
	CString str,sub;
	int i;
	str=Expression;
	min=str.Find("(");
	int n=0;
	if(min!=-1)
{
	
	for(i=min;i<str.GetLength();i++)
	{
			char ch=str.GetAt(i);
				if(ch=='(')
				{
					n++;
				}
				else if(ch==')')
				{
					n--;
					if(n==0)
					{
						rIndex=i;
						break;
					}
				}
		}
	if(i==str.GetLength())
	{
		str="括号不匹配";
		AfxMessageBox(str);
		return FALSE;
	}
	sub=str.Mid(min+1,rIndex-min+1-2);///括号里面的字符串
	if(CheckExpression(sub,importDim,exportDim)==FALSE) return FALSE;
    sub=str.Mid(min,rIndex-min+1);///包括括号的字符串
	///函数处理,现略
	CString str1,str3;
	str1=str.Mid(0,min);///获得前半段表达式,不包括括号.
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
	BOOL fff=FALSE;
	if(a1+1<str1.GetLength())
	{
		str3=str1.Mid(a1+1);
		CStringArray  label;
		label.Add("sin");
		label.Add("cos");
		label.Add("log");
		label.Add("exp");
	for(i=0;i<label.GetSize();i++)
	{
		if(str3==label.GetAt(i))
		{
			sub=str3+sub;
			fff=TRUE;
		}
	}
	if(fff==FALSE)
	{
		str="括号前面要有运算符号";
		AfxMessageBox(str);
		return FALSE;
	}
}
	////函数处理完毕
	str.Replace(sub,"1.23456789");
	return CheckExpression(str,importDim,exportDim);
}
min=0;
str=Expression.Tokenize("*+/-",min);
if(str.GetLength()!=min-1)
{
	str=""+str+"  前有重叠运算符号";
	str.Replace("1.23456789","括号");
	AfxMessageBox(str);
	return FALSE;
}
///是否实数
if(IsDouble(str))
{
	if(str==Expression)
	{
		return TRUE;
	}
	else
	{
		if(min==Expression.GetLength()) 
		{
			str="在  "+str+"   后面出现错误,运算符号后面必须有其他项";
			str.Replace("1.23456789","括号");
			AfxMessageBox(str);
			return FALSE;
		}
		str=Expression.Mid(min);
		return CheckExpression(str,importDim,exportDim);
	}
}

/////////////是否尺寸或参数标识
BOOL nFlag=FALSE;
BOOL pFlag=TRUE;
for(i=0;i<importDim->GetSize();i++)
{
	if(str==importDim->GetAt(i))
	{
		nFlag=TRUE;
		for(int m=0;m<exportDim->GetSize();m++)
		{
			if(str==exportDim->GetAt(m))
				pFlag=FALSE;
		}
		if(pFlag==TRUE)
		exportDim->Add(str);
	}
}
if(str=="PI"||str=="E")/////判断常数
	nFlag=TRUE;
if(nFlag==FALSE) 
{
	min=str.Find("1.23456789");
	if(min!=-1)
		str="多余的右括号,或不匹配";
	else
		str="找不到"+str+"这一项,请检查";
	AfxMessageBox(str);
	return FALSE;
}

if(str==Expression)
{
	return TRUE;
}
else
{
	if(min==Expression.GetLength()) 
	{
		str="在  "+str+"   后面出现错误,运算符号后面必须有其他项";
		str.Replace("1.23456789","括号");
		AfxMessageBox(str);
		return FALSE;
	}
	str=Expression.Mid(min);
	return CheckExpression(str,importDim,exportDim);
}

	return TRUE;
}

/************************************************************************/
/* 检查方程 
首先判断等号.
在分别处理左边表达式和右边表达式.
*/
/************************************************************************/
BOOL CMyUtil::CheckEquation(CString Expression,CStringArray * importDim,CStringArray * exportDim)
{
	int min;
	CString str,left,right;
	str=Expression;
	min=str.Find("=");
	if(min==-1)
	{
		str="方程没有等号";
		AfxMessageBox(str);
		return FALSE;
	}
   
	if(min==0)
	{
		str="方程等号前面无其他项";
		AfxMessageBox(str);
		return FALSE;
	}

	if(min==str.GetLength()-1)
	{
		str="方程等号后面无其他项";
		AfxMessageBox(str);
		return FALSE;
	}
	left=str.Left(min);
	right=str.Mid(min+1);
	min=right.Find("=");
	if(min!=-1)
	{
		str="方程等号有两项或更多";
		AfxMessageBox(str);
		return FALSE;
	}
	if(CheckExpression(left,importDim,exportDim))
	{
		if(CheckExpression(right,importDim,exportDim))
			return TRUE;
	}
	return FALSE;
	
}

/************************************************************************/
/*/////本函数可包括sin(),cos(),log(),pow();                                                                      */
/************************************************************************/

double CMyUtil::CalString(CString expression)
{
	int start,end;
	int t;
	CString str;
	CString str1,str2,str3;
	double temp=-1;
	double d1,d2;
	char c;
	try
{/////////////try前括号
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

			 str1=expression.Mid(0,start);///获得前半段计算函数
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
			 if(a1=str1.GetLength()-1)//重新
			 {
				 str3="";
			 }
			 else
			 {
				  str3=str1.Mid(a1+1);
			 }
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
				if(str3=="tan")
				{
					temp=tan(temp);
					str.Format("%f",temp);
					str.Replace('-','@');
					str2=str3+str2;
				}
				if(str3=="sqrt")
				{
					temp=sqrt(temp);
					str.Format("%f",temp);
					str.Replace('-','@');
					str2=str3+str2;
				}
				if(str3=="abs")
				{
					temp=(int)abs(temp);
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
		d1=CalString(str3);///调用自身
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
		if(expression=="PI")//////////返回常数
		{
			return M_PI;
		}
		if(expression=="E")
		{
			return M_E; 
		}
		expression.Replace('@','-');
		temp=atof(expression);
}//////////////////try后括号

	catch (...) {
		AfxMessageBox("出错");
		return -1;
	}
	return temp;
}


/////////////求解方程
double CMyUtil::ComputeEquation(CString expression,CString x)
{
try
{
	////////////////////
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
////如果未知数在等式右边，调到左边
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
			c1=CalString(str);
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
////////////////////////////
}///try后括号
		catch (...) {
			AfxMessageBox("方程计算出错");
			return -1;
		}
}

//////////////////////////求解方程结束
/************************************************************************/
/* 找出未知数在方程中的位置                                                                     */
/************************************************************************/

 int CMyUtil::FindX(CString expression,CString x)
 {
	 int nIndex;
	 char left,right;
	 CString str;
	 BOOL front=FALSE,end=FALSE;
	 nIndex=expression.Find(x);
	 while(nIndex!=-1)
	 {
		 if(nIndex==0)
			{
				front=TRUE;
			}
		 else
			{
				left=expression.GetAt(nIndex-1);
				if(left=='('||left=='='||left=='+'||left=='-'||left=='*'||left=='/'||left=='<'||left=='>')
					front=TRUE;
			}

			if(nIndex+x.GetLength()==expression.GetLength())
			{
				end=TRUE;
			}
			else
			{
				right=expression.GetAt(nIndex+x.GetLength());
				if(right==')'||right=='='||right=='+'||right=='-'||right=='*'||right=='/'||right=='<'||right=='>')
					end=TRUE;
			}
			if(front==TRUE&&end==TRUE)
			{
				return nIndex;
			}
			front=FALSE;
			end=FALSE;
			if(nIndex+x.GetLength()==expression.GetLength())
				return -1;
				nIndex=expression.Find(x,nIndex+1);
	 }
	 return -1;
 }

/************************************************************************/
/* 替换未知数为数值                                                                     */
/************************************************************************/

CString CMyUtil::ReplaceXtoDouble(CString expression,CString x,double result)
{
	    int nIndex;
		CString str;
		str=expression;
		nIndex=FindX(str,x);
		if(nIndex==-1) return expression;

	    str.Format("%f",result);
		if((nIndex+x.GetLength())<expression.GetLength())
			str=expression.Left(nIndex)+str+expression.Mid(nIndex+x.GetLength());
		else
			str=expression.Left(nIndex)+str;
		return str;
		
}

/************************************************************************/
/* 替换未知数为字符串                                                                     */
/************************************************************************/

CString CMyUtil::ReplaceXtoString(CString expression,CString x,CString result)
{
	int nIndex;
	CString str;
	str=expression;
	nIndex=FindX(str,x);
	if(nIndex==-1) return expression;

	str.Format("%s",result);
	if((nIndex+x.GetLength())<expression.GetLength())
		str=expression.Left(nIndex)+str+expression.Mid(nIndex+x.GetLength());
	else
		str=expression.Left(nIndex)+str;
	return str;

}

int CMyUtil::AddPostfix(CStringArray *strArray,CString strNum)
{
	CString str;
	for(int i=0;i<strArray->GetSize();i++)
	{
		str=(*strArray)[i];
        (*strArray)[i]=AddPostfix(str,strNum);
	}
	return 1;
}

CString CMyUtil::AddPostfix(CString expression,CString strNum)
{
	CStringArray tempArray;
	CString str,str1;
	int index=0;
	str=expression.Tokenize("+-*/()=",index);
	while(str!="")
	{
		tempArray.Add(str);
		str=expression.Tokenize("+-*/=()",index);
	}
for(int i=0;i<tempArray.GetSize();i++)
{
    str=tempArray[i];

	if(!IsDouble(str)&&!IsFunction(str))
	{
		str1=str+":"+strNum;
        expression=ReplaceXtoString(expression,str,str1);
	}
}

return expression;
}

/************************************************************************/
/* 检查不等式*/
/************************************************************************/
BOOL  CMyUtil::CheckInEquation(CString Expression,CStringArray * importDim,CStringArray * exportDim)
{
	int min;
	CString str,left,right,str1;
	str=Expression;
	min=str.FindOneOf("<>");
	if(min==-1)
	{
		str="不等式无不等号";
		AfxMessageBox(str);
		return FALSE;
	}

	if(min==0)
	{
		str="不等号前面无其他项";
		AfxMessageBox(str);
		return FALSE;
	}

	if(min==str.GetLength()-1)
	{
		str="不等号后面无其他项";
		AfxMessageBox(str);
		return FALSE;
	}
	left=str.Left(min);
	str1=str.Mid(min+1,1);
	if(str1=="=")
		right=str.Mid(min+2);
	else
	    right=str.Mid(min+1);
	min=right.FindOneOf("<>=");
	if(min!=-1)
	{
		str="表达式不等号重叠";
		AfxMessageBox(str);
		return FALSE;
	}
     min=left.Find("=");
	  if(min!=-1)
	  {
		  str="不等号前有等号";
		  AfxMessageBox(str);
		  return FALSE;
	  }
	if(CheckExpression(left,importDim,exportDim))
	{
		if(CheckExpression(right,importDim,exportDim))
			return TRUE;
	}
	return FALSE;
}

/************************************************************************/
/* 计算不等式                                                                     */
/************************************************************************/
BOOL  CMyUtil::ComputeInEquation(CString expression)
{
	int index;
	CString expre=expression;
	CString str;
	CString left,right;
	double dleft,dright;
	index=expre.FindOneOf("<>");
	if(index!=-1)//如果是小于号
	{
		str=expre.Mid(index+1,1);
		if(str!="=")///没有等号
		{
			left=expre.Left(index);
			right=expre.Mid(index+1);
			dleft=CalString(left);
			dright=CalString(right);
			str=expre.Mid(index,1);
			if(str==">")
			{
				if(dleft>dright) return TRUE;
				else return FALSE;
			}
			if(str=="<")
			{
				if(dleft<dright) return TRUE;
				else return FALSE;
			}
		}
		else ////有等号
		{
			left=expre.Left(index);
			right=expre.Mid(index+2);
			dleft=CalString(left);
			dright=CalString(right);
			str=expre.Mid(index,1);
			if(str==">")
			{
				if(dleft<dright) return FALSE;
				else return TRUE;
			}
			if(str=="<")
			{
				if(dleft>dright) return FALSE;
				else return TRUE;
			}
		}
	}
return FALSE;
}




