#pragma once

//////////////////////////////////////////////////////////////////////
#include <afxtempl.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CSortStringArray : public CStringArray  
{
public:
	CArray <DWORD,DWORD> dwArray;
	void Sort();
	void AddEx(CString str,DWORD dws);
	void RemoveAllEx();
	DWORD GetDW(int i);
private: 
	BOOL CompareAndSwap( int pos );

};



#include <WinInet.h>
#pragma comment (lib,"Wininet.lib")
// CFtpDirTreeCtrl

class CFtpDirTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CFtpDirTreeCtrl)

public:
	CFtpDirTreeCtrl();
	virtual ~CFtpDirTreeCtrl();
protected:
	CString m_connectString;
	char m_connectChar;
	BOOL setFlag;
	HINTERNET  hinternet;
	BOOL m_bFiles;
	CString m_strError;
	CImageList m_imgList;
	CString m_strRoot;
	CString m_strIP,m_user,m_password;
	int m_port;
	int m_iIcon,m_iIconSel;
public:
	BOOL SetConnect(CString strIP,int port,CString user,CString password);
	BOOL Connect();
	BOOL DisConnect();
	BOOL SetSelPath( LPCTSTR strPath );
	CString GetFullPath( HTREEITEM hItem );
	LPCTSTR GetSubPath( LPCTSTR strPath );
	BOOL DisplayTree( LPCTSTR strRoot, BOOL bFiles = FALSE );
	CString GetFullPathEx( HTREEITEM hItem );
protected:
	BOOL IsValidPath( LPCTSTR strPath );
	void ExpandItem( HTREEITEM hItem, UINT nCode );
	HTREEITEM SearchSiblingItem( HTREEITEM hItem, LPCTSTR strText );
	BOOL FindSubDir( LPCTSTR strPath );
	HTREEITEM AddItem( HTREEITEM hParent, LPCTSTR strPath , DWORD dwFileAttributes);
	void DisplayPath( HTREEITEM hParent, LPCTSTR strPath );
	BOOL DisplayDrives();
	BOOL GetSysImgList();
	CString ChangeString(CString older);
	void GetDefaultFolderIndex();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
};


