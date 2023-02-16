// DialogRect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TypeDef.h"
#include "BearingIns.h"
#include "DialogRect.h"
#include "BearingInsDlg.h"


// CDialogRect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogRect, CDialog)

CDialogRect::CDialogRect(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogRect::IDD, pParent)
{
	m_dZoomRatio = 1.;
	m_lImgWd = 1024;
	m_lImgHt = 5000;

	m_iDlgIdx = 0;

	m_iTrackerNum = 0;

	m_lPanSX = 0;
	m_lPanSY = 0;
	m_lPanEX = 0;
	m_lPanEY = 0;
	m_bPanDN = FALSE;
	
	m_dPX = 0.;
	m_dPY = 0.;
}

CDialogRect::~CDialogRect()
{
}

void CDialogRect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogRect, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(USER_MSG_IMG_RECT, OnImageRectMessage)
	ON_WM_SHOWWINDOW()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()


// CDialogRect 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogRect, CDialog)
	
END_EVENTSINK_MAP()

BOOL CDialogRect::OnInitDialog()
{
	CDialog::OnInitDialog();

	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	typeSetupPara setupPara;
	CRect rect;
	char temp[50];

	GetClientRect(&rect);
	m_dZoomRatio = (double)rect.Height() / MAX_CAMERA_HEIGHT;

	setupPara = pdlg->m_VisMod->GetSetupPara();
//
	for(int i = 0; i < TRACKER_MAX_NUM; i++)
	{
		m_Tracker[i] = C_CtrlTracker(this, 0, 4, 2);
		if(i == ALIGN_REGION)
		{
			m_Tracker[i].SetColor(RGB(255, 0, 0));
			m_Tracker[i].SetCaption("Needle");
		}
		else if(i == ALIGN_SEARCH_REGION_1 || i == ALIGN_SEARCH_REGION_1 + 1)
		{
			m_Tracker[i].SetColor(RGB(255, 255, 0));
			sprintf_s(temp, 50, "Search_%d", i);
			m_Tracker[i].SetCaption(temp);
		}
		else if(i == INSPECTION_NIDL_NG_REGION)
		{
			m_Tracker[i].SetColor(RGB(255, 200, 0));
			m_Tracker[i].SetCaption("Needle Ng Region");
		}
		else if(i > INSPECTION_NIDL_NG_REGION)
		{
			m_Tracker[i].SetColor(RGB(0, 255, 0));
			//sprintf_s(temp, 26, "Inspection Nidl Region %02d", i - INSPECTION_NIDL_REGION);
			sprintf_s(temp, 50, "                              %02d", i - INSPECTION_NIDL_REGION);
			//temp.Format("Inspection %02d", i - 2);
			m_Tracker[i].SetCaption(temp);
		}
		m_Tracker[i].SetZoom(0, 0, CAMERA_WIDTH / m_dZoomRatio, MAX_CAMERA_HEIGHT);
		if(i < setupPara.ModelPara.nModelCnt + INSPECTION_NIDL_REGION)
		{
			m_Tracker[i].SetRect(setupPara.InspPara.InsRegion[i].left * m_dZoomRatio, setupPara.InspPara.InsRegion[i].top * m_dZoomRatio,
						setupPara.InspPara.InsRegion[i].right * m_dZoomRatio, setupPara.InspPara.InsRegion[i].bottom * m_dZoomRatio);
		}
		else
		{
			m_Tracker[i].SetRect(100, 100, 200, 200);
		}
		
		m_Tracker[i].SetActive(FALSE);
	}

	return TRUE;
}



HWND CDialogRect::GetWinHwnd()
{
	return this->m_hWnd;
}

void CDialogRect::SetZoom(double ratio, long img_wd, long img_ht)
{
	m_lImgWd = img_wd;
	m_lImgHt = img_ht;
	m_dZoomRatio = ratio;
}

double CDialogRect::GetZoom()
{
	//Matrox에 맞게 계산해서 보냄
	return m_dZoomRatio;
}

void CDialogRect::SetDlgIdx(int idx)
{
	m_iDlgIdx = idx;
}

void CDialogRect::SetTrackerNum(int i)
{
	m_iTrackerNum = i;
}

void CDialogRect::SetEnable(int idx, BOOL enable)
{
	int i;

	for(i = 0; i < INS_RGN_MAX_NUM; i++)
	{
		m_Tracker[i].SetEnable(FALSE);
	}

	m_Tracker[idx].SetEnable(enable);
}

int CDialogRect::GetEnable()
{
	int i;

	for(i = 0; i < INS_RGN_MAX_NUM; i++)
	{
		if(m_Tracker[i].GetEnable())
		{
			return i;
		}
	}

	return -1;
}

void CDialogRect::SetTracker(int idx, double left, double top, double right, double bottom)
{
	CRect img_rect, win_rect;
	double ratio_x, ratio_y;
	double sx, sy, ex, ey;

	m_Tracker[idx].GetZoom(&sx, &sy, &ex, &ey);
	img_rect = CRect((int)sx, (int)sy,(int)ex, (int)ey);
	GetClientRect(&win_rect);

	ratio_x = ((double)win_rect.Width()) / ((double)img_rect.Width());
	ratio_y = ((double)win_rect.Height()) / ((double)img_rect.Height());
	
	m_Tracker[idx].SetRect(win_rect.left + (left - sx) * ratio_x, 
						   win_rect.top + (top - sy) * ratio_y, 
						   win_rect.left + (right - sx) * ratio_x, 
						   win_rect.top + (bottom - sy) * ratio_y);
}

void CDialogRect::GetTracker(int idx, double *left, double *top, double *right, double *bottom)
{
	CRect img_rect, win_rect;
	double ratio_x, ratio_y;
	double sx, sy, ex, ey;
	double win_left, win_top, win_right, win_bottom;

	m_Tracker[idx].GetZoom(&sx, &sy, &ex, &ey);
	img_rect = CRect((int)sx, (int)sy, (int)ex, (int)ey);
	GetClientRect(&win_rect);

	ratio_x = ((double)img_rect.Width()) / ((double)win_rect.Width());
	ratio_y = ((double)img_rect.Height()) / ((double)win_rect.Height());

	m_Tracker[idx].GetRect(&win_left, &win_top, &win_right, &win_bottom);
	
	*left = sx + (win_left - win_rect.left) * ratio_x;
	*top = sy + (win_top - win_rect.top) * ratio_y; 
	*right = sx + (win_right - win_rect.left) * ratio_x;
	*bottom = sy + (win_bottom - win_rect.top) * ratio_y; 

}

void CDialogRect::SetActiveTracker(int idx, BOOL active)
{
	m_Tracker[idx].SetActive(active);
}

void CDialogRect::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch(nIDEvent)
	{
	case 0:
		KillTimer(nIDEvent);
		//Tracker
		for(int i = 0; i < TRACKER_MAX_NUM; i++)
		{
			m_Tracker[i].Draw(i);
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CDialogRect::CheckTracker(int idx, CPoint point)
{
	CRect rect, win_rect;
	POINT pnt;

	rect = m_Tracker[idx].GetRect();

	GetClientRect(&win_rect);
	win_rect.left = win_rect.left + 10;
	win_rect.top = win_rect.top + 10;
	win_rect.right = win_rect.right - 10;
	win_rect.bottom = win_rect.bottom - 10;

	if(!win_rect.PtInRect(point))
	{
		return FALSE;
	}

	pnt.x = rect.left;
	pnt.y = rect.top;
	if(!win_rect.PtInRect(pnt))
	{
		return FALSE;
	}

	pnt.x = rect.right;
	pnt.y = rect.bottom;
	if(!win_rect.PtInRect(pnt))
	{
		return FALSE;
	}

	return TRUE;
}


void CDialogRect::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//Tracker
	int i;

	m_bPanDN = FALSE;

	for(i = 0; i < TRACKER_MAX_NUM; i++)
	{
		m_Tracker[i].OnLButtonUp(nFlags, point);
	}

	//CRect rect = CRect(100, 100, 300, 300);
	Invalidate();

	//Region confirm
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);

	/* Clear the overlay image. */
	//MdispControl(*pdlg->GetDisplay(m_iDlgIdx), M_OVERLAY_CLEAR, M_DEFAULT);

	::PostMessage(pdlg->m_hWnd, USER_MSG_IMG_RECT, m_iDlgIdx, 0);
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CDialogRect::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);

	//Tracker
	int i;
	BOOL chk = TRUE;

	for(i = 0; i < TRACKER_MAX_NUM; i++)
	{
		if(m_Tracker[i].GetEnable())
		{
			chk = FALSE;
		}
	}

	//활성화가 하나도 되어있지 않으면 image pan
	if(chk)
	{
		m_bPanDN = TRUE;

		m_lPanSX = point.x;
		m_lPanSY = point.y;
	}
	else
	{
		for(i = 0; i < TRACKER_MAX_NUM; i++)
		{
			m_Tracker[i].OnLButtonDown(nFlags, point);
		}	
	}
	::PostMessage(pdlg->m_hWnd, USER_MSG_IMG_RECT, m_iDlgIdx, 8);
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CDialogRect::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);

	//Tracker
	int i;
	CRect rect;
	double diff_x, diff_y, diff_img_x, diff_img_y;

	double win_left, win_top, win_right, win_bottom;
	double sx, sy, ex, ey;
	CRect img_rect;
	double ratio_x, ratio_y;
	double sx0, sy0, ex0, ey0;
	//double l, t, r, b;
	typeSetupPara setupPara;
	CRect subRect, zoomRect;
	setupPara = pdlg->m_VisMod->GetSetupPara();

	BOOL chk = TRUE;
	for(i = 0; i < TRACKER_MAX_NUM; i++)
	{
		if(m_Tracker[i].GetEnable())
		{
			chk = FALSE;
		}
	}

	//활성화가 하나도 되어있지 않으면 image pan
	if(chk)
	{
		if(m_bPanDN)
		{
			diff_x = point.x - m_lPanSX;
			diff_y = point.y - m_lPanSY;

			m_lPanEX = point.x;
			m_lPanEY = point.y;

			m_lPanSX = m_lPanEX;
			m_lPanSY = m_lPanEY;
			
			for(i = 0; i < TRACKER_MAX_NUM; i++)
			{
				m_Tracker[i].GetZoom(&sx, &sy, &ex, &ey);
				m_Tracker[i].GetRect(&win_left, &win_top, &win_right, &win_bottom);	

				//Zoom///////////////////////////

				img_rect = CRect((int)sx, (int)sy, (int)ex, (int)ey);	
				ratio_x = ((double)img_rect.Width()) / ((double)m_rectDlg.Width());
				ratio_y = ((double)img_rect.Height()) / ((double)m_rectDlg.Height());

				diff_img_x = diff_x * ratio_x;
				diff_img_y = diff_y * ratio_y;

				sx0 = sx - diff_img_x;
				if(0 > sx0)
				{
					sx0 = 0;
					diff_img_x = sx;
					m_bPanDN = FALSE;
				}

				sy0 = sy - diff_img_y;
				if(0 > sy0)
				{
					sy0 = 0;
					diff_img_y = sy;
					m_bPanDN = FALSE;
				}

				ex0 = ex - diff_img_x;
				if((CAMERA_WIDTH / pdlg->m_dZoomDefault) < ex0)
				{
					ex0 = CAMERA_WIDTH / pdlg->m_dZoomDefault;
					diff_img_x = ex - (CAMERA_WIDTH / pdlg->m_dZoomDefault);
					m_bPanDN = FALSE;
				}

				ey0 = ey - diff_img_y;
				if(MAX_CAMERA_HEIGHT < ey0)
				{
					ey0 = MAX_CAMERA_HEIGHT;
					diff_img_y = ey - MAX_CAMERA_HEIGHT;
					m_bPanDN = FALSE;
				}
			
				m_Tracker[i].SetZoom(sx - diff_img_x, sy - diff_img_y, ex - diff_img_x, ey - diff_img_y);

				//Rect///////////////////////////
				diff_x = diff_img_x / ratio_x;
				diff_y = diff_img_y / ratio_y;

				m_Tracker[i].SetRect(win_left + diff_x, win_top + diff_y, win_right + diff_x, win_bottom + diff_y);
				/*
				m_Tracker[i].GetZoom(&l, &t, &r, &b);
				subRect = m_Tracker[i].GetRect();
				subRect.left = subRect.left / pdlg->m_dZoomRatio;
				subRect.top = subRect.top / pdlg->m_dZoomRatio;
				subRect.right = subRect.right / pdlg->m_dZoomRatio;
				subRect.bottom = subRect.bottom / pdlg->m_dZoomRatio;
				setupPara.InspPara.InsRegion[i] = subRect;*/
			}
			//pdlg->m_VisMod->SetSetupPara(setupPara);

			MdispPan(*pdlg->m_VisMod->GetMilDisplay(0), sx - diff_img_x, sy - diff_img_y);
			pdlg->m_ScrMain_H.SetScrollPos((int)(sx - diff_img_x));
			pdlg->m_ScrMain_V.SetScrollPos((int)(sy - diff_img_y));
		}
	}
	else
	{
		for(i = 0; i < TRACKER_MAX_NUM; i++)
		{
			rect = m_Tracker[i].GetRect();
			m_Tracker[i].OnMouseMove(nFlags, point);
			
			if(!CheckTracker(i, point))
			{
				m_Tracker[i].SetRect(rect);
			}
			
		}
	}

	//체크가 하나도 않되어 있으면 영상을 움직인다.
	
	CDialog::OnMouseMove(nFlags, point);
}

void CDialogRect::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//Tracker
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	int i;
	double l, t, r, b;
	CRect subRect, zoomRect;
	typeSetupPara setupPara;
	setupPara = pdlg->m_VisMod->GetSetupPara();

	for(i = 0; i < TRACKER_MAX_NUM; i++)
	{
		m_Tracker[i].SetEnable(FALSE);
	}

	for(i = 0; i < TRACKER_MAX_NUM; i++)
	{
		if(m_Tracker[i].OnLButtonDblClk(nFlags, point))
		{
			m_Tracker[i].SetEnable(TRUE);
			break;
		}
		else
		{
			m_Tracker[i].SetEnable(FALSE);
			subRect = m_Tracker[i].GetRect();
			m_Tracker[i].GetZoom(&l, &t, &r, &b);
			zoomRect.SetRect(l, t, r, b);
			//subRect.SetRect(win_left + diff_x, win_top + diff_y, win_right + diff_x, win_bottom + diff_y);
			//zoomRect.SetRect(sx - diff_img_x, sy - diff_img_y, ex - diff_img_x, ey - diff_img_y);

			setupPara.InspPara.InsRegion[i].left = zoomRect.left + (subRect.left / pdlg->m_dZoomRatio);
			setupPara.InspPara.InsRegion[i].top = zoomRect.top + (subRect.top / pdlg->m_dZoomRatio);
			setupPara.InspPara.InsRegion[i].right = zoomRect.left + (subRect.right / pdlg->m_dZoomRatio);
			setupPara.InspPara.InsRegion[i].bottom = zoomRect.top + (subRect.bottom / pdlg->m_dZoomRatio);
		}
	}
	pdlg->m_VisMod->SetSetupPara(setupPara);

	::PostMessage(this->m_hWnd, USER_MSG_IMG_RECT, m_iDlgIdx, 0);
	
	CDialog::OnLButtonDblClk(nFlags, point);
}


LRESULT CDialogRect::OnImageRectMessage(WPARAM para0, LPARAM para1)
{

	// TODO: Add your control notification handler code here
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);

	//Tracker
/*	for(int i = 0; i < TRACKER_MAX_NUM; i++)
	{
		m_Tracker[i].Draw(i);
	}*/

	//Tracker
	SetTimer(0, 1, NULL);


	return 0;
}


void CDialogRect::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	//Tracker
	for(int i = 0; i < TRACKER_MAX_NUM; i++)
	{
		m_Tracker[i].Draw(i);
	}
	// Do not call CDialog::OnPaint() for painting messages
}
void CDialogRect::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{
		MoveWindow(57, 35, 576, 971);

		//57,35,576,971
		//MoveWindow(0, 0, (1024 * m_dZoomDefault), (5000 * m_dZoomDefault));
	}
}

void CDialogRect::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	long diff_x, diff_y;
	CRect img_rect;
	CRect main_rect;
	double ratio_x, ratio_y;
	long diff_img_x, diff_img_y;
	MIL_INT pitch;
	double sx, sy, ex, ey;
	BYTE *by_source;
	int idx;
	CString str_brightness;
	CString strX, strY;

	str_brightness.Format("0");
	strX.Format("0");
	strY.Format("0");

	m_Tracker[0].GetZoom(&sx, &sy, &ex, &ey);

	GetWindowRect(&main_rect);
	diff_x = point.x;
	diff_y = point.y;

	img_rect = CRect((int)sx, (int)sy, (int)ex, (int)ey);	
	ratio_x = ((double)img_rect.Width()) / ((double)m_rectDlg.Width());
	ratio_y = ((double)img_rect.Height()) / ((double)m_rectDlg.Height());

	diff_img_x = (long)((double)(diff_x) * ratio_x) + (int)(sx);
	diff_img_y = (long)((double)(diff_y) * ratio_y) + (int)(sy);

	MbufInquire(*pdlg->m_VisMod->GetMilDispBuf(MAIN_DISP), M_HOST_ADDRESS, &by_source);
	MbufInquire(*pdlg->m_VisMod->GetMilDispBuf(MAIN_DISP), M_PITCH, &pitch);

	if(diff_img_x <= MAX_CAMERA_WIDTH)
	{
		idx = diff_img_x * pitch + diff_img_y;
		str_brightness.Format("%d", by_source[idx]);
		strX.Format("%d", diff_img_x);
		strY.Format("%d", diff_img_y);
	}

	pdlg->GetDlgItem(IDC_STATIC_BRIGHTNESS)->SetWindowTextA(str_brightness);
	pdlg->GetDlgItem(IDC_STATIC_X)->SetWindowTextA(strX);
	pdlg->GetDlgItem(IDC_STATIC_Y)->SetWindowTextA(strY);

	CDialog::OnRButtonDown(nFlags, point);
}

void CDialogRect::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	/*CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	typeSetupPara setupPara;
	m_poEnd = point;
	setupPara = pdlg->m_VisMod->GetSetupPara();

	if(pdlg->m_dlgSetup->m_dlgSetupModel->m_bCalculGap)
	{
		pdlg->m_dlgSetup->m_dlgSetupModel->m_edit_dGap = (abs((m_poEnd.y / m_dZoomRatio) - (m_poStart.y / m_dZoomRatio)));// * setupPara.dPixelSize;
		pdlg->m_dlgSetup->m_dlgSetupModel->m_btnCalCul.SetValue(FALSE);
		pdlg->m_dlgSetup->m_dlgSetupModel->UpdateData(FALSE);
	}*/

	CDialog::OnRButtonUp(nFlags, point);
}

void CDialogRect::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CDialog::OnRButtonDblClk(nFlags, point);
}
