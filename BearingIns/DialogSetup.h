#pragma once
#include "DialogSetupModel.h"
#include "DialogSetupAlign.h"
#include "DialogSetupTeaching.h"
#include "DialogSetupLight.h"
#include "DialogSetupSystem.h"
#include "afxcmn.h"

// CDialogSetup 대화 상자입니다.

class CDialogSetup : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetup)

public:
	CDialogSetup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSetup();

	CDialogSetupModel *m_dlgSetupModel;
	CDialogSetupAlign *m_dlgSetupAlign;
	CDialogSetupTeaching *m_dlgSetupTeaching;
	CDialogSetupLight *m_dlgSetupLight;
	CDialogSetupSystem *m_dlgSetupSystem;

	double m_dPara[MAX_INS_PARA];
	int m_nPara[MAX_INS_PARA];

	CPoint m_InsCenterPoint[MAX_INS_PARA];

	void Update(BOOL isUpdate);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CTabCtrl m_ctrSetupMenu;
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlCancel();
	void ClickBtnenhctrlOk();
	afx_msg void OnTcnSelchangeTabSetupMenu(NMHDR *pNMHDR, LRESULT *pResult);
	void ClickBtnenhctrlApply();
	void ClickBtnenhctrlTest();
};
