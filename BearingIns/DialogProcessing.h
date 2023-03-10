#pragma once

// CDialogProcessing 대화 상자입니다.

class CDialogProcessing : public CDialog
{
	DECLARE_DYNAMIC(CDialogProcessing)

public:
	CDialogProcessing(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogProcessing();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PROCESSING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progress;
	int m_iStep;
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
