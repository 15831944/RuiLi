#include "stdafx.h"
#include "resource.h"
/************************************************************************/
/* ������ʽ 
�����ҵ�����,�����Լ�����������ı��ʽ,�ж��Ƿ��Ǻ���
�������滻��ĳһʵ��.���´�����ʽ.
��ͷ����,���ȿ��Ƿ�ʵ��.
����ʵ��,������Array��,�ҵ�ƥ����,��Arrayrray��;
���򷵻س���.
��������ͬ�������ȥ��,���´�����ʽ.

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
			str="���Ų�ƥ��";
			AfxMessageBox(str);
			return FALSE;
		}
		sub=str.Mid(min+1,rIndex-min+1-2);///����������ַ���
		if(CheckExpression(sub,importDim,exportDim)==FALSE) return FALSE;
		sub=str.Mid(min,rIndex-min+1);///�������ŵ��ַ���
		///��������,����
		CString str1,str3;
		str1=str.Mid(0,min);///���ǰ��α��ʽ,����������.
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
				str="����ǰ��Ҫ���������";
				AfxMessageBox(str);
				return FALSE;
			}
		}
		////�����������
		str.Replace(sub,"1.23456789");
		return CheckExpression(str,importDim,exportDim);
	}
	min=0;
	str=Expression.Tokenize("*+/-",min);
	if(str.GetLength()!=min-1)
	{
		str=""+str+"  ǰ���ص��������";
		str.Replace("1.23456789","����");
		AfxMessageBox(str);
		return FALSE;
	}
	///�Ƿ�ʵ��
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
				str="��  "+str+"   ������ִ���,������ź��������������";
				str.Replace("1.23456789","����");
				AfxMessageBox(str);
				return FALSE;
			}
			str=Expression.Mid(min);
			return CheckExpression(str,importDim,exportDim);
		}
	}

	/////////////�Ƿ�ߴ�������ʶ
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
	if(str=="PI"||str=="E")/////�жϳ���
		nFlag=TRUE;
	if(nFlag==FALSE) 
	{
		min=str.Find("1.23456789");
		if(min!=-1)
			str="�����������,��ƥ��";
		else
			str="�Ҳ���"+str+"��һ���������Ͳ�Ϊ��ֵ��,����";
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
			str="��  "+str+"   ������ִ���,������ź��������������";
			str.Replace("1.23456789","����");
			AfxMessageBox(str);
			return FALSE;
		}
		str=Expression.Mid(min);
		return CheckExpression(str,importDim,exportDim);
	}

	return TRUE;
}

/************************************************************************/
/* ��鷽�� 
�����жϵȺ�.
�ڷֱ�����߱��ʽ���ұ߱��ʽ.
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
		str="����û�еȺ�";
		AfxMessageBox(str);
		return FALSE;
	}

	if(min==0)
	{
		str="���̵Ⱥ�ǰ����������";
		AfxMessageBox(str);
		return FALSE;
	}

	if(min==str.GetLength()-1)
	{
		str="���̵Ⱥź�����������";
		AfxMessageBox(str);
		return FALSE;
	}
	left=str.Left(min);
	right=str.Mid(min+1);
	min=right.Find("=");
	if(min!=-1)
	{
		str="���̵Ⱥ�����������";
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
/* ����ģ���еĹ�ϵʽ(ProE�ڲ�����Ĺ�ϵʽ)                             */
/************************************************************************/
BOOL VisitSolidRel(ProSolid solid,CStringArray * relSet)
{
	int                          num=0;
	CArray<ProRelset,ProRelset> p_ProRelSetList;
	//����ģ�͵Ĺ�ϵʽ
	if(ProSolidRelsetVisit(solid,(ProSolidRelsetVisitAction)UsrSolidRelsetVisAc,
		(CArray<ProRelset,ProRelset>*)&p_ProRelSetList)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("���ʹ�ϵʽʧ�ܣ�");
		return false;
	}

	num=(int)p_ProRelSetList.GetCount();
	for(int i=0;i<num;i++)
	{
		ProWstring *w_array;                        //�ַ�������
		int n_lines;
		ProArrayAlloc(0,sizeof(ProWstring),1,(ProArray*)&w_array);
		ProRelsetRelationsGet(&p_ProRelSetList[i],&w_array);
		ProArraySizeGet((ProArray)w_array,&n_lines);
		for(int j=0;j<n_lines;j++)
		{
			CString temp;
			temp=CString(w_array[j]);
			//�ѿ��и��ų�
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
