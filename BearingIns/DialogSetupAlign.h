#pragma once


// CDialogSetupAlign ��ȭ �����Դϴ�.

class CDialogSetupAlign : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetupAlign)

public:
	CDialogSetupAlign(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSetupAlign();

	void ShowDlg(int nShow);
	void Update(BOOL isUpdate);
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SETUP_ALIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickBtnenhctrlTeach();
	void ClickBtnenhctrlTest();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	int m_edit_nScore;
	BOOL m_edit_nAlignType0;
	double m_edif_para0;
	afx_msg void OnBnClickedRadioAlignType0();
	afx_msg void OnBnClickedRadioAlignType1();
};
