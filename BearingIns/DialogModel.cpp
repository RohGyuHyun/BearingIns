// DialogModel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "BearingIns.h"
#include "DialogModel.h"


// CDialogModel ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogModel, CDialog)

CDialogModel::CDialogModel(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogModel::IDD, pParent)
{

}

CDialogModel::~CDialogModel()
{
}

void CDialogModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogModel, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDialogModel �޽��� ó�����Դϴ�.

void CDialogModel::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = m_hWnd;
    _TrackMouseEvent(&tme);

	CDialog::OnMouseMove(nFlags, point);
}

void CDialogModel::OnMouseLeave()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CPoint point;
	CRect rect;

	GetWindowRect(&rect);
	GetCursorPos(&point);

	if(!rect.PtInRect(point))
	{
		EndDialog(-1);
	}

	CDialog::OnMouseLeave();
}

void CDialogModel::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CRect rect;

	GetClientRect(&rect);

	MoveWindow(m_Rect.left - 150, m_Rect.top, rect.Width() + 5, rect.Height() + 5);
}

void CDialogModel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CRect rect;
	CPen pen,*oldPen;
	CBrush brush,*oldBrush;
	
	GetClientRect(&rect);
	
	//����� ä���
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	oldPen=dc.SelectObject(&pen);
	brush.CreateSolidBrush(RGB(0, 0, 0));
	oldBrush=dc.SelectObject(&brush);
	
	dc.Rectangle(&rect);
	
	dc.SelectObject(oldPen);
	pen.DeleteObject();
	dc.SelectObject(oldBrush);
	brush.DeleteObject();
}

BOOL CDialogModel::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE) 
		{
			EndDialog(-1);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
BEGIN_EVENTSINK_MAP(CDialogModel, CDialog)
	ON_EVENT(CDialogModel, IDC_BTNENHCTRL_MODEL_NEW, DISPID_CLICK, CDialogModel::ClickBtnenhctrlModelNew, VTS_NONE)
	ON_EVENT(CDialogModel, IDC_BTNENHCTRL_MODEL_DELETE, DISPID_CLICK, CDialogModel::ClickBtnenhctrlModelDelete, VTS_NONE)
	ON_EVENT(CDialogModel, IDC_BTNENHCTRL_MODEL_SELECT, DISPID_CLICK, CDialogModel::ClickBtnenhctrlModelSelect, VTS_NONE)
END_EVENTSINK_MAP()

void CDialogModel::ClickBtnenhctrlModelNew()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_Ret = 0;
	EndDialog(IDOK);
}

void CDialogModel::ClickBtnenhctrlModelDelete()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_Ret = 1;
	EndDialog(IDOK);
}

void CDialogModel::ClickBtnenhctrlModelSelect()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_Ret = 2;
	EndDialog(IDOK);
}
