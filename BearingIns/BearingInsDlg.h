
// BearingInsDlg.h : 헤더 파일
//

#pragma once
#include "mil.h"
#include "DialogRect.h"
#include "BtnEnh.h"
#include "DialogSetup.h"
#include "DialogModel.h"
#include "DialogProcessing.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "VisionModuleBearing.h"
#include "SerialPort.h"
#include "TriggerBoard.h"
#include "comi/Comidas.h"
#include "_CtrlModelManageDlg.h"

#define IMAGE_RECT_DIALOG	10000

#define	USER_MSG_IMG_RECT			WM_USER+10 
#define USER_MSG_PROCESS			WM_USER+11

// CBearingInsDlg 대화 상자
class CBearingInsDlg : public CDialog
{
// 생성입니다.
public:
	CBearingInsDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BEARINGINS_DIALOG };

	int m_test;

	CVisionModule *m_VisMod;
	CTriggerBoard *m_Trigger;
	CDialogRect *m_DlgRect;
	CDialogSetup *m_dlgSetup;
	CDialogProcessing m_dlgProcessing;

	CSerialPort m_cSerial;

	HANDLE m_hComiDevice32;

	CWinThread   *m_pMainThread;
	static UINT InspectThread(LPVOID pParam);
	BOOL m_bInspectStart;
	BOOL m_bInspectEnd;

	CWinThread *m_pIOThread;
	static UINT IOThread(LPVOID pParam);
	BOOL m_bIOCheck;
	BOOL m_bOUTCheck;
	BOOL m_bIOEnd;
	BOOL m_bInspectInput;

	CWinThread *m_pAutoDeleteThread;
	static UINT AutoDeleteThread(LPVOID pParam);
	BOOL m_bAutoDeleteStart;
	BOOL m_bAutoDeleteEnd;


	double m_EndTime;
	double m_StartTime;
	double m_ProcessTime;

	CString m_strDBPath;
	CString m_strRegiPath;
	CString m_strLogPath;
	CString m_strInspDate;
	CString m_strInspDateTime;

	BOOL m_bWidthIns;
	BOOL m_bHeightIns;
	BOOL m_bAreaIns;

	typeSetupPara m_SetupPara;
	typeModelInfo m_ModelInfo;
	typeInsRsltPara m_InsRsltPara;
	typeInsCntPara m_InsCntPara;
	
	double m_dZoomDefault;
	double m_dZoomRatio;
	CPoint m_poScroll;

	int m_nTemp_Space;

	CScrollBar m_ScrMain_V;
	CScrollBar m_ScrMain_H;

	CBtnEnh m_Live;
	CBtnEnh m_btnStartStop;
	CBtnEnh m_btnStart;

	CBtnEnh m_btnResult;

	//Insp Cnt
	CBtnEnh m_btnTotal;
	CBtnEnh m_btnOK1;
	CBtnEnh m_btnNG1;
	CBtnEnh m_btnOK2;
	CBtnEnh m_btnNG2;
	CBtnEnh m_btnSetup;
	CBtnEnh m_btnModel;
	CBtnEnh m_btnExit;

	BOOL m_chk_OUT1;
	BOOL m_chk_OUT2;
	BOOL m_chk_OUT3;
	BOOL m_chk_OUT4;
	
	CListCtrl m_list_rslt;

	BOOL m_bDelete;
	int m_iDel_Year, m_iDel_Month, m_iDel_Day, m_iDel_RecentDay;
	CString m_strImgPath;

	typeAllModel m_AllModel;

	void InitInsRsltPara();
	void InitOutput();

	BYTE byValue2Hexa_High(BYTE byData);
	BYTE byValue2Hexa_Low(BYTE byData);
	BYTE byValue2NibbleAscii(BYTE byData);
	void SetLineLight(int ch, int var);

	BOOL InspectionProcess();
	int Inspection();
	int InspectionEdge();
	int InspectionMod();
	int InspectionBlob();
	int InspectionMeas(BYTE *ins_image, CPoint center);//int ins_idx, CPoint center);
	void MorphologyErosion(BYTE *byte_source, int left, int top, int width, int height);
	void MorphologyDilation(BYTE *byte_source, int left, int top, int width, int height);
	void BirImage(BYTE *byte_source, int width, int height, int pitch);

	void SetInsCnt();
	BOOL RsltSave(int rslt_type, BOOL file_first = FALSE);
	void RsltView(int rslt);

	void ModelInfoLoad(BOOL isParaLoad = TRUE);
	void ModelInfoView();
	int HandleAllMessage();
	void UpdateLight(BOOL isUpdate);
	void WriteStatusList(LPCTSTR lpszItem);
	BOOL WriteLogStatus(CString path, CString file, CString bak_file, CString log, long limit_line);


	BOOL StartAutoDelete_Img();
	BOOL CheckDeleteDate();
	BOOL DeleteFolder(CString strFolder);
	BOOL DeleteFolderSearch(CString path);

	LRESULT OnProcessMessage(WPARAM para0, LPARAM para1);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;
	
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

public:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlExit();
	void ClickBtnenhctrlLive();
	void ClickBtnenhctrlSave();
	void ClickBtnenhctrlLoad();
	void ClickBtnenhctrlZoomIn();
	void ClickBtnenhctrlZoomOut();
	void ClickBtnenhctrlZoom11();
	void ClickBtnenhctrlZoomFit();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void ClickBtnenhctrlModel();
	void ClickBtnenhctrlSetup();
	CBtnEnh m_label_Model_Name;
	CBtnEnh m_label_Model_Width;
	CBtnEnh m_label_Model_Height;
	CBtnEnh m_label_Model_Cnt;
	CBtnEnh m_label_Model_PI;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonTest();
	void ClickBtnenhctrlRun();
	afx_msg void OnBnClickedButtonTest2();
	
	afx_msg void OnBnClickedButtonOut0();
	afx_msg void OnBnClickedButtonOut1();
	afx_msg void OnBnClickedButtonOut2();
	afx_msg void OnBnClickedButtonOut3();
	afx_msg void OnBnClickedButtonOut4();
	afx_msg void OnBnClickedButtonOut5();
	afx_msg void OnBnClickedButtonOut6();
	afx_msg void OnBnClickedButtonOut7();
	afx_msg void OnBnClickedButtonOut8();
	afx_msg void OnBnClickedButtonOut9();
	afx_msg void OnBnClickedButton1();
	void ClickBtnenhctrlCntReset();
};

