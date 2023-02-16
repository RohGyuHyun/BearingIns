#pragma once
#include "afxcmn.h"

#define NO_XLISTCTRL_TOOL_TIPS

#define VIEWCLASS					CDialogSetupTeaching
#define VIEWCLASS_HEADER_FILE		"DialogSetupTeaching.h"

#include "XListCtrl.h"



// CDialogSetupTeaching 대화 상자입니다.

class CDialogSetupTeaching : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetupTeaching)

public:
	CDialogSetupTeaching(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSetupTeaching();

	BOOL m_bGridlines;
	CFont m_font;
	int m_nCnt;

	//CStringArray m_sa1, m_sa2;
	BOOL	m_bRunning;
	static int m_nColWidths[];

	void GetListCtrl(CXListCtrl * pList);
	void FillListCtrl(CXListCtrl * pList, BOOL bInit = FALSE);
	void InitListCtrl(CXListCtrl * pList);
	
	void Update(BOOL isUpdate);
	void ShowDlg(int nShow);
private:

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_TEACHING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_edit_nScore;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnComboSelection(WPARAM, LPARAM);
	afx_msg LRESULT OnCheckbox(WPARAM, LPARAM);
	afx_msg LRESULT OnEditEnd(WPARAM, LPARAM);

	int m_edit_nMoveCnt;
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlTeach();
	void ClickBtnenhctrlTest();
	void ClickBtnenhctrlLeft();
	void ClickBtnenhctrlRight();
	void ClickBtnenhctrlUp();
	void ClickBtnenhctrlDown();
	afx_msg void OnEnKillfocusEditMoveCnt();
	void ClickBtnenhctrlAllTeach();
	CXListCtrl	m_List;
	double m_edit_dPara0;
	double m_edit_dPara1;
	double m_edit_dPara2;
	void ClickBtnenhctrlScaleUp();
	void ClickBtnenhctrlScaleDown();
	double m_edit_dScale;
	double m_edit_dpara_0;
	double m_edit_dpara_1;
	double m_edit_dpara_2;
	double m_edit_dpara_3;
	afx_msg void OnBnClickedButtonParaAll0();
	afx_msg void OnBnClickedButtonParaAll1();
	afx_msg void OnBnClickedButtonParaAll2();
	afx_msg void OnBnClickedButtonParaAll3();
	double m_edit_nScore2;
	BOOL m_bBrokenInspection;
	BOOL m_check_Inspction_0;
	BOOL m_check_Inspction_1;
	BOOL m_check_Inspction_2;
	BOOL m_check_Inspction_3;
};
