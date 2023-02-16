#pragma once


// CDialogSetupModel 대화 상자입니다.

class CDialogSetupModel : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetupModel)

public:
	CDialogSetupModel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSetupModel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_MODEL };
	BOOL m_bCalculGap;

	void Update(BOOL isUpdate);
	CBtnEnh m_btnCalCul;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	double m_edit_dWidht;
	double m_edit_dHeight;
	double m_edit_dCnt;
	double m_edit_dPi;
	double m_edit_dGap;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlCalculGap();
	
	double m_edit_dPixelSize;
	double m_edit_dDiameter;
	afx_msg void OnBnClickedButtonMasterSave();
	afx_msg void OnBnClickedButtonMasterLoad();
	int m_edit_nCol;
	BOOL m_chk_Save_0;
	BOOL m_chk_Save_1;
	BOOL m_chk_Save_2;
};
