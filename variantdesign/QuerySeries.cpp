// QuerySeries.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "QuerySeries.h"
#include "MyDatabase.h"
#include "ProWindows.h"


extern CMyDatabase m_db;


extern int thePrtModleNum;

// CQuerySeries 对话框

IMPLEMENT_DYNAMIC(CQuerySeries, CDialog)
CQuerySeries::CQuerySeries(CWnd* pParent /*=NULL*/)
	: CDialog(CQuerySeries::IDD, pParent)
{
}

CQuerySeries::~CQuerySeries()
{
}

void CQuerySeries::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SeriesList, m_SeriesList);
}


BEGIN_MESSAGE_MAP(CQuerySeries, CDialog)
	ON_BN_CLICKED(IDC_BtnQuit, OnBtnQuit)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CQuerySeries 消息处理程序

void CQuerySeries::OnBtnQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

BOOL CQuerySeries::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitSeriesData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CQuerySeries::InitSeriesData(void)
{
	CStringArray  strArr;
	strArr.RemoveAll();

		//delete old headerctrls
		int nColumn=0;
		nColumn=m_SeriesList.GetHeaderCtrl()->GetItemCount();
		for(int j=0;j<nColumn;j++)
		{
			m_SeriesList.DeleteColumn(0);
		}

		//query the database
		CString strSql;
		strSql.Format("select * from NPartVariantTable where PrtNum=%d",thePrtModleNum);
		if(m_db.Query(strSql)!=0)
		{
			int i=0;
			CString str,strCluName;
			m_db.MoveBegin();
			m_SeriesList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
			do{
				m_SeriesList.InsertColumn(i,m_db.getString("Name"),LVCFMT_LEFT,100,-1);
				str=m_db.getString("Rules");
				strArr.Add(str);
				i++;
			}while(m_db.MoveNext());
			//CString csValidCharacter="0123456789.";
			//m_SeriesList.SetColumnValidEditCtrlCharacters(csValidCharacter,2);
			m_SeriesList.DeleteAllItems();
			if(!strArr.IsEmpty())
			{
				int mRow=0,nCol=0;
				nCol=m_SeriesList.GetHeaderCtrl()->GetItemCount();

				int nStart,nEnd,nLength;
				CString str;
				str=strArr.GetAt(0);
				nLength=str.GetLength();
				nStart=0;
				nEnd=0;
				while(nEnd!=nLength-1)
				{
					nEnd=str.Find('*',nStart);
					if(nEnd!=-1)
					{
						//nCount=nEnd-nStart;
						//userAttr.strStand=str.Mid(nStart,nCount);
						nStart=nEnd+1;
						mRow++;
					}
					else
						break;
				}

				for(int i=0;i<mRow;i++)
				{
					CString strOneAll,strValue;
					strOneAll=strArr.GetAt(0);
					int nS=0,nE=0;
					for(int r=0;r<i+1;r++)
					{
						nS=nE;
						nE=strOneAll.Find('*',nS+1);						
					}
					if(i==0)
						strValue=strOneAll.Mid(0,nE);
					else
						strValue=strOneAll.Mid(nS+1,nE-nS-1);

					m_SeriesList.InsertItem(i,strValue);
					for(int j=1;j<nCol;j++)
					{
						nS=nE=0;
						strOneAll=strArr.GetAt(j);
						for(int r=0;r<i+1;r++)
						{
							nS=nE;
							nE=strOneAll.Find('*',nS+1);						
						}
						if(i==0)
							strValue=strOneAll.Mid(0,nE);
						else
							strValue=strOneAll.Mid(nS+1,nE-nS-1);

						m_SeriesList.SetItemText(i,j,strValue);
					}
				}
			}
		}
}

void CQuerySeries::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRefresh(PRO_VALUE_UNUSED);

	// TODO: 在此添加消息处理程序代码
}
