// DialogSetupLight.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BearingIns.h"
#include "DialogSetupLight.h"
#include "BearingInsDlg.h"

// CDialogSetupLight 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSetupLight, CDialog)

CDialogSetupLight::CDialogSetupLight(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetupLight::IDD, pParent)
{

}

CDialogSetupLight::~CDialogSetupLight()
{
}

void CDialogSetupLight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_LIGHT_0, m_slidLight_0);
	DDX_Control(pDX, IDC_SLIDER_LIGHT_1, m_slidLight_1);
	DDX_Control(pDX, IDC_SLIDER_LIGHT_2, m_slidLight_2);
	DDX_Control(pDX, IDC_SLIDER_LIGHT_3, m_slidLight_3);
}


BEGIN_MESSAGE_MAP(CDialogSetupLight, CDialog)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHT_0, &CDialogSetupLight::OnNMCustomdrawSliderLight0)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHT_1, &CDialogSetupLight::OnNMCustomdrawSliderLight1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHT_2, &CDialogSetupLight::OnNMCustomdrawSliderLight2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHT_3, &CDialogSetupLight::OnNMCustomdrawSliderLight3)
END_MESSAGE_MAP()


// CDialogSetupLight 메시지 처리기입니다.
BOOL CDialogSetupLight::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_slidLight_0.SetRange(MIN_LIGHT,MAX_LIGHT);
	m_slidLight_1.SetRange(MIN_LIGHT,MAX_LIGHT);
	m_slidLight_2.SetRange(MIN_LIGHT,MAX_LIGHT);
	m_slidLight_3.SetRange(MIN_LIGHT,MAX_LIGHT);

	//m_slidLight_2.EnableWindow(FALSE);

	return TRUE;
}
void CDialogSetupLight::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	ShowDlg(bShow);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDialogSetupLight::ShowDlg(int nShow)
{
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;
	typeSetupPara setupPara;
	CString strTeachPath;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	
	if(nShow)
	{
		m_slidLight_0.SetPos(setupPara.iLight[LIGHT_0 - 1]);
		m_slidLight_1.SetPos(setupPara.iLight[LIGHT_1 - 1]);
		m_slidLight_2.SetPos(setupPara.iLight[LIGHT_2 - 1]);
		m_slidLight_3.SetPos(setupPara.iLight[LIGHT_3 - 1]);
		
#ifdef LIGHT
		for(int i = 0; i < LIGHT_3; i++)
		{
			pdlg->SetLineLight(i + 1, setupPara.iLight[i]);
			Sleep(100);
		}
#endif
	}
	else
	{
#ifdef LIGHT
		for(int i = 0; i < LIGHT_3; i++)
		{
			pdlg->SetLineLight(i + 1, 0);
			Sleep(100);
		}
#endif
	}
}


void CDialogSetupLight::Update(BOOL isUpdate)
{
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	typeSetupPara setupPara;
	char tmp[DAT_STR_LENG];
	setupPara = pdlg->m_VisMod->GetSetupPara();
	if(isUpdate)
	{
		
		sprintf_s(tmp, DAT_STR_LENG, "%d", setupPara.iLight[LIGHT_0 - 1]);
		m_slidLight_0.SetPos(setupPara.iLight[LIGHT_0 - 1]);
		GetDlgItem(IDC_EDIT_LIGHT_0)->SetWindowTextA(tmp);

		sprintf_s(tmp, DAT_STR_LENG, "%d", setupPara.iLight[LIGHT_1 - 1]);
		m_slidLight_1.SetPos(setupPara.iLight[LIGHT_1]);
		GetDlgItem(IDC_EDIT_LIGHT_1)->SetWindowTextA(tmp);

		sprintf_s(tmp, DAT_STR_LENG, "%d", setupPara.iLight[LIGHT_2 - 1]);
		m_slidLight_2.SetPos(setupPara.iLight[LIGHT_2 - 1]);
		GetDlgItem(IDC_EDIT_LIGHT_2)->SetWindowTextA(tmp);

		sprintf_s(tmp, DAT_STR_LENG, "%d", setupPara.iLight[LIGHT_3 - 1]);
		m_slidLight_3.SetPos(setupPara.iLight[LIGHT_3 - 1]);
		GetDlgItem(IDC_EDIT_LIGHT_3)->SetWindowTextA(tmp);
		UpdateData(FALSE);
	}
	else
	{
		UpdateData(TRUE);
		
		GetDlgItem(IDC_EDIT_LIGHT_0)->GetWindowTextA(tmp, 3);
		m_slidLight_0.SetPos(atoi(tmp));
		setupPara.iLight[LIGHT_0 - 1] = atoi(tmp);

		GetDlgItem(IDC_EDIT_LIGHT_1)->GetWindowTextA(tmp, 3);
		m_slidLight_1.SetPos(atoi(tmp));
		setupPara.iLight[LIGHT_1 - 1] = atoi(tmp);

		GetDlgItem(IDC_EDIT_LIGHT_2)->GetWindowTextA(tmp, 3);
		m_slidLight_2.SetPos(atoi(tmp));
		setupPara.iLight[LIGHT_2 - 1] = atoi(tmp);

		GetDlgItem(IDC_EDIT_LIGHT_3)->GetWindowTextA(tmp, 3);
		m_slidLight_3.SetPos(atoi(tmp));
		setupPara.iLight[LIGHT_3 - 1] = atoi(tmp);

		pdlg->m_VisMod->SetSetupPara(setupPara);
	}
}

void CDialogSetupLight::OnNMCustomdrawSliderLight0(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char tmp[DAT_STR_LENG];
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;

	sprintf_s(tmp, DAT_STR_LENG, "%d", m_slidLight_0.GetPos());
	GetDlgItem(IDC_EDIT_LIGHT_0)->SetWindowText(tmp);
#ifdef LIGHT
	pdlg->SetLineLight(LIGHT_0, m_slidLight_0.GetPos());
#endif
	*pResult = 0;
}

void CDialogSetupLight::OnNMCustomdrawSliderLight1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char tmp[DAT_STR_LENG];
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;

	sprintf(tmp, "%d", m_slidLight_1.GetPos());
	GetDlgItem(IDC_EDIT_LIGHT_1)->SetWindowText(tmp);
#ifdef LIGHT
	pdlg->SetLineLight(LIGHT_1, m_slidLight_1.GetPos());
#endif
	*pResult = 0;
}

void CDialogSetupLight::OnNMCustomdrawSliderLight2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char tmp[DAT_STR_LENG];
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;

	sprintf(tmp, "%d", m_slidLight_2.GetPos());
	GetDlgItem(IDC_EDIT_LIGHT_2)->SetWindowText(tmp);
#ifdef LIGHT
	pdlg->SetLineLight(LIGHT_2, m_slidLight_2.GetPos());
#endif
	*pResult = 0;
}

void CDialogSetupLight::OnNMCustomdrawSliderLight3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char tmp[DAT_STR_LENG];
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;

	sprintf(tmp, "%d", m_slidLight_3.GetPos());
	GetDlgItem(IDC_EDIT_LIGHT_3)->SetWindowText(tmp);
#ifdef LIGHT
	pdlg->SetLineLight(LIGHT_3, m_slidLight_3.GetPos());
#endif
	*pResult = 0;
}

BOOL CDialogSetupLight::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}