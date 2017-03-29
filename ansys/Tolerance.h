#pragma once
#include "comdef.h"
#include "_Application.h"
#include "_Workbook.h"
#include "_Worksheet.h"
#include "Shape.h"
#include "Shapes.h"
#include "Workbooks.h"
#include "Worksheets.h"
#include "Range.h"

typedef struct ExcelPage
{
   CString Name;
   int PageIndex;
   CStringArray strArray;
   int rows;
   int cols;
}ExcelPage,* pExcelPage;
// CTolerance ÃüÁîÄ¿±ê

class CTolerance : public CObject
{
public:
	CTolerance();
	virtual ~CTolerance();
public:
	CArray<ExcelPage *,ExcelPage *> m_PageArray;
	int m_PageCount;


public:
	int Load();
	int GetCols(int PageIndex);
	int GetRows(int PageIndex);
	CString GetRowName(int PageIndex,int index);
    CString GetCellString(int PageIndex,int row,int col);
	int RemovePage();
	int KillExcel();




};


