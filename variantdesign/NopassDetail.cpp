// NopassDetail.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "NopassDetail.h"
#include "MyDatabase.h"

extern CMyDatabase m_db;
extern int thePrtModleNum;

extern CStringArray strValueArray;
extern CStringArray strInputValueArray;
extern CArray<int,int> IntNumArray;
extern CArray<int,int> IntSelNumArray;


int nCloumCount=0;

// CNopassDetail 对话框

IMPLEMENT_DYNAMIC(CNopassDetail, CDialog)
CNopassDetail::CNopassDetail(CWnd* pParent /*=NULL*/)
	: CDialog(CNopassDetail::IDD, pParent)
	, svalue(0.1)
	, Bfirst(TRUE)
{
}

CNopassDetail::~CNopassDetail()
{
}

void CNopassDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SeriesList, m_DetailList);
}


BEGIN_MESSAGE_MAP(CNopassDetail, CDialog)
	ON_BN_CLICKED(IDC_BtnQuit, OnBtnquit)
	ON_BN_CLICKED(IDC_RePiPei, OnRepipei)
END_MESSAGE_MAP()


// CNopassDetail 消息处理程序

BOOL CNopassDetail::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitListData();
	SetDlgItemText(IDC_Value,"10");

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CNopassDetail::OnBtnquit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

void CNopassDetail::InitListData(void)
{
	m_DetailList.DeleteAllItems();
	//query the database
	CString strSql;
	CString str;

	//chu shi hua lie 只需一次
	if(Bfirst)
	{
		strSql.Format("select * from NPartVariantTable where PrtNum=%d",thePrtModleNum);
		if(m_db.Query(strSql)!=0)
		{
			CString str,strCluName;
			m_db.MoveBegin();
			m_DetailList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
			do{
				m_DetailList.InsertColumn(nCloumCount,m_db.getString("Name"),LVCFMT_LEFT,100,-1);
				nCloumCount++;
			}while(m_db.MoveNext());
			m_DetailList.InsertColumn(nCloumCount,"零件名称",LVCFMT_LEFT,100,-1);
			m_DetailList.InsertColumn(nCloumCount+1,"零件编号",LVCFMT_LEFT,100,-1);
		}
		Bfirst=FALSE;
	}
	//insert the input values
	m_DetailList.InsertItem(0,strInputValueArray.GetAt(0));
	for(int i=1;i<strInputValueArray.GetCount();i++)
	{
		m_DetailList.SetItemText(0,i,strInputValueArray.GetAt(i));
	}
	m_DetailList.SetItemText(0,nCloumCount,"输入值");

	//insert the exist values
	for(int i=0;i<IntSelNumArray.GetCount();i++)
	{
		int nRank=0;
		int PrtNum=0;
		PrtNum=IntSelNumArray.GetAt(i);

		for(int j=0;j<IntNumArray.GetCount();j++)
		{
			if(PrtNum==IntNumArray.GetAt(j))
			{
				nRank=j;
				break;
			}
		}

		//insert data value
		str=strValueArray.GetAt(nRank*strInputValueArray.GetCount());
		m_DetailList.InsertItem(i+1,strValueArray.GetAt(nRank*strInputValueArray.GetCount()));
		for(int k=1;k<strInputValueArray.GetCount();k++)
		{
			str=strValueArray.GetAt(nRank*strInputValueArray.GetCount()+k);
			m_DetailList.SetItemText(i+1,k,strValueArray.GetAt(nRank*strInputValueArray.GetCount()+k));
		}
		//input name
		strSql.Format("select * from NPartInsInfoTable where PrtNum=%d",PrtNum);
		if(m_db.Query(strSql)!=0)
		{
			m_db.MoveBegin();
			str=m_db.getString("Name");
			str=m_db.getString("InsCode");
			m_DetailList.SetItemText(i+1,nCloumCount,m_db.getString("Name"));
			m_DetailList.SetItemText(i+1,nCloumCount+1,m_db.getString("InsCode"));
		}
	}

}

void CNopassDetail::OnRepipei()
{
	CString str;
	GetDlgItemText(IDC_Value,str);
	float fInput=0.1;
	fInput=atof(str);
	if(fInput<0)
		fInput=0;
	if(fInput>100)
		fInput=100;
	svalue=fInput/100;

	ExamTheDatas();
	InitListData();
}

BOOL CNopassDetail::ExamTheDatas(void)
{
	//get all the instance value
	//get the prtnum first
	CString strSql,str;
	strSql.Format("select DISTINCT PrtNum from NPartInsVariantTable where MdlNum=%d",thePrtModleNum);
	if(m_db.Query(strSql)!=0)
	{
		int num;
		m_db.MoveBegin();
		IntNumArray.RemoveAll();
		do{
			num=m_db.getInt("PrtNum");
			IntNumArray.Add(num);
		}while(m_db.MoveNext());
	}

	//get the str value
	strValueArray.RemoveAll();
	for(int i=0;i<IntNumArray.GetCount();i++)
	{
		int num=0;
		num=IntNumArray.GetAt(i);
		strSql.Format("select * from NPartInsVariantTable where PrtNum=%d",num);
		if(m_db.Query(strSql)!=0)
		{
			m_db.MoveBegin();
			do {
				str=m_db.getString("Value");
				strValueArray.Add(str);
			} while(m_db.MoveNext());
		}
	}

	//compare the values
	//compute the standard value
	double theStandardValue=0.0;
	for(int i=0;i<strInputValueArray.GetCount();i++)
	{
		theStandardValue+=abs(atof(strInputValueArray.GetAt(i)))*svalue;
	}

	//compute the result
	int nCount=0;
	double theResult=0;
	IntSelNumArray.RemoveAll();
	nCount=strValueArray.GetCount()/strInputValueArray.GetCount();
	for(int i=0;i<nCount;i++)
	{
		theResult=0;
		for(int j=0;j<strInputValueArray.GetCount();j++)
		{
			theResult+=abs(atof(strValueArray.GetAt(i*strInputValueArray.GetCount()+j))-atof(strInputValueArray.GetAt(j)));
		}
		if(theResult<=theStandardValue)
			IntSelNumArray.Add(IntNumArray.GetAt(i));
	}
	if(IntSelNumArray.GetCount()>0)
        return false;
	return TRUE;
}
