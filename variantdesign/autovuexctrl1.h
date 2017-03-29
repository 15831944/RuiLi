#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意：不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被改写。

/////////////////////////////////////////////////////////////////////////////
// CAutovuexctrl1 包装类

class CAutovuexctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CAutovuexctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xB6FCC215, 0xD303, 0x11D1, { 0xBC, 0x6C, 0x0, 0x0, 0xC0, 0x78, 0x79, 0x7F } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:


// 操作
public:

// _DAutoVueX

// Functions
//

void ZoomFit()
{
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void ZoomPrevious()
{
	InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void ZoomWidth()
{
	InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void ZoomHeight()
{
	InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void ZoomByFactor(double factor)
{
	static BYTE parms[] = VTS_R8 ;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, factor);
}
void ZoomFullResolution()
{
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void PrintIt(BOOL bPrintDirect)
{
	static BYTE parms[] = VTS_BOOL ;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bPrintDirect);
}
void PrintPreview(BOOL bPreviewDirect, BOOL bWantFrame)
{
	static BYTE parms[] = VTS_BOOL VTS_BOOL ;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bPreviewDirect, bWantFrame);
}
long GetVcetHandle()
{
	long result;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
BOOL EnterMarkupMode()
{
	BOOL result;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}
BOOL ExitMarkupMode()
{
	BOOL result;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}
LPDISPATCH MrkObj()
{
	LPDISPATCH result;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}
LPDISPATCH PrnObj()
{
	LPDISPATCH result;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}
short GetMode()
{
	short result;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}
void PageNext()
{
	InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void PagePrevious()
{
	InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void PageSelect()
{
	InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
BOOL GetMousePos(double * pPosX, double * pPosY)
{
	BOOL result;
	static BYTE parms[] = VTS_PR8 VTS_PR8 ;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, pPosX, pPosY);
	return result;
}
long MrkWnd()
{
	long result;
	InvokeHelper(0x27, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long PrnWnd()
{
	long result;
	InvokeHelper(0x28, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
BOOL CanClose()
{
	BOOL result;
	InvokeHelper(0x29, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}
long GetContrast()
{
	long result;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long SetContrast(long contrastValue)
{
	long result;
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x2b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, contrastValue);
	return result;
}
long GetAntiAlias()
{
	long result;
	InvokeHelper(0x2c, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long SetAntiAlias(long antialiasValue)
{
	long result;
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x2d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, antialiasValue);
	return result;
}
long ShowLayersDlg()
{
	long result;
	InvokeHelper(0x2e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long ShowBlocksDlg()
{
	long result;
	InvokeHelper(0x2f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long ShowNamedViewsDlg()
{
	long result;
	InvokeHelper(0x30, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long ShowXREFDlg()
{
	long result;
	InvokeHelper(0x31, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
BOOL EnableMarkup(BOOL fShow)
{
	BOOL result;
	static BYTE parms[] = VTS_BOOL ;
	InvokeHelper(0x32, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, fShow);
	return result;
}
BOOL SetMarkupMode(LPCTSTR sMarkup)
{
	BOOL result;
	static BYTE parms[] = VTS_BSTR ;
	InvokeHelper(0x33, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sMarkup);
	return result;
}
void SetPrintOptions(LPCTSTR sEntry, LPCTSTR sValue)
{
	static BYTE parms[] = VTS_BSTR VTS_BSTR ;
	InvokeHelper(0x34, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sEntry, sValue);
}
BOOL SetLicenseFilePath(LPCTSTR sLicPath)
{
	BOOL result;
	static BYTE parms[] = VTS_BSTR ;
	InvokeHelper(0x35, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sLicPath);
	return result;
}
void ZoomInWorld(double MinX, double MinY, double MaxX, double MaxY)
{
	static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
	InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MinX, MinY, MaxX, MaxY);
}
void EnablePanMode(BOOL bEnable)
{
	static BYTE parms[] = VTS_BOOL ;
	InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bEnable);
}
long GetNumCrossProbeEntities()
{
	long result;
	InvokeHelper(0x38, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
CString GetCrossProbeEntityType(long nIndex)
{
	CString result;
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x39, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nIndex);
	return result;
}
CString GetCrossProbeEntityName(long nIndex)
{
	CString result;
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x3a, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nIndex);
	return result;
}
void ClearCrossProbeEntities()
{
	InvokeHelper(0x3b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void AddCrossProbeEntity(LPCTSTR sType, LPCTSTR sName)
{
	static BYTE parms[] = VTS_BSTR VTS_BSTR ;
	InvokeHelper(0x3c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sType, sName);
}
void ZoomSelected()
{
	InvokeHelper(0x3d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void ShowNetConnectivity()
{
	InvokeHelper(0x3e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
long GetNumEcadEntities(LPCTSTR sType)
{
	long result;
	static BYTE parms[] = VTS_BSTR ;
	InvokeHelper(0x3f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sType);
	return result;
}
CString GetEcadEntityName(LPCTSTR sType, long lIndex)
{
	CString result;
	static BYTE parms[] = VTS_BSTR VTS_I4 ;
	InvokeHelper(0x40, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sType, lIndex);
	return result;
}
long GetEcadEntityNAttr(LPCTSTR sType, long lIndex)
{
	long result;
	static BYTE parms[] = VTS_BSTR VTS_I4 ;
	InvokeHelper(0x41, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sType, lIndex);
	return result;
}
CString GetEcadEntityAttrName(LPCTSTR sType, long lIndex, long j)
{
	CString result;
	static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 ;
	InvokeHelper(0x42, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sType, lIndex, j);
	return result;
}
CString GetEcadEntityAttrValue(LPCTSTR sType, long lIndex, long j)
{
	CString result;
	static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 ;
	InvokeHelper(0x43, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sType, lIndex, j);
	return result;
}
long GetNumEcadEntityDefs()
{
	long result;
	InvokeHelper(0x44, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
CString GetEcadEntityDefName(long lIndex)
{
	CString result;
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x45, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, lIndex);
	return result;
}
long GetEcadEntityDefFlags(long lIndex)
{
	long result;
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x46, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lIndex);
	return result;
}
long GetEcadEntityDefNAttr(long lIndex)
{
	long result;
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x47, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lIndex);
	return result;
}
CString GetEcadEntityDefAttrName(long lIndex, long lAttr)
{
	CString result;
	static BYTE parms[] = VTS_I4 VTS_I4 ;
	InvokeHelper(0x48, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, lIndex, lAttr);
	return result;
}
long GetEcadEntityDefAttrFlags(long lIndex, long lAttr)
{
	long result;
	static BYTE parms[] = VTS_I4 VTS_I4 ;
	InvokeHelper(0x49, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lIndex, lAttr);
	return result;
}
long GetEcadEntityDefAttrType(long lIndex, long lAttr)
{
	long result;
	static BYTE parms[] = VTS_I4 VTS_I4 ;
	InvokeHelper(0x4a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lIndex, lAttr);
	return result;
}
long ShowEntityTypeFilterDlg()
{
	long result;
	InvokeHelper(0x4b, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long ShowVerifyDesignDlg()
{
	long result;
	InvokeHelper(0x4c, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long ShowEntityBrowserDlg()
{
	long result;
	InvokeHelper(0x4d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long ShowBillOfMaterialDlg()
{
	long result;
	InvokeHelper(0x4e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long ShowEntityPropertiesDlg()
{
	long result;
	InvokeHelper(0x4f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long ShowFileVersionInfoDlg()
{
	long result;
	InvokeHelper(0x50, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
void SetPageByTitle(LPCTSTR sTitle)
{
	static BYTE parms[] = VTS_BSTR ;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sTitle);
}
void SetNamedView(long lIndex)
{
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lIndex);
}
void SetNamedViewByName(LPCTSTR sName)
{
	static BYTE parms[] = VTS_BSTR ;
	InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sName);
}
long ShowImportDesignDlg()
{
	long result;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
void EnableZoomBoxMode(BOOL bEnable)
{
	static BYTE parms[] = VTS_BOOL ;
	InvokeHelper(0x55, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bEnable);
}
void EnableRotateMode(BOOL bEnable)
{
	static BYTE parms[] = VTS_BOOL ;
	InvokeHelper(0x56, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bEnable);
}
long ReCenter(short nType)
{
	long result;
	static BYTE parms[] = VTS_I2 ;
	InvokeHelper(0x57, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nType);
	return result;
}
long ShowPMIFilteringDlg()
{
	long result;
	InvokeHelper(0x58, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long ShowLightingDlg()
{
	long result;
	InvokeHelper(0x59, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long ShowDefineSectionDlg()
{
	long result;
	InvokeHelper(0x5a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long ShowUserCoordSystemsDlg()
{
	long result;
	InvokeHelper(0x5b, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
void EnableManipulators(BOOL bEnable)
{
	static BYTE parms[] = VTS_BOOL ;
	InvokeHelper(0x5c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bEnable);
}
long ShowPartAlignmentDlg()
{
	long result;
	InvokeHelper(0x5d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long ShowModelTransformDlg()
{
	long result;
	InvokeHelper(0x5e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
void ResetTransformation()
{
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
long ShowInterferenceCheckDlg()
{
	long result;
	InvokeHelper(0x60, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long SelectOverlay()
{
	long result;
	InvokeHelper(0x61, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
BOOL RemoveOverlay(long ID)
{
	BOOL result;
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x62, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID);
	return result;
}
void RemoveOverlays()
{
	InvokeHelper(0x63, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
long LoadOverlay(LPCTSTR sFileName, double x, double y, double scale, BOOL positionByUser, BOOL sizeByUser)
{
	long result;
	static BYTE parms[] = VTS_BSTR VTS_R8 VTS_R8 VTS_R8 VTS_BOOL VTS_BOOL ;
	InvokeHelper(0x64, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sFileName, x, y, scale, positionByUser, sizeByUser);
	return result;
}
CString GetOverlayParameters(long ID, double * x, double * y, double * scale)
{
	CString result;
	static BYTE parms[] = VTS_I4 VTS_PR8 VTS_PR8 VTS_PR8 ;
	InvokeHelper(0x65, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, ID, x, y, scale);
	return result;
}
CString GetOverlayFileName(long ID)
{
	CString result;
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x66, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, ID);
	return result;
}
double GetOverlayPosX(long ID)
{
	double result;
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x67, DISPATCH_METHOD, VT_R8, (void*)&result, parms, ID);
	return result;
}
double GetOverlayPosY(long ID)
{
	double result;
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x68, DISPATCH_METHOD, VT_R8, (void*)&result, parms, ID);
	return result;
}
double GetOverlayScale(long ID)
{
	double result;
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x69, DISPATCH_METHOD, VT_R8, (void*)&result, parms, ID);
	return result;
}
long AddOverlay(LPCTSTR sFileName)
{
	long result;
	static BYTE parms[] = VTS_BSTR ;
	InvokeHelper(0x6a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sFileName);
	return result;
}
BOOL SetOverlayParameters(long ID, double x, double y, double scale, BOOL positionByUser, BOOL sizeByUser)
{
	BOOL result;
	static BYTE parms[] = VTS_I4 VTS_R8 VTS_R8 VTS_R8 VTS_BOOL VTS_BOOL ;
	InvokeHelper(0x6b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID, x, y, scale, positionByUser, sizeByUser);
	return result;
}
BOOL Export3DBom(LPCTSTR sFileName)
{
	BOOL result;
	static BYTE parms[] = VTS_BSTR ;
	InvokeHelper(0x6c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sFileName);
	return result;
}
BOOL ExportEdaBom(LPCTSTR sFileName, LPCTSTR sFormat, BOOL bCurPage, LPCTSTR sAttributes)
{
	BOOL result;
	static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL VTS_BSTR ;
	InvokeHelper(0x6d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sFileName, sFormat, bCurPage, sAttributes);
	return result;
}
BOOL DumpModelTree(LPCTSTR sFileName)
{
	BOOL result;
	static BYTE parms[] = VTS_BSTR ;
	InvokeHelper(0x6e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sFileName);
	return result;
}
long ShowConvertDialog()
{
	long result;
	InvokeHelper(0x6f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
void ConvertIt()
{
	InvokeHelper(0x70, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
void SetConvertOptions(LPCTSTR sEntry, LPCTSTR sValue1, LPCTSTR sValue2)
{
	static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
	InvokeHelper(0x71, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sEntry, sValue1, sValue2);
}
long ShowExplodeDialog()
{
	long result;
	InvokeHelper(0x72, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
long Show3DSearchDialog()
{
	long result;
	InvokeHelper(0x73, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
void AboutBox()
{
	InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

// Properties
//

CString GetSRC()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}
void SetSRC(CString propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}
BOOL GetShowScrollBars()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}
void SetShowScrollBars(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}
BOOL GetShowMainToolBar()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}
void SetShowMainToolBar(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}
BOOL GetShowAuxiToolBar()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}
void SetShowAuxiToolBar(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}
BOOL GetShowStatusBar()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}
void SetShowStatusBar(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}
BOOL GetEnablePopupMenu()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}
void SetEnablePopupMenu(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}
BOOL GetMrkMainToolbar()
{
	BOOL result;
	GetProperty(0x7, VT_BOOL, (void*)&result);
	return result;
}
void SetMrkMainToolbar(BOOL propVal)
{
	SetProperty(0x7, VT_BOOL, propVal);
}
BOOL GetMrkPensToolbar()
{
	BOOL result;
	GetProperty(0x8, VT_BOOL, (void*)&result);
	return result;
}
void SetMrkPensToolbar(BOOL propVal)
{
	SetProperty(0x8, VT_BOOL, propVal);
}
BOOL GetMrkEntitiesToolbar()
{
	BOOL result;
	GetProperty(0x9, VT_BOOL, (void*)&result);
	return result;
}
void SetMrkEntitiesToolbar(BOOL propVal)
{
	SetProperty(0x9, VT_BOOL, propVal);
}
BOOL GetMrkColorsToolbar()
{
	BOOL result;
	GetProperty(0xa, VT_BOOL, (void*)&result);
	return result;
}
void SetMrkColorsToolbar(BOOL propVal)
{
	SetProperty(0xa, VT_BOOL, propVal);
}
CString GetMrkFileLocation()
{
	CString result;
	GetProperty(0x14, VT_BSTR, (void*)&result);
	return result;
}
void SetMrkFileLocation(CString propVal)
{
	SetProperty(0x14, VT_BSTR, propVal);
}
unsigned long GetBgColor()
{
	unsigned long result;
	GetProperty(0xb, VT_UI4, (void*)&result);
	return result;
}
void SetBgColor(unsigned long propVal)
{
	SetProperty(0xb, VT_UI4, propVal);
}
short GetRotate()
{
	short result;
	GetProperty(0xc, VT_I2, (void*)&result);
	return result;
}
void SetRotate(short propVal)
{
	SetProperty(0xc, VT_I2, propVal);
}
short GetFlip()
{
	short result;
	GetProperty(0xd, VT_I2, (void*)&result);
	return result;
}
void SetFlip(short propVal)
{
	SetProperty(0xd, VT_I2, propVal);
}
short GetPage()
{
	short result;
	GetProperty(0xe, VT_I2, (void*)&result);
	return result;
}
void SetPage(short propVal)
{
	SetProperty(0xe, VT_I2, propVal);
}
short GetExtents()
{
	short result;
	GetProperty(0xf, VT_I2, (void*)&result);
	return result;
}
void SetExtents(short propVal)
{
	SetProperty(0xf, VT_I2, propVal);
}
double GetExtMinX()
{
	double result;
	GetProperty(0x10, VT_R8, (void*)&result);
	return result;
}
void SetExtMinX(double propVal)
{
	SetProperty(0x10, VT_R8, propVal);
}
double GetExtMinY()
{
	double result;
	GetProperty(0x11, VT_R8, (void*)&result);
	return result;
}
void SetExtMinY(double propVal)
{
	SetProperty(0x11, VT_R8, propVal);
}
double GetExtMaxX()
{
	double result;
	GetProperty(0x12, VT_R8, (void*)&result);
	return result;
}
void SetExtMaxX(double propVal)
{
	SetProperty(0x12, VT_R8, propVal);
}
double GetExtMaxY()
{
	double result;
	GetProperty(0x13, VT_R8, (void*)&result);
	return result;
}
void SetExtMaxY(double propVal)
{
	SetProperty(0x13, VT_R8, propVal);
}


};
