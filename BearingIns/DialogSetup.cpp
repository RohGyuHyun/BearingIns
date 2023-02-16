// DialogSetup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BearingIns.h"
#include "DialogSetupModel.h"
#include "DialogSetupAlign.h"
#include "DialogSetupTeaching.h"
#include "DialogSetup.h"
#include "BearingInsDlg.h"


// CDialogSetup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSetup, CDialog)

CDialogSetup::CDialogSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetup::IDD, pParent)
{
	for(int i = 0; i < MAX_DOUBLE_PARA; i++)
	{
		m_dPara[i] = 0.0;
	}
	for(int i = 0; i < MAX_INT_PARA; i++)
	{
		m_nPara[i] = 0;
		m_InsCenterPoint[i].SetPoint(0, 0);
	}
}

CDialogSetup::~CDialogSetup()
{
	delete m_dlgSetupModel;
	delete m_dlgSetupAlign;
	delete m_dlgSetupLight;
	delete m_dlgSetupTeaching;
	delete m_dlgSetupSystem;
}

void CDialogSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SETUP_MENU, m_ctrSetupMenu);
}


BEGIN_MESSAGE_MAP(CDialogSetup, CDialog)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETUP_MENU, &CDialogSetup::OnTcnSelchangeTabSetupMenu)
END_MESSAGE_MAP()


// CDialogSetup 메시지 처리기입니다.

BOOL CDialogSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	TC_ITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_TEXT;
	TabCtrlItem.pszText = "Model";
	m_ctrSetupMenu.InsertItem(0, &TabCtrlItem);
	TabCtrlItem.pszText = "Light";
	m_ctrSetupMenu.InsertItem(1, &TabCtrlItem);
	TabCtrlItem.pszText = "Align";
	m_ctrSetupMenu.InsertItem(2, &TabCtrlItem);
	TabCtrlItem.pszText = "Tiching";
	m_ctrSetupMenu.InsertItem(3, &TabCtrlItem);
	TabCtrlItem.pszText = "System";
	m_ctrSetupMenu.InsertItem(4, &TabCtrlItem);

	//m_ctrSetupMenu;
	m_dlgSetupModel = new CDialogSetupModel;
	m_dlgSetupModel->Create(IDD_DIALOG_SETUP_MODEL, this);
	m_dlgSetupModel->MoveWindow(10,50,535,530);

	m_dlgSetupAlign = new CDialogSetupAlign;
	m_dlgSetupAlign->Create(IDD_DIALOG_SETUP_ALIGN, this);
	m_dlgSetupAlign->MoveWindow(10,50,535,530);

	m_dlgSetupTeaching = new CDialogSetupTeaching;
	m_dlgSetupTeaching->Create(IDD_DIALOG_SETUP_TEACHING, this);
	m_dlgSetupTeaching->MoveWindow(10,50,535,530);

	m_dlgSetupLight = new CDialogSetupLight;
	m_dlgSetupLight->Create(IDD_DIALOG_SETUP_LIGHT, this);
	m_dlgSetupLight->MoveWindow(10,50,535,530);

	m_dlgSetupSystem = new CDialogSetupSystem;
	m_dlgSetupSystem->Create(IDD_DIALOG_SETUP_SYSTEM, this);
	m_dlgSetupSystem->MoveWindow(10,50,535,530);

	return TRUE;
}

void CDialogSetup::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	int select;
	double l, t, r, b;

	if(bShow)
	{
		CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
		CString strModelName;
		strModelName.Format("Model Name : %s", pdlg->m_ModelInfo.strModelName);
		m_dlgSetupModel->GetDlgItem(IDC_STATIC_GROUP_MODEL)->SetWindowTextA(strModelName);

		pdlg->m_VisMod->LoadSetupPara(pdlg->m_ModelInfo.strModelPath);
		pdlg->m_VisMod->LoadSystemPara();
		typeSetupPara setupPara;
		setupPara = pdlg->m_VisMod->GetSetupPara();
		Update(TRUE);
		MdispControl(*pdlg->m_VisMod->GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
		
		for(int i = 0; i < INS_RGN_MAX_NUM; i++)
		{
			pdlg->m_DlgRect->m_Tracker[i].GetZoom(&l, &t, &r, &b);
			pdlg->m_DlgRect->m_Tracker[i].SetRect((setupPara.InspPara.InsRegion[i].left * pdlg->m_dZoomRatio) - (l * pdlg->m_dZoomRatio),
												(setupPara.InspPara.InsRegion[i].top * pdlg->m_dZoomRatio) - (t * pdlg->m_dZoomRatio),
												(setupPara.InspPara.InsRegion[i].right * pdlg->m_dZoomRatio) - (l * pdlg->m_dZoomRatio),
												(setupPara.InspPara.InsRegion[i].bottom * pdlg->m_dZoomRatio) - (t * pdlg->m_dZoomRatio));
		}

		select = m_ctrSetupMenu.GetCurSel();

		if((NULL != m_dlgSetupModel) && (NULL != m_dlgSetupLight) && (NULL != m_dlgSetupAlign) && (NULL != m_dlgSetupTeaching) && (NULL != m_dlgSetupSystem))
		{
			switch(select)
			{
				case SETUP_MODEL:
					m_dlgSetupTeaching->ShowWindow(SW_HIDE);
					m_dlgSetupTeaching->ShowDlg(SW_HIDE);
					m_dlgSetupAlign->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowDlg(SW_HIDE);
					m_dlgSetupLight->ShowWindow(SW_HIDE);
					m_dlgSetupLight->ShowDlg(SW_HIDE);
					m_dlgSetupSystem->ShowWindow(SW_HIDE);
					m_dlgSetupModel->ShowWindow(SW_SHOW);
					break;
				case SETUP_LIGHT:
					m_dlgSetupModel->ShowWindow(SW_HIDE);
					m_dlgSetupTeaching->ShowWindow(SW_HIDE);
					m_dlgSetupTeaching->ShowDlg(SW_HIDE);
					m_dlgSetupAlign->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowDlg(SW_HIDE);
					m_dlgSetupSystem->ShowWindow(SW_HIDE);
					m_dlgSetupLight->ShowWindow(SW_SHOW);
					m_dlgSetupLight->ShowDlg(SW_SHOW);
					break;
				case SETUP_ALIGN:
					m_dlgSetupTeaching->ShowWindow(SW_HIDE);
					m_dlgSetupTeaching->ShowDlg(SW_HIDE);
					m_dlgSetupModel->ShowWindow(SW_HIDE);
					m_dlgSetupLight->ShowWindow(SW_HIDE);
					m_dlgSetupLight->ShowDlg(SW_HIDE);
					m_dlgSetupSystem->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowWindow(SW_SHOW);
					m_dlgSetupAlign->ShowDlg(SW_SHOW);
					break;	
				case SETUP_TEACHING:
					m_dlgSetupModel->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowDlg(SW_HIDE);
					m_dlgSetupLight->ShowWindow(SW_HIDE);
					m_dlgSetupLight->ShowDlg(SW_HIDE);
					m_dlgSetupSystem->ShowWindow(SW_HIDE);
					m_dlgSetupTeaching->ShowWindow(SW_SHOW);
					m_dlgSetupTeaching->ShowDlg(SW_SHOW);
					break;
				case SETUP_SYSTEM:
					m_dlgSetupModel->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowDlg(SW_HIDE);
					m_dlgSetupLight->ShowWindow(SW_HIDE);
					m_dlgSetupLight->ShowDlg(SW_HIDE);
					m_dlgSetupTeaching->ShowWindow(SW_HIDE);
					m_dlgSetupTeaching->ShowDlg(SW_HIDE);
					m_dlgSetupSystem->ShowWindow(SW_SHOW);
					break;
			}
		}
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDialogSetup::Update(BOOL isUpdate)
{
	m_dlgSetupTeaching->m_nCnt = (int)m_dlgSetupModel->m_edit_dCnt;
	m_dlgSetupModel->Update(isUpdate);
	m_dlgSetupTeaching->Update(isUpdate);
	m_dlgSetupAlign->Update(isUpdate);
	m_dlgSetupLight->Update(isUpdate);
	m_dlgSetupSystem->Update(isUpdate);
}

BEGIN_EVENTSINK_MAP(CDialogSetup, CDialog)
ON_EVENT(CDialogSetup, IDC_BTNENHCTRL_CANCEL, DISPID_CLICK, CDialogSetup::ClickBtnenhctrlCancel, VTS_NONE)
ON_EVENT(CDialogSetup, IDC_BTNENHCTRL_OK, DISPID_CLICK, CDialogSetup::ClickBtnenhctrlOk, VTS_NONE)
ON_EVENT(CDialogSetup, IDC_BTNENHCTRL_APPLY, DISPID_CLICK, CDialogSetup::ClickBtnenhctrlApply, VTS_NONE)
ON_EVENT(CDialogSetup, IDC_BTNENHCTRL_TEST, DISPID_CLICK, CDialogSetup::ClickBtnenhctrlTest, VTS_NONE)
END_EVENTSINK_MAP()

BOOL CDialogSetup::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialogSetup::ClickBtnenhctrlCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();

	for(int i = 0; i < setupPara.ModelPara.nModelCnt + INSPECTION_NIDL_REGION; i++)
	{
		//pdlg->m_DlgRect->SetActiveTracker(i, FALSE);
		pdlg->m_DlgRect->m_Tracker[i].SetActive(FALSE);
	}
	pdlg->m_DlgRect->Invalidate();
	MdispControl(*pdlg->m_VisMod->GetMilDisplay(0), M_OVERLAY_CLEAR, M_DEFAULT);
	
	EndDialog(0);
}


void CDialogSetup::OnTcnSelchangeTabSetupMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	CString strModelName;
	int select;
	double l, t, r, b;
	strModelName.Format("Model Name : %s", pdlg->m_ModelInfo.strModelName);
	m_dlgSetupModel->GetDlgItem(IDC_STATIC_GROUP_MODEL)->SetWindowTextA(strModelName);

	pdlg->m_VisMod->LoadSetupPara(pdlg->m_ModelInfo.strModelPath);
	pdlg->m_VisMod->LoadSystemPara();
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	Update(TRUE);

	select = m_ctrSetupMenu.GetCurSel();
	MdispControl(*pdlg->m_VisMod->GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);

	for(int i = 0; i < INS_RGN_MAX_NUM; i++)
	{
		pdlg->m_DlgRect->m_Tracker[i].GetZoom(&l, &t, &r, &b);
		pdlg->m_DlgRect->m_Tracker[i].SetRect((setupPara.InspPara.InsRegion[i].left * pdlg->m_dZoomRatio) - (l * pdlg->m_dZoomRatio),
												(setupPara.InspPara.InsRegion[i].top * pdlg->m_dZoomRatio) - (t * pdlg->m_dZoomRatio),
												(setupPara.InspPara.InsRegion[i].right * pdlg->m_dZoomRatio) - (l * pdlg->m_dZoomRatio),
												(setupPara.InspPara.InsRegion[i].bottom * pdlg->m_dZoomRatio) - (t * pdlg->m_dZoomRatio));
	}

	if((NULL != m_dlgSetupModel) && (NULL != m_dlgSetupLight) && (NULL != m_dlgSetupAlign) && (NULL != m_dlgSetupTeaching) && (NULL != m_dlgSetupSystem))
		{
			switch(select)
			{
				case SETUP_MODEL:
					m_dlgSetupTeaching->ShowWindow(SW_HIDE);
					m_dlgSetupTeaching->ShowDlg(SW_HIDE);
					m_dlgSetupAlign->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowDlg(SW_HIDE);
					m_dlgSetupLight->ShowWindow(SW_HIDE);
					m_dlgSetupLight->ShowDlg(SW_HIDE);
					m_dlgSetupSystem->ShowWindow(SW_HIDE);
					m_dlgSetupModel->ShowWindow(SW_SHOW);
					break;
				case SETUP_LIGHT:
					m_dlgSetupModel->ShowWindow(SW_HIDE);
					m_dlgSetupTeaching->ShowWindow(SW_HIDE);
					m_dlgSetupTeaching->ShowDlg(SW_HIDE);
					m_dlgSetupAlign->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowDlg(SW_HIDE);
					m_dlgSetupSystem->ShowWindow(SW_HIDE);
					m_dlgSetupLight->ShowWindow(SW_SHOW);
					m_dlgSetupLight->ShowDlg(SW_SHOW);
					break;
				case SETUP_ALIGN:
					m_dlgSetupTeaching->ShowWindow(SW_HIDE);
					m_dlgSetupTeaching->ShowDlg(SW_HIDE);
					m_dlgSetupModel->ShowWindow(SW_HIDE);
					m_dlgSetupLight->ShowWindow(SW_HIDE);
					m_dlgSetupLight->ShowDlg(SW_HIDE);
					m_dlgSetupSystem->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowWindow(SW_SHOW);
					m_dlgSetupAlign->ShowDlg(SW_SHOW);
					break;	
				case SETUP_TEACHING:
					m_dlgSetupModel->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowDlg(SW_HIDE);
					m_dlgSetupLight->ShowWindow(SW_HIDE);
					m_dlgSetupLight->ShowDlg(SW_HIDE);
					m_dlgSetupSystem->ShowWindow(SW_HIDE);
					m_dlgSetupTeaching->ShowWindow(SW_SHOW);
					m_dlgSetupTeaching->ShowDlg(SW_SHOW);
					break;
				case SETUP_SYSTEM:
					m_dlgSetupModel->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowWindow(SW_HIDE);
					m_dlgSetupAlign->ShowDlg(SW_HIDE);
					m_dlgSetupLight->ShowWindow(SW_HIDE);
					m_dlgSetupLight->ShowDlg(SW_HIDE);
					m_dlgSetupTeaching->ShowWindow(SW_HIDE);
					m_dlgSetupTeaching->ShowDlg(SW_HIDE);
					m_dlgSetupSystem->ShowWindow(SW_SHOW);
					break;
			}
		}
	*pResult = 0;
}

void CDialogSetup::ClickBtnenhctrlApply()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	Update(FALSE);
	typeSetupPara setupPara;
	CRect rect;
	double l, t, r, b;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	for(int i = 0; i < setupPara.ModelPara.nModelCnt + INSPECTION_NIDL_REGION; i++)
	{
		rect = pdlg->m_DlgRect->m_Tracker[i].GetRect();
		pdlg->m_DlgRect->m_Tracker[i].GetZoom(&l, &t, &r, &b);
		rect.left = (long)(((rect.left / pdlg->m_dZoomRatio) + 0.5) + l);
		rect.top = (long)(((rect.top / pdlg->m_dZoomRatio) + 0.5) + t);
		rect.right = (long)(((rect.right / pdlg->m_dZoomRatio) + 0.5) + l);
		rect.bottom = (long)(((rect.bottom / pdlg->m_dZoomRatio) + 0.5) + t);
		setupPara.InspPara.InsRegion[i] = rect;
	}
	pdlg->m_VisMod->SetSetupPara(setupPara);

	pdlg->m_dlgProcessing.ShowWindow(SW_SHOW);

	MakeDirectories(pdlg->m_ModelInfo.strModelPath);

	pdlg->m_VisMod->SaveSetupPara(pdlg->m_ModelInfo.strModelPath);
	pdlg->m_VisMod->SaveSystemPara();

	pdlg->m_DlgRect->Invalidate();

	pdlg->ModelInfoView();
}


void CDialogSetup::ClickBtnenhctrlOk()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	Update(FALSE);
	typeSetupPara setupPara;
	CRect rect;
	double l, t, r, b;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	for(int i = 0; i < setupPara.ModelPara.nModelCnt + INSPECTION_NIDL_REGION; i++)
	{
		rect = pdlg->m_DlgRect->m_Tracker[i].GetRect();
		pdlg->m_DlgRect->m_Tracker[i].GetZoom(&l, &t, &r, &b);
		rect.left = (long)(((rect.left / pdlg->m_dZoomRatio) + 0.5) + l);
		rect.top = (long)(((rect.top / pdlg->m_dZoomRatio) + 0.5) + t);
		rect.right = (long)(((rect.right / pdlg->m_dZoomRatio) + 0.5) + l);
		rect.bottom = (long)(((rect.bottom / pdlg->m_dZoomRatio) + 0.5) + t);
		setupPara.InspPara.InsRegion[i] = rect;
	}
	pdlg->m_VisMod->SetSetupPara(setupPara);

	pdlg->m_dlgProcessing.ShowWindow(SW_SHOW);

	MakeDirectories(pdlg->m_ModelInfo.strModelPath);

	pdlg->m_VisMod->SaveSetupPara(pdlg->m_ModelInfo.strModelPath);
	pdlg->m_VisMod->SaveSystemPara();

	for(int i = 0; i < setupPara.ModelPara.nModelCnt + INSPECTION_NIDL_REGION; i++)
	{
		pdlg->m_DlgRect->m_Tracker[i].SetActive(FALSE);
	}

	pdlg->m_DlgRect->Invalidate();

	pdlg->ModelInfoView();

	EndDialog(0);
}
void CDialogSetup::ClickBtnenhctrlTest()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	CString strLog;
	double p_time, s_time, e_time;
	CTime cTime;
	cTime = CTime::GetCurrentTime();
	pdlg->m_strInspDateTime.Format("%04dY%02dM%02dD_%02dH%02dM%02dS", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());
	s_time = clock();
	int rslt = -1;
	UpdateData(TRUE);
	pdlg->InitInsRsltPara();
	rslt = pdlg->Inspection();
	//pdlg->InspectionMeas();
	if(rslt == 0)
	{
#ifndef DCF
		//rslt = pdlg->InspectionEdge();
		//rslt = pdlg->InspectionMod();
#endif
	}

	pdlg->RsltSave(rslt);
	pdlg->RsltView(rslt);

	e_time = clock();
	p_time = (double)((e_time - s_time));
	strLog.Format("Test Inspection Time : %.5f ms", p_time);
	pdlg->WriteStatusList(strLog);
	switch(rslt)
	{
		//OK
		case 0:
			pdlg->m_InsCntPara.nOk1Cnt++;
			pdlg->m_InsCntPara.nOk2Cnt++;
			break;
		//Needle 검사 불량
		case 1:
		case 2:
		case 3:
		case 4:
			pdlg->m_InsCntPara.nNg1Cnt++;
			break;
		//사출 검사 불량
		case 5:
		case 6:
		case 7:
		case 8:
			pdlg->m_InsCntPara.nNg2Cnt++;
			break;
	}
	pdlg->m_InsCntPara.nTotalCnt++;
	pdlg->SetInsCnt();
}
