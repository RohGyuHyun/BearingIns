#pragma once
#include "mil.h"
#include "_CtrlTracker.h"
#include "BtnEnh.h"

// CDialogRect 대화 상자입니다.

class CDialogRect : public CDialog
{
	DECLARE_DYNAMIC(CDialogRect)

public:
	CDialogRect(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogRect();

	C_CtrlTracker m_Tracker[TRACKER_MAX_NUM];

	double m_dZoomRatio;
	CRect m_rectDlg;

	long m_lImgWd;
	long m_lImgHt;

	int m_iDlgIdx;

	int m_iTrackerNum;

	long m_lPanSX;
	long m_lPanSY;
	long m_lPanEX;
	long m_lPanEY;
	BOOL m_bPanDN;

	double m_dPX;
	double m_dPY;

	CPoint m_poStart;
	CPoint m_poEnd;

	int GetEnable();
	void SetEnable(int idx, BOOL enable);
	void SetTrackerNum(int i);
	void SetDlgIdx(int idx);
	double GetZoom();
	void SetZoom(double ratio, long img_wd, long img_ht);
	HWND GetWinHwnd();
	void GetTracker(int idx, double *left, double *top, double *right, double *bottom);
	void SetTracker(int idx, double left, double top, double right, double bottom);
	void SetActiveTracker(int idx, BOOL active);
	BOOL CheckTracker(int idx, CPoint point);

	//Image display
	LRESULT OnImageRectMessage(WPARAM para0, LPARAM para1);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
};
