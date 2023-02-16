
// BearingInsDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "BearingIns.h"
#include "_CtrlModelManageDlg.h"
#include "DialogRect.h"
#include "DialogModel.h"
#include <omp.h>
#include "BearingInsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//폴더 삭제
BOOL DeleteDirectorie(CString strFolder)
{
	CFileFind ff;
	CString strTarget = strFolder;
	CStringArray strFileArray;

	if(strTarget.Right(1) != '\\')
	{
		strTarget += '\\';
	}
	
	strTarget += "*.*";
	if(ff.FindFile(strTarget)==TRUE)
	{
		CString Filename;
		BOOL bFlag = true;
		while(bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if(ff.IsDots())
			{
				continue;
			}
			if(ff.IsDirectory())
			{
				DeleteDirectorie(Filename);
				RemoveDirectory(Filename);
			}
			else
			{
				DeleteFile(Filename);
			}
		}
	}
	ff.Close();

	return TRUE;
}


BOOL FileSearch(CString FilePath, CString FileName)
{
	CFileFind ff;
	CString strTarget = FilePath;
	CStringArray strFileArray;
	BOOL rslt = FALSE;

	if(strTarget.Right(1) != '\\')
	{
		strTarget += '\\';
	}
	
	strTarget += "*.*";
	if(ff.FindFile(strTarget)==TRUE)
	{
		CString Filename;
		BOOL bFlag = true;
		while(bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if(ff.IsDots())
			{
				continue;
			}
			if(ff.IsDirectory())
			{
				continue;
			}
			if(ff.GetFileName() == FileName)
			{
				rslt = TRUE;
				break;
			}
		}
	}
	ff.Close();
	
	return rslt;
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CBearingInsDlg 대화 상자
CBearingInsDlg::CBearingInsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBearingInsDlg::IDD, pParent)
	, m_chk_OUT1(FALSE)
	, m_chk_OUT2(FALSE)
	, m_chk_OUT3(FALSE)
	, m_chk_OUT4(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_ModelInfo.strModelName.Empty();
	m_ModelInfo.strModelPath.Empty();
	m_ModelInfo.strModelDesc.Empty();
	m_ModelInfo.dModelWidth = 0.;
	m_ModelInfo.dModelHeight = 0.;
	m_ModelInfo.nModelCnt = 0;
	m_ModelInfo.dModelPI = 0.;

	m_bWidthIns = TRUE;
	m_bHeightIns = TRUE;
	m_bAreaIns = TRUE;

	m_hComiDevice32 = NULL;
	m_pMainThread = NULL;
	m_pIOThread = NULL;
	m_bInspectStart = FALSE;
	m_nTemp_Space = 0;
	m_bInspectEnd = FALSE;
	m_bInspectInput = FALSE;
	m_bIOCheck = FALSE;
	m_InsCntPara.nTotalCnt = 0;
	m_InsCntPara.nOk1Cnt = 0;
	m_InsCntPara.nNg1Cnt = 0;
	m_InsCntPara.nOk2Cnt = 0;
	m_InsCntPara.nNg2Cnt = 0;
	m_bIOEnd = FALSE;
	m_test = 0;
	m_bAutoDeleteStart = FALSE;
	m_bAutoDeleteEnd = FALSE;
	m_iDel_Year = 0;
	m_iDel_Month = 0;
	m_iDel_Day = 0;
	m_iDel_RecentDay = 0;
	m_bDelete = FALSE;
	m_AllModel.nMaxModelCnt = 0;
	for(int i = 0; i < MAX_MODEL_NUM; i++)
	{
		memset(&m_AllModel.Model[i], NULL, sizeof(m_AllModel.Model[i]));
	}
}

void CBearingInsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNENHCTRL_LIVE, m_Live);
	DDX_Control(pDX, IDC_SCROLLBAR_MAIN_V, m_ScrMain_V);
	DDX_Control(pDX, IDC_SCROLLBAR_MAIN_H, m_ScrMain_H);
	DDX_Control(pDX, IDC_BTNENHCTRL_RUN, m_btnStartStop);
	DDX_Control(pDX, IDC_LIST_STATUS, m_list_rslt);
	DDX_Control(pDX, IDC_BTNENHCTRL_MODEL_NAME, m_label_Model_Name);
	DDX_Control(pDX, IDC_BTNENHCTRL_MODEL_WIDTH, m_label_Model_Width);
	DDX_Control(pDX, IDC_BTNENHCTRL_MODEL_HEIGHT, m_label_Model_Height);
	DDX_Control(pDX, IDC_BTNENHCTRL_MODEL_BEARING_CNT, m_label_Model_Cnt);
	DDX_Control(pDX, IDC_BTNENHCTRL_MODEL_PI, m_label_Model_PI);
	DDX_Control(pDX, IDC_BTNENHCTRLSTART, m_btnStart);
	DDX_Control(pDX, IDC_BTNENHCTRL_TOTAL, m_btnTotal);
	DDX_Control(pDX, IDC_BTNENHCTRL_OK1, m_btnOK1);
	DDX_Control(pDX, IDC_BTNENHCTRL_NG1, m_btnNG1);
	DDX_Control(pDX, IDC_BTNENHCTRL_OK2, m_btnOK2);
	DDX_Control(pDX, IDC_BTNENHCTRL_NG2, m_btnNG2);
	DDX_Check(pDX, IDC_CHECK1, m_chk_OUT1);
	DDX_Check(pDX, IDC_CHECK2, m_chk_OUT2);
	DDX_Check(pDX, IDC_CHECK3, m_chk_OUT3);
	DDX_Check(pDX, IDC_CHECK4, m_chk_OUT4);
	DDX_Control(pDX, IDC_BTNENHCTRL_SETUP, m_btnSetup);
	DDX_Control(pDX, IDC_BTNENHCTRL_MODEL, m_btnModel);
	DDX_Control(pDX, IDC_BTNENHCTRL_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_BTNENHCTRL_RESULT, m_btnResult);
}

BEGIN_MESSAGE_MAP(CBearingInsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CBearingInsDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_TEST2, &CBearingInsDlg::OnBnClickedButtonTest2)
	ON_BN_CLICKED(IDC_BUTTON_OUT_0, &CBearingInsDlg::OnBnClickedButtonOut0)
	ON_BN_CLICKED(IDC_BUTTON_OUT_1, &CBearingInsDlg::OnBnClickedButtonOut1)
	ON_BN_CLICKED(IDC_BUTTON_OUT_2, &CBearingInsDlg::OnBnClickedButtonOut2)
	ON_BN_CLICKED(IDC_BUTTON_OUT_3, &CBearingInsDlg::OnBnClickedButtonOut3)
	ON_BN_CLICKED(IDC_BUTTON_OUT_4, &CBearingInsDlg::OnBnClickedButtonOut4)
	ON_BN_CLICKED(IDC_BUTTON_OUT_5, &CBearingInsDlg::OnBnClickedButtonOut5)
	ON_BN_CLICKED(IDC_BUTTON_OUT_6, &CBearingInsDlg::OnBnClickedButtonOut6)
	ON_BN_CLICKED(IDC_BUTTON_OUT_7, &CBearingInsDlg::OnBnClickedButtonOut7)
	ON_BN_CLICKED(IDC_BUTTON_OUT_8, &CBearingInsDlg::OnBnClickedButtonOut8)
	ON_BN_CLICKED(IDC_BUTTON_OUT_9, &CBearingInsDlg::OnBnClickedButtonOut9)
	ON_BN_CLICKED(IDC_BUTTON1, &CBearingInsDlg::OnBnClickedButton1)
	ON_MESSAGE(USER_MSG_PROCESS, OnProcessMessage)
END_MESSAGE_MAP()


// CBearingInsDlg 메시지 처리기

BOOL CBearingInsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	CRect rect;
	HWND _hwnd[4];
	SCROLLINFO scrinfo;
	double ratio, ratiow;
	CString temp;

	m_btnStartStop.SetBackColor(RGB(0,255,0));
	m_btnSetup.SetBackColor(RGB(0,255,0));
	m_btnModel.SetBackColor(RGB(0,255,0));
	m_btnExit.SetBackColor(RGB(0,255,0));
	m_btnStart.SetBackColor(RGB(255, 0, 0));

	m_VisMod = new CVisionModule();
	/*
	if(!m_VisMod->SafeNet_Lock_LogIn())
	{
		exit(0);
	}*/
	m_Trigger = new CTriggerBoard(&this->m_hWnd);
	m_DlgRect = new CDialogRect();

	m_DlgRect->Create(IDD_DIALOG_RECT, this);
	m_DlgRect->MoveWindow(57,35,576,971,TRUE);
	m_DlgRect->GetWindowRect(rect);

	ratio = (double)rect.Height() / MAX_CAMERA_HEIGHT;
	ratiow = (double)rect.Width() / (CAMERA_WIDTH / ratio);
	m_dZoomDefault = ratio;
	//
	m_DlgRect->SetZoom(ratio, (long)(CAMERA_WIDTH / m_dZoomDefault), MAX_CAMERA_HEIGHT);
	m_DlgRect->ShowWindow(SW_SHOW);
	m_DlgRect->m_rectDlg.SetRect(0, 0, (int)(MAX_CAMERA_HEIGHT * ratio), (int)(MAX_CAMERA_HEIGHT * ratio));

	_hwnd[0] = m_DlgRect->m_hWnd;
	_hwnd[1] = GetDlgItem(IDC_STATIC_NG_DISP1)->m_hWnd;
	_hwnd[2] = GetDlgItem(IDC_STATIC_NG_DISP2)->m_hWnd;
	_hwnd[3] = GetDlgItem(IDC_STATIC_NG_DISP3)->m_hWnd;

	GetDlgItem(IDC_STATIC_NG_DISP1)->GetClientRect(&rect);

	m_VisMod->Open(_hwnd, ratio);

	m_dZoomRatio = ratio;
	
	
	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_ALL;
	scrinfo.nMin = 0;
	scrinfo.nMax = 0;
	scrinfo.nPage = 1;
	scrinfo.nTrackPos = 1;
	scrinfo.nPos = 0;
	m_ScrMain_V.SetScrollInfo(&scrinfo);

	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_ALL;
	scrinfo.nMin = 0;
	scrinfo.nMax = 0;
	scrinfo.nPage = 1;
	scrinfo.nTrackPos = 1;
	scrinfo.nPos = 0;
	m_ScrMain_H.SetScrollInfo(&scrinfo);	

	m_dlgProcessing.Create(IDD_DIALOG_PROCESSING, this);
	m_dlgProcessing.ShowWindow(SW_HIDE);

	m_strDBPath.Format(DB_PATH);
	MakeDirectories(m_strDBPath);
	m_strRegiPath.Format(REG_PATH);
	m_strLogPath.Format(LOG_PATH);
	MakeDirectories(m_strLogPath);

	LV_COLUMN m_lvColumn;
	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_LEFT;
	m_lvColumn.pszText="SYSTEM LOG";
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=700;
	m_list_rslt.InsertColumn(0, &m_lvColumn);

	//Model
	ModelInfoLoad();

	typeSetupPara setupPara;
	//m_VisMod->LoadSetupPara(m_ModelInfo.strModelPath);
	setupPara = m_VisMod->GetSetupPara();

	m_dlgSetup = new CDialogSetup();
    m_dlgSetup->Create(IDD_DIALOG_SETUP, this);
	m_dlgSetup->ShowWindow(SW_HIDE);

	m_dlgSetup->m_dlgSetupTeaching->GetDlgItem(IDC_BTNENHCTRL_TEACH)->ShowWindow(FALSE);
	m_dlgSetup->m_dlgSetupTeaching->GetDlgItem(IDC_STATIC_SCORE)->ShowWindow(FALSE);
	m_dlgSetup->m_dlgSetupTeaching->GetDlgItem(IDC_EDIT_SCORE)->ShowWindow(FALSE);
	
	SetInsCnt();
	
	if(m_Trigger->Init(m_strDBPath))
	{
		WriteStatusList("Success : Trigger Board Init Ok");
	}
	else
	{
		WriteStatusList("Error : Trigger Board Init Fail");
	}

#ifdef LIGHT
	m_cSerial.Open(LINE_LIGHT_COM_PORT, LINE_LIGHT_COM_BAUDRATE);
	if(m_cSerial.IsOpen())
	{
		WriteStatusList("Success : Line Light Initialize OK !");
	}
	else
	{
		WriteStatusList("Error : Line Light Initialize Failed !");
	}
#else
	WriteStatusList("Success : Line Light Initialize OK !");
#endif

	if(COMI_LoadDll())
	{
		m_hComiDevice32 = COMI_LoadDevice(0xB424, 0);	// DIO 32
		if(m_hComiDevice32 != (HANDLE)0xffffffffffffffff)
		{
			WriteStatusList("Success : I/O Initialize OK !");
			InitOutput();
		}
		else
		{
			WriteStatusList("Error : I/O Initialize failed(LoadDevice) !");
		}
	}
	else
	{
		WriteStatusList("Error : I/O Initialize failed(LoadDLL) !");
	}

	m_pAutoDeleteThread = AfxBeginThread(AutoDeleteThread, this, THREAD_PRIORITY_NORMAL);
	if(m_pAutoDeleteThread)
		m_pAutoDeleteThread->m_bAutoDelete = TRUE;

	

	m_bAutoDeleteStart = TRUE;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBearingInsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBearingInsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		CPen pen,*oldPen;
		CBrush brush,*oldBrush;

		CPaintDC dc(this);

		GetClientRect(&rect);

		//빈공간 채우기
		pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		oldPen=dc.SelectObject(&pen);
		brush.CreateSolidBrush(RGB(0, 0, 0));
		oldBrush=dc.SelectObject(&brush);
		
		dc.Rectangle(&rect);

		dc.SelectObject(oldPen);
		pen.DeleteObject();
		dc.SelectObject(oldBrush);
		brush.DeleteObject();

		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBearingInsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// CDialogRect 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CBearingInsDlg, CDialog)
	ON_EVENT(CBearingInsDlg, IDC_BTNENHCTRL_EXIT, DISPID_CLICK, CBearingInsDlg::ClickBtnenhctrlExit, VTS_NONE)
	ON_EVENT(CBearingInsDlg, IDC_BTNENHCTRL_LIVE, DISPID_CLICK, CBearingInsDlg::ClickBtnenhctrlLive, VTS_NONE)
	ON_EVENT(CBearingInsDlg, IDC_BTNENHCTRL_SAVE, DISPID_CLICK, CBearingInsDlg::ClickBtnenhctrlSave, VTS_NONE)
	ON_EVENT(CBearingInsDlg, IDC_BTNENHCTRL_LOAD, DISPID_CLICK, CBearingInsDlg::ClickBtnenhctrlLoad, VTS_NONE)
	ON_EVENT(CBearingInsDlg, IDC_BTNENHCTRL_ZOOM_IN, DISPID_CLICK, CBearingInsDlg::ClickBtnenhctrlZoomIn, VTS_NONE)
	ON_EVENT(CBearingInsDlg, IDC_BTNENHCTRL_ZOOM_OUT, DISPID_CLICK, CBearingInsDlg::ClickBtnenhctrlZoomOut, VTS_NONE)
	ON_EVENT(CBearingInsDlg, IDC_BTNENHCTRL_ZOOM_11, DISPID_CLICK, CBearingInsDlg::ClickBtnenhctrlZoom11, VTS_NONE)
	ON_EVENT(CBearingInsDlg, IDC_BTNENHCTRL_ZOOM_FIT, DISPID_CLICK, CBearingInsDlg::ClickBtnenhctrlZoomFit, VTS_NONE)
	ON_EVENT(CBearingInsDlg, IDC_BTNENHCTRL_MODEL, DISPID_CLICK, CBearingInsDlg::ClickBtnenhctrlModel, VTS_NONE)
	ON_EVENT(CBearingInsDlg, IDC_BTNENHCTRL_SETUP, DISPID_CLICK, CBearingInsDlg::ClickBtnenhctrlSetup, VTS_NONE)
	ON_EVENT(CBearingInsDlg, IDC_BTNENHCTRL_RUN, DISPID_CLICK, CBearingInsDlg::ClickBtnenhctrlRun, VTS_NONE)
	ON_EVENT(CBearingInsDlg, IDC_BTNENHCTRL_CNT_RESET, DISPID_CLICK, CBearingInsDlg::ClickBtnenhctrlCntReset, VTS_NONE)
END_EVENTSINK_MAP()

void CBearingInsDlg::ClickBtnenhctrlExit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
#ifdef LIGHT
	for(int i = 0; i < LIGHT_3; i++)
	{
		SetLineLight(i + 1, 0);
		Sleep(100);
	}
#endif
	m_bInspectStart = FALSE;
	m_bAutoDeleteStart = FALSE;
	m_bAutoDeleteEnd = TRUE;
	m_bInspectEnd = TRUE;
	m_bIOCheck = FALSE;
	m_bIOEnd = TRUE;
	m_bIOCheck = FALSE;
	m_Trigger->CloseTrigger();
	m_VisMod->SaveSetupPara(m_ModelInfo.strModelPath);
	m_VisMod->Close();
	m_cSerial.Close();
	COMI_UnloadDll();
	Sleep(100);

	//delete m_pMainThread;
	//delete m_pIOThread;

	delete m_Trigger;
	delete m_VisMod;
	delete m_DlgRect;
	delete m_dlgSetup;

	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}

void CBearingInsDlg::ClickBtnenhctrlLive()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_Live.GetValue())
	{
		if(!m_btnStartStop.GetValue())
		{
			//SetTimer(0, 100, NULL);
			//m_DlgRect->SetTimer(0,100,NULL);
		}
		else
		{
			AfxMessageBox("검사 진행중 입니다. Stop 버튼누르고 다시 누르십시오.");
		}
	}
	else
	{
		KillTimer(0);
		//m_DlgRect->KillTimer(0);
	}
}


void CBearingInsDlg::ClickBtnenhctrlSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString tmpPath_Name;
	CFileDialog dlgFile(FALSE,"Image","*.bmp",
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"Bitmap Image(*.bmp)|*.bmp|");
	if(dlgFile.DoModal() == IDOK)
	{
		tmpPath_Name = dlgFile.GetPathName();
		MbufExport((char *)(LPCTSTR)tmpPath_Name, M_BMP, *m_VisMod->GetMilDispBuf(MAIN_DISP));
		//MbufCopy(*m_VisMod->GetMilGrabBuf(), *m_VisMod->GetMilDispBuf(MAIN_DISP));
	}

	delete dlgFile;	
}

void CBearingInsDlg::ClickBtnenhctrlLoad()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString tmpPath_Name;
	typeSetupPara setupPara;
	//m_VisMod->LoadSetupPara(m_ModelInfo.strModelPath);
	setupPara = m_VisMod->GetSetupPara();
	CFileDialog dlgFile(TRUE,"Image","*.bmp",
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"Bitmap Image(*.bmp)|*.bmp|");
	if(dlgFile.DoModal() == IDOK)
	{ 
		tmpPath_Name = dlgFile.GetPathName();
		MbufImport((char *)(LPCTSTR)tmpPath_Name, M_BMP, M_LOAD, *m_VisMod->GetMilSystem(), m_VisMod->GetMilGrabBuf());
		
		/*double p_time, s_time, e_time;
		CString temp;
		s_time = clock();
		double diameter = 0.;
		diameter = setupPara.ModelPara.dDiameter / 45.0;
		MimResize(*m_VisMod->GetMilGrabBuf(), *m_VisMod->GetMilDispBuf(MAIN_DISP), 1, diameter, M_DEFAULT );
		e_time = clock();
		p_time = e_time - s_time;
		temp.Format("%.5f ms", p_time);
		WriteStatusList(temp);*/

		MbufCopy(*m_VisMod->GetMilGrabBuf(), *m_VisMod->GetMilDispBuf(0));
	}

	delete dlgFile;	
}

void CBearingInsDlg::ClickBtnenhctrlZoomIn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	double win_left, win_top, win_right, win_bottom;
	double sx, sy, ex, ey, o_rat, rat, rrat;
	double cx, cy;
	BOOL over_disp = FALSE;;
	
	o_rat = m_dZoomRatio;
	m_dZoomRatio = m_dZoomRatio + 0.2;
	m_DlgRect->m_dZoomRatio = m_dZoomRatio;
	
	for(int i = 0; i < INS_RGN_MAX_NUM; i++)
	{
		m_DlgRect->m_Tracker[i].GetRect(&win_left, &win_top, &win_right, &win_bottom);	
		m_DlgRect->m_Tracker[i].GetZoom(&sx, &sy, &ex, &ey);
		rat = m_dZoomRatio / o_rat;
		rrat = (o_rat / m_dZoomRatio);

		//Tracker zoom///////////////////////////
		cx = (sx + ex) * 0.5;
		cy = (sy + ey) * 0.5;

		sx = sx - cx;
		sy = sy - cy;
		ex = ex - cx;
		ey = ey - cy;

		sx = sx * rrat + cx;
		sy = sy * rrat + cy;
		ex = ex * rrat + cx;
		ey = ey * rrat + cy;

		m_DlgRect->m_Tracker[i].SetZoom(sx, sy, ex, ey);

		//Tracker rect///////////////////////////
		cx = m_DlgRect->m_rectDlg.Width() * 0.5;
		cy = m_DlgRect->m_rectDlg.Height() * 0.5;

		win_left = win_left - cx;
		win_top = win_top - cy;
		win_right = win_right - cx;
		win_bottom = win_bottom - cy;

		win_left = win_left * rat + cx;
		win_top = win_top * rat + cy;
		win_right = win_right * rat + cx;
		win_bottom = win_bottom * rat + cy;

		m_DlgRect->m_Tracker[i].SetRect(win_left, win_top, win_right, win_bottom);
	}

	m_DlgRect->SetZoom(m_dZoomRatio, (long)(ex - sx), (long)(ey - sy));
	MdispZoom(*m_VisMod->GetMilDisplay(0), m_DlgRect->GetZoom(), m_DlgRect->GetZoom());
	MdispPan(*m_VisMod->GetMilDisplay(0), sx, sy);
	

	SCROLLINFO scrinfo;
	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_ALL;
	scrinfo.nMin = 0;
	scrinfo.nMax = 4500 - m_DlgRect->m_lImgHt;
	scrinfo.nPage = 1;
	scrinfo.nTrackPos = 1;
	scrinfo.nPos = (int)sy;
	m_ScrMain_V.SetScrollInfo(&scrinfo);

	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_ALL;
	scrinfo.nMin = 0;
	scrinfo.nMax = 1024 - m_DlgRect->m_lImgWd;
	scrinfo.nPage = 1;
	scrinfo.nTrackPos = 1;
	scrinfo.nPos = (int)sx;
	m_ScrMain_H.SetScrollInfo(&scrinfo);
	m_poScroll.x = (int)sx;
	m_poScroll.y = (int)sy;
}

void CBearingInsDlg::ClickBtnenhctrlZoomOut()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	double win_left, win_top, win_right, win_bottom;
	double sx, sy, ex, ey, o_rat, rat, rrat;
	double cx, cy;

	o_rat = m_dZoomRatio;
	m_dZoomRatio = m_dZoomRatio - 0.2;
	m_DlgRect->m_dZoomRatio = m_dZoomRatio;

	if(m_dZoomDefault > m_dZoomRatio)
	{
		m_dZoomRatio = m_dZoomDefault;
	}

	for(int i = 0; i < INS_RGN_MAX_NUM; i++)
	{
		m_DlgRect->m_Tracker[i].GetRect(&win_left, &win_top, &win_right, &win_bottom);	
		m_DlgRect->m_Tracker[i].GetZoom(&sx, &sy, &ex, &ey);
		rat = m_dZoomRatio / o_rat;
		rrat = (o_rat / m_dZoomRatio);

		//Tracker zoom///////////////////////////
		cx = (sx + ex) * 0.5;
		cy = (sy + ey) * 0.5;

		sx = sx - cx;
		sy = sy - cy;
		ex = ex - cx;
		ey = ey - cy;

		sx = sx * rrat + cx;
		sy = sy * rrat + cy;
		ex = ex * rrat + cx;
		ey = ey * rrat + cy;

		m_DlgRect->m_Tracker[i].SetZoom(sx, sy, ex, ey);

		//Tracker rect///////////////////////////
		cx = m_DlgRect->m_rectDlg.Width() * 0.5;
		cy = m_DlgRect->m_rectDlg.Height() * 0.5;

		win_left = win_left - cx;
		win_top = win_top - cy;
		win_right = win_right - cx;
		win_bottom = win_bottom - cy;

		win_left = win_left * rat + cx;
		win_top = win_top * rat + cy;
		win_right = win_right * rat + cx;
		win_bottom = win_bottom * rat + cy;

		m_DlgRect->m_Tracker[i].SetRect(win_left, win_top, win_right, win_bottom);
	}
	m_DlgRect->SetZoom(m_dZoomRatio, (long)(ex - sx), (long)(ey - sy));
	MdispZoom(*m_VisMod->GetMilDisplay(0), m_DlgRect->GetZoom(), m_DlgRect->GetZoom());

	MdispPan(*m_VisMod->GetMilDisplay(0), sx, sy);

	
	SCROLLINFO scrinfo;
	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_ALL;
	scrinfo.nMin = 0;
	if(m_dZoomDefault == m_dZoomRatio)
	{
		scrinfo.nMax = 0;
	}
	else
	{
		scrinfo.nMax = CAMERA_HEIGHT - (int)(ey - sy);
	}
	scrinfo.nPage = 1;
	scrinfo.nTrackPos = 1;
	scrinfo.nPos = (int)sy;
	m_ScrMain_V.SetScrollInfo(&scrinfo);

	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_ALL;
	scrinfo.nMin = 0;
	if(m_dZoomDefault == m_dZoomRatio)
	{
		scrinfo.nMax = 0;
	}
	else
	{
		scrinfo.nMax = CAMERA_WIDTH - (int)(ex - sx);
	}
	scrinfo.nPage = 1;
	scrinfo.nTrackPos = 1;
	scrinfo.nPos = (int)sx;
	m_ScrMain_H.SetScrollInfo(&scrinfo);
	m_poScroll.x = (int)sx;
	m_poScroll.y = (int)sy;
}

void CBearingInsDlg::ClickBtnenhctrlZoom11()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	double win_left, win_top, win_right, win_bottom;
	double sx, sy, ex, ey, o_rat, rat, rrat;
	double cx, cy;

	o_rat = m_dZoomRatio;
	m_dZoomRatio = 1.0;
	m_DlgRect->m_dZoomRatio = m_dZoomRatio;

	for(int i = 0; i < INS_RGN_MAX_NUM; i++)
	{
		m_DlgRect->m_Tracker[i].GetRect(&win_left, &win_top, &win_right, &win_bottom);	
		m_DlgRect->m_Tracker[i].GetZoom(&sx, &sy, &ex, &ey);
		rat = m_dZoomRatio / o_rat;
		rrat = (o_rat / m_dZoomRatio);

		//Tracker zoom///////////////////////////
		cx = (sx + ex) * 0.5;
		cy = (sy + ey) * 0.5;

		sx = sx - cx;
		sy = sy - cy;
		ex = ex - cx;
		ey = ey - cy;

		sx = sx * rrat + cx;
		sy = sy * rrat + cy;
		ex = ex * rrat + cx;
		ey = ey * rrat + cy;

		m_DlgRect->m_Tracker[i].SetZoom(sx, sy, ex, ey);

		//Tracker rect///////////////////////////
		cx = m_DlgRect->m_rectDlg.Width() * 0.5;
		cy = m_DlgRect->m_rectDlg.Height() * 0.5;

		win_left = win_left - cx;
		win_top = win_top - cy;
		win_right = win_right - cx;
		win_bottom = win_bottom - cy;

		win_left = win_left * rat + cx;
		win_top = win_top * rat + cy;
		win_right = win_right * rat + cx;
		win_bottom = win_bottom * rat + cy;

		m_DlgRect->m_Tracker[i].SetRect(win_left, win_top, win_right, win_bottom);
	}

	m_DlgRect->SetZoom(m_dZoomRatio, (long)(ex - sx), (long)(ey - sy));
	MdispZoom(*m_VisMod->GetMilDisplay(0), m_DlgRect->GetZoom(), m_DlgRect->GetZoom());
	

	MdispPan(*m_VisMod->GetMilDisplay(0), sx, sy);
	SCROLLINFO scrinfo;
	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_ALL;
	scrinfo.nMin = 0;
	scrinfo.nMax = 0;
	scrinfo.nPage = 1;
	scrinfo.nTrackPos = 1;
	scrinfo.nPos = 0;
	m_ScrMain_V.SetScrollInfo(&scrinfo);

	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_ALL;
	scrinfo.nMin = 0;
	scrinfo.nMax = 0;
	scrinfo.nPage = 1;
	scrinfo.nTrackPos = 1;
	scrinfo.nPos = 0;
	m_ScrMain_H.SetScrollInfo(&scrinfo);
	m_poScroll.x = 0;
	m_poScroll.y = 0;
}

void CBearingInsDlg::ClickBtnenhctrlZoomFit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	double win_left, win_top, win_right, win_bottom;
	double sx, sy, ex, ey, o_rat, rat, rrat;
	double cx, cy;

	o_rat = m_dZoomRatio;
	m_dZoomRatio = m_dZoomDefault;
	m_DlgRect->m_dZoomRatio = m_dZoomRatio;

	for(int i = 0; i < INS_RGN_MAX_NUM; i++)
	{
		m_DlgRect->m_Tracker[i].GetRect(&win_left, &win_top, &win_right, &win_bottom);	
		m_DlgRect->m_Tracker[i].GetZoom(&sx, &sy, &ex, &ey);
		rat = m_dZoomRatio / o_rat;
		rrat = (o_rat / m_dZoomRatio);

		//Tracker zoom///////////////////////////
		cx = (sx + ex) * 0.5;
		cy = (sy + ey) * 0.5;

		sx = sx - cx;
		sy = sy - cy;
		ex = ex - cx;
		ey = ey - cy;

		sx = sx * rrat + cx;
		sy = sy * rrat + cy;
		ex = ex * rrat + cx;
		ey = ey * rrat + cy;

		m_DlgRect->m_Tracker[i].SetZoom(sx, sy, ex, ey);

		//Tracker rect///////////////////////////
		cx = m_DlgRect->m_rectDlg.Width() * 0.5;
		cy = m_DlgRect->m_rectDlg.Height() * 0.5;

		win_left = win_left - cx;
		win_top = win_top - cy;
		win_right = win_right - cx;
		win_bottom = win_bottom - cy;

		win_left = win_left * rat + cx;
		win_top = win_top * rat + cy;
		win_right = win_right * rat + cx;
		win_bottom = win_bottom * rat + cy;

		m_DlgRect->m_Tracker[i].SetRect(win_left, win_top, win_right, win_bottom);
	}

	m_DlgRect->SetZoom(m_dZoomRatio, (long)(ex - sx), (long)(ey - sy));
	MdispZoom(*m_VisMod->GetMilDisplay(0), m_DlgRect->GetZoom(), m_DlgRect->GetZoom());
	

	MdispPan(*m_VisMod->GetMilDisplay(0), sx, sy);
	SCROLLINFO scrinfo;
	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_ALL;
	scrinfo.nMin = 0;
	scrinfo.nMax = 0;
	scrinfo.nPage = 1;
	scrinfo.nTrackPos = 1;
	scrinfo.nPos = 0;
	m_ScrMain_V.SetScrollInfo(&scrinfo);

	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_ALL;
	scrinfo.nMin = 0;
	scrinfo.nMax = 0;
	scrinfo.nPage = 1;
	scrinfo.nTrackPos = 1;
	scrinfo.nPos = 0;
	m_ScrMain_H.SetScrollInfo(&scrinfo);
	m_poScroll.x = 0;
	m_poScroll.y = 0;
}

void CBearingInsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SCROLLINFO scrinfo;
	int i;
	CRect rect;
	double diff_x, diff_y, diff_img_x, diff_img_y;

	double win_left, win_top, win_right, win_bottom;
	double sx, sy, ex, ey;
	CRect img_rect, win_rect;
	double ratio_x, ratio_y;
	double sx0, sy0, ex0, ey0;

	if(pScrollBar)
	{
		if(pScrollBar == (CScrollBar*)&m_ScrMain_H)
		{
			if(pScrollBar->GetScrollInfo(&scrinfo))
			{
				if(scrinfo.nMax > 0)
				{
					switch(nSBCode)
					{
						case SB_PAGEUP:
						case SB_LINEUP:
							scrinfo.nPos -= scrinfo.nPage;
							break;
						case SB_PAGEDOWN:
						case SB_LINEDOWN:
							scrinfo.nPos += scrinfo.nPage;
							break;
						case SB_THUMBPOSITION:
						case SB_THUMBTRACK:
							scrinfo.nPos = scrinfo.nTrackPos;
							break;
					}

					pScrollBar->SetScrollPos(scrinfo.nPos);
					m_poScroll.x = scrinfo.nPos;

					m_DlgRect->m_Tracker[0].GetZoom(&sx, &sy, &ex, &ey);

					diff_x = sx - m_DlgRect->m_lPanSX;
					diff_y = sy - m_DlgRect->m_lPanSY;

					m_DlgRect->m_lPanEX = (long)sx;
					m_DlgRect->m_lPanEY = (long)sy;

					m_DlgRect->m_lPanSX = m_DlgRect->m_lPanEX;
					m_DlgRect->m_lPanSY = m_DlgRect->m_lPanEY;
					
					for(i = 0; i < INS_RGN_MAX_NUM; i++)
					{
						m_DlgRect->m_Tracker[i].GetZoom(&sx, &sy, &ex, &ey);
						m_DlgRect->m_Tracker[i].GetRect(&win_left, &win_top, &win_right, &win_bottom);	

						//Zoom///////////////////////////

						img_rect = CRect((int)sx, (int)sy, (int)ex, (int)ey);
					

						ratio_y = ((double)img_rect.Height()) / ((double)969);
						ratio_x = ((double)img_rect.Width()) / ((double)199);

						diff_img_x = diff_x * ratio_x;
						diff_img_y = diff_y * ratio_y;

						sx0 = sx - diff_img_x;
						if(0 > sx0)
						{
							sx0 = 0;
							diff_img_x = sx;
							m_DlgRect->m_bPanDN = FALSE;
						}

						sy0 = sy - diff_img_y;
						if(0 > sy0)
						{
							sy0 = 0;
							diff_img_y = sy;
							m_DlgRect->m_bPanDN = FALSE;
						}

						ex0 = ex - diff_img_x;
						if(CAMERA_WIDTH < ex0)
						{
							ex0 = CAMERA_WIDTH;
							diff_img_x = ex - CAMERA_WIDTH;
							m_DlgRect->m_bPanDN = FALSE;
						}

						ey0 = ey - diff_img_y;
						if(CAMERA_HEIGHT < ey0)
						{
							ey0 = CAMERA_HEIGHT;
							diff_img_y = ey - CAMERA_HEIGHT;
							m_DlgRect->m_bPanDN = FALSE;
						}

					
						m_DlgRect->m_Tracker[i].SetZoom(sx - diff_img_x, sy - diff_img_y, ex - diff_img_x, ey - diff_img_y);

						//Rect///////////////////////////
						diff_x = diff_img_x / ratio_x;
						diff_y = diff_img_y / ratio_y;

						m_DlgRect->m_Tracker[i].SetRect(win_left + diff_x, win_top + diff_y, win_right + diff_x, win_bottom + diff_y);
					}
					
					MdispPan(*m_VisMod->GetMilDisplay(0), sx - diff_img_x, sy - diff_img_y);
					m_ScrMain_H.SetScrollPos((int)(sx - diff_img_x));
					m_ScrMain_V.SetScrollPos((int)(sy - diff_img_y));
					
					MdispPan(*m_VisMod->GetMilDisplay(0), m_poScroll.x, m_poScroll.y);
				}
			}
		}
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CBearingInsDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SCROLLINFO scrinfo;

	if(pScrollBar)
	{
		if(pScrollBar == (CScrollBar*)&m_ScrMain_V)
		{
			if(pScrollBar->GetScrollInfo(&scrinfo))
			{
				if(scrinfo.nMax > 0)
				{
					switch(nSBCode)
					{
						case SB_PAGEUP:
						case SB_LINEUP:
							scrinfo.nPos -= scrinfo.nPage;
							break;
						case SB_PAGEDOWN:
						case SB_LINEDOWN:
							scrinfo.nPos += scrinfo.nPage;
							break;
						case SB_THUMBPOSITION:
						case SB_THUMBTRACK:
							scrinfo.nPos = scrinfo.nTrackPos;
							break;
					}

					pScrollBar->SetScrollPos(scrinfo.nPos);
					m_poScroll.y = scrinfo.nPos;
					
					MdispPan(*m_VisMod->GetMilDisplay(0), m_poScroll.x, m_poScroll.y);
				}
			}
		}
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

int CBearingInsDlg::HandleAllMessage()
{
	int returnValue;
	MSG Mess;

	do {
		returnValue=::PeekMessage(&Mess, NULL, 0, 0, PM_REMOVE);
		if (returnValue)
		{
			::TranslateMessage(&Mess);
			::DispatchMessage (&Mess);
		}
	} while(returnValue);
	
	return returnValue;
}

void CBearingInsDlg::UpdateLight(BOOL isUpdate)
{

}

void CBearingInsDlg::WriteStatusList(LPCTSTR lpszItem)
{
	//Log
	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);
	char path[MAX_PATH];
	char temp[DAT_STR_LENG];
	char name[DAT_STR_LENG];
	CString Dir;

	memset(temp, NULL, DAT_STR_LENG);
	sprintf_s(temp, DAT_STR_LENG, "[%02d:%02d:%02d]%s", lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpszItem);


	memset(name, NULL, sizeof(temp));
	//sprintf(name, "System_Log_%04d%02d%02d.txt", lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	sprintf_s(name, 24,"System_Log_%04d%02d%02d.txt", lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);

	m_list_rslt.InsertItem(0, temp);

	if(LIST_MONI_NUM <= m_list_rslt.GetItemCount())
	{
		m_list_rslt.DeleteItem(LIST_MONI_NUM - 1);
	}

	memset(path, NULL, MAX_PATH);
	//sprintf(path, "%s\\%04d%02d%02d\\SYSTEM_LOG\\", m_strLogPath, lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
	sprintf_s(path, m_strLogPath.GetLength() + 22,"%s\\%04d%02d%02d\\SYSTEM_LOG\\", m_strLogPath, lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);

	Dir.Format("%s", path);
	MakeDirectories(Dir);

	WriteLogStatus(path, name, "System_Log.bak", temp, 0);
}

BOOL CBearingInsDlg::WriteLogStatus(CString path, CString file, CString bak_file, CString log, long limit_line)
{
	// check message level
	FILE*	stream;
	char	strFile[MAX_PATH], strToFile[MAX_PATH];
    long	result, line;
	char	szTime[DAT_STR_LENG]; 

	char buffer[DAT_STR_LENG];

	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);

	//sprintf(strFile, "%s\\%s", path, file); 
	sprintf_s(strFile, path.GetLength() + file.GetLength() + 2, "%s\\%s", path, file);

	//if((stream = fopen( strFile, "a+" )) == NULL)
	if((stream = _fsopen(strFile, "a+", _SH_DENYNO)) == NULL)
	{
		HANDLE fd = CreateFile( strFile,
							GENERIC_READ|GENERIC_WRITE,
							//GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
		//stream = fopen( strFile, "a+" );
		stream = _fsopen(strFile, "a+", _SH_DENYNO);
		if(stream == NULL)
		{
			//20150206 by IMSI
			//AfxMessageBox("Error : log file open fail!(WriteLogStatus)" );
			CloseHandle(fd);
			return FALSE;
		}

		CloseHandle(fd);
	}

	//Check first time
    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}
	line = 0;
	while(fgets(buffer, DAT_STR_LENG, stream)) 
	{
		line++;
		if(1 < line)
		{
			break;
		}
	}

    result = fseek(stream, 0L, SEEK_END);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	memset(szTime, NULL, sizeof(szTime));
	//sprintf(szTime, "%s", log);
	sprintf_s(szTime, log.GetLength() + 1, "%s", log);
    fputs(szTime, stream);
	fputs("\n", stream);

    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	//Check limit line
	line = 0;
	if(limit_line)
	{
		while(fgets(buffer, DAT_STR_LENG, stream)) line++;
		if(line > limit_line)
		{
			//sprintf(strToFile, "%s\\%s", path, bak_file); 
			sprintf_s(strToFile, path.GetLength() + bak_file.GetLength() + 2, "%s\\%s", path, bak_file); 
			CopyFile(strFile, strToFile, FALSE);
			fclose(stream);
			DeleteFile(strFile);
		}
		else
		{
			fclose(stream);
		}
	}
	else
	{
		fclose(stream);
	}

	return TRUE;
}
void CBearingInsDlg::ClickBtnenhctrlModel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int idx = 0;
 	CRect rect;
 	char temp[MAX_MOD_NAME];
 	CString str0, str1, temp1;	
 	CString Dir;
 
 	if(m_btnStartStop.GetValue())
 	{
 		MessageBox("Error : 현재 검사 진행 중입니다! 검사 정지 후 재 시도 바랍니다.", NULL, MB_ICONEXCLAMATION);
 		return;
 	}
 
 	//Close setup dialog
 	m_dlgSetup->ShowWindow(SW_HIDE);
 
 	CDialogModel dlg;
 
 	GetDlgItem(IDC_BTNENHCTRL_MODEL)->GetWindowRect(rect);
 	dlg.m_Rect = rect;
 
 	idx = (int)dlg.DoModal();
 	if(idx == 3) return; 
 
 	idx = dlg.m_Ret;
 
 	//Model
 	C_CtrlModelManageDlg *moddlg = new C_CtrlModelManageDlg();
	
 	switch(idx)
 	{
 	//New
 	case 0:
 		moddlg->DoModal(idx);
 		WriteStatusList("MODEL CHANGE-NEW");
 		//Make directory
 		Dir.Format("%s", moddlg->GetPath());
 		MakeDirectories(Dir);
 		//MakeDefault Param
 		break;
 	//Delete
 	case 1:
 		if(moddlg->DoModal(idx))
 		{
 			WriteStatusList("MODEL CHANGE-DELETE");
 		}
 		break;
 	//Select
 	case 2:
 		//Model save
 		moddlg->Open();
 		//sprintf(temp, "%s", moddlg->GetName());
		temp1.Format("%s", moddlg->GetName());
		sprintf_s(temp, temp1.GetLength() + 1, "%s", temp1);
 
 		str0 = moddlg->GetName();
 		str1 = moddlg->GetPath();
 		if( (str0.IsEmpty()) || (str1.IsEmpty()) )
 		{
 			WriteStatusList("Error : invalid model!");
 		}
 		else
 		{
 			m_dlgProcessing.ShowWindow(SW_SHOW);
 			HandleAllMessage();
 
 			//no need
 			UpdateLight(TRUE);

 			m_dlgProcessing.ShowWindow(SW_HIDE);
 		}
 
 
 		if(moddlg->DoModal(idx))
 		{
 			WriteStatusList("MODEL CHANGE-SELECT");
 		}
 
 		m_dlgProcessing.ShowWindow(SW_SHOW);
 		HandleAllMessage();
 
 		//Load setup parameter
 		moddlg->Open();
 		
 		str0 = moddlg->GetName();
 		str1 = moddlg->GetPath();
 		if( (str0.IsEmpty()) || (str1.IsEmpty()) )
 		{
 			//MessageBox("Error : invalid model!", NULL, MB_ICONEXCLAMATION);
 			WriteStatusList("Error : invalid model!");
 		
 		}
 		else
 		{
 			//모델 로딩 이므로 기존 등록 모델 삭제 한다.?
 			//clear_all_variation_models(); //20110508 lee
			m_ModelInfo.strModelName.Format("%s", moddlg->GetName());
			m_ModelInfo.strModelPath.Format("%s", moddlg->GetPath());
			m_ModelInfo.strModelDesc.Format("%s", moddlg->GetDesc());
			m_VisMod->LoadSetupPara(m_ModelInfo.strModelPath);
 			//Load setup INI file
 		}

		m_dlgProcessing.ShowWindow(SW_HIDE);
 
 		break;
 	//Convertion
 	case 3:
 		//moddlg->DoModal(idx);
 	
 		break;
 	default:
 		
 		break;
 	}
	
	moddlg->AllModelLoad();
	m_AllModel.nMaxModelCnt = moddlg->GetMaxModel();
	for(int i = 0; i < m_AllModel.nMaxModelCnt; i++)
	{
		m_AllModel.Model[i] = moddlg->GetModelData(i);
	}
	
	//모델 정보 표시
	m_ModelInfo.strModelName = moddlg->GetName();
	ModelInfoLoad(FALSE);
 	delete moddlg;
}

void CBearingInsDlg::ModelInfoLoad(BOOL isParaLoad)
{
	CString temp;
	C_CtrlModelManageDlg *moddlg = new C_CtrlModelManageDlg();
	moddlg->Open();
	m_ModelInfo.strModelName.Format("%s", moddlg->GetName());
	m_ModelInfo.strModelPath.Format("%s", moddlg->GetPath());
	m_ModelInfo.strModelDesc.Format("%s", moddlg->GetDesc());

	moddlg->AllModelLoad();
	m_AllModel.nMaxModelCnt = moddlg->GetMaxModel();
	for(int i = 0; i < m_AllModel.nMaxModelCnt; i++)
	{
		m_AllModel.Model[i] = moddlg->GetModelData(i);
	}

	if(m_VisMod->LoadSetupPara(m_ModelInfo.strModelPath))
	{
		//파라메터 로드
		
		ModelInfoView();
	}
	else
	{
		::AfxMessageBox("Parameter load Fail");
		return;
	}

	ModelInfoView();
	delete moddlg;
}

void CBearingInsDlg::ModelInfoView()
{
	CString temp;
	typeSetupPara setupPara;
	setupPara = m_VisMod->GetSetupPara();
	temp.Format("Model Name : %s", m_ModelInfo.strModelName);
 	m_label_Model_Name.SetCaption(temp);
	temp.Empty();
	temp.Format("Model Width : %.2fmm", setupPara.ModelPara.dModelWidth);
	m_label_Model_Width.SetCaption(temp);
	temp.Empty();
	temp.Format("Model Height : %.2fmm", setupPara.ModelPara.dModelHeight);
	m_label_Model_Height.SetCaption(temp);
	temp.Empty();
	temp.Format("Model Bearing Cnt : %d x %d", setupPara.ModelPara.nCol, setupPara.ModelPara.nModelCnt);
	m_label_Model_Cnt.SetCaption(temp);
	temp.Empty();
	temp.Format("Model Diameter : %.2fmm", setupPara.ModelPara.dDiameter);
	m_label_Model_PI.SetCaption(temp);
	temp.Empty();
}

void CBearingInsDlg::ClickBtnenhctrlSetup()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_dlgSetup->ShowWindow(SW_SHOW);
}

void CBearingInsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch((int)nIDEvent)
	{
	case 0:
		m_VisMod->Grab();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CBearingInsDlg::InspectionProcess()
{
	BOOL rslt = TRUE;
	CString temp;
	typeSetupPara setupPara;
	m_VisMod->LoadSetupPara(m_ModelInfo.strModelPath);
	setupPara = m_VisMod->GetSetupPara();

	return rslt;
}

int CBearingInsDlg::InspectionMeas(BYTE *ins_image, CPoint center)
{
	int length = 0;
	int rslt = 1;
	
	typeSetupPara setupPara;
	setupPara = m_VisMod->GetSetupPara();
	CRect rect((int)(center.x - (setupPara.InspPara.InsRegion[4].Width() / 2)),
				(int)(center.y - (setupPara.InspPara.InsRegion[4].Height() / 2) - setupPara.InspPara.InsCenterPointOffSet.y),
				(int)(center.x + (setupPara.InspPara.InsRegion[4].Width() / 2)),
				(int)(center.y + (setupPara.InspPara.InsRegion[4].Height() / 2) - setupPara.InspPara.InsCenterPointOffSet.y));

	MgraColor(M_DEFAULT, M_COLOR_GREEN);
	MgraRect(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.left, rect.top, rect.right, rect.bottom);
	MgraRect(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.left + 1, rect.top + 1, rect.right + 1, rect.bottom + 1);

	

	int sumBright = 0;
	int avgBright = 0;
	long long pitch;
	register int col, row;
	CRect _rect = rect;
	typeSetupPara _setupPara;
	_setupPara = setupPara;
	MbufInquire(*m_VisMod->GetMilDispBuf(MAIN_DISP), M_PITCH, &pitch);
	for(col = _rect.left; col < _rect.right; col =  col + 16)
	{
		sumBright = 0;
		for(row = _rect.top; row < _rect.bottom; row++)
		{
			sumBright +=ins_image[(row * (pitch)) + col];
		}
		avgBright = (sumBright / rect.Height());
		if(avgBright <= _setupPara.InspPara.dInspBright)
		{
			length = length + 16;
		}
	}

	CString temp;
	if(length >= setupPara.InspPara.dInspBrokenLength)
	{
		MgraColor(M_DEFAULT, M_COLOR_RED);
		rslt = 0;
	}
	else
	{
		MgraColor(M_DEFAULT, M_COLOR_YELLOW);
	}

	temp.Format("Broken %.2f mm", (length * setupPara.dPixelSize));
	MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.left, rect.top, MIL_TEXT(temp));

	return rslt;
}

int CBearingInsDlg::InspectionBlob()
{
	int needle_idx = 0;
	int ng_cnt = 0;
	int rslt = 0;
	typeSetupPara setupPara;
	int ins_idx = ALIGN_SEARCH_REGION_1;
	MIL_ID MilBinImage, MilBlobResult, MilBlobFeatureList;
	MIL_ID image, MilTransImage, MilChildImage;
	MIL_INT TotalBlobs;
	double *Area;//, *CogY, *CogX, *Width, *Height;  
	//double area;
	CString strLog, temp;
	//int i, j;
	double p_time, s_time, e_time;
	double diameter = 0.;
	double high_thresh = 0.;
	
	setupPara = m_VisMod->GetSetupPara();
	diameter = setupPara.ModelPara.dDiameter / 45.0;
	s_time = clock();
	//MdispControl(*m_VisMod->GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);

	MbufAlloc2d(*m_VisMod->GetMilSystem(), CAMERA_WIDTH, MAX_CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image);
	MbufCopy(*m_VisMod->GetMilDispBuf(MAIN_DISP), image);	
	MbufAlloc2d(*m_VisMod->GetMilSystem(), CAMERA_WIDTH, MAX_CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage);
	high_thresh = setupPara.InspPara.dInspEdgeThreshHeigh;

	for(ins_idx = ALIGN_SEARCH_REGION_1; ins_idx < ALIGN_SEARCH_REGION_1 + setupPara.ModelPara.nCol; ins_idx++)
	{
		for(needle_idx = 0; needle_idx < setupPara.ModelPara.nModelCnt; needle_idx++)
		{
			
			CRect rect((int)(setupPara.InspPara.InsCenterPoint[0].x - (setupPara.InspPara.dInspWidth[0] / 2)),
						(int)(setupPara.InspPara.InsCenterPoint[needle_idx].y - (setupPara.InspPara.InsRegion[4].Height() / 2) - setupPara.InspPara.InsCenterPointOffSet.y),
						(int)(setupPara.InspPara.InsCenterPoint[0].x + (setupPara.InspPara.dInspWidth[0] / 2)),
						(int)(setupPara.InspPara.InsCenterPoint[needle_idx].y + (setupPara.InspPara.InsRegion[4].Height() / 2) - setupPara.InspPara.InsCenterPointOffSet.y));
			/*CRect rect((int)(setupPara.InspPara.InsCenterPoint[0].x - (setupPara.InspPara.InsRegion[4].Width() / 2)),
						(int)(setupPara.InspPara.InsCenterPoint[needle_idx].y - (setupPara.InspPara.InsRegion[4].Height() / 2) - setupPara.InspPara.InsCenterPointOffSet.y),
						(int)(setupPara.InspPara.InsCenterPoint[0].x + (setupPara.InspPara.InsRegion[4].Width() / 2)),
						(int)(setupPara.InspPara.InsCenterPoint[needle_idx].y + (setupPara.InspPara.InsRegion[4].Height() / 2) - setupPara.InspPara.InsCenterPointOffSet.y));*/
			MbufClear(MilTransImage, 0x00);

			MbufAlloc2d(*m_VisMod->GetMilSystem(), rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
			MbufClear(MilBinImage, 0x00);

			MbufChild2d(image, rect.left, rect.top, rect.Width(), rect.Height(), &MilChildImage);

			MimBinarize(MilChildImage, MilBinImage, M_GREATER_OR_EQUAL, high_thresh, M_NULL);

			//Transform
			MimTranslate(MilBinImage, MilTransImage, rect.left, rect.top, M_DEFAULT);

			/* Allocate a blob result buffer. */
			MblobAllocResult(*m_VisMod->GetMilSystem(), &MilBlobResult); 

			/* Allocate a feature list. */ 
			MblobAllocFeatureList(*m_VisMod->GetMilSystem(), &MilBlobFeatureList); 

			MblobSelectFeature(MilBlobFeatureList, M_AREA); 

			MblobCalculate(MilTransImage, M_NULL, MilBlobFeatureList, MilBlobResult); 

			/* Get the total number of selected blobs. */ 
			MblobGetNumber(MilBlobResult, &TotalBlobs); 

			BOOL first = FALSE;
			int first_idx = 0;
			int Cog_idx = 0;
			double add_x = 0., avg_x = 0.;
			int x_idx = 0;
		
			CRect search(rect);
			int search_width = search.Width();
			int search_height = search.Height();
			double area = 0.;
			int r = 255, g = 255, b = 0;
			/* Get the total number of selected blobs. */ 
			MblobGetNumber(MilBlobResult, &TotalBlobs); 

			if (Area = (double *)malloc(TotalBlobs*sizeof(double)))
			{ 
				MblobGetResult(MilBlobResult, M_AREA, Area); 

				for(int i = 0; i < TotalBlobs; i++)
				{
					area+=Area[i];
				}

				MgraColor(M_DEFAULT, M_RGB888(r, g, b));

				temp.Format("Area : %.0f  Blobs Cnt : %d", area, TotalBlobs);
				MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.left, rect.bottom, MIL_TEXT(temp));
				
				free(Area);	
			}
			else
			{
				MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
				MgraColor(M_DEFAULT, M_COLOR_RED);
				MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.right, rect.top - 20, MIL_TEXT("Teaching fail...!"));
			}

			

			MblobDraw(M_DEFAULT, MilBlobResult, *m_VisMod->GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);

			MblobFree(MilBlobResult); 
			MblobFree(MilBlobFeatureList); 
			MbufFree(MilChildImage);
			MbufFree(MilBinImage);
		}
	}

	e_time = clock();
	p_time = (double)((e_time - s_time));
	strLog.Format("Blob Inspection Time : %.5f ms", p_time);
	WriteStatusList(strLog);

	MbufFree(MilTransImage);
	MbufFree(image);

	return rslt;
}

void CBearingInsDlg::BirImage(BYTE *byte_source, int width, int height, int pitch)
{
	register int i, j;
	typeSetupPara setupPara;
	setupPara = m_VisMod->GetSetupPara();

	for(j = 0; j < height; j++)
	{
		for(i = 0; i < width; i++)
		{
			if(byte_source[j + (i * pitch)] >= setupPara.InspPara.dInspThreshold)
			{
				byte_source[j + (i * pitch)] = 255;
			}
			else
			{
				byte_source[j + (i * pitch)] = 0;
			}
		}
	}
}

void CBearingInsDlg::MorphologyErosion(BYTE *byte_source, int left, int top, int width, int height)
{
	register int i, j;
	long long pitch;
	MIL_ID buf;
	MbufAlloc2d(*m_VisMod->GetMilSystem(), width, height, 8+M_UNSIGNED, M_IMAGE+M_PROC,  &buf);
	MbufPut(buf, byte_source);
	MbufInquire(buf, M_PITCH, &pitch);
	BirImage(byte_source, width, height, pitch);
	MbufPut(buf, byte_source);
	MbufExport(MIL_TEXT("D:\\a3.bmp"), M_BMP, buf);
	
	for(j = top; j < height - 1; j++)
	{
		for(i = left; i < width - 1; i++)
		{
			if((byte_source[j - 1 + ((i - 1) * pitch)] == 0) || (byte_source[j - 1 + ((i) * pitch)] == 0) || (byte_source[j - 1 + ((i + 1) * pitch)] == 0) ||
				(byte_source[j + ((i - 1) * pitch)] == 0) || (byte_source[j + ((i + 1) * pitch)] == 0) || (byte_source[j + 1 + ((i - 1) * pitch)] == 0) ||
				(byte_source[j + 1 + ((i) * pitch)] == 0) || (byte_source[j + 1 + ((i + 1) * pitch)] == 0))
			{
				byte_source[j + (i * pitch)] = 0;
			}
		}
	}
}

void CBearingInsDlg::MorphologyDilation(BYTE *byte_source, int left, int top, int width, int height)
{
	register int i, j;
	long long pitch;
	MIL_ID buf;
	MbufAlloc2d(*m_VisMod->GetMilSystem(), CAMERA_WIDTH, MAX_CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC,  &buf);
	MbufPut2d(buf, 0, 0, CAMERA_WIDTH, MAX_CAMERA_HEIGHT, byte_source);
	MbufInquire(buf, M_PITCH, &pitch);
	//BirImage(byte_source, MAX_CAMERA_WIDTH, MAX_CAMERA_HEIGHT, pitch);
	//MbufPut(buf, byte_source);
	MbufExport(MIL_TEXT("D:\\a1.bmp"), M_BMP, buf);

	if(top < 1)
	{
		top = 1;
	}

	for(j = top; j < height - 1; j++)
	{
		for(i = left; i < width - 1; i++)
		{
			if((byte_source[j - 1 + ((i - 1) * pitch)] == 255) || (byte_source[j - 1 + ((i) * pitch)] == 255) || (byte_source[j - 1 + ((i + 1) * pitch)] == 255) ||
				(byte_source[j + ((i - 1) * pitch)] == 255) || (byte_source[j + ((i + 1) * pitch)] == 255) || (byte_source[j + 1 + ((i - 1) * pitch)] == 255) ||
				(byte_source[j + 1 + ((i) * pitch)] == 255) || (byte_source[j + 1 + ((i + 1) * pitch)] == 255))
			{
				byte_source[j + (i * pitch)] = 255;
			}
		}
	}
	MbufPut2d(buf, 0, 0, CAMERA_WIDTH, MAX_CAMERA_HEIGHT, byte_source);
	MbufExport(MIL_TEXT("D:\\a2.bmp"), M_BMP, buf);
}

int CBearingInsDlg::Inspection()
{
	BYTE *by_source;//, *byte_source;
	int needle_idx = 0;
	int ng_cnt = 0;
	int rslt = 0;
	typeSetupPara setupPara;
	int ins_idx = ALIGN_SEARCH_REGION_1;
	MIL_ID MilBinImage, MilBlobResult, MilBlobFeatureList;
	MIL_ID image, MilTransImage, MilChildImage;
	MIL_INT TotalBlobs;
	double *CogX, *CogY, *Area, *Width;  
	double area;
	CString strLog, temp;
	int i, j;
	double p_time, s_time, e_time;
	double p_time1, s_time1, e_time1;
	double diameter = 0.;
	
	setupPara = m_VisMod->GetSetupPara();
	diameter = setupPara.ModelPara.dDiameter / 45.0;
	s_time = clock();
	MdispControl(*m_VisMod->GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);

	MbufAlloc2d(*m_VisMod->GetMilSystem(), CAMERA_WIDTH, MAX_CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image);
	MbufCopy(*m_VisMod->GetMilDispBuf(MAIN_DISP), image);	
	MbufAlloc2d(*m_VisMod->GetMilSystem(), CAMERA_WIDTH, MAX_CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilTransImage);


	MIL_ID im,r,g,b;
	MbufAllocColor(*m_VisMod->GetMilSystem(), 3, 4864, 3232, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &im);
	MbufChildColor(im, M_RED, &r);
	MbufChildColor(im, M_GREEN, &g);
	MbufChildColor(im, M_BLUE, &b);

	MbufImport(_T("D:\\Project\\LGD\\CICB\\CIC\\CIC_Data\\GrabImages\\g16.bmp"), M_BMP, M_LOAD, *m_VisMod->GetMilSystem(), &im);

	
	BYTE *ir,*ig,*ib;
	//MbufCopy(r, image);
	MbufInquire(r, M_HOST_ADDRESS, &ir);
	//MbufCopy(g, image);
	MbufInquire(g, M_HOST_ADDRESS, &ig);
	//MbufCopy(b, image);
	MbufInquire(b, M_HOST_ADDRESS, &ib);

	for(ins_idx = ALIGN_SEARCH_REGION_1; ins_idx < ALIGN_SEARCH_REGION_1 + setupPara.ModelPara.nCol; ins_idx++)
	{
		CRect rect(setupPara.InspPara.InsRegion[ins_idx].left - 10,
					0,
					setupPara.InspPara.InsRegion[ins_idx].right + 10,
					(int)(CAMERA_HEIGHT * diameter));
		MbufClear(MilTransImage, 0x00);

		MbufAlloc2d(*m_VisMod->GetMilSystem(), rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);
		MbufClear(MilBinImage, 0x00);

		MbufChild2d(image, rect.left, rect.top, rect.Width(), rect.Height(), &MilChildImage);

		MimBinarize(MilChildImage, MilBinImage, M_GREATER_OR_EQUAL, setupPara.InspPara.dInspThreshold, M_NULL);

		//Transform
		MimTranslate(MilBinImage, MilTransImage, rect.left, rect.top, M_DEFAULT);

		/* Allocate a blob result buffer. */
		MblobAllocResult(*m_VisMod->GetMilSystem(), &MilBlobResult); 

		/* Allocate a feature list. */ 
		MblobAllocFeatureList(*m_VisMod->GetMilSystem(), &MilBlobFeatureList); 

		MblobSelectFeature(MilBlobFeatureList, M_AREA); 
		MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY); 
		MblobSelectFeature(MilBlobFeatureList, M_FERET_X); 
		MblobSelectFeature(MilBlobFeatureList, M_FERET_Y);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MAX);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MAX);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_X_MIN);
		MblobSelectFeature(MilBlobFeatureList, M_BOX_Y_MIN);

		//MbufExport(MIL_TEXT("D:\\test1.bmp"), M_BMP, MilTransImage);
		if(setupPara.InspPara.dInspSmooth == 1)
		{
			s_time1 = clock();
			MimDilate(MilTransImage, MilTransImage, 1, M_BINARY);
			MimErode(MilTransImage, MilTransImage, 1, M_BINARY);
			e_time1 = clock();
			p_time1 = (double)((e_time1 - s_time1));
			strLog.Format("Mol Time : %.5f ms", p_time1);
			WriteStatusList(strLog);
			//MbufExport(MIL_TEXT("D:\\test2.bmp"), M_BMP, MilTransImage);
		}
		/* Calculate selected features for each blob. */ 
		MblobCalculate(MilTransImage, M_NULL, MilBlobFeatureList, MilBlobResult); 

		CRect rect0(setupPara.InspPara.InsRegion[ALIGN_REGION].left,
						setupPara.InspPara.InsRegion[ALIGN_REGION].top,
						setupPara.InspPara.InsRegion[ALIGN_REGION].right,
						setupPara.InspPara.InsRegion[ALIGN_REGION].bottom);

		double limt_area, limt_width, limt_height, limt_width_max, /*limt_height_max,*/ first_x, first_y;

		limt_width = rect0.Width() * 0.9;
		limt_height = rect0.Height() * 0.3;
		if(limt_height < 1)
		{
			limt_height = 1;
		}
		limt_width_max = rect0.Width() / 0.8;
		//limt_height_max = (setupPara.ModelPara.dModelHeight * 0.5) + setupPara.ModelPara.dModelHeight;
		//limt_height_max = setupPara.ModelPara.dModelHeight / 0.1;
		limt_area = (rect0.Width() * rect0.Height()) * 0.8;
		first_x = setupPara.InspPara.dAlignX[ins_idx - ALIGN_SEARCH_REGION_1];
		first_y = setupPara.InspPara.dAlignY[ins_idx - ALIGN_SEARCH_REGION_1];

		/* Exclude blobs whose area is too small. */ 
		MblobSelect(MilBlobResult, M_EXCLUDE, M_AREA, M_LESS_OR_EQUAL, limt_area, M_NULL); 
		MblobSelect(MilBlobResult, M_EXCLUDE, M_FERET_X, M_LESS_OR_EQUAL, limt_width, M_NULL); 
		MblobSelect(MilBlobResult, M_EXCLUDE, M_FERET_Y, M_LESS_OR_EQUAL, limt_height, M_NULL); 
		//MblobSelect(MilBlobResult, M_EXCLUDE, M_FERET_X, M_GREATER_OR_EQUAL, limt_width_max, M_NULL); 
		//MblobSelect(MilBlobResult, M_EXCLUDE, M_FERET_Y, M_GREATER_OR_EQUAL, limt_height_max, M_NULL); 

		/* Get the total number of selected blobs. */ 
		MblobGetNumber(MilBlobResult, &TotalBlobs); 

		BOOL first = FALSE;
		int first_idx = 0;
		int Cog_idx = 0;
		double add_x = 0., avg_x = 0.;
		int x_idx = 0;
	
		CRect search(setupPara.InspPara.InsRegion[ins_idx].left,
						setupPara.InspPara.InsRegion[ins_idx].top,
						setupPara.InspPara.InsRegion[ins_idx].right,
						setupPara.InspPara.InsRegion[ins_idx].bottom);
		int search_width = search.Width();
		int search_height = search.Height();

		if ((CogX = (double *)malloc(TotalBlobs*sizeof(double))) && 
		(CogY = (double *)malloc(TotalBlobs*sizeof(double))) && 
		(Area = (double *)malloc(TotalBlobs*sizeof(double))) &&
		(Width = (double *)malloc(TotalBlobs*sizeof(double)))
		)
		{ 
			MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_X, CogX); 
			MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_Y, CogY); 
			MblobGetResult(MilBlobResult, M_AREA, Area); 
			MblobGetResult(MilBlobResult, M_FERET_X, Width); 

			area= 0;
			for(i = 0; i < TotalBlobs; i++)
			{
				if((CogY[i] >= 60))
				{
					area = area + Area[i];
					if(!first)
					{
						first_idx = i;
						first = TRUE;
					}
					add_x = add_x + CogX[i];
					x_idx++;
				}
				avg_x = add_x / x_idx;
			}

			search.left = (LONG)(avg_x - (search_width / 2));
			search.top = (LONG)(CogY[first_idx] - 30);
			search.right = search.left + search_width;
			search.bottom = search.top + search_height;

			MblobSelect(MilBlobResult, M_EXCLUDE, M_BOX_X_MIN, M_LESS, search.left, M_NULL); 
			MblobSelect(MilBlobResult, M_EXCLUDE, M_BOX_Y_MIN, M_LESS, search.top, M_NULL); 

			MgraColor(M_DEFAULT, M_COLOR_YELLOW);
			MgraRect(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), search.left, search.top, search.right, search.bottom);
			MgraRect(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), search.left + 1, search.top + 1, search.right + 1, search.bottom + 1);

			free(CogX);
			free(CogY);
			free(Width);
			free(Area);	
		}
		else
		{
			MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
			MgraColor(M_DEFAULT, M_COLOR_RED);
			MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.right, rect.top - 20, MIL_TEXT("Teaching fail...!"));
		}

		m_VisMod->SetSetupPara(setupPara);

		MblobSelect(MilBlobResult, M_EXCLUDE, M_BOX_X_MAX, M_GREATER_OR_EQUAL, search.right, M_NULL); 
		MblobSelect(MilBlobResult, M_EXCLUDE, M_BOX_Y_MAX, M_GREATER_OR_EQUAL, search.bottom, M_NULL); 

		MblobGetNumber(MilBlobResult, &TotalBlobs); 

		if ((CogX = (double *)malloc(TotalBlobs*sizeof(double))) && 
		(CogY = (double *)malloc(TotalBlobs*sizeof(double))) && 
		(Area = (double *)malloc(TotalBlobs*sizeof(double))) &&
		(Width = (double *)malloc(TotalBlobs*sizeof(double)))
		)
		{ 
			MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_X, CogX); 
			MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_Y, CogY); 
			MblobGetResult(MilBlobResult, M_AREA, Area); 
			MblobGetResult(MilBlobResult, M_FERET_X, Width); 

			area= 0;
			for(i = 0; i < TotalBlobs; i++)
			{
				area = area + Area[i];
				if((CogX[i] <= search.right) && (CogY[i] <= search.bottom))
				{
					//setupPara.InspPara.InsCenterPoint[ins_idx - ALIGN_SEARCH_REGION_1].SetPoint(CogX[i], CogY[i]);
					setupPara.InspPara.InsCenterPoint[Cog_idx].SetPoint((int)CogX[i], (int)CogY[i]);
					setupPara.InspPara.dInspWidth[Cog_idx] = search.Width();
					//20151222 ngh
					if(setupPara.InspPara.bInspBroken)
					{
						if(!InspectionMeas(by_source, setupPara.InspPara.InsCenterPoint[Cog_idx]))
						{
							m_InsRsltPara.poNgPoint[ng_cnt].x = CogX[i];
							m_InsRsltPara.poNgPoint[ng_cnt].y = CogY[i];
							m_InsRsltPara.NgRect[ng_cnt].left = CogX[i] - (Width[i] / 2) - 10;
							m_InsRsltPara.NgRect[ng_cnt].top = CogY[i] - ((setupPara.ModelPara.dModelGap / setupPara.dPixelSize) / 2);
							m_InsRsltPara.NgRect[ng_cnt].right = CogX[i] + (Width[i] / 2) + 10;
							m_InsRsltPara.NgRect[ng_cnt++].bottom = CogY[i] + ((setupPara.ModelPara.dModelGap / setupPara.dPixelSize) / 2);
							rslt = 5;
						}
					}
					temp.Format("col : %d, cnt : %d, x = %.0f, y = %.0f, Width = %.1f(mm)", ins_idx, i, CogX[i], CogY[i], (Width[i] * setupPara.dPixelSize));
					MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), CogX[i] - (Width[i] / 2), CogY[i], MIL_TEXT(temp));
					Cog_idx++;
				}
			}
		}

		//setupPara.InspPara.InsCenterPoint[0].SetPoint(search.CenterPoint().x, search.CenterPoint().y);

		m_VisMod->SetSetupPara(setupPara);

		MgraColor(M_DEFAULT, M_COLOR_GREEN);
		MblobDraw(M_DEFAULT, MilBlobResult, *m_VisMod->GetMilOverlay(MAIN_DISP), M_DRAW_BLOBS , M_INCLUDED_BLOBS, M_DEFAULT);

		MblobFree(MilBlobResult); 
		MblobFree(MilBlobFeatureList); 
		MbufFree(MilChildImage);
		MbufFree(MilBinImage);
		
		//check 니들 빠짐
		if(TotalBlobs < setupPara.ModelPara.nModelCnt)
		{
			MgraColor(M_DEFAULT, M_COLOR_RED);
			temp.Format("col : %d Needle Miss NG(rslt cnt : %d, model cnt : %d)",  ins_idx, TotalBlobs, setupPara.ModelPara.nModelCnt);
			MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), search.left, search.top - 15, MIL_TEXT(temp));
			m_InsRsltPara.poNgPoint[ng_cnt].x = search.left + (search.Width() / 2);
			m_InsRsltPara.poNgPoint[ng_cnt].y = 160;
			m_InsRsltPara.NgRect[ng_cnt].left = search.left - 10;
			m_InsRsltPara.NgRect[ng_cnt].top = 160 - ((setupPara.ModelPara.dModelGap / setupPara.dPixelSize) / 2);
			m_InsRsltPara.NgRect[ng_cnt].right = search.right + 10;
			m_InsRsltPara.NgRect[ng_cnt++].bottom = 160 + ((setupPara.ModelPara.dModelGap / setupPara.dPixelSize) / 2);
			rslt = 1;
		}
		else
		{
			TotalBlobs = setupPara.ModelPara.nModelCnt;
		}

		if(setupPara.InspPara.bInspWidth[0] && (rslt == 0))
		{
			double r_width;
			double r_width_dif;
			double r_width_tol;
			double r_width_rat = (setupPara.InspPara.dInspOffSetX[0] / 100.0);
			//같은게 5개 이상이면 양품
			for(i = 0; (i < TotalBlobs) && (0 == rslt); i++)
			{
				r_width = Width[i] * setupPara.dPixelSize;
				r_width_tol = setupPara.ModelPara.dModelWidth * r_width_rat;
				r_width_dif = fabs(r_width - setupPara.ModelPara.dModelWidth);
				if(r_width_tol < r_width_dif)
				{
					MgraColor(M_DEFAULT, M_COLOR_RED);
					//temp.Format("col : %d, Needle Width NG(No : %d)", ins_idx, i);
					//MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), search.left, search.top - 30, MIL_TEXT(temp));
					temp.Format("col : %d, Needle Width NG(No : %d)(rslt Width : %.1f(mm), Model Width : %.1f(mm))", ins_idx, i, r_width, setupPara.ModelPara.dModelWidth);
					MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), search.left, search.top - 15, MIL_TEXT(temp));
					m_InsRsltPara.poNgPoint[ng_cnt].x = CogX[i];
					m_InsRsltPara.poNgPoint[ng_cnt].y = CogY[i];
					m_InsRsltPara.NgRect[ng_cnt].left = CogX[i] - (Width[i] / 2) - 10;
					m_InsRsltPara.NgRect[ng_cnt].top = CogY[i] - ((setupPara.ModelPara.dModelGap / setupPara.dPixelSize) / 2);
					m_InsRsltPara.NgRect[ng_cnt].right = CogX[i] + (Width[i] / 2) + 10;
					m_InsRsltPara.NgRect[ng_cnt++].bottom = CogY[i] + ((setupPara.ModelPara.dModelGap / setupPara.dPixelSize) / 2);
					rslt = 2;
				}
			}
		}

		if(setupPara.InspPara.bInspHeight[0] && (rslt == 0))
		{
			double center_x;
			double r_center_x;
			double r_center_x_dif;
			double r_center_x_tol;
			double r_center_x_rat = (setupPara.InspPara.dInspOffSetY[0] / 100.0);
			//중심
			for(i = 0; (i < TotalBlobs) && (0 == rslt); i++)
			{
				rect = search;
				center_x = rect.left + (rect.Width() / 2.0);
				r_center_x = CogX[i];
				r_center_x_tol = rect.Width() * r_center_x_rat;
				r_center_x_dif = fabs(r_center_x - center_x);

				
				if(r_center_x_tol < r_center_x_dif)
				{
					MgraColor(M_DEFAULT, M_COLOR_RED);
					temp.Format("col : %d, Needle Center Width NG(No : %d)(rslt Center Width : %.1f(mm), Model Center Width : %.1f(mm))", ins_idx, i, r_center_x, center_x);
					MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), search.left, search.top - 15, MIL_TEXT(temp));
					m_InsRsltPara.poNgPoint[ng_cnt].x = CogX[i];
					m_InsRsltPara.poNgPoint[ng_cnt].y = CogY[i];
					m_InsRsltPara.NgRect[ng_cnt].left = CogX[i] - (Width[i] / 2) - 10;
					m_InsRsltPara.NgRect[ng_cnt].top = CogY[i] - ((setupPara.ModelPara.dModelGap / setupPara.dPixelSize) / 2);
					m_InsRsltPara.NgRect[ng_cnt].right = CogX[i] + (Width[i] / 2) + 10;
					m_InsRsltPara.NgRect[ng_cnt++].bottom = CogY[i] + ((setupPara.ModelPara.dModelGap / setupPara.dPixelSize) / 2);

					rslt = 3;
				}
			}
		}

		if(setupPara.InspPara.bInspLength[0] && (rslt == 0))
		{
			double r_pitch;
			double r_next_pitch;
			int r_idx;
			double r_pitch_dif;
			double r_pitch_tol;
			double r_pitch_rat = (setupPara.InspPara.dInspOffSetZ[0] / 100.0);

			for(i = 1; (i < TotalBlobs - 2) && (0 == rslt); i++)//맨처음것 마직막것은 하지 않는댜 시작 = 1, 끝 = -1, 추가로 마지막은 피치가 없다 시작 = 1 끝 = -2
			{
				r_pitch = abs(CogY[i + 1] - CogY[i]);
				m_InsRsltPara.dCentY[i] = (r_pitch);
				r_idx = 0;
				for(j = 1; j < TotalBlobs - 1; j++)//같은개 5개 이상이면 무조건 양품이다
				{
					r_next_pitch = fabs(CogY[j + 1] - CogY[j]);
					r_pitch_dif = fabs(r_next_pitch - r_pitch);
					r_pitch_tol = r_pitch * r_pitch_rat;

					if(r_pitch_tol < r_pitch_dif)
					{
						
					}
					else
					{
						//조건만족
						r_idx++;
					}
				}

				if(5 <= r_idx)
				{
					//양품처리

				}
				else
				{
					//불량처리
					m_InsRsltPara.bCentY_NG[i] = TRUE;
				}
			}

			BOOL r_chk;
			r_chk = TRUE;
			int r_pitch_num;
			r_pitch_num = 0;
	
			for(i = 1; i < (TotalBlobs - 1); i++)//맨처음것 마직막것은 하지 않는댜 시작 = 1, 끝 = -1, 추가로 마지막은 피치가 없다 시작 = 1 끝 = -2
			{
				if(m_InsRsltPara.bCentY_NG[i])
				{
					if(((setupPara.ModelPara.dModelGap / setupPara.dPixelSize) * 2) - ((setupPara.ModelPara.dModelGap / setupPara.dPixelSize) * r_pitch_rat) <= m_InsRsltPara.dCentY[i])
					{
						continue;
					}
					//Display
					MgraColor(M_DEFAULT, M_COLOR_RED);
					temp.Format("col : %d, Pitch NG(%d) pitch : %.1f", ins_idx, i, m_InsRsltPara.dCentY[i]);
					MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), CogX[i] - (Width[i] / 2), CogY[i] + 15, MIL_TEXT(temp));
					m_InsRsltPara.poNgPoint[ng_cnt].x = CogX[i];
					m_InsRsltPara.poNgPoint[ng_cnt].y = CogY[i];
					m_InsRsltPara.NgRect[ng_cnt].left = CogX[i] - (Width[i] / 2) - 10;
					m_InsRsltPara.NgRect[ng_cnt].top = CogY[i] - ((setupPara.ModelPara.dModelGap / setupPara.dPixelSize) / 2);
					m_InsRsltPara.NgRect[ng_cnt].right = CogX[i] + (Width[i] / 2) + 10;
					m_InsRsltPara.NgRect[ng_cnt++].bottom = CogY[i] + ((setupPara.ModelPara.dModelGap / setupPara.dPixelSize) / 2);
					r_pitch_num++;
					r_chk = FALSE;
				}
			}

			if(!r_chk)
			{
				MgraColor(M_DEFAULT, M_COLOR_RED);
				temp.Format("col : %d, Needle pitch NG(NG Cnt : %d)", ins_idx, r_pitch_num);
				MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), search.left, search.top - 15, MIL_TEXT(temp));
				rslt = 4;
			}
		}

		free(CogX);
		free(CogY);
		free(Width);
		free(Area);	

		if(0 == rslt)
		{
			MgraColor(M_DEFAULT, M_COLOR_GREEN);
			temp.Format("col : %d, OK", ins_idx);
			MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), search.left, search.top - 15, MIL_TEXT(temp));
		}
	}

	e_time = clock();
	p_time = (double)((e_time - s_time));
	strLog.Format("Needle Inspection Time : %.5f ms", p_time);
	WriteStatusList(strLog);
	m_InsRsltPara.strNgName[0] = temp;
	m_InsRsltPara.nNgCnt = ng_cnt - 1;

	MbufFree(MilTransImage);
	MbufFree(image);

	return rslt;
}

int CBearingInsDlg::InspectionEdge()
{
	int rslt = 0;
	int ins_idx = ALIGN_SEARCH_REGION_1;
	int needle_idx = 0;
	int i, j;
	BYTE *by_source;
	double high_thresh, low_thresh;
	MIL_INT width, height, pitch;
	MIL_ID image;
	MIL_ID MaskImage;
	MIL_ID 
				MilEdgeContext,                         /* Edge context.             */                   
				MilEdgeResult;                          /* Edge result identifier.   */   
	long		NumEdgeFound  = 0L,                     /* Number of edges found.    */
				NumResults    = 0L;                     /* Number of results found.  */   
	CString temp, strLog;
	typeSetupPara setupPara;
	setupPara = m_VisMod->GetSetupPara();
	double p_time, s_time, e_time;
	s_time = clock();
	
	high_thresh = setupPara.InspPara.dInspEdgeThreshHeigh;
	//MdispControl(*m_VisMod->GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
	MbufAlloc2d(*m_VisMod->GetMilSystem(), MAX_CAMERA_WIDTH, MAX_CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &image);
	MbufCopy(*m_VisMod->GetMilDispBuf(MAIN_DISP), image);

	for(ins_idx = ALIGN_SEARCH_REGION_1; ins_idx < ALIGN_SEARCH_REGION_1 + setupPara.ModelPara.nCol; ins_idx++)
	{
		for(needle_idx = 0; needle_idx < setupPara.ModelPara.nModelCnt; needle_idx++)
		{
			CRect rect((int)(setupPara.InspPara.InsCenterPoint[needle_idx].x - (setupPara.InspPara.InsRegion[4].Width() / 2)),
						(int)(setupPara.InspPara.InsCenterPoint[needle_idx].y - (setupPara.InspPara.InsRegion[4].Height() / 2) - setupPara.InspPara.InsCenterPointOffSet.y),
						(int)(setupPara.InspPara.InsCenterPoint[needle_idx].x + (setupPara.InspPara.InsRegion[4].Width() / 2)),
						(int)(setupPara.InspPara.InsCenterPoint[needle_idx].y + (setupPara.InspPara.InsRegion[4].Height() / 2) - setupPara.InspPara.InsCenterPointOffSet.y));
			//setupPara.InspPara.InsCenterPoint[ins_idx - ALIGN_SEARCH_REGION_1].x;

			MbufAlloc2d(*m_VisMod->GetMilSystem(), MAX_CAMERA_WIDTH, MAX_CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MaskImage);
			MbufClear(MaskImage, 0xFF);

			MbufInquire(MaskImage, M_HOST_ADDRESS, &by_source);
			MbufInquire(MaskImage, M_PITCH, &pitch);
			MbufInquire(MaskImage, M_SIZE_X, &width);
			MbufInquire(MaskImage, M_SIZE_Y, &height);
			
			MgraColor(M_DEFAULT, M_COLOR_BLUE);
			MgraRect(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.left, rect.top, rect.right, rect.bottom);
			MgraRect(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.left + 1, rect.top + 1, rect.right + 1, rect.bottom + 1);
			
			for(i = rect.top; i < rect.bottom; i++)
			{
				for(j = rect.left; j < rect.right; j++)
				{
					by_source[i * pitch + j] = 0;
				}
			}

			/* Allocate a Edge Finder context. */
			MedgeAlloc(*m_VisMod->GetMilSystem(), M_CONTOUR, M_DEFAULT, &MilEdgeContext); 
			
			/* Allocate a result buffer. */
			MedgeAllocResult(*m_VisMod->GetMilSystem(), M_DEFAULT, &MilEdgeResult);
			
			/* Enable features to compute. */
			//MedgeControl(MilEdgeContext, M_MOMENT_ELONGATION,                M_ENABLE);
			//MedgeControl(MilEdgeContext, M_FERET_MEAN_DIAMETER+M_SORT1_DOWN, M_ENABLE);
			MedgeControl(MilEdgeContext, M_FAST_LENGTH,						 M_ENABLE);
			MedgeControl(MilEdgeContext, M_ACCURACY,						 M_DISABLE);
			MedgeControl(MilEdgeContext, M_THRESHOLD_MODE,					 M_USER_DEFINED);
			
			if(0 > high_thresh)
			{
				high_thresh = 0;
			}
			if(100. > high_thresh)
			{
				low_thresh = high_thresh;
			}
			else
			{
				low_thresh = high_thresh - 100;
			}

			MedgeControl(MilEdgeContext, M_THRESHOLD_HIGH,					 high_thresh);
			MedgeControl(MilEdgeContext, M_THRESHOLD_LOW,					 low_thresh);
		 
			MedgeMask(MilEdgeContext, MaskImage, M_DEFAULT);

			/* Calculate edges and features. */
			MedgeCalculate(MilEdgeContext, image, M_NULL, M_NULL, M_NULL, MilEdgeResult, M_DEFAULT);

			/* Get the number of edges found. */
			MedgeGetResult(MilEdgeResult, M_DEFAULT, M_NUMBER_OF_CHAINS+M_TYPE_LONG, &NumResults, M_NULL);

			/* Draw remaining edges and their index to show the result. */
			MgraColor(M_DEFAULT, M_COLOR_GREEN);
			MedgeDraw(M_DEFAULT, MilEdgeResult, *m_VisMod->GetMilOverlay(MAIN_DISP), M_DRAW_EDGES, M_DEFAULT, M_DEFAULT);

			double tot_length = 0.;
			double *length;

			if(NumResults > 0)
			{
				length = new double[NumResults];

				/* Get the mean Feret diameters. */      
				MedgeGetResult(MilEdgeResult, M_DEFAULT, M_FAST_LENGTH, length, M_NULL);

				for(i = 0; i < NumResults; i++)
				{
					tot_length = tot_length + length[i];
				}
				tot_length;
			
				MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
				MgraColor(M_DEFAULT, M_COLOR_RED);
				temp.Format("LENGTH : %d", (long)tot_length);
				MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.left, rect.bottom + 2, MIL_TEXT((char *)(LPCTSTR)temp));

				delete[] length;
			}
			else
			{
				MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
				MgraColor(M_DEFAULT, M_COLOR_RED);
				temp.Format("LENGTH : %d", (long)tot_length);
				MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.left, rect.bottom + 2, MIL_TEXT((char *)(LPCTSTR)temp));
			}

			MedgeFree(MilEdgeContext);
			MedgeFree(MilEdgeResult);
			MbufFree(MaskImage);
		}
	}
	
	
	MbufFree(image);

	e_time = clock();
	p_time = (double)((e_time - s_time));
	strLog.Format("Edge Inspection Time : %.5f ms", p_time);
	WriteStatusList(strLog);


	return rslt;
}

int CBearingInsDlg::InspectionMod()
{
	int rslt = 0;
	int ins_idx = 0;
	int needle_idx = 0;
	MIL_ID image, pat_rslt;
	CString strModPath, strTeachPath, temp;
	long count;
	double x[PATTERN_FIND_MAX_NUM], y[PATTERN_FIND_MAX_NUM], ang[PATTERN_FIND_MAX_NUM], score[PATTERN_FIND_MAX_NUM];
	BOOL search_rslt = FALSE;
	CString strLog;
	double p_time, s_time, e_time;
	s_time = clock();

	typeSetupPara setupPara;
	setupPara = m_VisMod->GetSetupPara();
	//MbufCopy(*m_VisMod->GetMilDispBuf(0), *m_VisMod->GetMilTeachBuf());
	MbufAlloc2d(*m_VisMod->GetMilSystem(), CAMERA_WIDTH, CAMERA_HEIGHT, 8+M_UNSIGNED, M_IMAGE + M_PROC, &image);
	//MbufCopy(*m_VisMod->GetMilDispBuf(MAIN_DISP), *m_VisMod->GetMilTeachBuf());
	MbufCopy(*m_VisMod->GetMilDispBuf(MAIN_DISP), image);
	MdispControl(*m_VisMod->GetMilDisplay(MAIN_DISP), M_OVERLAY_CLEAR, M_DEFAULT);
	
	if(setupPara.InspPara.bCreatMdID[ins_idx])
	{
		if(setupPara.InspPara.MilSearchContext[ins_idx])
		{
			MmodFree(setupPara.InspPara.MilSearchContext[ins_idx]);
			setupPara.InspPara.MilSearchContext[ins_idx] = NULL;
		}
	}

	MmodAlloc(*m_VisMod->GetMilSystem(), M_GEOMETRIC, M_DEFAULT, &setupPara.InspPara.MilSearchContext[ins_idx]);

	strModPath.Format("%s\\Align_Teaching", m_ModelInfo.strModelPath);
	MmodRestore(MIL_TEXT((char *)(LPCTSTR)strModPath), *m_VisMod->GetMilSystem(), M_DEFAULT , &setupPara.InspPara.MilSearchContext[ALIGN_REGION]);
	MmodPreprocess(setupPara.InspPara.MilSearchContext[ALIGN_REGION], M_DEFAULT);


	for(ins_idx = ALIGN_SEARCH_REGION_1; ins_idx < ALIGN_SEARCH_REGION_1 + setupPara.ModelPara.nCol; ins_idx++)
	{
		for(needle_idx = 0; needle_idx < setupPara.ModelPara.nModelCnt; needle_idx++)
		{
			CRect rect((int)(setupPara.InspPara.InsCenterPoint[0].x - (setupPara.InspPara.InsRegion[4].Width() / 2)),
						(int)(setupPara.InspPara.InsCenterPoint[needle_idx].y - (setupPara.InspPara.InsRegion[4].Height() / 2) - setupPara.InspPara.InsCenterPointOffSet.y),
						(int)(setupPara.InspPara.InsCenterPoint[0].x + (setupPara.InspPara.InsRegion[4].Width() / 2)),
						(int)(setupPara.InspPara.InsCenterPoint[needle_idx].y + (setupPara.InspPara.InsRegion[4].Height() / 2) - setupPara.InspPara.InsCenterPointOffSet.y));
				
			//Test///////////////////////////////////////
			//CRect search(rect.left - 10, rect.top - 10, rect.right + 10, rect.bottom + 10);
			CRect search(rect);
			CPoint cent = rect.CenterPoint();
			double ndx, pdx, ndy, pdy;
			ndx = cent.x - rect.left;
			pdx = rect.right - cent.x;
			ndy = cent.y - rect.top;
			pdy = rect.bottom - cent.y;
			/////////////////////////////////////////////
			MgraColor(M_DEFAULT, M_COLOR_BLUE);
			MgraRect(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), ndx, ndy, ndx + search.Width(), ndy + search.Height());
			//MgraRect(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), search.left + 1, search.top + 1, search.right + 1, search.bottom + 1);

			if((search.left <= rect.left) && (rect.right <= search.right) && (search.top <= rect.top) && (rect.bottom <= search.bottom))
			{
				search_rslt = TRUE;
			} 

			if(search_rslt)
			{
				if(setupPara.InspPara.bCreatMdID[ALIGN_REGION])
				{
					/* Allocate a result buffer. */
					MmodAllocResult(*m_VisMod->GetMilSystem(), M_DEFAULT, &pat_rslt);
					MmodControl(setupPara.InspPara.MilSearchContext[ALIGN_REGION], M_CONTEXT, M_SPEED, M_VERY_HIGH);

					MmodControl(setupPara.InspPara.MilSearchContext[ALIGN_REGION], M_DEFAULT, M_ACCEPTANCE, 10);

					//Test 0820//////////////////////////////
					//Ins region
					MmodControl(setupPara.InspPara.MilSearchContext[ALIGN_REGION], M_DEFAULT,  M_POSITION_X , cent.x);
					MmodControl(setupPara.InspPara.MilSearchContext[ALIGN_REGION], M_DEFAULT,  M_POSITION_Y , cent.y);

					MmodControl(setupPara.InspPara.MilSearchContext[ALIGN_REGION], M_DEFAULT,  M_POSITION_DELTA_NEG_X , ndx);
					MmodControl(setupPara.InspPara.MilSearchContext[ALIGN_REGION], M_DEFAULT,  M_POSITION_DELTA_POS_X , pdx);
					MmodControl(setupPara.InspPara.MilSearchContext[ALIGN_REGION], M_DEFAULT,  M_POSITION_DELTA_NEG_Y , ndy);
					MmodControl(setupPara.InspPara.MilSearchContext[ALIGN_REGION], M_DEFAULT,  M_POSITION_DELTA_POS_Y , pdy);

					/////////////////////////////////////////

					/* Dummy first find operation for better function timing accuracy (model cache effet,...). */
					MmodFind(setupPara.InspPara.MilSearchContext[ALIGN_REGION], image, pat_rslt);
				
					/* Get the number of models found. */
					MmodGetResult(pat_rslt, M_DEFAULT, M_NUMBER+M_TYPE_LONG, &count);

					MmodGetResult(pat_rslt, M_DEFAULT, M_POSITION_X, x);
					MmodGetResult(pat_rslt, M_DEFAULT, M_POSITION_Y, y);
					MmodGetResult(pat_rslt, M_DEFAULT, M_ANGLE, ang);
					MmodGetResult(pat_rslt, M_DEFAULT, M_SCORE, score);
					
					for (int i=0; i<count; i++)
					{         
						MgraColor(M_DEFAULT, M_COLOR_GREEN);
						MmodDraw(M_DEFAULT,  pat_rslt, *m_VisMod->GetMilOverlay(MAIN_DISP), M_DRAW_EDGES+M_DRAW_POSITION, i, M_DEFAULT);

						temp.Format("X : %f, Y : %f, Ang : %f, Score : %f", x[0], y[0], ang[0], score[0]);
						MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
						MgraColor(M_DEFAULT, M_COLOR_GREEN);
						MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.left + 20, rect.top + 20, MIL_TEXT((char *)(LPCTSTR)temp));
						WriteStatusList(temp);
					}

					if(0 == count)
					{
						MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
						MgraColor(M_DEFAULT, M_COLOR_RED);
						MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.left + 20, rect.top + 20, MIL_TEXT("Error : can't find pattern!"));
						WriteStatusList("Error : can't find pattern!");
					}

				}
				else
				{
					MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
					MgraColor(M_DEFAULT, M_COLOR_RED);
					MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.left + 20, rect.top + 20, MIL_TEXT("Error : no pattern!"));
					WriteStatusList("Error : no pattern!");
				}
			}
			else
			{
				MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
				MgraColor(M_DEFAULT, M_COLOR_RED);
				MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(MAIN_DISP), rect.left + 20, rect.top + 20, MIL_TEXT("Error : Align Search fail!"));
				WriteStatusList("Error : Align Search fail!");
			}
		}
	}

	if(pat_rslt)
	{
		MmodFree(pat_rslt);
	}

	MappControl(M_ERROR, M_PRINT_ENABLE);
	MbufFree(image);

	e_time = clock();
	p_time = (double)((e_time - s_time));
	strLog.Format("Edge Inspection Time : %.5f ms", p_time);
	WriteStatusList(strLog);

	return rslt;
}

void CBearingInsDlg::OnBnClickedButtonTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString save_path;
	InitInsRsltPara();
	save_path.Format("%s\\GrabImage\\", m_ModelInfo.strModelPath);
	MakeDirectories(save_path);

	m_bInspectStart = TRUE;	
}

void CBearingInsDlg::InitInsRsltPara()
{
	m_InsRsltPara.bAlign = TRUE;
	m_InsRsltPara.nNgCnt = 0;
	m_InsRsltPara.dAlignX = 0.;
	m_InsRsltPara.dAlignY = 0.;
	m_InsRsltPara.nFindCnt = 0;
	
	for(int i = 0; i < MAX_INS_PARA; i++)
	{
		m_InsRsltPara.NgRect[i].SetRect(0,0,0,0);
		m_InsRsltPara.poNgPoint[i].SetPoint(0,0);
		m_InsRsltPara.dCentX[i] = 0.;
		m_InsRsltPara.dCentY[i] = 0.;
		m_InsRsltPara.dWidth[i] = 0.;
		m_InsRsltPara.dHeight[i] = 0.;
		m_InsRsltPara.bCheck[i] = FALSE;
		m_InsRsltPara.bCentX_NG[i] = FALSE;
		m_InsRsltPara.bCentY_NG[i] = FALSE;
		m_InsRsltPara.bWidth_NG[i] = FALSE;
		m_InsRsltPara.bHeight_NG[i] = FALSE;

		m_InsRsltPara.poNgPoint[i].SetPoint(0,0);
		m_InsRsltPara.strNgName[i].Empty();
		for(int j = 0; j < MAX_INSPECTION; j++)
		{
			m_InsRsltPara.bRslt[i][j] = TRUE;
		}
	}
}

void CBearingInsDlg::RsltView(int bIsNG)
{
	CString temp;
	typeSetupPara setupPara;
	setupPara = m_VisMod->GetSetupPara();
	MdispControl(*m_VisMod->GetMilDisplay(NG_DISP_1), M_OVERLAY_CLEAR, M_DEFAULT);
	MbufClear(*m_VisMod->GetMilDispBuf(NG_DISP_1), M_NULL);

	if(bIsNG)
	{
		MgraColor(M_DEFAULT, M_COLOR_RED);
		MgraRect(M_DEFAULT, *m_VisMod->GetMilOverlay(NG_DISP_1), m_InsRsltPara.NgRect[m_InsRsltPara.nNgCnt].left, m_InsRsltPara.NgRect[m_InsRsltPara.nNgCnt].top,
			m_InsRsltPara.NgRect[m_InsRsltPara.nNgCnt].right, m_InsRsltPara.NgRect[m_InsRsltPara.nNgCnt].bottom);
		MgraRect(M_DEFAULT, *m_VisMod->GetMilOverlay(NG_DISP_1), m_InsRsltPara.NgRect[m_InsRsltPara.nNgCnt].left - 1, m_InsRsltPara.NgRect[m_InsRsltPara.nNgCnt].top - 1,
			m_InsRsltPara.NgRect[m_InsRsltPara.nNgCnt].right + 1, m_InsRsltPara.NgRect[m_InsRsltPara.nNgCnt].bottom + 1);
		MbufCopy(*m_VisMod->GetMilDispBuf(MAIN_DISP), *m_VisMod->GetMilDispBuf(NG_DISP_1));
		MdispPan(*m_VisMod->GetMilDisplay(NG_DISP_1), (LONG)(m_InsRsltPara.poNgPoint[m_InsRsltPara.nNgCnt].x - 250), (LONG)(m_InsRsltPara.poNgPoint[m_InsRsltPara.nNgCnt].y - 160));
	
		//temp.Format("%04dY%02dM%02dD-%02dH%02dM%02dS", lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond);
		MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(NG_DISP_1), (LONG)(m_InsRsltPara.poNgPoint[m_InsRsltPara.nNgCnt].x - 250) + 10, (LONG)(m_InsRsltPara.poNgPoint[0].y - 150), MIL_TEXT(m_strInspDateTime));
		temp.Empty();
		AfxExtractSubString(temp, m_InsRsltPara.strNgName[0], 0, 'G');
		temp+='G';
		MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(NG_DISP_1), (LONG)(m_InsRsltPara.poNgPoint[m_InsRsltPara.nNgCnt].x - 250) + 10, (LONG)(m_InsRsltPara.poNgPoint[0].y - 130), MIL_TEXT(temp));
		temp.Empty();
		AfxExtractSubString(temp, m_InsRsltPara.strNgName[0], 1, 'G');
		if(temp.GetLength() >= 4)
		{
			MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(NG_DISP_1), (LONG)(m_InsRsltPara.poNgPoint[m_InsRsltPara.nNgCnt].x - 250) + 10, (LONG)(m_InsRsltPara.poNgPoint[0].y - 110), MIL_TEXT(temp));
		}
	}
	else
	{
		MdispPan(*m_VisMod->GetMilDisplay(NG_DISP_1), 0, 0);
		MdispPan(*m_VisMod->GetMilDisplay(NG_DISP_2), 0, 0);
		MdispPan(*m_VisMod->GetMilDisplay(NG_DISP_3), 0, 0);
		MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(NG_DISP_1), 10, 10, MIL_TEXT(m_strInspDateTime));
		MgraText(M_DEFAULT, *m_VisMod->GetMilOverlay(NG_DISP_1), 10, 30, MIL_TEXT("Inspection OK"));

	}
}


BYTE CBearingInsDlg::byValue2Hexa_High(BYTE byData)
{
	BYTE byValue1 = (byData >> 4);

	return byValue2NibbleAscii(byValue1);
}

BYTE CBearingInsDlg::byValue2Hexa_Low(BYTE byData)
{
	BYTE byValue1 = (byData & 0x0f);

	return byValue2NibbleAscii(byValue1);
}

BYTE CBearingInsDlg::byValue2NibbleAscii(BYTE byData)
{
	if((0 <= byData) && (byData <=9))
	{
		return byData + '0';
	}

	if(byData == 10)	byData = 'A';
	if(byData == 11)	byData = 'B';
	if(byData == 12)	byData = 'C';
	if(byData == 13)	byData = 'D';
	if(byData == 14)	byData = 'E';
	if(byData == 15)	byData = 'F';
	return byData;

	return '*';
}

void CBearingInsDlg::SetLineLight(int ch, int var)
{
	BYTE bySndData[L_LIGHT_PACKET_SIZE];
	int checksum;
	int sendcnt = L_LIGHT_PACKET_SIZE - 1;
	CString strCh;
	strCh.Format("%1d", ch);
	
	bySndData[0] = 0x02;
	bySndData[1] = 0x30;
	bySndData[2] = 0x31;
	bySndData[3] = 0x30;
	bySndData[4] = 0x30;
	bySndData[5] = 0x34;
	bySndData[6] = 0x30;
	bySndData[7] = strCh.GetAt(0);
	bySndData[8] = (BYTE)(byValue2Hexa_High((BYTE)var));
	bySndData[9] = (BYTE)(byValue2Hexa_Low((BYTE)var));

	checksum = 0;

	for(int i = 0; i < sendcnt; i++)
	{
		checksum += bySndData[i];
	}

	bySndData[10] = LOBYTE(checksum);
	bySndData[11] = HIBYTE(checksum);

	bySndData[12] = 0x03;
	bySndData[13] = NULL;

	m_cSerial.Write(bySndData, sendcnt);
}
void CBearingInsDlg::ClickBtnenhctrlRun()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.	
	if(m_btnStartStop.GetValue())
	{
#ifdef DCF
		MdigControl(*m_VisMod->GetMilDigitizer(), M_GRAB_ABORT, M_DEFAULT);
#endif
		typeSetupPara setupPara;
		m_VisMod->LoadSetupPara(m_ModelInfo.strModelPath);
		setupPara = m_VisMod->GetSetupPara();
		m_btnStartStop.SetWindowTextA("Stop");
		m_btnStartStop.SetBackColor(RGB(255,0,0));
		m_bInspectEnd = FALSE;
		m_bIOCheck = TRUE;
		m_bOUTCheck = TRUE;
		m_bIOEnd = FALSE;

		//if(FileSearch())
		//{
		//	RsltSave(0,TRUE);
		//}
#ifdef LIGHT
		for(int i = 0; i < LIGHT_3; i++)
		{
			SetLineLight(i + 1, setupPara.iLight[i]);
			Sleep(100);
		}
#endif
		m_pMainThread = AfxBeginThread(InspectThread, this, THREAD_PRIORITY_NORMAL);
		if(m_pMainThread)
			m_pMainThread->m_bAutoDelete = TRUE;

		if(m_hComiDevice32 != (HANDLE)0xffffffffffffffff)
		{
			m_pIOThread = AfxBeginThread(IOThread, this, THREAD_PRIORITY_NORMAL);
			if(m_pIOThread)
				m_pIOThread->m_bAutoDelete = TRUE;
		}
	}
	else
	{
#ifdef DCF
		MdigGrabWait(*m_VisMod->GetMilDigitizer(), M_GRAB_FRAME_END);
		MdigControl(*m_VisMod->GetMilDigitizer(), M_GRAB_ABORT, M_DEFAULT);
#endif
		m_btnStartStop.SetWindowTextA("Run");
		m_btnStartStop.SetBackColor(RGB(0,255,0));
		m_bInspectStart = FALSE;
		m_bIOCheck = FALSE;
		m_bInspectEnd = TRUE;
		m_bIOEnd = TRUE;
		m_bIOCheck = FALSE;
#ifdef LIGHT
		for(int i = 0; i < LIGHT_3; i++)
		{
			SetLineLight(i + 1, 0);
			Sleep(100);
		}
#endif
	}
}

LRESULT CBearingInsDlg::OnProcessMessage(WPARAM para0, LPARAM para1)
{
	switch(para1)
	{
	case 0:
		m_VisMod->Grab();
		m_Trigger->SetTrigger_Trig_Pos_Reset();
		break;
	}
	return 0;
}

UINT CBearingInsDlg::AutoDeleteThread(LPVOID pParam)
{
	CBearingInsDlg *pdlg = (CBearingInsDlg *)pParam;
	SYSTEMTIME	lpSystemTime;
	typeSystemPara systemPara;
	
	while(TRUE)
	{
		if(pdlg->m_bAutoDeleteStart && !pdlg->m_dlgSetup->IsWindowVisible())
		{
			GetLocalTime(&lpSystemTime);
			systemPara = pdlg->m_VisMod->GetSystemPara();
			if((systemPara.nHour == lpSystemTime.wHour) && (systemPara.nMinute == lpSystemTime.wMinute) && !pdlg->m_bDelete)
			{
				pdlg->StartAutoDelete_Img();
				pdlg->m_bDelete = TRUE;
			}
			else if((systemPara.nMinute != lpSystemTime.wMinute) && pdlg->m_bDelete)
			{
				pdlg->m_bDelete = FALSE;
			}
		}

		if(pdlg->m_bAutoDeleteEnd)
		{
			pdlg->m_bAutoDeleteStart = FALSE;
			pdlg->m_bAutoDeleteEnd = FALSE;
			break;
		}
		Sleep(1);
	}

	return 0;
}

UINT CBearingInsDlg::InspectThread(LPVOID pParam)
{
	CBearingInsDlg *pdlg = (CBearingInsDlg *)pParam;
	CString save_path;
	int rslt = 0;
	typeSetupPara setupPara;
	CTime cTime;
	double p_time, e_time;//, s_time;

	while(TRUE)
	{
		if(pdlg->m_bInspectStart)
		{
			pdlg->m_StartTime = clock();
			pdlg->m_bIOCheck = FALSE;
			pdlg->m_bInspectStart = FALSE;	
			pdlg->m_InsCntPara.nTotalCnt++;
			setupPara = pdlg->m_VisMod->GetSetupPara();
			cTime = CTime::GetCurrentTime();
			rslt = -1;

			pdlg->m_strInspDate.Format("%04d%02d%02d", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());
			pdlg->m_strInspDateTime.Format("%04dY%02dM%02dD_%02dH%02dM%02dS", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

			
			if(pdlg->m_VisMod->Grab())
			{
				pdlg->m_EndTime = clock();
#ifdef RELEAS
#ifndef EDGE_INPEECTION
				rslt = pdlg->Inspection();

				if(rslt == 0)
				{
					pdlg->m_btnResult.SetBackColor(RGB(0, 255, 0);
				}
				else
				{
					pdlg->m_btnResult.SetBackColor(RGB(255, 0, 0);
				}
				
#else
				if(rslt == 0)
				{
					rslt = pdlg->InspectionEdge();
				}	
#endif
				e_time = clock();

				switch(rslt)
				{
					//OK
					case -1:
					case 0:
						COMI_DO_PutOne(pdlg->m_hComiDevice32, COMI_OUTPUT_OK1, TRUE);
						Sleep(100);
						COMI_DO_PutOne(pdlg->m_hComiDevice32, COMI_OUTPUT_OK2, TRUE);
						Sleep(300);
						COMI_DO_PutOne(pdlg->m_hComiDevice32, COMI_OUTPUT_OK1, FALSE);
						Sleep(100);
						COMI_DO_PutOne(pdlg->m_hComiDevice32, COMI_OUTPUT_OK2, FALSE);
						pdlg->m_InsCntPara.nOk1Cnt++;
						pdlg->m_InsCntPara.nOk2Cnt++;
						if(setupPara.bOKSaveImage)
						{
							save_path.Format("%s\\OKImage\\%s\\", pdlg->m_ModelInfo.strModelPath, pdlg->m_strInspDate);
							MakeDirectories(save_path);
							pdlg->m_VisMod->SaveImage(save_path);
						}
						break;
					//Needle 검사 불량
					case 1:
					case 2:
					case 3:
					case 4:
						COMI_DO_PutOne(pdlg->m_hComiDevice32, COMI_OUTPUT_NG1, TRUE);
						Sleep(500);
						COMI_DO_PutOne(pdlg->m_hComiDevice32, COMI_OUTPUT_NG1, FALSE);
						pdlg->m_InsCntPara.nNg1Cnt++;
						if(setupPara.bNGSaveImage)
						{
							save_path.Format("%s\\NGImage\\%s\\", pdlg->m_ModelInfo.strModelPath, pdlg->m_strInspDate);
							MakeDirectories(save_path);
							pdlg->m_VisMod->SaveImage(save_path);
						}
						break;
					//사출 검사 불량
					case 5:
					case 6:
					case 7:
					case 8:
						COMI_DO_PutOne(pdlg->m_hComiDevice32, COMI_OUTPUT_NG1, TRUE);
						Sleep(500);
						COMI_DO_PutOne(pdlg->m_hComiDevice32, COMI_OUTPUT_NG1, FALSE);
						pdlg->m_InsCntPara.nNg2Cnt++;
						if(setupPara.bNGSaveImage)
						{
							save_path.Format("%s\\NGImage\\%s\\", pdlg->m_ModelInfo.strModelPath, pdlg->m_strInspDate);
							MakeDirectories(save_path);
							pdlg->m_VisMod->SaveImage(save_path);
						}
						break;
				}
				p_time = (double)((pdlg->m_EndTime - pdlg->m_StartTime));
				CString strLog;
				strLog.Format("%06d : Grab Time : %.5f ms", pdlg->m_InsCntPara.nTotalCnt, p_time);
				pdlg->WriteStatusList(strLog);

				p_time = (double)((e_time - pdlg->m_StartTime));
				strLog.Format("%06d : Process Time : %.5f ms", pdlg->m_InsCntPara.nTotalCnt, p_time);
				pdlg->WriteStatusList(strLog);
#endif
			}
			if(setupPara.bSaveImage)
			{
				save_path.Format("%s\\GrabImage\\%s\\", pdlg->m_ModelInfo.strModelPath, pdlg->m_strInspDate);
				MakeDirectories(save_path);
				pdlg->m_VisMod->SaveImage(save_path);
			}
#ifdef RELEAS
			pdlg->SetInsCnt();
#ifdef RESULT_SAVE
			pdlg->RsltSave(rslt);
#endif
			pdlg->RsltView(rslt);
#endif
			pdlg->m_Trigger->SetTrigger_Trig_Pos_Reset();
			
			pdlg->m_bIOCheck = TRUE;
		}

		if(pdlg->m_bInspectEnd)
		{
			break;
		}	

		Sleep(1);
	}
	return 0;
}


UINT CBearingInsDlg::IOThread(LPVOID pParam)
{
	CBearingInsDlg *pdlg = (CBearingInsDlg *)pParam;
	CString save_path;
	
	while(TRUE)
	{
		if(pdlg->m_bIOCheck)
		{
			if(COMI_DI_GetOne(pdlg->m_hComiDevice32, COMI_INPUT_START))
			{
				pdlg->m_Trigger->SetTrigger_Trig_Pos_Reset();
				pdlg->m_StartTime = clock();
				pdlg->InitInsRsltPara();
				pdlg->m_bInspectStart = TRUE;
				pdlg->m_bIOCheck = FALSE;
				pdlg->m_btnStart.SetBackColor(RGB(0, 255, 0));
			}
			else 
			{
				pdlg->m_bInspectStart = FALSE;
				pdlg->m_bIOCheck = TRUE;
				pdlg->m_btnStart.SetBackColor(RGB(255, 0, 0));
			}
		}
		
		if(pdlg->m_bIOEnd)
		{
			break;
		}
		Sleep(1);
	}
	return 0;
}

void CBearingInsDlg::OnBnClickedButtonTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_VisMod->Grab();
	CTime cTime;
	CString save_path;
	cTime = CTime::GetCurrentTime();
	save_path.Format("%s\\GrabImage\\%04d%02d%02d\\", m_ModelInfo.strModelPath, cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());
	MakeDirectories(save_path);
	m_VisMod->SaveImage(save_path);
}

void CBearingInsDlg::OnBnClickedButtonOut0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COMI_DO_PutOne(m_hComiDevice32, 0, TRUE);
}

void CBearingInsDlg::OnBnClickedButtonOut1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COMI_DO_PutOne(m_hComiDevice32, 1, TRUE);
}

void CBearingInsDlg::OnBnClickedButtonOut2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COMI_DO_PutOne(m_hComiDevice32, 2, TRUE);
}

void CBearingInsDlg::OnBnClickedButtonOut3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COMI_DO_PutOne(m_hComiDevice32, 3, TRUE);
}

void CBearingInsDlg::OnBnClickedButtonOut4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COMI_DO_PutOne(m_hComiDevice32, 4, TRUE);
}

void CBearingInsDlg::OnBnClickedButtonOut5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COMI_DO_PutOne(m_hComiDevice32, 0, FALSE);
}

void CBearingInsDlg::OnBnClickedButtonOut6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COMI_DO_PutOne(m_hComiDevice32, 1, FALSE);
}

void CBearingInsDlg::OnBnClickedButtonOut7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COMI_DO_PutOne(m_hComiDevice32, 2, FALSE);
}

void CBearingInsDlg::OnBnClickedButtonOut8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COMI_DO_PutOne(m_hComiDevice32, 3, FALSE);
}

void CBearingInsDlg::OnBnClickedButtonOut9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COMI_DO_PutOne(m_hComiDevice32, 4, FALSE);
}

void CBearingInsDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	InitInsRsltPara();
	int rslt = 0;
	rslt = Inspection();
	if(!rslt)
	{
		rslt = InspectionEdge();
	}
	RsltSave(rslt);
}

void CBearingInsDlg::SetInsCnt()
{
	CString cnt;

	cnt.Format("%d", m_InsCntPara.nTotalCnt);
	m_btnTotal.SetWindowTextA(cnt);

	cnt.Format("%d", m_InsCntPara.nOk1Cnt);
	m_btnOK1.SetWindowTextA(cnt);

	cnt.Format("%d", m_InsCntPara.nNg1Cnt);
	m_btnNG1.SetWindowTextA(cnt);

	cnt.Format("%d", m_InsCntPara.nOk2Cnt);
	m_btnOK2.SetWindowTextA(cnt);

	cnt.Format("%d", m_InsCntPara.nNg2Cnt);
	m_btnNG2.SetWindowTextA(cnt);
}

void CBearingInsDlg::InitOutput()
{
	for(int i = 0; i < MAX_OUTPUT; i++)
	{
		COMI_DO_PutOne(m_hComiDevice32, i, FALSE);
		Sleep(10);
	}

}
void CBearingInsDlg::ClickBtnenhctrlCntReset()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_InsCntPara.nTotalCnt = 0;
	m_InsCntPara.nOk1Cnt = 0;
	m_InsCntPara.nNg1Cnt = 0;
	m_InsCntPara.nOk2Cnt = 0;
	m_InsCntPara.nNg2Cnt = 0;

	SetInsCnt();
}

BOOL CBearingInsDlg::RsltSave(int rslt_type, BOOL file_first)
{
	FILE*	stream;
	char	strFile[MAX_PATH], strToFile[MAX_PATH];
    long	result, line;
	char	szTime[DAT_STR_LENG]; 
	CString bak_file, path, filePath, save_path, text;
	int limit_line = 0;
	char buffer[DAT_STR_LENG];

	save_path.Format("%s\\Result\\%s\\", m_ModelInfo.strModelPath, m_strInspDate);
	MakeDirectories(save_path);

	if(rslt_type == RESULT_TYPE_OK)
	{
		return TRUE;
	}

	filePath.Empty();
	bak_file.Empty();
	bak_file.Format("%s%s_rslt.bak", save_path, m_strInspDate);
	filePath.Format("%s%s%s", save_path, m_strInspDate, RESULT_FILE_NAME);

	if(!file_first)
	{
		switch(rslt_type)
		{
		case INSPECTION_FAIL:
			text.Format("%s,%d,Inspection Fail", m_strInspDateTime, m_InsCntPara.nTotalCnt);
			break;
		case RESULT_TYPE_MISS_NG:
			text.Format("%s%d,Needle Miss NG", m_strInspDateTime, m_InsCntPara.nTotalCnt);
			break;
		case RESULT_TYPE_WIDTH_NG:
			text.Format("%s,%d,Needle Width NG", m_strInspDateTime, m_InsCntPara.nTotalCnt);
			break;
		case RESULT_TYPE_CENTER_WIDTH_NG:
			text.Format("%s,%d,Needle Center Width NG", m_strInspDateTime, m_InsCntPara.nTotalCnt);
			break;
		case RESULT_TYPE_PITCH_NG:
			text.Format("%s,%d,Needle pitch NG", m_strInspDateTime, m_InsCntPara.nTotalCnt);
			break;
		case RESULT_TYPE_BROKEN_NG:
			text.Format("%s,%d,Broken NG", m_strInspDateTime, m_InsCntPara.nTotalCnt);
			break;
		}
	}
	else
	{
		text.Format("Date,Needle Idx,Inspection");
	}

	sprintf_s(strFile, filePath.GetLength() + 1, "%s", filePath);

	if((stream = _fsopen( strFile, "a+" , _SH_DENYNO)) == NULL)
	{
		HANDLE fd = CreateFile( strFile,
							GENERIC_READ|GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
		stream = _fsopen( strFile, "a+" , _SH_DENYNO);
		if(stream == NULL)
		{
		
			CloseHandle(fd);
			return FALSE;
		}

		CloseHandle(fd);
	}

	//Check first time
    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}
	line = 0;
	while(fgets(buffer, MAX_STRING_LINE, stream)) 
	{
		line++;
		if(1 < line)
		{
			break;
		}
	}

    result = fseek(stream, 0L, SEEK_END);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	memset(szTime, NULL, sizeof(szTime));
	sprintf_s(szTime, text.GetLength() + 1, "%s", text);
    fputs(szTime, stream);
	fputs("\n", stream);

    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}

	//Check limit line
	line = 0;
	if(limit_line)
	{
		while(fgets(buffer, MAX_STRING_LINE, stream)) line++;
		if(line > limit_line)
		{
			sprintf_s(strToFile, filePath.GetLength() + bak_file.GetLength() + 2 + 1, "%s\\%s", path, bak_file);
			CopyFile(strFile, strToFile, FALSE);
			fclose(stream);
			DeleteFile(strFile);
		}
		else
		{
			fclose(stream);
		}
	}
	else
	{
		fclose(stream);
	}
	
	return TRUE;
}

BOOL CBearingInsDlg::CheckDeleteDate()
{
	CString str;
	CTime now_time = CTime::GetCurrentTime();
	int temp_year, temp_month, temp_day;
	CTime delete_time;
	int TempDeleteDay=0;
	int i=0;
	temp_year=0;
	temp_month=0;
	temp_day=0;
	typeSystemPara systemPara;

	temp_day = now_time.GetDay();
	temp_month = now_time.GetMonth();
	temp_year = now_time.GetYear();

	systemPara = m_VisMod->GetSystemPara();
 
	int save_day = systemPara.nDay;
	if( (temp_day - save_day) < 0)
	{
		if(now_time.GetMonth() == 2 || now_time.GetMonth() == 4 || now_time.GetMonth() == 6 ||
			now_time.GetMonth() == 8 || now_time.GetMonth() == 9 || now_time.GetMonth() == 11)
		{
			m_iDel_Year = now_time.GetYear();
			m_iDel_Month = now_time.GetMonth() - 1;
			m_iDel_RecentDay = 31 - (save_day - temp_day);
		}
		else if(now_time.GetMonth() == 5 || now_time.GetMonth() == 7 || now_time.GetMonth() == 10 || now_time.GetMonth() == 12)
		{
			m_iDel_Year = now_time.GetYear();
			m_iDel_Month = now_time.GetMonth() - 1;
			m_iDel_RecentDay = 30 - (save_day - temp_day);
		}
		else if(now_time.GetMonth() == 3)
		{
			m_iDel_Year = now_time.GetYear();
			m_iDel_Month = now_time.GetMonth() - 1;
			m_iDel_RecentDay = 28 - (save_day - temp_day);
		}
		else if(now_time.GetMonth() == 1)
		{
			m_iDel_Year = now_time.GetYear() - 1;
			m_iDel_Month = 12;
			m_iDel_RecentDay = 31 - (save_day - temp_day);
		}
	}
	else if( (temp_day - save_day) == 0)
	{
		if(now_time.GetMonth() == 1)
		{
			m_iDel_Year = now_time.GetYear() - 1;
			m_iDel_Month = 12;
			m_iDel_RecentDay = 0;
		}
		else
		{
			m_iDel_Year = now_time.GetYear();
			m_iDel_Month = now_time.GetMonth() - 1;
			m_iDel_RecentDay = 0;
		}
	}
	else
	{
		m_iDel_Year = now_time.GetYear();
		m_iDel_Month = now_time.GetMonth();
		m_iDel_RecentDay = temp_day - save_day;
	}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  저장 기간을 개월로 했을 경우
	int save_month = m_dlgSetup->m_SysPara.m_iPara[0];
	if( (temp_month - save_month) < 0)
	{
		m_iDel_Year = now_time.GetYear() - 1;

		for(i = 0; i <= (save_month - temp_month); i++)
		{
			if(temp_month == i+1)
			{
				m_iDel_Month = 12 - (save_month - temp_month);
			}
		}
	}
	else if( (temp_month - save_month) == 0)
	{
		m_iDel_Year = now_time.GetYear() - 1;

		m_iDel_Month = 12;
	}
	else
	{
		m_iDel_Year = now_time.GetYear();

		m_iDel_Month = temp_month - save_month;
	}
*/// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/*  //저장 기간 3개월 일 경우
	if( (temp_month - 2) <= 0)
	{
		m_iDel_Year = now_time.GetYear() - 1;

		if(temp_month == 1)
			m_iDel_Month = 10;
		else if(temp_month == 2)
			m_iDel_Month = 11;
	}
	else
	{
		m_iDel_Year = now_time.GetYear();

		if(temp_month == 3)
		{
			m_iDel_Year = now_time.GetYear() - 1;
			m_iDel_Month = 12;
		}
		else
			m_iDel_Month = temp_month - 3;
	}
	*/
	return TRUE;
}

BOOL CBearingInsDlg::DeleteFolderSearch(CString strFolder)
{
	CFileFind ff;
	CString strTarget = strFolder;
	CStringArray strFileArray;

	if(strTarget.Right(1) != '\\')
	{
		strTarget += '\\';
	}
	
	strTarget += "*.*";
	if(ff.FindFile(strTarget)==TRUE)
	{
		CString Filename;
		BOOL bFlag = true;
		while(bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if(ff.IsDots())
			{
				continue;
			}
			if(ff.IsDirectory())
			{
				return FALSE;
			}
		}
	}
	ff.Close();

	return TRUE;

}


BOOL CBearingInsDlg::DeleteFolder(CString strFolder)
{
	CFileFind ff;
	CString strTarget = strFolder;
	CStringArray strFileArray;

	if(strTarget.Right(1) != '\\')
	{
		strTarget += '\\';
	}
	
	strTarget += "*.*";
	if(ff.FindFile(strTarget)==TRUE)
	{
		CString Filename;
		BOOL bFlag = true;
		while(bFlag == TRUE)
		{
			bFlag = ff.FindNextFile();
			Filename = ff.GetFilePath();
			if(ff.IsDots())
			{
				continue;
			}
			if(ff.IsDirectory())
			{
				DeleteFolder(Filename);
				RemoveDirectory(Filename);
			}
			else
			{
				DeleteFile(Filename);
			}
		}
	}
	ff.Close();

	return TRUE;
}

BOOL CBearingInsDlg::StartAutoDelete_Img()
{
	CString Delete_Img_dir,Delete_Data_dir,Delete_Ng_Data_dir,Delete_Tab_Data_dir;
	CString Delete_Product_dir,Delete_Spc_dir;
	CTime now_time = CTime::GetCurrentTime();
	BOOL bDelFolder[31];
	typeSystemPara systemPara;
	systemPara = m_VisMod->GetSystemPara();
	
	for(int i = 0; i < m_AllModel.nMaxModelCnt; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			switch(j)
			{
			case 0:
				m_strImgPath.Format("%s\\GrabImage", m_AllModel.Model[i].szPath);
				break;
			case 1:
				m_strImgPath.Format("%s\\NGImage", m_AllModel.Model[i].szPath);
				break;
			case 2:
				m_strImgPath.Format("%s\\OKImage", m_AllModel.Model[i].szPath);
				break;
			}
			CheckDeleteDate();

			int nYear = m_iDel_Year;
			int nDelYear = now_time.GetYear();

			if(1 == now_time.GetMonth() && now_time.GetDay() < m_iDel_RecentDay)
			{
				nDelYear = m_iDel_Year;
			}
			else if(1 == now_time.GetMonth() && now_time.GetDay() > m_iDel_RecentDay)
			{
				nDelYear = m_iDel_Year - 1;
			}

			for(int nYear = m_iDel_Year; nYear >= nDelYear; nYear--)		// 110930 ytlee
			{
				if(nYear != now_time.GetYear())
				{
					m_iDel_Month = 12;
					m_iDel_Day = m_iDel_RecentDay;		// 120104 ytlee
				}

				for(int nMonth = m_iDel_Month; nMonth > 0; nMonth--)
				{
					if(nMonth == 1 || nMonth == 3 || nMonth == 5 ||
						nMonth == 7 || nMonth == 8 || nMonth == 10 || nMonth == 12)		// 120104 ytlee
					{
						m_iDel_Day = 31;
					}
					else if(nMonth == 4 || nMonth == 6 || nMonth == 9 || nMonth == 11)
					{
						m_iDel_Day = 30;
					}
					else if(nMonth == 2)
					{
						m_iDel_Day = 28;
					}
					if(nYear == now_time.GetYear() && (nMonth == now_time.GetMonth() || systemPara.nDay > now_time.GetDay()))		// 110901 ytlee
					{
						m_iDel_Day = m_iDel_RecentDay;
					}
					else
					{
						m_iDel_Day = 31;
					}

					for(int i = 0; i < 31; i++)
					{
						bDelFolder[i] = FALSE;
					}


					for(int nDay = 1; nDay <= m_iDel_Day; nDay++)
					{
						if(nMonth>9 && nDay>9)
							Delete_Img_dir.Format("%s\\%d%d%d",m_strImgPath, nYear, nMonth, nDay);
						else if(nMonth<10 && nDay>9)
							Delete_Img_dir.Format("%s\\%d0%d%d",m_strImgPath, nYear, nMonth, nDay);
						else if(nMonth>9 && nDay<10)
							Delete_Img_dir.Format("%s\\%d%d0%d",m_strImgPath, nYear, nMonth, nDay);
						else if(nMonth<10 && nDay<10)
							Delete_Img_dir.Format("%s\\%d0%d0%d",m_strImgPath, nYear, nMonth, nDay);

						CFileFind dbfile;

						if(dbfile.FindFile(Delete_Img_dir,0))
						{
							if(DeleteFolder(Delete_Img_dir))
								RemoveDirectory(Delete_Img_dir);
						}

						if(DeleteFolderSearch(Delete_Img_dir))
						{
							bDelFolder[nDay - 1] = TRUE;
						}
						else
						{
							bDelFolder[nDay - 1] = FALSE;
						}
					}

					for(int i = 0; i < m_iDel_Day; i++)
					{
						if(!bDelFolder[i])
						{
							return FALSE;
						}
					}
				}
			}
		}
	}

	return TRUE;
}