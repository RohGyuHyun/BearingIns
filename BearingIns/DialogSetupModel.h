#pragma once


// CDialogSetupModel ��ȭ �����Դϴ�.

class CDialogSetupModel : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetupModel)

public:
	CDialogSetupModel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSetupModel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SETUP_MODEL };
	BOOL m_bCalculGap;

	void Update(BOOL isUpdate);
	CBtnEnh m_btnCalCul;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
