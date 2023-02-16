// VisionModuleBearing.h : VisionModuleBearing DLL의 기본 헤더 파일입니다.
//

#pragma once
#ifdef PTSMOUSEKEYBOARDCTRLEXPORT
 #define PTSMOUSEKEYBOARDCTRL_EXPORT _declspec(dllexport)
#else 
 #define PTSMOUSEKEYBOARDCTRL_EXPORT _declspec(dllimport)
#endif



#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include "_FuncModuleBasic.h"
#include "include/hasp_api.h"
//#define DCF

#define DB_PATH					"D:\\DATABASE\\MODEL"
#define REG_PATH				"SOFTWARE\\BearingIns\\MODEL"
#define LOG_PATH				"D:\\DATABASE\\LOG"
#define GRAB_IMAGE_SAVE_PATH	"\\GrabImage\\"
#define GRAB_IMAGE_NAME			"GrabImage.bmp"
#define INSPECTION_PARA			"\\inspection.par"
#define SYSTEM_PARA				"D:\\DATABASE\\System.par"

#define MAIN_DISP				0
#define NG_DISP_1				1
#define NG_DISP_2				2
#define NG_DISP_3				3

#define MAX_DISP				4
#define CAMERA_WIDTH			1024
#define CAMERA_HEIGHT			2480

#define MAX_CAMERA_WIDTH		1024
#define MAX_CAMERA_HEIGHT		5000

#define LIST_MONI_NUM			50
#define DAT_STR_LENG			512
#define MAX_INS_PARA			100
#define MAX_INT_PARA			MAX_INS_PARA
#define MAX_DOUBLE_PARA			MAX_INT_PARA

#define MAX_LIGHT_PARA			4

#define PATTERN_FIND_MAX_NUM	1
#define EDGE_POS_MAX_NUM		10

#define	USER_MSG_IMG_RECT		WM_USER+10 

#define INS_RGN_MAX_NUM			100
#define TRACKER_MAX_NUM			INS_RGN_MAX_NUM

#define MAX_INSPECTION			3

#define GRAB_IMAGE_SAVE			0
#define NG_IMAGE_SAVE			1

#define MAX_NEEDLE_CNT			5

typedef struct ModelInfo
{
	CString strModelName;
	CString strModelPath;
	CString strModelDesc;
	double dModelWidth;
	double dModelHeight;
	int nModelCnt;
	double dModelPI;
}typeModelInfo;

typedef struct ModelParam
{
	double dModelWidth;
	double dModelHeight;
	int nModelCnt;
	double dModelPI;
	double dModelGap;
	double dDiameter;
	int nCol;
}typeModelPara;

typedef struct InspParam
{
	CRect InsRegion[MAX_INS_PARA];
	BOOL bCreatMdID[MAX_INS_PARA];
	BOOL bInspWidth[MAX_INS_PARA];
	BOOL bInspHeight[MAX_INS_PARA];
	BOOL bInspArea[MAX_INS_PARA];
	BOOL bInspLength[MAX_INS_PARA];
	BOOL bInspBroken;
	int iScore;
	int iAlignScore;
	int iAlignType;
	double dInspThreshold;
	double dAlignX[MAX_NEEDLE_CNT];
	double dAlignY[MAX_NEEDLE_CNT];
	double dAlignAng[MAX_NEEDLE_CNT];
	double dInspWidth[MAX_INS_PARA];
	double dInspHeight[MAX_INS_PARA];
	double dInspArea[MAX_INS_PARA];
	double dInspLength[MAX_INS_PARA];
	double dInspOffSetX[MAX_INS_PARA];
	double dInspOffSetY[MAX_INS_PARA];
	double dInspOffSetA[MAX_INS_PARA];
	double dInspOffSetZ[MAX_INS_PARA];
	double dInspEdgeThreshHeigh;
	double dInspBright;
	double dInspBrokenLength;
	double dInspSmooth;
	CPoint InsCenterPoint[MAX_INS_PARA];
	CPoint InsCenterPointOffSet;
	MIL_ID MilSearchContext[MAX_INS_PARA];
}typeInspPara;

typedef struct SetupParam
{
	typeModelPara ModelPara;
	typeInspPara InspPara;
	int iLight[MAX_LIGHT_PARA];
	int iMoveCnt;
	double dPixelSize;
	BOOL bSaveImage;
	BOOL bNGSaveImage;
	BOOL bOKSaveImage;
}typeSetupPara;

typedef struct RsltWidthPoint
{
	double dStartX;
	double dEndX;
}typeRsltWidthPoint;

typedef struct RsltHeightPoint
{
	double dStartY;
	double dEndY;
}typeRsltHeightPoint;

typedef struct RsltPoint
{
	typeRsltWidthPoint RsltWidthPoint;
	typeRsltHeightPoint RsltHeightPoint;
	double dWidth;
	double dHeight;
	double dCentX;
	double dCentY;
	double dOffsetY;
	//MIL_ID MIL_NG_Meas[MAX_INS_PARA];
}typeRsltPoint;

typedef struct InsRsltParam
{
	double dCentX[MAX_INS_PARA];
	double dCentY[MAX_INS_PARA];
	double dWidth[MAX_INS_PARA];
	double dHeight[MAX_INS_PARA];
	BOOL bCheck[MAX_INS_PARA];
	BOOL bCentX_NG[MAX_INS_PARA];
	BOOL bCentY_NG[MAX_INS_PARA];
	BOOL bWidth_NG[MAX_INS_PARA];
	BOOL bHeight_NG[MAX_INS_PARA];
	int nFindCnt;

	CString strNgName[MAX_INS_PARA];
	CPoint poNgPoint[MAX_INS_PARA];
	CRect NgRect[MAX_INS_PARA];
	BOOL bRslt[MAX_INS_PARA][MAX_INSPECTION];
	BOOL bAlign;
	int nNgCnt;
	double dAlignX;
	double dAlignY;
	typeRsltPoint RsltPoint[MAX_INS_PARA];
}typeInsRsltPara;

typedef struct InsRsltCntParam
{
	int nTotalCnt;
	int nOk1Cnt;
	int nNg1Cnt;
	int nOk2Cnt;
	int nNg2Cnt;
}typeInsCntPara;

typedef struct SystemParam
{
	int nHour;
	int nMinute;
	int nDay;
}typeSystemPara;

class AFX_EXT_CLASS CVisionModule
{
public:
	CVisionModule();
	~CVisionModule();
private:
	MIL_ID m_MilApplication;
	MIL_ID m_MilSystem;
#ifdef DCF
	MIL_ID m_MilDigitizer;
#endif
	MIL_ID m_MilGrabBuf;
	MIL_ID m_MilDisplay[MAX_DISP];
	MIL_ID m_MilDispBuf[MAX_DISP];

	MIL_ID m_MilOverlay[MAX_DISP];

	
	hasp_status_t m_SafeNet_USB_status;
	hasp_feature_t m_SafeNet_USB_feature;
	hasp_handle_t m_SafeNet_USB_handle;
	
	typeSetupPara m_SetupPara;
	typeSystemPara m_SystemPara;
public:
	BOOL Open(HWND *_disp_hwnd, double dZoom_rat);
	BOOL Grab();
	BOOL Close();

	BOOL SafeNet_Lock_LogIn();
	void SafeNet_Lock_LogOut();

	void SetSetupPara(typeSetupPara setupPara);
	typeSetupPara GetSetupPara();

	void SetSystemPara(typeSystemPara systemPara);
	typeSystemPara GetSystemPara();

	BOOL SaveSetupPara(CString iniPath);
	BOOL LoadSetupPara(CString iniPath);

	BOOL SaveSystemPara();
	BOOL LoadSystemPara();

	void SaveImage(CString _path);

	MIL_ID *GetMilSystem();
//#ifdef DCF
	MIL_ID *GetMilDigitizer();
//#endif
	MIL_ID *GetMilGrabBuf();
	MIL_ID *GetMilDisplay(int index);
	MIL_ID *GetMilDispBuf(int index);
	MIL_ID *GetMilOverlay(int index);
};

// CVisionModuleBearingApp
// 이 클래스의 구현을 보려면 VisionModuleBearing.cpp를 참조하십시오.
//
/*
class CVisionModuleBearingApp : public CWinApp
{
public:
	CVisionModuleBearingApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
*/