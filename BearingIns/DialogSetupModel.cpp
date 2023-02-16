// DialogSetupModel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TypeDef.h"
#include "BearingIns.h"
#include "BearingInsDlg.h"
#include "DialogSetupModel.h"


// CDialogSetupModel 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSetupModel, CDialog)

CDialogSetupModel::CDialogSetupModel(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetupModel::IDD, pParent)
	, m_edit_dWidht(0)
	, m_edit_dHeight(0)
	, m_edit_dCnt(0)
	, m_edit_dPi(0)
	, m_edit_dGap(0)
	, m_edit_dPixelSize(0.061)
	, m_edit_dDiameter(0)
	, m_edit_nCol(1)
	, m_chk_Save_0(FALSE)
	, m_chk_Save_1(FALSE)
	, m_chk_Save_2(FALSE)
{
	m_bCalculGap = FALSE;
}

CDialogSetupModel::~CDialogSetupModel()
{
}

void CDialogSetupModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MODEL_WIDTH, m_edit_dWidht);
	DDX_Text(pDX, IDC_EDIT_MODEL_HEIGHT, m_edit_dHeight);
	DDX_Text(pDX, IDC_EDIT_MODEL_CNT, m_edit_dCnt);
	DDX_Text(pDX, IDC_EDIT_MODEL_PI, m_edit_dPi);
	DDX_Text(pDX, IDC_EDIT_MODEL_GAP, m_edit_dGap);
	DDX_Control(pDX, IDC_BTNENHCTRL_CALCUL_GAP, m_btnCalCul);
	DDX_Text(pDX, IDC_EDIT_MODEL_PIXEL_SIZE, m_edit_dPixelSize);
	DDX_Text(pDX, IDC_EDIT_MODEL_DIAMETER, m_edit_dDiameter);
	DDX_Text(pDX, IDC_EDIT_MODEL_COL, m_edit_nCol);
	DDX_Check(pDX, IDC_CHECK_SAVE_0, m_chk_Save_0);
	DDX_Check(pDX, IDC_CHECK_SAVE_1, m_chk_Save_1);
	DDX_Check(pDX, IDC_CHECK_SAVE_2, m_chk_Save_2);
}


BEGIN_MESSAGE_MAP(CDialogSetupModel, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_MASTER_SAVE, &CDialogSetupModel::OnBnClickedButtonMasterSave)
	ON_BN_CLICKED(IDC_BUTTON_MASTER_LOAD, &CDialogSetupModel::OnBnClickedButtonMasterLoad)
END_MESSAGE_MAP()


// CDialogSetupModel 메시지 처리기입니다.
void CDialogSetupModel::Update(BOOL isUpdate)
{
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	
	if(isUpdate)
	{
		m_edit_dWidht = setupPara.ModelPara.dModelWidth;
		m_edit_dHeight = setupPara.ModelPara.dModelHeight;
		m_edit_dCnt = setupPara.ModelPara.nModelCnt;
		m_edit_dPi = setupPara.ModelPara.dModelPI;
		m_edit_dGap = setupPara.ModelPara.dModelGap;
		m_edit_dPixelSize = setupPara.dPixelSize;
		m_edit_dDiameter = setupPara.ModelPara.dDiameter;
		m_edit_nCol = setupPara.ModelPara.nCol;
		m_chk_Save_0 = setupPara.bSaveImage;
		m_chk_Save_1 = setupPara.bOKSaveImage;
		m_chk_Save_2 = setupPara.bNGSaveImage;
		UpdateData(FALSE);
	}
	else
	{
		UpdateData(TRUE);
		setupPara.ModelPara.dModelWidth = m_edit_dWidht;
		setupPara.ModelPara.dModelHeight = m_edit_dHeight;
		setupPara.ModelPara.nModelCnt = (int)m_edit_dCnt;
		setupPara.ModelPara.dModelPI = m_edit_dPi;
		setupPara.ModelPara.dModelGap = m_edit_dGap;
		setupPara.dPixelSize = m_edit_dPixelSize;
		setupPara.ModelPara.dDiameter = m_edit_dDiameter;
		setupPara.ModelPara.nCol = m_edit_nCol;
		setupPara.bSaveImage = m_chk_Save_0;
		setupPara.bOKSaveImage = m_chk_Save_1;
		setupPara.bNGSaveImage = m_chk_Save_2;
		pdlg->m_VisMod->SetSetupPara(setupPara);
	}
}
void CDialogSetupModel::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bCalculGap = FALSE;
	if(bShow)
	{
		Update(TRUE);
	}
	else
	{
		Update(FALSE);
	}
}
BEGIN_EVENTSINK_MAP(CDialogSetupModel, CDialog)
	ON_EVENT(CDialogSetupModel, IDC_BTNENHCTRL_CALCUL_GAP, DISPID_CLICK, CDialogSetupModel::ClickBtnenhctrlCalculGap, VTS_NONE)
END_EVENTSINK_MAP()

void CDialogSetupModel::ClickBtnenhctrlCalculGap()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_bCalculGap = m_btnCalCul.GetValue();
}

void CDialogSetupModel::OnBnClickedButtonMasterSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	CString path;
	path.Format("%s\\Master_Image.bmp", pdlg->m_ModelInfo.strModelPath);
	MbufExport((char *)(LPCTSTR)path, M_BMP, *pdlg->m_VisMod->GetMilDispBuf(MAIN_DISP));
}

void CDialogSetupModel::OnBnClickedButtonMasterLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	CString path;
	path.Format("%s\\Master_Image.bmp", pdlg->m_ModelInfo.strModelPath);
	MbufImport((char *)(LPCTSTR)path, M_BMP, M_LOAD, *pdlg->m_VisMod->GetMilSystem(), pdlg->m_VisMod->GetMilGrabBuf());
	//double diameter = 0.;
	//diameter = setupPara.ModelPara.dDiameter / 45.0;
	MbufClear(*pdlg->m_VisMod->GetMilDispBuf(MAIN_DISP), M_NULL);
	//MimResize(*pdlg->m_VisMod->GetMilGrabBuf(), *pdlg->m_VisMod->GetMilDispBuf(MAIN_DISP), 1, diameter, M_DEFAULT );
	MbufCopy(*pdlg->m_VisMod->GetMilGrabBuf(), *pdlg->m_VisMod->GetMilDispBuf(MAIN_DISP));
}

BOOL CDialogSetupModel::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}