// DialogSetupSystem.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TypeDef.h"
#include "BearingIns.h"
#include "BearingInsDlg.h"
#include "DialogSetupSystem.h"


// CDialogSetupSystem ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogSetupSystem, CDialog)

CDialogSetupSystem::CDialogSetupSystem(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetupSystem::IDD, pParent)
	, m_edit_nHour(0)
	, m_edit_nMinute(0)
	, m_edit_nDay(0)
{

}

CDialogSetupSystem::~CDialogSetupSystem()
{
}

void CDialogSetupSystem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SYSTEM_HOUR, m_edit_nHour);
	DDX_Text(pDX, IDC_EDIT_SYSTEM_MINUTE, m_edit_nMinute);
	DDX_Text(pDX, IDC_EDIT_SYSTEM_DAY, m_edit_nDay);
	DDV_MinMaxInt(pDX, m_edit_nHour, 0, 23);
	DDV_MinMaxInt(pDX, m_edit_nMinute, 0, 59);
}


BEGIN_MESSAGE_MAP(CDialogSetupSystem, CDialog)
END_MESSAGE_MAP()


// CDialogSetupSystem �޽��� ó�����Դϴ�.

void CDialogSetupSystem::Update(BOOL isUpdate)
{
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	typeSystemPara systemPara;
	systemPara = pdlg->m_VisMod->GetSystemPara();
	
	if(isUpdate)
	{
		m_edit_nHour = systemPara.nHour;
		m_edit_nMinute = systemPara.nMinute;
		m_edit_nDay = systemPara.nDay;
		UpdateData(FALSE);
	}
	else
	{
		UpdateData(TRUE);
		systemPara.nHour = m_edit_nHour;
		systemPara.nMinute = m_edit_nMinute;
		systemPara.nDay = m_edit_nDay;
		pdlg->m_VisMod->SetSystemPara(systemPara);
	}
}