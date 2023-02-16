#include "stdafx.h"
#include "VisionModuleBearing.h"

//USB key 코드
unsigned char m_SafeNet_USB_vendor_code[] =
"6LCHxDCsAh5qK0OZUDIfbf4aLTYbXbz0bWitlU0/K24xtYoDMTgLEY6I+zwlqUqTW7gooSghysUj8YdV"
"NpgQUcDCBprT2tGa+DPInMWVq3aXeBcJMQyeumBk4JhXnB8WMWbc1QEF2q5S+WD7OtDpvLIk8EBEOChT"
"xRtyF+HbeEAcDFfJnmoBz95nzWI7GRwdLYHyIXXHqKPTczKzN3K03SmewwS/3JOyUa9dpfJIcwl362Sj"
"4IaCNQLAC/wycEGmJ4ADX/FlZA4cCNgHiK5Qllwl/0tl33J/L8wIP8VBh6xUk/bDckAOuB0X6f5ZQ4e/"
"4S6kyCIquM84+OmJHpjo670oH/kDGspjsQzhZR6YaMJuPGAhCaD11AcjW7gwgxnIvjwaL6oe3mVYpHbI"
"GKoKKbH1dWOmuwgEW61lv0a7DSrvaCHYQAYVOWIn8cZ8IeSyxvnCSr3jGFSnbN3bxWa7jDk/qFcCCma3"
"X+T4Ok49iQbrEMnatE/yWpcBEErGBGFSeMDoQbucdxyoZll1HucNJ/MdLzabon4qeyC9FcL7v+anKH+F"
"7pUy4XlFFFWVJ1itujg5HwqUK8zBgssGRPykGjxnclA1PrzgH8mrEaLyhuSWi0xejtJsIMdcovM1xjr6"
"TUSyyToTxCFA05+15CHGEfR5uaNDy8DTivtiv8Fg+UuGcW2aKTcRhzbsstwYUfcPl8tZ/hv8wP6J2I7y"
"qCO8GRfQTQU+L2jQfO2xVMn2gAm9n4zJTvRLXye2nP4+V7BhZrabOG75zwjJUf5E+u1xcqAlkxLea/40"
"ld2d68BkE8Cy1gdrlkh5t9TCS/EQYZNSQtA/jM5DnOfl1PycCGhOJqW2GbNjt4bTmmm5edbO60VWFRiP"
"kyWPhTW00uVQfaq+NVXSnxXAjzxCj+GnJEE4K0t80zB+KLG1ly0FotDfOaluef8t4xYBUr8VHoE6Vi9Q"
"zo/W0nFU91uV6k3yPr4nsA==";

CVisionModule::CVisionModule()
{
	m_SetupPara.dPixelSize = 0.061;
	m_SetupPara.iMoveCnt = 0;
	m_SetupPara.ModelPara.dModelWidth = 0.;
	m_SetupPara.ModelPara.dModelHeight = 0.;
	m_SetupPara.ModelPara.nModelCnt = 0;
	m_SetupPara.ModelPara.dModelPI = 0.;
	m_SetupPara.ModelPara.dModelGap = 0.;
	m_SetupPara.ModelPara.dDiameter = 0.;
	m_SetupPara.ModelPara.nCol = 1;

	m_SetupPara.InspPara.iScore = 0;
	m_SetupPara.InspPara.dInspThreshold = 0.;
	m_SetupPara.InspPara.iAlignScore = 0;
	m_SetupPara.InspPara.iAlignType = 0;
	
	
	m_SetupPara.InspPara.bInspBroken = FALSE;
	m_SetupPara.bSaveImage = FALSE;
	m_SetupPara.bNGSaveImage = FALSE;
	m_SetupPara.bOKSaveImage = FALSE;
	m_SetupPara.InspPara.dInspEdgeThreshHeigh = 0.;
	m_SetupPara.InspPara.dInspSmooth = 0.;

	m_SetupPara.InspPara.dInspBright = 30;
	m_SetupPara.InspPara.dInspBrokenLength = 5;

	m_SetupPara.InspPara.InsCenterPointOffSet.SetPoint(0, 0);

	for(int i = 0; i < MAX_INS_PARA; i++)
	{
		m_SetupPara.InspPara.MilSearchContext[i] = NULL;
		m_SetupPara.InspPara.dAlignX[i] = 0.;
		m_SetupPara.InspPara.dAlignY[i] = 0.;
		m_SetupPara.InspPara.dAlignAng[i] = 0.;
		m_SetupPara.InspPara.bCreatMdID[i] = FALSE;
		m_SetupPara.InspPara.InsRegion[i].SetRect(100,100,200,200);

		m_SetupPara.InspPara.bInspArea[i] = FALSE;
		m_SetupPara.InspPara.bInspHeight[i] = FALSE;
		m_SetupPara.InspPara.bInspWidth[i] = FALSE;
		m_SetupPara.InspPara.bInspLength[i] = FALSE;
		
		m_SetupPara.InspPara.dInspArea[i] = 0.;
		m_SetupPara.InspPara.dInspWidth[i] = 0.;
		m_SetupPara.InspPara.dInspHeight[i] = 0.;
		m_SetupPara.InspPara.dInspLength[i] = 0.;

		m_SetupPara.InspPara.dInspOffSetX[i] = 0.;
		m_SetupPara.InspPara.dInspOffSetY[i] = 0.;
		m_SetupPara.InspPara.dInspOffSetA[i] = 0.;
		m_SetupPara.InspPara.dInspOffSetZ[i] = 0.;
		m_SetupPara.InspPara.InsCenterPoint[i].SetPoint(0, 0);
	}
}

CVisionModule::~CVisionModule()
{
	
}


BOOL CVisionModule::Open(HWND *_disp_hwnd, double dZoom_rat)
{
	char dcf_path[100];
	memset(dcf_path, NULL, 100);
	MappAlloc(M_DEFAULT, &m_MilApplication);
#ifndef DCF
	MsysAlloc(M_SYSTEM_HOST, 0, M_DEFAULT, &m_MilSystem);
#else
	MsysAlloc(M_SYSTEM_DEFAULT, 0, M_DEFAULT, &m_MilSystem);
	sprintf(dcf_path, "%s", "D:\\Project\\dcf\\1k_ext.dcf");
	MdigAlloc(m_MilSystem, 0, dcf_path, M_DEFAULT, &m_MilDigitizer);

	MdigControl(m_MilDigitizer, M_GRAB_MODE, M_ASYNCHRONOUS);
	MdigControl(m_MilDigitizer, M_GRAB_TIMEOUT, M_INFINITE);
	MdigControl(m_MilDigitizer, M_GRAB_TRIGGER, M_ACTIVATE);
	MdigControl(m_MilDigitizer, M_GRAB_TRIGGER_SOURCE, M_HARDWARE_PORT10);

	MdigControl(m_MilDigitizer, M_GRAB_ABORT, M_DEFAULT);
#endif

	MbufAlloc2d(m_MilSystem, CAMERA_WIDTH, CAMERA_HEIGHT, 8 + M_UNSIGNED, M_IMAGE + M_PROC + M_GRAB, &m_MilGrabBuf);
	MbufClear(m_MilGrabBuf, 0x00);
	for(int i = 0; i < MAX_DISP; i++)
	{
		MbufAlloc2d(m_MilSystem, MAX_CAMERA_WIDTH, MAX_CAMERA_HEIGHT, 8 + M_UNSIGNED, M_IMAGE + M_DISP + M_PROC, &m_MilDispBuf[i]);
		MbufClear(m_MilDispBuf[i], 0x00);

		MdispAlloc(m_MilSystem, M_DEFAULT, "M_DEFAULT", M_WINDOWED, &m_MilDisplay[i]);
		MdispSelectWindow(m_MilDisplay[i], m_MilDispBuf[i], _disp_hwnd[i]);
		MdispControl(m_MilDisplay[i], M_INTERPOLATION_MODE, M_FAST);

		if(i == MAIN_DISP)
		{
			MdispZoom(m_MilDisplay[i], dZoom_rat, dZoom_rat);
		}
		else
		{
			MdispZoom(m_MilDisplay[i], 0.9, 0.9);
		}

		MdispControl(m_MilDisplay[i], M_OVERLAY, M_ENABLE);
		MdispInquire(m_MilDisplay[i], M_OVERLAY_ID, &m_MilOverlay[i]);
	}	

#ifdef TRIGGER
	
#endif

	return TRUE;
}
BOOL CVisionModule::Grab()
{
#ifdef DCF
	MdigGrab(m_MilDigitizer, m_MilGrabBuf);
	MdigGrabWait(m_MilDigitizer, M_GRAB_FRAME_END);
	MdigControl(m_MilDigitizer, M_GRAB_ABORT, M_DEFAULT);	

	double diameter = 0.;
	diameter = m_SetupPara.ModelPara.dDiameter / 45.0;

	MimResize(m_MilGrabBuf, m_MilDispBuf[MAIN_DISP], 1, diameter, M_DEFAULT );
#endif
	return TRUE;
}

void CVisionModule::SaveImage(CString _path)
{
	CTime cTime;
	cTime = CTime::GetCurrentTime();
	CString path;

	path.Format("%s%04d%02d%02d_%02d%02d%02d.bmp", _path, cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());
	MbufExport((char *)(LPCTSTR)path, M_BMP, m_MilDispBuf[MAIN_DISP]);
}

BOOL CVisionModule::Close()
{
	for(int i = 0; i < MAX_DISP; i++)
	{
		MdispFree(m_MilDisplay[i]);
		MbufFree(m_MilDispBuf[i]);
	}
	MbufFree(m_MilGrabBuf);
	
#ifdef DCF
	MdigFree(m_MilDigitizer);
#endif
	MsysFree(m_MilSystem);
 	MappFree(m_MilApplication);

	return TRUE;
}

MIL_ID *CVisionModule::GetMilSystem()
{
	return &m_MilSystem;
}
#ifdef DCF
MIL_ID *CVisionModule::GetMilDigitizer()
{
	return &m_MilDigitizer;
}
#endif

MIL_ID *CVisionModule::GetMilGrabBuf()
{
	return &m_MilGrabBuf;
}

MIL_ID *CVisionModule::GetMilDisplay(int index)
{
	return &m_MilDisplay[index];
}

MIL_ID *CVisionModule::GetMilDispBuf(int index)
{
	return &m_MilDispBuf[index];
}

MIL_ID *CVisionModule::GetMilOverlay(int index)
{
	return &m_MilOverlay[index];
}

void CVisionModule::SetSetupPara(typeSetupPara setupPara)
{
	m_SetupPara = setupPara;
}

typeSetupPara CVisionModule::GetSetupPara()
{
	return m_SetupPara;
}

void CVisionModule::SetSystemPara(typeSystemPara systemPara)
{
	m_SystemPara = systemPara;
}

typeSystemPara CVisionModule::GetSystemPara()
{
	return m_SystemPara;
}


BOOL CVisionModule::SaveSetupPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	int i;
	CString file_path, pat_path, extention, master_path, strModPath;

	file_path = path + INSPECTION_PARA;

	//file 존재 확인 
	HANDLE fd=CreateFile(   file_path,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);

		//write INI
		key.Empty();
		key.Format("Move_Cnt");
		dat.Empty();
		dat.Format("%d", m_SetupPara.iMoveCnt);
		WritePrivateProfileString("SETUP_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Pixel_Size");
		dat.Empty();
		dat.Format("%.5f", m_SetupPara.dPixelSize);
		WritePrivateProfileString("SETUP_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Grab_Save");
		dat.Empty();
		dat.Format("%d", m_SetupPara.bSaveImage);
		WritePrivateProfileString("SETUP_PARA", key, dat, file_path);

		key.Empty();
		key.Format("OK_Save");
		dat.Empty();
		dat.Format("%d", m_SetupPara.bOKSaveImage);
		WritePrivateProfileString("SETUP_PARA", key, dat, file_path);

		key.Empty();
		key.Format("NG_Save");
		dat.Empty();
		dat.Format("%d", m_SetupPara.bNGSaveImage);
		WritePrivateProfileString("SETUP_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Model_Width");
		dat.Empty();
		dat.Format("%.5f", m_SetupPara.ModelPara.dModelWidth);
		WritePrivateProfileString("MODEL_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Model_Hegith");
		dat.Empty();
		dat.Format("%.5f", m_SetupPara.ModelPara.dModelHeight);
		WritePrivateProfileString("MODEL_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Model_Cnt");
		dat.Empty();
		dat.Format("%d", m_SetupPara.ModelPara.nModelCnt);
		WritePrivateProfileString("MODEL_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Model_PI");
		dat.Empty();
		dat.Format("%.5f", m_SetupPara.ModelPara.dModelPI);
		WritePrivateProfileString("MODEL_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Model_Gap");
		dat.Empty();
		dat.Format("%.5f", m_SetupPara.ModelPara.dModelGap);
		WritePrivateProfileString("MODEL_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Model_Diameter");
		dat.Empty();
		dat.Format("%.5f", m_SetupPara.ModelPara.dDiameter);
		WritePrivateProfileString("MODEL_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Model_Col");
		dat.Empty();
		dat.Format("%d", m_SetupPara.ModelPara.nCol);
		WritePrivateProfileString("MODEL_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Align_Score");
		dat.Empty();
		dat.Format("%d", m_SetupPara.InspPara.iAlignScore);
		WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);
		
		key.Empty();
		key.Format("Threshold");
		dat.Empty();
		dat.Format("%.5f", m_SetupPara.InspPara.dInspThreshold);
		WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Align_Type");
		dat.Empty();
		dat.Format("%.5f", m_SetupPara.InspPara.iAlignType);
		WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

		for(i = 0; i < MAX_NEEDLE_CNT; i++)
		{
			key.Empty();
			key.Format("Align_X_%d",i);
			dat.Empty();
			dat.Format("%.5f", m_SetupPara.InspPara.dAlignX[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Align_Y_%d",i);
			dat.Empty();
			dat.Format("%.5f", m_SetupPara.InspPara.dAlignY[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Align_Ang_%d",i);
			dat.Empty();
			dat.Format("%.5f", m_SetupPara.InspPara.dAlignAng[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);
		}

		key.Empty();
		key.Format("Insp_Score");
		dat.Empty();
		dat.Format("%d", m_SetupPara.InspPara.iScore);
		WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Insp_EdgeThreshHeigh");
		dat.Empty();
		dat.Format("%.5f", m_SetupPara.InspPara.dInspEdgeThreshHeigh);
		WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Insp_Smooth");
		dat.Empty();
		dat.Format("%.5f", m_SetupPara.InspPara.dInspSmooth);
		WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Insp_Broken");
		dat.Empty();
		dat.Format("%d", m_SetupPara.InspPara.bInspBroken);
		WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Insp_Broken_Bright");
		dat.Empty();
		dat.Format("%.5f", m_SetupPara.InspPara.dInspBright);
		WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Insp_Broken_Length");
		dat.Empty();
		dat.Format("%.5f", m_SetupPara.InspPara.dInspBrokenLength);
		WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Insp_CenterPointOffSet_X");
		dat.Empty();
		dat.Format("%d", m_SetupPara.InspPara.InsCenterPointOffSet.x);
		WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

		key.Empty();
		key.Format("Insp_CenterPointOffSet_Y");
		dat.Empty();
		dat.Format("%d", m_SetupPara.InspPara.InsCenterPointOffSet.y);
		WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

		for(i = 0; i < MAX_INS_PARA; i++)
		{
			key.Empty();
			key.Format("Insp_CenterPoint_X[%d]", i);
			dat.Empty();
			dat.Format("%d", m_SetupPara.InspPara.InsCenterPoint[i].x);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_CenterPoint_Y[%d]", i);
			dat.Empty();
			dat.Format("%d", m_SetupPara.InspPara.InsCenterPoint[i].y);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_CreatMdID[%d]", i);
			dat.Empty();
			dat.Format("%d", m_SetupPara.InspPara.bCreatMdID[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_Region[%d]_Left", i);
			dat.Empty();
			dat.Format("%d", m_SetupPara.InspPara.InsRegion[i].left);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_Region[%d]_Top", i);
			dat.Empty();
			dat.Format("%d", m_SetupPara.InspPara.InsRegion[i].top);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_Region[%d]_Right", i);
			dat.Empty();
			dat.Format("%d", m_SetupPara.InspPara.InsRegion[i].right);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_Region[%d]_Bottom", i);
			dat.Empty();
			dat.Format("%d", m_SetupPara.InspPara.InsRegion[i].bottom);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_Length[%d]", i);
			dat.Empty();
			dat.Format("%d", m_SetupPara.InspPara.bInspLength[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_Length_Size[%d]", i);
			dat.Empty();
			dat.Format("%.5f", m_SetupPara.InspPara.dInspLength[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_Area[%d]", i);
			dat.Empty();
			dat.Format("%d", m_SetupPara.InspPara.bInspArea[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_Area_Size[%d]", i);
			dat.Empty();
			dat.Format("%.5f", m_SetupPara.InspPara.dInspArea[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_Width[%d]", i);
			dat.Empty();
			dat.Format("%d", m_SetupPara.InspPara.bInspWidth[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_Width_Size[%d]", i);
			dat.Empty();
			dat.Format("%.5f", m_SetupPara.InspPara.dInspWidth[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_Height[%d]", i);
			dat.Empty();
			dat.Format("%d", m_SetupPara.InspPara.bInspHeight[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_Height_Size[%d]", i);
			dat.Empty();
			dat.Format("%.5f", m_SetupPara.InspPara.dInspHeight[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);
			
			key.Empty();
			key.Format("Insp_OffSet_X[%d]", i);
			dat.Empty();
			dat.Format("%.5f", m_SetupPara.InspPara.dInspOffSetX[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_OffSet_Y[%d]", i);
			dat.Empty();
			dat.Format("%.5f", m_SetupPara.InspPara.dInspOffSetY[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);

			key.Empty();
			key.Format("Insp_OffSet_A[%d]", i);
			dat.Empty();
			dat.Format("%.5f", m_SetupPara.InspPara.dInspOffSetA[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);
				
			key.Empty();
			key.Format("Insp_OffSet_Z[%d]", i);
			dat.Empty();
			dat.Format("%.5f", m_SetupPara.InspPara.dInspOffSetZ[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);
		}

		for(int i = 0; i < MAX_LIGHT_PARA; i++)
		{
			key.Empty();
			key.Format("LIGHT_[%d]", i);
			dat.Empty();
			dat.Format("%d", m_SetupPara.iLight[i]);
			WritePrivateProfileString("INSPECTION_PARA", key, dat, file_path);
		}
	}

	return TRUE;
}

BOOL CVisionModule::LoadSetupPara(CString path)
{
	char temp[DAT_STR_LENG];
	BOOL rslt = TRUE;
	CString key, dat;
	CString file_path, strModPath;
	int i;

	file_path = path + INSPECTION_PARA;
	//file 존재 확인 
	HANDLE fd = CreateFile( file_path,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);

		//Read INI
		key.Empty();
		key.Format("Move_Cnt");
		GetPrivateProfileString("SETUP_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.iMoveCnt = atoi(dat);

		key.Empty();
		key.Format("Pixel_Size");
		GetPrivateProfileString("SETUP_PARA", key, _T("0.06100"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.dPixelSize = atof(dat);

		key.Empty();
		key.Format("Grab_Save");
		GetPrivateProfileString("SETUP_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.bSaveImage = atoi(dat);

		key.Empty();
		key.Format("OK_Save");
		GetPrivateProfileString("SETUP_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.bOKSaveImage = atoi(dat);

		key.Empty();
		key.Format("NG_Save");
		GetPrivateProfileString("SETUP_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.bNGSaveImage = atoi(dat);

		key.Empty();
		key.Format("Model_Width");
		GetPrivateProfileString("MODEL_PARA", key, _T("0.00000"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.ModelPara.dModelWidth = atof(dat);

		key.Empty();
		key.Format("Model_Hegith");
		GetPrivateProfileString("MODEL_PARA", key, _T("0.00000"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.ModelPara.dModelHeight = atof(dat);

		key.Empty();
		key.Format("Model_Cnt");
		GetPrivateProfileString("MODEL_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.ModelPara.nModelCnt = atoi(dat);

		key.Empty();
		key.Format("Model_PI");
		GetPrivateProfileString("MODEL_PARA", key, _T("0.00000"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.ModelPara.dModelPI = atof(dat);

		key.Empty();
		key.Format("Model_Gap");
		GetPrivateProfileString("MODEL_PARA", key, _T("0.00000"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.ModelPara.dModelGap = atof(dat);

		key.Empty();
		key.Format("Model_Diameter");
		GetPrivateProfileString("MODEL_PARA", key, _T("0.00000"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.ModelPara.dDiameter = atof(dat);

		key.Empty();
		key.Format("Model_Col");
		GetPrivateProfileString("MODEL_PARA", key, _T("1"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.ModelPara.nCol = atoi(dat);

		
		
		key.Empty();
		key.Format("Align_Score");
		GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.InspPara.iAlignScore = atoi(dat);

		key.Empty();
		key.Format("Threshold");
		GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.InspPara.dInspThreshold = atof(dat);

		key.Empty();
		key.Format("Align_Type");
		GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.InspPara.iAlignType = atoi(dat);

		for(i = 0; i < MAX_NEEDLE_CNT; i++)
		{
			key.Empty();
			key.Format("Align_X_%d",i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.dAlignX[i] = atof(dat);

			key.Empty();
			key.Format("Align_Y_%d",i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.dAlignY[i] = atof(dat);

			key.Empty();
			key.Format("Align_Ang_%d",i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.dAlignAng[i] = atof(dat);
		}

		key.Empty();
		key.Format("Insp_Score");
		GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.InspPara.iScore = atoi(dat);

		key.Empty();
		key.Format("Insp_EdgeThreshHeigh");
		GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.InspPara.dInspEdgeThreshHeigh = atof(dat);

		key.Empty();
		key.Format("Insp_Smooth");
		GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.InspPara.dInspSmooth = atof(dat);

		key.Empty();
		key.Format("Insp_Broken");
		GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.InspPara.bInspBroken = atoi(dat);

		m_SetupPara.InspPara.dInspBright = atof(dat);
		key.Empty();
		key.Format("Insp_Broken_Bright");
		GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.InspPara.dInspBright = atof(dat);

		key.Empty();
		key.Format("Insp_Broken_Length");
		GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.InspPara.dInspBrokenLength = atof(dat);

		key.Empty();
		key.Format("Insp_CenterPointOffSet_X");
		GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.InspPara.InsCenterPointOffSet.x = atoi(dat);

		key.Empty();
		key.Format("Insp_CenterPointOffSet_Y");
		GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SetupPara.InspPara.InsCenterPointOffSet.y = atoi(dat);

		for(i = 0; i < MAX_INS_PARA; i++)
		{
			key.Empty();
			key.Format("Insp_CenterPoint_X[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.InsCenterPoint[i].x = atoi(dat);

			key.Empty();
			key.Format("Insp_CenterPoint_Y[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.InsCenterPoint[i].y = atoi(dat);

			key.Empty();
			key.Format("Insp_CreatMdID[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.bCreatMdID[i] = atoi(dat);

			key.Empty();
			key.Format("Insp_Region[%d]_Left", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			if(atoi(dat) < 0)
			{
				dat.Format("100");
			}
			m_SetupPara.InspPara.InsRegion[i].left = atoi(dat);

			key.Empty();
			key.Format("Insp_Region[%d]_Top", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			if(atoi(dat) < 0)
			{
				dat.Format("100");
			}
			m_SetupPara.InspPara.InsRegion[i].top = atoi(dat);

			key.Empty();
			key.Format("Insp_Region[%d]_Right", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			if(atoi(dat) < 0)
			{
				dat.Format("200");
			}
			m_SetupPara.InspPara.InsRegion[i].right = atoi(dat);

			key.Empty();
			key.Format("Insp_Region[%d]_Bottom", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			if(atoi(dat) < 0)
			{
				dat.Format("200");
			}
			m_SetupPara.InspPara.InsRegion[i].bottom = atoi(dat);

			
			key.Empty();
			key.Format("Insp_Length[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.bInspLength[i] = atoi(dat);

			key.Empty();
			key.Format("Insp_Length_Size[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.dInspLength[i] = atof(dat);

			key.Empty();
			key.Format("Insp_Area[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.bInspArea[i] = atoi(dat);

			key.Empty();
			key.Format("Insp_Area_Size[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.dInspArea[i] = atof(dat);

			key.Empty();
			key.Format("Insp_Width[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.bInspWidth[i] = atoi(dat);

			key.Empty();
			key.Format("Insp_Width_Size[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.dInspWidth[i] = atof(dat);

			key.Empty();
			key.Format("Insp_Height[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.bInspHeight[i] = atoi(dat);

			key.Empty();
			key.Format("Insp_Height_Size[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.dInspHeight[i] = atof(dat);

			key.Empty();
			key.Format("Insp_OffSet_X[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.dInspOffSetX[i] = atof(dat);

			key.Empty();
			key.Format("Insp_OffSet_Y[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.dInspOffSetY[i] = atof(dat);

			key.Empty();
			key.Format("Insp_OffSet_A[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.dInspOffSetA[i] = atof(dat);

			key.Empty();
			key.Format("Insp_OffSet_Z[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.InspPara.dInspOffSetZ[i] = atof(dat);
		}

		for(int i = 0; i < MAX_LIGHT_PARA; i++)
		{
			key.Empty();
			key.Format("LIGHT_[%d]", i);
			GetPrivateProfileString("INSPECTION_PARA", key, _T("0"), temp, 256, file_path);
			dat.Format("%s", temp);
			m_SetupPara.iLight[i] = atoi(dat);
		}
	}

	return TRUE;
}


//20140630 ngh SafeNet USB LockKey login
BOOL CVisionModule::SafeNet_Lock_LogIn()
{
	/*m_SafeNet_USB_feature = HASP_DEFAULT_FID;
	m_SafeNet_USB_handle = HASP_INVALID_HANDLE_VALUE;

	m_SafeNet_USB_status = hasp_login(m_SafeNet_USB_feature, m_SafeNet_USB_vendor_code, &m_SafeNet_USB_handle);

	if (m_SafeNet_USB_status != HASP_STATUS_OK)
	{
		AfxMessageBox(_T("USB LockKey를 확인해주세요"));
		return FALSE;
	}*/
	return TRUE;

}

//20140630 ngh SafeNet USB LockKey logout
void CVisionModule::SafeNet_Lock_LogOut()
{
	/*m_SafeNet_USB_status = hasp_logout(m_SafeNet_USB_handle);

	if (m_SafeNet_USB_status != HASP_STATUS_OK)
	{
		switch (m_SafeNet_USB_status)
		{
			case HASP_INV_HND:
				break;
			default:
				break;
		}
	}*/
}



BOOL CVisionModule::SaveSystemPara()
{
	BOOL rslt = TRUE;
	CString key, dat;
	CString file_path;

	file_path = SYSTEM_PARA;

	//file 존재 확인 
	HANDLE fd=CreateFile(   file_path,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);

		//write INI
		key.Empty();
		key.Format("AUTO_DELETE_HOUR");
		dat.Empty();
		dat.Format("%d", m_SystemPara.nHour);
		WritePrivateProfileString("SYSTEM_PARA", key, dat, file_path);

		key.Empty();
		key.Format("AUTO_DELETE_MINUTE");
		dat.Empty();
		dat.Format("%d", m_SystemPara.nMinute);
		WritePrivateProfileString("SYSTEM_PARA", key, dat, file_path);

		key.Empty();
		key.Format("AUTO_DELETE_DAY");
		dat.Empty();
		dat.Format("%d", m_SystemPara.nDay);
		WritePrivateProfileString("SYSTEM_PARA", key, dat, file_path);
	}

	return rslt;
}
BOOL CVisionModule::LoadSystemPara()
{
	char temp[DAT_STR_LENG];
	BOOL rslt = TRUE;
	CString key, dat;
	CString file_path;

	file_path = SYSTEM_PARA;
	//file 존재 확인 
	HANDLE fd = CreateFile( file_path,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);

		//Read INI
		key.Empty();
		key.Format("AUTO_DELETE_HOUR");
		GetPrivateProfileString("SYSTEM_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SystemPara.nHour = atoi(dat);

		key.Empty();
		key.Format("AUTO_DELETE_MINUTE");
		GetPrivateProfileString("SYSTEM_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SystemPara.nMinute = atoi(dat);

		key.Empty();
		key.Format("AUTO_DELETE_DAY");
		GetPrivateProfileString("SYSTEM_PARA", key, _T("0"), temp, 256, file_path);
		dat.Format("%s", temp);
		m_SystemPara.nDay = atoi(dat);
	}
	
	return rslt;
}