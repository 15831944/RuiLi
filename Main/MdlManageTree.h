#pragma once


// CMdlManageTree

class CMdlManageTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CMdlManageTree)

public:
	CMdlManageTree();
	virtual ~CMdlManageTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void Show(void);
	void SetTypeBeShow(int type);
	int nFlagType;
	CPartMdlRecordSet *m_pPartSet;
	void PartTreeList(HTREEITEM fathernode);
	void AddSubPartItem(HTREEITEM item);
};


