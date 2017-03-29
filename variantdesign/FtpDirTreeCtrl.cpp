// FtpDirTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"

#include "FtpDirTreeCtrl.h"


void CSortStringArray::AddEx(CString str,DWORD dws)
{
	this->Add(str);
	dwArray.Add(dws);
}

void CSortStringArray::RemoveAllEx()
{
	this->RemoveAll();
	dwArray.RemoveAll();
}

DWORD CSortStringArray::GetDW(int i)
{
	if(i<0||i>dwArray.GetSize()-1)
		return 0;
	return dwArray[i];
}
void CSortStringArray::Sort()
{
	BOOL bNotDone = TRUE;   
	while (bNotDone)   
	{      bNotDone = FALSE;
	for(int pos = 0;pos < GetUpperBound();pos++)
		bNotDone |= CompareAndSwap(pos);   
	}
}
BOOL CSortStringArray::CompareAndSwap( int pos )
{
	CString temp;  
	DWORD word;
	int posFirst = pos;   
	int posNext = pos + 1;
	if (GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0)   
	{
		temp = GetAt(posFirst);  
		SetAt(posFirst, GetAt(posNext));
		SetAt(posNext, temp);   
		word=dwArray.GetAt(posFirst);
		dwArray. SetAt(posFirst,dwArray.GetAt(posNext));
		dwArray.SetAt(posNext,word);  
		return TRUE;   
	}   

	return FALSE;
}


// CFtpDirTreeCtrl

IMPLEMENT_DYNAMIC(CFtpDirTreeCtrl, CTreeCtrl)
CFtpDirTreeCtrl::CFtpDirTreeCtrl()
{
	m_strRoot="";
	setFlag=FALSE;
	m_connectString="/";
	m_connectChar='/';
	this->GetDefaultFolderIndex();
}

CFtpDirTreeCtrl::~CFtpDirTreeCtrl()
{
	m_imgList.Detach();
}


BEGIN_MESSAGE_MAP(CFtpDirTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnTvnItemexpanded)
END_MESSAGE_MAP()



// CFtpDirTreeCtrl 消息处理程序
/************************************************************************/
/* ftp连接                                                                     */
/************************************************************************/
BOOL CFtpDirTreeCtrl::SetConnect(CString strIP,int port,CString user,CString password)
{
	m_strIP=strIP;
	m_port=port;
	m_user=user;
	m_password=password;
	setFlag=TRUE;
return TRUE;
}



BOOL CFtpDirTreeCtrl::Connect()
{
	if(setFlag==FALSE) return FALSE;
	HINTERNET hInternetRoot;
	hInternetRoot = InternetOpen("WinInet Example", 
		INTERNET_OPEN_TYPE_DIRECT,NULL,NULL, 0);
	hinternet=InternetConnect(hInternetRoot,m_strIP,m_port,m_user,m_password,INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE,NULL );
	if(hinternet==NULL) return FALSE;
	return TRUE;
}

BOOL CFtpDirTreeCtrl::DisConnect()
{
	return InternetCloseHandle(hinternet);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CFtpDirTreeCtrl::DisplayTree(LPCTSTR strRoot, BOOL bFiles)
{
	DWORD dwStyle = GetStyle();   // read the windowstyle
	if ( dwStyle & TVS_EDITLABELS ) 
	{
		// Don't allow the user to edit ItemLabels
		ModifyStyle( TVS_EDITLABELS , 0 );
	}

	// Display the DirTree with the Rootname e.g. C:\
	// if Rootname == NULL then Display all Drives on this PC
	// First, we need the system-ImageList

	DeleteAllItems();

	if ( !GetSysImgList() )
		return FALSE;
	m_bFiles = bFiles;  // if TRUE, Display Path- and Filenames 
	if ( strRoot == NULL || strRoot[0] == '\0' )
	{
		m_strRoot = "";
		if ( m_strRoot.Right(1) != m_connectChar )
			m_strRoot +=m_connectString;
	
		DisplayPath( NULL,NULL);
	}
	else
	{
		m_strRoot = strRoot;
		if ( m_strRoot.Right(1) != m_connectChar )
			m_strRoot +=m_connectString;

		if ( m_strRoot.Left(1) == m_connectChar )
			m_strRoot.Delete(0,1);
		HTREEITEM hParent = AddItem( TVI_ROOT, m_strRoot ,FILE_ATTRIBUTE_DIRECTORY);
		DisplayPath( hParent, strRoot );
	}	

	return TRUE;	
}



/////////////////////////////////////////////////
BOOL CFtpDirTreeCtrl::GetSysImgList()
{
	SHFILEINFO shFinfo;
	HIMAGELIST hImgList = NULL;

	if ( GetImageList( TVSIL_NORMAL ) )
		m_imgList.Detach();

	hImgList = (HIMAGELIST)SHGetFileInfo( "C:\\",
		0,
		&shFinfo,
		sizeof( shFinfo ),
		SHGFI_SYSICONINDEX | 
		SHGFI_SMALLICON );
	if ( !hImgList )
	{
		 
		m_strError = "Cannot retrieve the Handle of SystemImageList!";
		return FALSE;
	}

	if ( !m_imgList.Attach( hImgList ) )
	{
		  
		m_strError = "Cannot Attach SystemImageList-Handle";
		return FALSE;
	}

	SetImageList( &m_imgList, TVSIL_NORMAL );
	return TRUE;   // OK
}

/////////////////////////////////////////////////
void CFtpDirTreeCtrl::DisplayPath(HTREEITEM hParent, LPCTSTR strPath)
{
	//
	// Displaying the Path in the TreeCtrl
	//
if(!(this->Connect()))
{
	AfxMessageBox("ftp连接不成功");
	return;
}

	//CFileFind find;
	WIN32_FIND_DATA  finddata;

	CString   strPathFiles = strPath;
	BOOL      bFind;
	CSortStringArray strDirArray;
	CSortStringArray strFileArray;
	HINTERNET hnet;

	if ( strPathFiles.Right(1) != m_connectString )
		strPathFiles += m_connectString;
	//strPathFiles += "*.*";

	hnet=FtpFindFirstFile(hinternet,strPathFiles,&finddata,INTERNET_FLAG_NO_CACHE_WRITE,NULL);
if(hnet==NULL)  	
{
	if(strPath!=NULL)
	AfxMessageBox("错误的目录,用主目录");
	this->DeleteAllItems();
	hParent=NULL;
	hnet=FtpFindFirstFile(hinternet,NULL,&finddata,INTERNET_FLAG_NO_CACHE_WRITE,NULL);
}
bFind=TRUE;
	while(bFind)
	{
		DWORD flag1,flag2,flag3;
		flag1=finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY;
		flag2=finddata.dwFileAttributes&FILE_ATTRIBUTE_SYSTEM;
		flag3=finddata.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN;
       BOOL bResult;
	   bResult=TRUE;
		if (finddata.cFileName[0] == '.')
		{
			if (finddata.cFileName[1] == '\0' ||
				(finddata.cFileName[1] == '.' &&
				finddata.cFileName[2] == '\0'))
			{
				bResult = FALSE;
			}
		}
		CString str=CString(finddata.cFileName);
		if(str=="Recycled") bResult=FALSE;
		if(flag1!=0&&bResult&&flag2==0)
		{
			strDirArray.AddEx(finddata.cFileName,finddata.dwFileAttributes);
		}

		if(flag1==0&&bResult&&flag2==0)
		{
			strFileArray.AddEx(finddata.cFileName,finddata.dwFileAttributes);
		}

		bFind=InternetFindNextFile(hnet,&finddata);
	}

	this->DisConnect();
	strDirArray.Sort();
	SetRedraw( FALSE );
	CWaitCursor wait;

	for ( int i = 0; i < strDirArray.GetSize(); i++ )
	{
		HTREEITEM hItem = AddItem( hParent, strDirArray.GetAt(i),strDirArray.GetDW(i));
		if ( FindSubDir( strDirArray.GetAt(i) ) )
			InsertItem( "", 0, 0, hItem );
	}

	if ( m_bFiles )
	{
		strFileArray.Sort();
		for ( i = 0; i < strFileArray.GetSize(); i++ )
		{
			HTREEITEM hItem = AddItem( hParent, strFileArray.GetAt(i),strFileArray.GetDW(i) );
		}
	}

	SetRedraw( TRUE );
}


HTREEITEM CFtpDirTreeCtrl::AddItem(HTREEITEM hParent, LPCTSTR strPath,DWORD dwFileAttributes)
{
	// Adding the Item to the TreeCtrl with the current Icons
	SHFILEINFO shFinfo;
	int iIcon, iIconSel;
	CString    strTemp = strPath;
  
	if ( strTemp.Right(1) !=m_connectChar)
		strTemp += m_connectString;
	CString str;

	str=ChangeString(strTemp);
	if ( !SHGetFileInfo( str,
		dwFileAttributes,
		&shFinfo,
		sizeof( shFinfo ),
		SHGFI_ICON | 
		SHGFI_SMALLICON|SHGFI_USEFILEATTRIBUTES) )
	{
		m_strError = "Error Gettting SystemFileInfo!";
		//return NULL;
		iIcon=m_iIcon;
	}
	else
	{
		iIcon = shFinfo.iIcon;
	}


	if ( !SHGetFileInfo( str,
		dwFileAttributes,
		&shFinfo,
		sizeof( shFinfo ),
		SHGFI_ICON | SHGFI_OPENICON |
		SHGFI_SMALLICON|SHGFI_USEFILEATTRIBUTES  ) )
	{
		m_strError = "Error Gettting SystemFileInfo!";
		//return NULL;
		iIconSel=m_iIconSel;
	}
	else
	{
		iIconSel = shFinfo.iIcon;
	}
	
	if ( strTemp.Right(1) == m_connectChar )
		strTemp.SetAt( strTemp.GetLength() - 1, '\0' );

	if ( hParent == TVI_ROOT )
		return InsertItem( strTemp, iIcon, iIconSel, hParent );

	return InsertItem( GetSubPath( strTemp ), iIcon, iIconSel, hParent );
}

LPCTSTR CFtpDirTreeCtrl::GetSubPath(LPCTSTR strPath)
{
	//
	// getting the last SubPath from a PathString
	// e.g. C:\temp\readme.txt
	// the result = readme.txt
	static CString strTemp;
	int     iPos;

	strTemp = strPath;
	if ( strTemp.Right(1) ==m_connectChar)
		strTemp.SetAt( strTemp.GetLength() - 1, '\0' );
	iPos = strTemp.ReverseFind( m_connectChar );
	if ( iPos != -1 )
		strTemp = strTemp.Mid( iPos + 1);

	return (LPCTSTR)strTemp;
}

BOOL CFtpDirTreeCtrl::DisplayDrives()
{
	return TRUE;
}

BOOL CFtpDirTreeCtrl::FindSubDir( LPCTSTR strPath )
{
	return TRUE;
}

CString CFtpDirTreeCtrl::GetFullPath(HTREEITEM hItem)
{
	// get the Full Path of the item
	CString strReturn;
	CString strTemp;
	HTREEITEM hParent = hItem;

	strReturn = "";

	while ( hParent )
	{

		strTemp  = GetItemText( hParent );
		strTemp += m_connectString;
		strReturn = strTemp + strReturn;
		hParent = GetParentItem( hParent );
	}
	if ( strReturn.Right(1) == m_connectChar )
		strReturn.SetAt( strReturn.GetLength() - 1, '\0' );

	return strReturn;

}


BOOL CFtpDirTreeCtrl::SetSelPath(LPCTSTR strPath)
{
	// Setting the Selection in the Tree
	HTREEITEM hParent  = TVI_ROOT;
	int       iLen    = (int) strlen(strPath) + 2;
	char*     pszPath = new char[iLen];
	char*     pPath   = pszPath;
	BOOL      bRet    = FALSE;

	if ( !IsValidPath( strPath ) )
	{
		delete [] pszPath; // this must be added 29.03.99
		return FALSE;
	}

	strcpy( pszPath, strPath );
	strupr( pszPath );

	if ( pszPath[strlen(pszPath)-1] != m_connectChar )
		strcat( pszPath, m_connectString );

	int iLen2 = (int) strlen( pszPath );

	for (WORD i = 0; i < iLen2; i++ )
	{
		if ( pszPath[i] ==m_connectChar )
		{
			SetRedraw( FALSE );
			pszPath[i] = '\0';
			hParent = SearchSiblingItem( hParent, pPath );
			if ( !hParent )  // Not found!
				break;
			else
			{				
				// Info:
				// the notification OnItemExpanded 
				// will not called every time 
				// after the call Expand. 
				// You must call Expand with TVE_COLLAPSE | TVE_COLLAPSERESET
				// to Reset the TVIS_EXPANDEDONCE Flag

				UINT uState;
				uState = GetItemState( hParent, TVIS_EXPANDEDONCE );
				if ( uState )
				{
					Expand( hParent, TVE_EXPAND );
					Expand( hParent, TVE_COLLAPSE | TVE_COLLAPSERESET );
					InsertItem("", hParent ); // insert a blank child-item
					Expand( hParent, TVE_EXPAND ); // now, expand send a notification
				}
				else
					Expand( hParent, TVE_EXPAND );
			}
			pPath += strlen(pPath) + 1;
		}
	}

	delete [] pszPath;

	if ( hParent ) // Ok the last subpath was found
	{		
		SelectItem( hParent ); // select the last expanded item
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	SetRedraw( TRUE );

	return bRet;
}

HTREEITEM CFtpDirTreeCtrl::SearchSiblingItem( HTREEITEM hItem, LPCTSTR strText)
{
	HTREEITEM hFound = GetChildItem( hItem );
	CString   strTemp;
	while ( hFound )
	{
		strTemp = GetItemText( hFound );
		strTemp.MakeUpper();
		if ( strTemp == strText )
			return hFound;
		hFound = GetNextItem( hFound, TVGN_NEXT );
	}

	return NULL;
}

void CFtpDirTreeCtrl::ExpandItem(HTREEITEM hItem, UINT nCode)
{	
	CString strPath;

	if ( nCode == TVE_EXPAND )
	{
		HTREEITEM hChild = GetChildItem( hItem );
		while ( hChild )
		{
			DeleteItem( hChild );
			hChild = GetChildItem( hItem );
		}

		strPath = GetFullPath( hItem );
		DisplayPath( hItem, strPath );
	}
}

void CFtpDirTreeCtrl::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CString strPath;

	if ( pNMTreeView->itemNew.state & TVIS_EXPANDED )
	{
		//UINT uTest = TVIS_EXPANDEDONCE;
		ExpandItem( pNMTreeView->itemNew.hItem, TVE_EXPAND );
		/*
		//
		// Delete All items
		// And display the subpath
		//
		HTREEITEM hChild = GetChildItem( pNMTreeView->itemNew.hItem );
		while ( hChild )
		{
		DeleteItem( hChild );
		hChild = GetChildItem( pNMTreeView->itemNew.hItem );
		}

		strPath = GetFullPath( pNMTreeView->itemNew.hItem );
		DisplayPath( pNMTreeView->itemNew.hItem, strPath );
		*/
	}
	else
	{
		//
		// Delete the Items, but leave one there, for 
		// expanding the item next time
		//
		HTREEITEM hChild = GetChildItem( pNMTreeView->itemNew.hItem );

		while ( hChild ) 
		{
			DeleteItem( hChild );
			hChild = GetChildItem( pNMTreeView->itemNew.hItem );
		}
		InsertItem( "", pNMTreeView->itemNew.hItem );
	}

	*pResult = 0;
}


BOOL CFtpDirTreeCtrl::IsValidPath(LPCTSTR strPath)
{
	// This function check the Pathname

	//HTREEITEM hChild;
	//CString   strItem;
	//CString   strTempPath = strPath;
	//BOOL      bFound = FALSE;
	//CFileFind find;

	//hChild = GetChildItem( TVI_ROOT );
	//strTempPath.MakeUpper();

	//while ( hChild )
	//{
	//	strItem = GetItemText( hChild );//
	//	strItem.MakeUpper();
	//	if ( strItem == strTempPath.Mid( 0, strItem.GetLength() ) )
	//	{
	//		bFound = TRUE;
	//		break;
	//	}
	//	hChild = GetNextItem( hChild, TVGN_NEXT );
	//}

	//if ( !bFound )
	//	return FALSE;

	//if ( find.FindFile( strPath ) )
	//	return TRUE;

	//return FALSE;
	return TRUE;
}

CString CFtpDirTreeCtrl::ChangeString(CString older)
{
  CString thenew=older;
  thenew.Replace("/","\\");
  return thenew;
}

void CFtpDirTreeCtrl::GetDefaultFolderIndex()
{
CString str;
SHFILEINFO shFinfo;
str.Format("c:\\windows");
	if ( !SHGetFileInfo( str,
		0,
		&shFinfo,
		sizeof( shFinfo ),
		SHGFI_ICON | 
		SHGFI_SMALLICON) )
	{
		m_strError = "Error Gettting SystemFileInfo!";
		return;
	}

	m_iIcon = shFinfo.iIcon;

	if ( !SHGetFileInfo( str,
		0,
		&shFinfo,
		sizeof( shFinfo ),
		SHGFI_ICON | SHGFI_OPENICON |
		SHGFI_SMALLICON  ) )
	{
		m_strError = "Error Gettting SystemFileInfo!";
		return;
	}
	m_iIconSel = shFinfo.iIcon;
}


CString CFtpDirTreeCtrl::GetFullPathEx( HTREEITEM hItem )
{
   CString str;
   str=this->GetFullPath(hItem);
   if(str.Left(1)!=m_connectString)
   {
	   str=m_connectString+str;
   }
   return str;
}