#pragma once


// CDialogSetupSystem ��ȭ �����Դϴ�.

class CDialogSetupSystem : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetupSystem)

public:
	CDialogSetupSystem(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSetupSystem();

	void Update(BOOL isUpdate);

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SETUP_SYSTEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_edit_nHour;
	int m_edit_nMinute;
	int m_edit_nDay;
};
