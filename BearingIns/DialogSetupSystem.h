#pragma once


// CDialogSetupSystem 대화 상자입니다.

class CDialogSetupSystem : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetupSystem)

public:
	CDialogSetupSystem(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSetupSystem();

	void Update(BOOL isUpdate);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP_SYSTEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_edit_nHour;
	int m_edit_nMinute;
	int m_edit_nDay;
};
