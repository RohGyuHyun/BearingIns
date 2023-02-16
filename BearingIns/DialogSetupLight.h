#pragma once
#include "afxcmn.h"


// CDialogSetupLight ��ȭ �����Դϴ�.

class CDialogSetupLight : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetupLight)

public:
	CDialogSetupLight(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSetupLight();

	void Update(BOOL isUpdate);
	void ShowDlg(int nShow);

	CSliderCtrl m_slidLight_0;
	CSliderCtrl m_slidLight_1;
	CSliderCtrl m_slidLight_2;
	CSliderCtrl m_slidLight_3;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SETUP_LIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
