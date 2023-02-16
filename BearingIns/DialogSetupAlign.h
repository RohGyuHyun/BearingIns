#pragma once


// CDialogSetupAlign 대화 상자입니다.

class CDialogSetupAlign : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetupAlign)

public:
	CDialogSetupAlign(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSetupAlign();

	void ShowDlg(int nShow);
	void Update(BOOL isUpdate);
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_ALIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
