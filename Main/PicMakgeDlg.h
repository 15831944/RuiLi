#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ListDlg.h"


// CPicMakgeDlg �Ի���

class CPicMakgeDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicMakgeDlg)

public:
	CPicMakgeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPicMakgeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	int m_flag;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton2();
	// �ж��Ƿ�Ϊ��һ��
	int m_nSetp;
	// ��ʼ��X
	int m_nStartX;
	// ��ʼ��Y
	int m_nStartY;
	// ��ֹ��X
	int m_nEndX;
	// ��ֹ��Y
	int m_nEndY;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnPaint();
	// �ļ������·��
	CString csPath;
	// ������ļ���
	CString fileName;
	CButton m_CreatButton;
	virtual BOOL OnInitDialog();
	// ͼƬ���
	int m_Width;
	// ͼƬ�߶�
	int m_Height;
	
	CButton m_SelectButton;
	// ���ô洢·��
	void SetPath(CString filePath);
	CListCtrl m_List;
	afx_msg void OnPicEdit();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	// ѡȡ������ʵʱ��ʾ���
	int m_pic_width;
	// ѡȡ������ʵʱ��ʾ�߶�
	int m_pic_height;
	int FreshList(void);
	afx_msg void OnDelPic();
	afx_msg void OnPicView();
	afx_msg void OnClose();

	
	CSliderCtrl m_slidercrtrl;
	afx_msg void OnBiaozhu();
	//��������ȷ��ͼƬ�ĳߴ���
	CComboBox m_cobGuiGe;
	//�Զ����ѧ
	void TypeDIY(CPoint * point, UINT nFlags);   
	//�ƶ���С
	void TypeBiaoZhun(CPoint * point, UINT nFlags,int iWidth, int iHeight);
	bool MakePic(CPoint * point, int iWidth, int iHeight);
	afx_msg void OnViewPicOfDrawing();
	//�б�Ի���
	CListDlg * list_dlg;
	//��Ϣ����
	afx_msg LRESULT OnListDlgOk(WPARAM wParam,LPARAM lParam);
};
