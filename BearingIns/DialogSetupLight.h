#pragma once
#include "afxcmn.h"


// CDialogSetupLight 대화 상자입니다.

class CDialogSetupLight : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetupLight)

public:
	CDialogSetupLight(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSetupLight();

	void Update(BOOL isUpdate);
	void ShowDlg(int nShow);

	CSliderCtrl m_slidLight_0;
	CSliderCtrl m_slidLight_1;
	CSliderCtrl m_slidLight_2;
	CSliderCtrl m_slidLight_3;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_LIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnNMCustomdrawSliderLight0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderLight1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderLight2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderLight3(NMHDR *pNMHDR, LRESULT *pResult);
};
