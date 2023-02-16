// DialogSetupTiching.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BearingIns.h"
#include "DialogSetupTeaching.h"
#include "BearingInsDlg.h"



// CDialogSetupTeaching 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSetupTeaching, CDialog)

CDialogSetupTeaching::CDialogSetupTeaching(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetupTeaching::IDD, pParent)
	, m_edit_nScore(0)
	, m_edit_nMoveCnt(0)
	, m_edit_dPara0(1)
	, m_edit_dPara1(100)
	, m_edit_dPara2(0)
	, m_edit_dScale(1)
	, m_edit_dpara_0(0)
	, m_edit_dpara_1(0)
	, m_edit_dpara_2(0)
	, m_edit_dpara_3(0)
	, m_edit_nScore2(0)
	, m_bBrokenInspection(FALSE)
	, m_check_Inspction_0(FALSE)
	, m_check_Inspction_1(FALSE)
	, m_check_Inspction_2(FALSE)
	, m_check_Inspction_3(FALSE)
{
	m_nCnt = 0;
}

CDialogSetupTeaching::~CDialogSetupTeaching()
{
	
}

void CDialogSetupTeaching::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SCORE, m_edit_nScore);
	DDX_Text(pDX, IDC_EDIT_MOVE_CNT, m_edit_nMoveCnt);
	DDX_Control(pDX, IDC_LIST_INSP, m_List);
	DDX_Text(pDX, IDC_EDIT_INSP_PARA_0, m_edit_dPara0);
	DDX_Text(pDX, IDC_EDIT_INSP_PARA_1, m_edit_dPara1);
	DDX_Text(pDX, IDC_EDIT_INSP_PARA_2, m_edit_dPara2);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_edit_dScale);
	DDX_Text(pDX, IDC_EDIT_PARA_0, m_edit_dpara_0);
	DDX_Text(pDX, IDC_EDIT_PARA_1, m_edit_dpara_1);
	DDX_Text(pDX, IDC_EDIT_PARA_2, m_edit_dpara_2);
	DDX_Text(pDX, IDC_EDIT_PARA_3, m_edit_dpara_3);
	DDX_Text(pDX, IDC_EDIT_SCORE2, m_edit_nScore2);
	DDX_Check(pDX, IDC_CHECK_INSP_BROKEN, m_bBrokenInspection);
	DDX_Check(pDX, IDC_CHECK_INSPECTION_0, m_check_Inspction_0);
	DDX_Check(pDX, IDC_CHECK_INSPECTION_1, m_check_Inspction_1);
	DDX_Check(pDX, IDC_CHECK_INSPECTION_2, m_check_Inspction_2);
	DDX_Check(pDX, IDC_CHECK_INSPECTION_3, m_check_Inspction_3);
}


BEGIN_MESSAGE_MAP(CDialogSetupTeaching, CDialog)
	ON_WM_SHOWWINDOW()
	ON_EN_KILLFOCUS(IDC_EDIT_MOVE_CNT, &CDialogSetupTeaching::OnEnKillfocusEditMoveCnt)
	ON_NOTIFY(NM_CLICK, IDC_LIST_INSP, OnClick)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_INSP, OnDblClick)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_INSP, OnColumnClick)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_INSP, OnItemChanged)
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_COMBO_SELECTION, OnComboSelection)
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_CHECKBOX_CLICKED, OnCheckbox)
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_EDIT_END, OnEditEnd)
	ON_BN_CLICKED(IDC_BUTTON_PARA_ALL_0, &CDialogSetupTeaching::OnBnClickedButtonParaAll0)
	ON_BN_CLICKED(IDC_BUTTON_PARA_ALL_1, &CDialogSetupTeaching::OnBnClickedButtonParaAll1)
	ON_BN_CLICKED(IDC_BUTTON_PARA_ALL_2, &CDialogSetupTeaching::OnBnClickedButtonParaAll2)
	ON_BN_CLICKED(IDC_BUTTON_PARA_ALL_3, &CDialogSetupTeaching::OnBnClickedButtonParaAll3)
END_MESSAGE_MAP()


// CDialogSetupTeaching 메시지 처리기입니다.
void CDialogSetupTeaching::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.	
	ShowDlg(bShow);
}

void CDialogSetupTeaching::ShowDlg(int nShow)
{
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;
	typeSetupPara setupPara;
	CString strTeachPath;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	
	if(nShow)
	{
		//Update(TRUE);
#ifndef DCF
		//for(int i = INSPECTION_NIDL_REGION; i < setupPara.ModelPara.nModelCnt + INSPECTION_NIDL_REGION; i++)
		for(int i = INSPECTION_NIDL_REGION; i < INSPECTION_NIDL_REGION + 1; i++)
#else
		for(int i = INSPECTION_NIDL_REGION; i < INSPECTION_NIDL_REGION + 1; i++)
#endif
		{
			pdlg->m_DlgRect->m_Tracker[i].SetActive(TRUE);
		}
	}
	else
	{
#ifndef DCF
		//for(int i = INSPECTION_NIDL_REGION; i < setupPara.ModelPara.nModelCnt + INSPECTION_NIDL_REGION; i++)
		for(int i = INSPECTION_NIDL_REGION; i < INSPECTION_NIDL_REGION + 1; i++)
#else
		for(int i = INSPECTION_NIDL_REGION; i < INSPECTION_NIDL_REGION + 1; i++)
#endif
		{
			pdlg->m_DlgRect->m_Tracker[i].SetActive(FALSE);
		}
		pdlg->m_DlgRect->Invalidate();
	}
}

BEGIN_EVENTSINK_MAP(CDialogSetupTeaching, CDialog)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_TEACH, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrlTeach, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_TEST, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrlTest, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_LEFT, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrlLeft, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_RIGHT, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrlRight, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_UP, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrlUp, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_DOWN, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrlDown, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_ALL_TEACH, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrlAllTeach, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_SCALE_UP, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrlScaleUp, VTS_NONE)
	ON_EVENT(CDialogSetupTeaching, IDC_BTNENHCTRL_SCALE_DOWN, DISPID_CLICK, CDialogSetupTeaching::ClickBtnenhctrlScaleDown, VTS_NONE)
END_EVENTSINK_MAP()

BOOL CDialogSetupTeaching::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
/*	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	if(pMsg->message == WM_MOUSEMOVE)
	{
		CRect rect;
		m_List.GetWindowRect(&rect);
		if(rect.left <= pMsg->pt.x && pMsg->pt.x >=rect.right)
		{
			return TRUE;
		}
		
	}	*/
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialogSetupTeaching::ClickBtnenhctrlTeach()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
#ifndef DCF
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();

	setupPara.InspPara.dInspEdgeThreshHeigh = m_edit_nScore;
	pdlg->m_InsRsltPara.RsltPoint[0].dOffsetY = m_edit_nScore2;

	pdlg->m_VisMod->SetSetupPara(setupPara);
		
	MdispControl(*pdlg->m_VisMod->GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
	pdlg->Inspection();
	//pdlg->InspectionEdge();
	//pdlg->InspectionMod();
	//pdlg->InspectionBlob();
	//pdlg->InspectionMeas();
#endif
}

void CDialogSetupTeaching::ClickBtnenhctrlTest()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;
	//typeSetupPara setupPara;
	//typeRsltPoint rsltPoint[100];
	//MIL_ID ins_Image;
	//MIL_INT StripeMarker;
	//MIL_ID MilBinImage, MilChildImage;
	//CPoint center;
	//
	//UpdateData(TRUE);
	//double *CogX,                       
	//       *CogY,					    
	//	   *Area,
	//	   *Width;  
	//long area;
	//CString temp;
	//int i;
	//int ins_idx = 0;

	//setupPara = pdlg->m_VisMod->GetSetupPara();
	////center = setupPara.InspPara.InsRegion[4].CenterPoint();

	//MbufAlloc2d(*pdlg->m_VisMod->GetMilSystem(), MAX_CAMERA_WIDTH, MAX_CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &ins_Image);

	//MbufCopy(*pdlg->m_VisMod->GetMilDispBuf(0), ins_Image);

	////CRect rect((int)(center.x - (setupPara.InspPara.InsRegion[4].Width() / 2)) - 10,
	////			(int)(center.y - (setupPara.InspPara.InsRegion[4].Height() / 2)) - 10,
	////			(int)(center.x + (setupPara.InspPara.InsRegion[4].Width() / 2)) + 10,
	////			(int)(center.y + (setupPara.InspPara.InsRegion[4].Height() / 2)) + 10);
	//CRect rect((int)(setupPara.InspPara.InsRegion[4].left - 10),
	//			(int)(setupPara.InspPara.InsRegion[4].top - 10),
	//			(int)(setupPara.InspPara.InsRegion[4].right + 10),
	//			(int)(setupPara.InspPara.InsRegion[4].bottom + 10));

	//MbufAlloc2d(*pdlg->m_VisMod->GetMilSystem(), rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
	//MbufClear(MilBinImage, 0x00);

	//MbufChild2d(ins_Image, rect.left, rect.top, rect.Width(), rect.Height(), &MilChildImage);

	//MimBinarize(MilChildImage, MilBinImage, M_GREATER_OR_EQUAL, setupPara.InspPara.dInspEdgeThreshHeigh, M_NULL);

	//CString fileName;
	//fileName.Format("D:\\DATABASE\\MODEL\\TEST\\NIDL_%02d.bmp", ins_idx);
	//MbufExport(MIL_TEXT(fileName), M_BMP, MilChildImage);

	///* Allocate a stripe marker. */
	//MmeasAllocMarker(*pdlg->m_VisMod->GetMilSystem(), M_STRIPE, M_DEFAULT, &StripeMarker);

	//MmeasSetMarker(StripeMarker, M_POLARITY, M_ANY, M_ANY);
	//MmeasSetMarker(StripeMarker, M_BOX_ANGLE, 180, M_NULL);
	//MmeasSetMarker(StripeMarker, M_BOX_CENTER, rect.Width() / 2, rect.Height() / 2);
	//MmeasSetMarker(StripeMarker, M_BOX_SIZE, rect.Width() - 20, rect.Height() - 20);

	//MmeasSetMarker(StripeMarker, M_SUB_REGIONS_NUMBER, 1, M_NULL);
	//MmeasSetMarker(StripeMarker, M_NUMBER, 1, M_NULL);
	////M_FILTER_TYPE 
	//MmeasSetMarker(StripeMarker, M_FILTER_TYPE, M_PREWITT, M_NULL);
	//MmeasSetMarker(StripeMarker, M_FILTER_SMOOTHNESS, setupPara.InspPara.dInspSmooth, M_NULL);
	//MmeasSetMarker(StripeMarker, M_EDGEVALUE_MIN, setupPara.InspPara.dInspEdgeMin, M_NULL);
	//MmeasSetMarker(StripeMarker, M_EDGEVALUE_VAR_MIN, setupPara.InspPara.dInspEdgeValMin, M_NULL);

	//MmeasFindMarker(M_DEFAULT, MilChildImage, StripeMarker, M_DEFAULT);
	//MgraColor(M_DEFAULT, M_COLOR_BLUE);
	//MmeasDraw(M_DEFAULT, StripeMarker, *pdlg->m_VisMod->GetMilOverlay(MAIN_DISP), M_DRAW_SEARCH_REGION, M_DEFAULT, M_MARKER);

	//// Draw the search region orientation
	//MmeasDraw(M_DEFAULT, StripeMarker, *pdlg->m_VisMod->GetMilOverlay(MAIN_DISP), M_DRAW_SEARCH_DIRECTION, M_DEFAULT, M_MARKER);

	//MIL_INT NumberOccurrencesFound;
	//MmeasGetResult(StripeMarker, M_NUMBER+M_TYPE_MIL_INT, &NumberOccurrencesFound, M_NULL);

	//if (NumberOccurrencesFound >= 1)
	//{
	//	bool DrawGlobalEdge   = true;
	//	bool DrawSubPositions = false;

	//	if(DrawGlobalEdge)
	//	{
	//		// Draw the found marker edges.
	//		MgraColor(M_DEFAULT, M_COLOR_RED);
	//		MmeasDraw(M_DEFAULT, StripeMarker, *pdlg->m_VisMod->GetMilOverlay(MAIN_DISP), M_DRAW_EDGES, M_DEFAULT, M_RESULT);

	//		// Draw a cross at the found position.
	//		MgraColor(M_DEFAULT, M_COLOR_YELLOW);
	//		MmeasDraw(M_DEFAULT, StripeMarker, *pdlg->m_VisMod->GetMilOverlay(MAIN_DISP), M_DRAW_POSITION, M_DEFAULT, M_RESULT);
	//	}

	//	if(DrawSubPositions)
	//	{
	//		// Draw the sub edges position used to fit the marker.
	//		MgraColor(M_DEFAULT, M_COLOR_GREEN);
	//		MmeasDraw(M_DEFAULT, StripeMarker, *pdlg->m_VisMod->GetMilOverlay(MAIN_DISP), M_DRAW_SUB_POSITIONS, M_DEFAULT, M_RESULT);
	//	}
	//}
	//else
	//{
	//	// Print the result.
	//	MgraColor(M_DEFAULT, M_COLOR_RED);
	//	MgraText(M_DEFAULT, *pdlg->m_VisMod->GetMilOverlay(MAIN_DISP), rect.left, rect.top, MIL_TEXT("The marker was not found."));
	//}

	////찾은 lead개수
	//double edgenum = 0;
	//MmeasGetResult(StripeMarker, M_NUMBER , &edgenum,  M_NULL);

	////각 lead의 시작점 끝점
	//double fx[EDGE_POS_MAX_NUM], fy[EDGE_POS_MAX_NUM], sx[EDGE_POS_MAX_NUM], sy[EDGE_POS_MAX_NUM], ang[EDGE_POS_MAX_NUM], ang2[EDGE_POS_MAX_NUM];

	//MmeasGetResult(StripeMarker, M_POSITION + M_EDGE_FIRST, fx, fy);
	//MmeasGetResult(StripeMarker, M_POSITION + M_EDGE_SECOND, sx, sy);
	//MmeasGetResult(StripeMarker, M_ANGLE, ang, ang2);

	//rsltPoint[ins_idx].RsltWidthPoint.dStartX = fx[0];
	//rsltPoint[ins_idx].RsltWidthPoint.dEndX = sx[0];
	//rsltPoint[ins_idx].dWidth = abs(rsltPoint[ins_idx].RsltWidthPoint.dStartX - rsltPoint[ins_idx].RsltWidthPoint.dEndX);

	//
	//MmeasDraw(M_DEFAULT, StripeMarker, *pdlg->m_VisMod->GetMilOverlay(MAIN_DISP), M_DRAW_POSITION, M_DEFAULT, M_RESULT);
	//MmeasFree(StripeMarker);
	//MbufFree(MilChildImage);
	//MbufFree(MilBinImage);
	//

	//temp.Format("Index : %04d, width : %04.4f", ins_idx, rsltPoint[ins_idx].dWidth);
	//pdlg->WriteStatusList(temp);
}
void CDialogSetupTeaching::ClickBtnenhctrlLeft()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	int i;
	CRect beginRect, endRect;
	CRect tempRect;
	double l, t, r, b;
	
	pdlg->m_DlgRect->Invalidate();

	for(i = INSPECTION_NIDL_REGION; i < setupPara.ModelPara.nModelCnt + INSPECTION_NIDL_REGION; i++)
	{
		pdlg->m_DlgRect->m_Tracker[i].SetEnable(TRUE);
		beginRect = pdlg->m_DlgRect->m_Tracker[i].GetRect();
		endRect = beginRect;
		endRect.left = beginRect.left - setupPara.iMoveCnt;
		endRect.right = beginRect.right - setupPara.iMoveCnt;

		tempRect.left = (long)(endRect.left / pdlg->m_dZoomRatio);
		tempRect.top = (long)(endRect.top / pdlg->m_dZoomRatio);
		tempRect.right = (long)(endRect.right / pdlg->m_dZoomRatio);
		tempRect.bottom = (long)(endRect.bottom / pdlg->m_dZoomRatio);

		pdlg->m_DlgRect->m_Tracker[i].SetRect(endRect);
		pdlg->m_DlgRect->m_Tracker[i].GetZoom(&l, &t, &r, &b);
		tempRect.left = (long)(tempRect.left + l);
		tempRect.top = (long)(tempRect.top + t);
		tempRect.right = (long)(tempRect.right + l);
		tempRect.bottom = (long)(tempRect.bottom + t);
		setupPara.InspPara.InsRegion[i] = tempRect;
		pdlg->m_DlgRect->m_Tracker[i].SetEnable(FALSE);
	}
	pdlg->m_VisMod->SetSetupPara(setupPara);
	::PostMessage(pdlg->m_DlgRect->m_hWnd, USER_MSG_IMG_RECT, 0, 8);
}

void CDialogSetupTeaching::ClickBtnenhctrlRight()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	int i;
	CRect beginRect, endRect;
	CRect tempRect;
	double l, t, r, b;

	pdlg->m_DlgRect->Invalidate();

	for(i = INSPECTION_NIDL_REGION; i < setupPara.ModelPara.nModelCnt + INSPECTION_NIDL_REGION; i++)
	{
		pdlg->m_DlgRect->m_Tracker[i].SetEnable(TRUE);
		beginRect = pdlg->m_DlgRect->m_Tracker[i].GetRect();
		endRect = beginRect;
		endRect.left = beginRect.left + setupPara.iMoveCnt;
		endRect.right = beginRect.right + setupPara.iMoveCnt;

		tempRect.left = (long)(endRect.left / pdlg->m_dZoomRatio);
		tempRect.top = (long)(endRect.top / pdlg->m_dZoomRatio);
		tempRect.right = (long)(endRect.right / pdlg->m_dZoomRatio);
		tempRect.bottom = (long)(endRect.bottom / pdlg->m_dZoomRatio);

		pdlg->m_DlgRect->m_Tracker[i].SetRect(endRect);
		pdlg->m_DlgRect->m_Tracker[i].GetZoom(&l, &t, &r, &b);
		tempRect.left = (long)(tempRect.left + l);
		tempRect.top = (long)(tempRect.top + t);
		tempRect.right = (long)(tempRect.right + l);
		tempRect.bottom = (long)(tempRect.bottom + t);
		setupPara.InspPara.InsRegion[i] = tempRect;
		pdlg->m_DlgRect->m_Tracker[i].SetEnable(FALSE);
	}
	pdlg->m_VisMod->SetSetupPara(setupPara);
	::PostMessage(pdlg->m_DlgRect->m_hWnd, USER_MSG_IMG_RECT, 0, 8);
}

void CDialogSetupTeaching::ClickBtnenhctrlUp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	int i;
	CRect beginRect, endRect;
	CRect tempRect;
	double l, t, r, b;

	pdlg->m_DlgRect->Invalidate();

	for(i = INSPECTION_NIDL_REGION; i < setupPara.ModelPara.nModelCnt + INSPECTION_NIDL_REGION; i++)
	{
		pdlg->m_DlgRect->m_Tracker[i].SetEnable(TRUE);
		beginRect = pdlg->m_DlgRect->m_Tracker[i].GetRect();
		endRect = beginRect;
		endRect.top = beginRect.top - setupPara.iMoveCnt;
		endRect.bottom = beginRect.bottom - setupPara.iMoveCnt;

		tempRect.left = (long)(endRect.left / pdlg->m_dZoomRatio);
		tempRect.top = (long)(endRect.top / pdlg->m_dZoomRatio);
		tempRect.right = (long)(endRect.right / pdlg->m_dZoomRatio);
		tempRect.bottom = (long)(endRect.bottom / pdlg->m_dZoomRatio);

		pdlg->m_DlgRect->m_Tracker[i].SetRect(endRect);
		pdlg->m_DlgRect->m_Tracker[i].GetZoom(&l, &t, &r, &b);
		tempRect.left = (long)(tempRect.left + l);
		tempRect.top = (long)(tempRect.top + t);
		tempRect.right = (long)(tempRect.right + l);
		tempRect.bottom = (long)(tempRect.bottom + t);
		setupPara.InspPara.InsRegion[i] = tempRect;
		pdlg->m_DlgRect->m_Tracker[i].SetEnable(FALSE);
	}
	pdlg->m_VisMod->SetSetupPara(setupPara);
	::PostMessage(pdlg->m_DlgRect->m_hWnd, USER_MSG_IMG_RECT, 0, 8);
}

void CDialogSetupTeaching::ClickBtnenhctrlDown()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	int i;
	CRect beginRect, endRect, tempRect;
	double l, t, r, b;

	pdlg->m_DlgRect->Invalidate();

	for(i = INSPECTION_NIDL_REGION; i < setupPara.ModelPara.nModelCnt + INSPECTION_NIDL_REGION; i++)
	{
		pdlg->m_DlgRect->m_Tracker[i].SetEnable(TRUE);
		beginRect = pdlg->m_DlgRect->m_Tracker[i].GetRect();
		endRect = beginRect;
		endRect.top = beginRect.top + setupPara.iMoveCnt;
		endRect.bottom = beginRect.bottom + setupPara.iMoveCnt;

		tempRect.left = (long)(endRect.left / pdlg->m_dZoomRatio);
		tempRect.top = (long)(endRect.top / pdlg->m_dZoomRatio);
		tempRect.right = (long)(endRect.right / pdlg->m_dZoomRatio);
		tempRect.bottom = (long)(endRect.bottom / pdlg->m_dZoomRatio);

		pdlg->m_DlgRect->m_Tracker[i].SetRect(endRect);
		pdlg->m_DlgRect->m_Tracker[i].GetZoom(&l, &t, &r, &b);
		tempRect.left = (long)(tempRect.left + l);
		tempRect.top = (long)(tempRect.top + t);
		tempRect.right = (long)(tempRect.right + l);
		tempRect.bottom = (long)(tempRect.bottom + t);
		setupPara.InspPara.InsRegion[i] = tempRect;
		pdlg->m_DlgRect->m_Tracker[i].SetEnable(FALSE);
	}
	pdlg->m_VisMod->SetSetupPara(setupPara);
	::PostMessage(pdlg->m_DlgRect->m_hWnd, USER_MSG_IMG_RECT, 0, 8);
}

void CDialogSetupTeaching::OnEnKillfocusEditMoveCnt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;
	UpdateData(TRUE);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	setupPara.iMoveCnt = m_edit_nMoveCnt;
	pdlg->m_VisMod->SetSetupPara(setupPara);
}

void CDialogSetupTeaching::Update(BOOL isUpdate)
{
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	typeSetupPara setupPara;
	
	if(isUpdate)
	{
		setupPara = pdlg->m_VisMod->GetSetupPara();
		m_edit_nScore = setupPara.InspPara.iScore;
		m_edit_nMoveCnt = setupPara.iMoveCnt;
		m_edit_nScore = setupPara.InspPara.dInspEdgeThreshHeigh;
		m_edit_dPara0 = setupPara.InspPara.dInspBright;
		m_edit_dPara1 = setupPara.InspPara.dInspBrokenLength;
		m_edit_dPara2 = setupPara.InspPara.dInspSmooth;
		//InitListCtrl(&m_List);
		m_edit_dpara_0 = setupPara.InspPara.dInspOffSetY[0];
		m_edit_dpara_1 = setupPara.InspPara.dInspOffSetZ[0];
		m_edit_dpara_2 = setupPara.InspPara.dInspOffSetX[0];
		m_edit_dpara_3 = setupPara.InspPara.dInspOffSetA[0];
		m_edit_nScore2 = setupPara.InspPara.InsCenterPointOffSet.y;
		m_bBrokenInspection = setupPara.InspPara.bInspBroken;
		//m_check_Inspection_0 = setupPara.InspPara.bInspWidth[0];
		//m_check_Inspection_1 = setupPara.InspPara.bInspHeight[0];
		//FillListCtrl(&m_List);
		UpdateData(FALSE);
	}
	else
	{
		UpdateData(TRUE);
		//GetListCtrl(&m_List);
		//FillListCtrl(&m_List);
		setupPara = pdlg->m_VisMod->GetSetupPara();
		setupPara.InspPara.iScore = m_edit_nScore;
		setupPara.iMoveCnt = m_edit_nMoveCnt;
		setupPara.InspPara.dInspEdgeThreshHeigh = m_edit_nScore;

		setupPara.InspPara.dInspBright = m_edit_dPara0;
		setupPara.InspPara.dInspBrokenLength = m_edit_dPara1;
		setupPara.InspPara.dInspSmooth = m_edit_dPara2;
		setupPara.InspPara.dInspOffSetY[0] = m_edit_dpara_0;
		setupPara.InspPara.dInspOffSetZ[0] = m_edit_dpara_1;
		setupPara.InspPara.dInspOffSetX[0] = m_edit_dpara_2;
		setupPara.InspPara.dInspOffSetA[0] = m_edit_dpara_3;
		setupPara.InspPara.InsCenterPointOffSet.y = m_edit_nScore2;
		setupPara.InspPara.bInspBroken = m_bBrokenInspection;
		
		//pdlg->m_VisMod->SetSetupPara(setupPara);
		//FillListCtrl(&m_List);
		//GetListCtrl(&m_List);
		pdlg->m_VisMod->SetSetupPara(setupPara);
	}
}

void CDialogSetupTeaching::ClickBtnenhctrlAllTeach()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	typeSetupPara setupPara;
	int i = 0;
	double l, t, r, b;
	CRect beginRect, endRect, tempRect, firstRect;

	setupPara = pdlg->m_VisMod->GetSetupPara();

	pdlg->m_DlgRect->Invalidate();

	beginRect = pdlg->m_DlgRect->m_Tracker[3].GetRect();
	double gap = 0.;
	gap = setupPara.ModelPara.dModelGap / setupPara.dPixelSize;
	for(i = INSPECTION_NIDL_REGION ; i < setupPara.ModelPara.nModelCnt + INSPECTION_NIDL_REGION; i++)
	{
		pdlg->m_DlgRect->m_Tracker[i].SetEnable(TRUE);
		//beginRect = pdlg->m_DlgRect->m_Tracker[i - 1].GetRect();
		//pdlg->m_DlgRect->m_Tracker[i - 1].GetRect(&l, &t, &r, &b);

		//endRect = beginRect;
		endRect.left = (long)(beginRect.left / pdlg->m_dZoomRatio);//l / pdlg->m_dZoomRatio;
		endRect.top = (long)((beginRect.top / pdlg->m_dZoomRatio) + ((gap * (i - INSPECTION_NIDL_REGION)) * m_edit_dScale));
		endRect.right = (long)(beginRect.right / pdlg->m_dZoomRatio);//r / pdlg->m_dZoomRatio;
		endRect.bottom = endRect.top + (long)(beginRect.Height() / pdlg->m_dZoomRatio);// / setupPara.dPixelSize);

		tempRect.left = (long)(endRect.left * pdlg->m_dZoomRatio);
		tempRect.top = (long)(endRect.top * pdlg->m_dZoomRatio);
		tempRect.right = (long)(endRect.right * pdlg->m_dZoomRatio);
		tempRect.bottom = (long)(endRect.bottom * pdlg->m_dZoomRatio);

		pdlg->m_DlgRect->m_Tracker[i].SetRect(tempRect);
		pdlg->m_DlgRect->m_Tracker[i].GetZoom(&l, &t, &r, &b);
		tempRect.left = (long)(endRect.left + l);
		tempRect.top = (long)(endRect.top + t);
		tempRect.right = (long)(endRect.right + l);
		tempRect.bottom = (long)(endRect.bottom + t);
		setupPara.InspPara.InsRegion[i] = tempRect;
		pdlg->m_DlgRect->m_Tracker[i].SetEnable(FALSE);
	}
	pdlg->m_VisMod->SetSetupPara(setupPara);
	::PostMessage(pdlg->m_DlgRect->m_hWnd, USER_MSG_IMG_RECT, 0, 8);
}

BOOL CDialogSetupTeaching::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwExStyle = LVS_EX_FULLROWSELECT
					/*| LVS_EX_TRACKSELECT*/;	// for hot tracking
	BOOL	m_bGridlines = TRUE;
	if (m_bGridlines)
		dwExStyle |= LVS_EX_GRIDLINES;

	m_List.SetExtendedStyle(dwExStyle);

	// call EnableToolTips to enable tooltip display
	m_List.EnableToolTips(FALSE);

	//InitListCtrl(&m_List);
	//FillListCtrl(&m_List, TRUE);
	return TRUE;
}
void CDialogSetupTeaching::ClickBtnenhctrlScaleUp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_edit_dScale = m_edit_dScale + 0.001;
	UpdateData(FALSE);
	ClickBtnenhctrlAllTeach();
}

void CDialogSetupTeaching::ClickBtnenhctrlScaleDown()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_edit_dScale = m_edit_dScale - 0.001;
	UpdateData(FALSE);
	ClickBtnenhctrlAllTeach();
}

void CDialogSetupTeaching::OnBnClickedButtonParaAll0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	UpdateData(TRUE);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	int i = 0;
	
	for(i = 0; i < setupPara.ModelPara.nModelCnt; i++)
	{
		//setupPara.InspPara.dInspHeight[i] = m_edit_dpara_0;
		setupPara.InspPara.dInspOffSetY[i] = m_edit_dpara_0;
	}
	pdlg->m_VisMod->SetSetupPara(setupPara);
	//FillListCtrl(&m_List);
}

void CDialogSetupTeaching::OnBnClickedButtonParaAll1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	UpdateData(TRUE);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	int i = 0;
	
	for(i = 0; i < setupPara.ModelPara.nModelCnt; i++)
	{
		//setupPara.InspPara.dInspLength[i] = m_edit_dpara_1;
		setupPara.InspPara.dInspOffSetZ[i] = m_edit_dpara_1;
	}
	pdlg->m_VisMod->SetSetupPara(setupPara);
	//FillListCtrl(&m_List);
}

void CDialogSetupTeaching::OnBnClickedButtonParaAll2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	UpdateData(TRUE);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	int i = 0;
	
	for(i = 0; i < setupPara.ModelPara.nModelCnt; i++)
	{
		//setupPara.InspPara.dInspWidth[i] = m_edit_dpara_2;
		setupPara.InspPara.dInspOffSetX[i] = m_edit_dpara_2;
	}
	pdlg->m_VisMod->SetSetupPara(setupPara);
	//FillListCtrl(&m_List);
}

void CDialogSetupTeaching::OnBnClickedButtonParaAll3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	UpdateData(TRUE);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	int i = 0;
	
	for(i = 0; i < setupPara.ModelPara.nModelCnt; i++)
	{
		//setupPara.InspPara.dInspArea[i] = m_edit_dpara_3;
		setupPara.InspPara.dInspOffSetA[i] = m_edit_dpara_3;
	}
	pdlg->m_VisMod->SetSetupPara(setupPara);
	//FillListCtrl(&m_List);
}