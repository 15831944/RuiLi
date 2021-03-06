#include "stdafx.h"
#include "XtPdmDocuments.h"
BOOL UsrGetPartAndProductFromPdm(CString PdmNum,int type,CString & d3file,CString & d2file)
{
	CXtPdmDocuments  m_set;
	m_set.m_strFilter.Format("Code='%s'",PdmNum);
	try
	{
		m_set.Open();
		if(!m_set.IsBOF())
		{
			m_set.MoveFirst();
			do {
				if (m_set.m_FileName.Find(".drw",0)==-1)
				{
					d3file.Format("/Xt/Proven/%s/%s/%s/%s",m_set.m_ProductDwgCode,m_set.m_Code,m_set.m_RevNumber,m_set.m_FileName);					
				}
				else
				{
					d2file.Format("/Xt/Proven/%s/%s/%s/%s",m_set.m_ProductDwgCode,m_set.m_Code,m_set.m_RevNumber,m_set.m_FileName);
				}
				m_set.MoveNext();
			} while(!m_set.IsEOF());			
		}
		else
			return FALSE;
	}
	catch (CDBException * e ) {
		AfxMessageBox(e->m_strError);
		delete e;
		return FALSE;
	}
	return TRUE;
}
 