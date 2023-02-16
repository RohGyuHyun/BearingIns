// DialogSetupAlign.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BearingIns.h"
#include "DialogSetupAlign.h"
#include "BearingInsDlg.h"


// CDialogSetupAlign 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSetupAlign, CDialog)

CDialogSetupAlign::CDialogSetupAlign(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetupAlign::IDD, pParent)
	, m_edit_nScore(0)
	, m_edit_nAlignType0(0)
	, m_edif_para0(0)
{

}

CDialogSetupAlign::~CDialogSetupAlign()
{
}

void CDialogSetupAlign::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SCORE, m_edit_nScore);
	DDX_Radio(pDX, IDC_RADIO_ALIGN_TYPE0, m_edit_nAlignType0);
	DDX_Text(pDX, IDC_EDIT_THRD, m_edif_para0);
}


BEGIN_MESSAGE_MAP(CDialogSetupAlign, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RADIO_ALIGN_TYPE0, &CDialogSetupAlign::OnBnClickedRadioAlignType0)
	ON_BN_CLICKED(IDC_RADIO_ALIGN_TYPE1, &CDialogSetupAlign::OnBnClickedRadioAlignType1)
END_MESSAGE_MAP()


// CDialogSetupAlign 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogSetupAlign, CDialog)
	ON_EVENT(CDialogSetupAlign, IDC_BTNENHCTRL_TEACH, DISPID_CLICK, CDialogSetupAlign::ClickBtnenhctrlTeach, VTS_NONE)
	ON_EVENT(CDialogSetupAlign, IDC_BTNENHCTRL_TEST, DISPID_CLICK, CDialogSetupAlign::ClickBtnenhctrlTest, VTS_NONE)
END_EVENTSINK_MAP()

void CDialogSetupAlign::ClickBtnenhctrlTeach()
{
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;
	typeSetupPara setupPara;
	int ins_idx = ALIGN_SEARCH_REGION_1;
	MIL_ID MilBinImage, MilBlobResult, MilBlobFeatureList;
	MIL_ID image, MilTransImage, MilChildImage;
	MIL_INT TotalBlobs;
	UpdateData(TRUE);
	double *CogX,                       
	       *CogY,					    
		   *Area,
		   *Width,
		   *height;
	double area;
	//CString str_temp;
	char str_temp[100];
	
	setupPara = pdlg->m_VisMod->GetSetupPara();

	for(ins_idx = ALIGN_SEARCH_REGION_1; ins_idx < ALIGN_SEARCH_REGION_1 + setupPara.ModelPara.nCol; ins_idx++)
	{
		
		MdispControl(*pdlg->m_VisMod->GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);

		MbufAlloc2d(*pdlg->m_VisMod->GetMilSystem(), CAMERA_WIDTH, MAX_CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image);

		MbufCopy(*pdlg->m_VisMod->GetMilDispBuf(MAIN_DISP), image);	
		
		CRect rect(setupPara.InspPara.InsRegion[ins_idx].left,
						setupPara.InspPara.InsRegion[ins_idx].top,
						setupPara.InspPara.InsRegion[ins_idx].right,
						setupPara.InspPara.InsRegion[ins_idx].bottom);

		MbufAlloc2d(*pdlg->m_VisMod->GetMilSystem(), CAMERA_WIDTH, MAX_CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage);
		MbufClear(MilTransImage, 0x00);

		MbufAlloc2d(*pdlg->m_VisMod->GetMilSystem(), rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
		MbufClear(MilBinImage, 0x00);

		MbufChild2d(image, rect.left, rect.top, rect.Width(), rect.Height(), &MilChildImage);

		MimBinarize(MilChildImage, MilBinImage, M_GREATER_OR_EQUAL, m_edif_para0, M_NULL);

		//Transform
		MimTranslate(MilBinImage, MilTransImage, rect.left, rect.top, M_DEFAULT);

		/* Allocate a blob result buffer. */
		MblobAllocResult(*pdlg->m_VisMod->GetMilSystem(), &MilBlobResult); 

		/* Allocate a feature list. */ 
		MblobAllocFeatureList(*pdlg->m_VisMod->GetMilSystem(), &MilBlobFeatureList); 

		MblobSelectFeature(MilBlobFeatureList, M_AREA); 
		MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY); 
		MblobSelectFeature(MilBlobFeatureList, M_FERET_X); 
		MblobSelectFeature(MilBlobFeatureList, M_FERET_Y); 

		/* Calculate selected features for each blob. */ 
		MblobCalculate(MilTransImage, M_NULL, MilBlobFeatureList, MilBlobResult); 

		CRect rect0(setupPara.InspPara.InsRegion[ALIGN_REGION].left,
						setupPara.InspPara.InsRegion[ALIGN_REGION].top,
						setupPara.InspPara.InsRegion[ALIGN_REGION].right,
						setupPara.InspPara.InsRegion[ALIGN_REGION].bottom);

		double limt_area, limt_width, limt_height;
		CString temp;
		int i;

		limt_width = rect0.Width() * 0.9;
		limt_height = rect0.Height() * 0.3;
		if(limt_height < 1)
		{
			limt_height = 1;
		}
		limt_area = (rect0.Width() * rect0.Height()) * 0.8;

		/* Exclude blobs whose area is too small. */ 
		MblobSelect(MilBlobResult, M_EXCLUDE, M_AREA, M_LESS_OR_EQUAL, limt_area, M_NULL); 
		MblobSelect(MilBlobResult, M_EXCLUDE, M_FERET_X, M_LESS_OR_EQUAL, limt_width, M_NULL); 
		MblobSelect(MilBlobResult, M_EXCLUDE, M_FERET_Y, M_LESS_OR_EQUAL, limt_height, M_NULL); 


		/* Get the total number of selected blobs. */ 
		MblobGetNumber(MilBlobResult, &TotalBlobs); 

		if ((CogX = (double *)malloc(TotalBlobs*sizeof(double))) && 
		(CogY = (double *)malloc(TotalBlobs*sizeof(double))) && 
		(Area = (double *)malloc(TotalBlobs*sizeof(double))) &&
		(Width = (double *)malloc(TotalBlobs*sizeof(double))) &&
		(height = (double *)malloc(TotalBlobs*sizeof(double)))
		)
		{ 

			MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_X, CogX); 
			MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_Y, CogY); 
			MblobGetResult(MilBlobResult, M_AREA, Area); 
			MblobGetResult(MilBlobResult, M_FERET_X, Width); 
			MblobGetResult(MilBlobResult, M_FERET_Y, height); 
			setupPara.ModelPara.dModelHeight = height[0];// * setupPara.dPixelSize;

			area= 0;
			for(i = 0; i < TotalBlobs; i++)
			{
				area = area + Area[i];
				MgraColor(M_DEFAULT, M_COLOR_GREEN);
				sprintf(str_temp, "col : %d, cnt : %d, x = %.0f, y = %.0f, Width = %.1f(mm)", ins_idx, i, CogX[i], CogY[i], (Width[i] * setupPara.dPixelSize));
				//str_temp.Format(_T("col : %d, cnt : %d, x = %.0f, y = %.0f, Width = %.1f(mm)"), ins_idx, i, CogX[i], CogY[i], (Width[i] * setupPara.dPixelSize));
				MgraText(M_DEFAULT, *pdlg->m_VisMod->GetMilOverlay(MAIN_DISP), CogX[i] - (Width[i] / 2), CogY[i], MIL_TEXT(str_temp));
				setupPara.InspPara.InsCenterPoint[i].SetPoint(CogX[i], CogY[i]);
			}

			setupPara.InspPara.dAlignX[ins_idx - ALIGN_SEARCH_REGION_1] = CogX[0] - rect.left;
			setupPara.InspPara.dAlignY[ins_idx - ALIGN_SEARCH_REGION_1] = CogY[0] - rect.top;

			free(CogX);
			free(CogY);
			free(Area);
			free(Width);
			free(height);
		}
		else
		{
			MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
			MgraColor(M_DEFAULT, M_COLOR_RED);
			MgraText(M_DEFAULT, *pdlg->m_VisMod->GetMilOverlay(MAIN_DISP), rect.right, rect.top, MIL_TEXT((char *)"Teaching fail...!"));
		}

		MgraColor(M_DEFAULT, M_COLOR_GREEN);
		MblobDraw(M_DEFAULT, MilBlobResult, *pdlg->m_VisMod->GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS, M_INCLUDED_BLOBS, M_DEFAULT);

		MblobFree(MilBlobResult); 
		MblobFree(MilBlobFeatureList); 
		MbufFree(MilChildImage);
		MbufFree(MilBinImage);
		MbufFree(MilTransImage);
		MbufFree(image);
	}
	pdlg->m_dlgSetup->m_dlgSetupModel->m_edit_dHeight = setupPara.ModelPara.dModelHeight;
	pdlg->m_VisMod->SetSetupPara(setupPara);
	pdlg->m_VisMod->SaveSetupPara(pdlg->m_ModelInfo.strModelPath);
}

void CDialogSetupAlign::ClickBtnenhctrlTest()
{
	
}

void CDialogSetupAlign::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	ShowDlg(bShow);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDialogSetupAlign::ShowDlg(int nShow)
{
	CBearingInsDlg *pdlg = (CBearingInsDlg*)AfxGetApp()->m_pMainWnd;
	typeSetupPara setupPara;
	CString strTeachPath;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	if(nShow)
	{
		for(int i = ALIGN_REGION; i < ALIGN_SEARCH_REGION_1 + setupPara.ModelPara.nCol; i++)
		{
			pdlg->m_DlgRect->m_Tracker[i].SetActive(TRUE);
		}
		switch(setupPara.InspPara.iAlignType)
		{
		case ALIGN_TYPE_SEARCH:
			GetDlgItem(IDC_EDIT_SCORE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_THRD)->EnableWindow(TRUE);
			break;
		case ALIGN_TYPE_PATTERN:
			GetDlgItem(IDC_EDIT_SCORE)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_THRD)->EnableWindow(FALSE);
			break;
		}
	}
	else
	{
		//Update(FALSE);
		for(int i = ALIGN_REGION; i < ALIGN_SEARCH_REGION_1 + 2; i++)
		{
			pdlg->m_DlgRect->m_Tracker[i].SetActive(FALSE);
		}
		pdlg->m_DlgRect->Invalidate();
	}
}

void CDialogSetupAlign::Update(BOOL isUpdate)
{
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	if(isUpdate)
	{
		m_edit_nScore = setupPara.InspPara.iAlignScore;
		m_edif_para0 = setupPara.InspPara.dInspThreshold;
		m_edit_nAlignType0 = setupPara.InspPara.iAlignType;
		UpdateData(FALSE);
	}
	else
	{
		UpdateData(TRUE);
		setupPara.InspPara.iAlignScore = m_edit_nScore;
		setupPara.InspPara.dInspThreshold = m_edif_para0;
		setupPara.InspPara.iAlignType = m_edit_nAlignType0;
		pdlg->m_VisMod->SetSetupPara(setupPara);
	}
}
void CDialogSetupAlign::OnBnClickedRadioAlignType0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_EDIT_SCORE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_THRD)->EnableWindow(TRUE);
}

void CDialogSetupAlign::OnBnClickedRadioAlignType1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_EDIT_SCORE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_THRD)->EnableWindow(FALSE);
}

BOOL CDialogSetupAlign::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}