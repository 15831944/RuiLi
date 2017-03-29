#include "stdafx.h"
#include "resource.h"
/************************************************************************/
/* 检验表达式 
首先找到括号,调用自己处理括号里的表达式,判断是否是函数
将括号替换成某一实数.重新处理表达式.
从头处理,首先看是否实数.
不是实数,在输入Array中,找到匹配项,并Arrayrray中;
否则返回出错.
将此项连同运算符号去掉,重新处理表达式.

*/
/************************************************************************/
BOOL CheckExpression(CString Expression,CStringArray * importDim,CStringArray * exportDim)
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
			str="找不到"+str+"这一项或该项类型不为数值型,请检查";
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
BOOL CheckEquation(CString Expression,CStringArray * importDim,CStringArray * exportDim)
{
	int     min;
	CString str,left,right;
	str=Expression.MakeLower();
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
/* 访问模型中的关系式(ProE内部定义的关系式)                             */
/************************************************************************/
BOOL VisitSolidRel(ProSolid solid,CStringArray * relSet)
{
	int                          num=0;
	CArray<ProRelset,ProRelset> p_ProRelSetList;
	//访问模型的关系式
	if(ProSolidRelsetVisit(solid,(ProSolidRelsetVisitAction)UsrSolidRelsetVisAc,
		(CArray<ProRelset,ProRelset>*)&p_ProRelSetList)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("访问关系式失败！");
		return false;
	}

	num=(int)p_ProRelSetList.GetCount();
	for(int i=0;i<num;i++)
	{
		ProWstring *w_array;                        //字符串数组
		int n_lines;
		ProArrayAlloc(0,sizeof(ProWstring),1,(ProArray*)&w_array);
		ProRelsetRelationsGet(&p_ProRelSetList[i],&w_array);
		ProArraySizeGet((ProArray)w_array,&n_lines);
		for(int j=0;j<n_lines;j++)
		{
			CString temp;
			temp=CString(w_array[j]);
			//把空行给排除
			if(temp=="") continue;
			relSet->Add(temp);
		}
		ProWstringproarrayFree(w_array);

	}
	return TRUE;
}
ProError UsrSolidRelsetVisAc(ProRelset *p_relset,CArray<ProRelset,ProRelset>* caller_data)
{
	caller_data->Add(*p_relset);
	return PRO_TK_NO_ERROR;
}
BOOL IsDigit(int c)
{
	if(c>='0'&&c<='9')
		return TRUE;
	else
		return FALSE;
}

BOOL IsDouble(CString str)
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
