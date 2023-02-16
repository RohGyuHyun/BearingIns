// InitList.cpp
//

#include "stdafx.h"
#include "Resource.h"
#include "BearingInsDlg.h"
#include VIEWCLASS_HEADER_FILE

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_ITEM						1
#define MAX_SUB_ITEM					13

#define SUB_ITEM_INSP_LENGTH			2
#define SUB_ITEM_INSP_WIDTH				4
#define SUB_ITEM_INSP_HEIGHT			6
#define SUB_ITEM_INSP_AREA				8
#define SUB_ITEM_INSP_LENGTH_OFFSET		9
#define SUB_ITEM_INSP_WIDTH_OFFSET		10
#define SUB_ITEM_INSP_HEIGHT_OFFSET		11
#define SUB_ITEM_INSP_AREA_OFFSET		12

int VIEWCLASS::m_nColWidths[MAX_SUB_ITEM] = { 120, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };	// sixty-fourths

///////////////////////////////////////////////////////////////////////////////
// InitListCtrl
void VIEWCLASS::InitListCtrl(CXListCtrl * pList)
{
	ASSERT(pList);
	if (!pList)
		return;
	//pList->setf
	// set column width according to window rect
	CRect rect;
	pList->GetWindowRect(&rect);

	int w = rect.Width() - 2;

	TCHAR *	lpszHeaders[] = { _T("Index"),
							  _T("간격검사"),
							  _T("간격"),
							  _T("길이검사"),
							  _T("길이(w)"),
							  _T("두께검사"),
							  _T("두께(h)"),
							  _T("면적검사"),
							  _T("면적"),
							  _T("간격 허용율"),
							  _T("길이 허용율"),
							  _T("두께 허용율"),
							  _T("면적 허용율"),
							  NULL };
	int i;
	int total_cx = 0;
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// add columns
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = (i == 1) ? LVCFMT_LEFT : LVCFMT_CENTER;
		lvcolumn.fmt = (i == MAX_ITEM) ? LVCFMT_RIGHT : lvcolumn.fmt;
		lvcolumn.pszText = lpszHeaders[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = m_nColWidths[i];
		pList->InsertColumn(i, &lvcolumn);
	}

#if 0  // -----------------------------------------------------------
	// create the image list from bitmap resource
	VERIFY(pList->m_cImageList.Create(13, 13, ILC_COLOR24, 3, 1));
	CBitmap bm;
	bm.LoadBitmap(IDB_CHECKBOXES);
	pList->m_cImageList.Add(&bm, RGB(255, 0, 255));
	pList->m_HeaderCtrl.SetImageList(&pList->m_cImageList);
#endif // -----------------------------------------------------------

	// iterate through header items and attach the image list
	HDITEM hditem;

	for (i = 0; i < pList->m_HeaderCtrl.GetItemCount(); i++)
	{
		hditem.mask = HDI_IMAGE | HDI_FORMAT;
		pList->m_HeaderCtrl.GetItem(i, &hditem);
		hditem.fmt |=  HDF_IMAGE;
		if (i == 1 || i == 3 || i == 5 || i == 7)
			hditem.iImage = XHEADERCTRL_UNCHECKED_IMAGE;
		else
			hditem.iImage = XHEADERCTRL_NO_IMAGE;

		pList->m_HeaderCtrl.SetItem(i, &hditem);
	}

	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// set the format again - must do this twice or first column does not get set
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
		lvcolumn.fmt = (i == 1) ? LVCFMT_LEFT : LVCFMT_CENTER;
		lvcolumn.fmt = (i == MAX_SUB_ITEM) ? LVCFMT_RIGHT : lvcolumn.fmt;
		lvcolumn.iSubItem = i;
		pList->SetColumn(i, &lvcolumn);
	}

	m_font.CreatePointFont(100, "굴림");

	pList->SetFont(&m_font, TRUE);

}

///////////////////////////////////////////////////////////////////////////////
// FillListCtrl
void VIEWCLASS::FillListCtrl(CXListCtrl * pList, BOOL bInit)
{
	/*static TCHAR * InsPection[] = 
	{
		_T("Width_Inspection"),
		_T("Hegith_Inspection"),
		_T("Area_Inspection"),
		_T("Space_Inspection"),
		_T(""),
		NULL
	};*/
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	//
	// Note - the string arrays must persist for life of list control
	//
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////

	// a list of some of my favorite things - note that this list
	// is not sorted
/*	m_sa1.RemoveAll();
	m_sa1.Add(_T("banana"));*/


	pList->LockWindowUpdate();	// ***** lock window updates while filling list *****

	pList->DeleteAllItems();

	CString str = _T("");
	int nItem, nSubItem;
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	typeSetupPara setupPara;
	int i = 0;
	setupPara = pdlg->m_VisMod->GetSetupPara();
	int cnt = 0;
	//int idx = setupPara.ModelPara.nModelCnt - 1;
	// insert the items and subitems into the list
	if(bInit)
	{
		cnt = 100;
	}
	else
	{
		cnt = setupPara.ModelPara.nModelCnt;
	}
	//for(i = 0; i < cnt; i++)
	//{
		for (nItem = 0; nItem < cnt; nItem++)
		{
			for (nSubItem = 0; nSubItem < MAX_SUB_ITEM; nSubItem++)
			{
				if (nSubItem == 0)				// index
					str.Format("InsPection_%d", nItem);
				else if (nSubItem == 1 || nSubItem == 3 || nSubItem == 5 || nSubItem == 7 )			// inspection	
				{
					str.Format("검사");
				}
				else if (nSubItem == SUB_ITEM_INSP_LENGTH)			// width
				{
					str.Format("%.5f", setupPara.InspPara.dInspLength[nItem]);
				}
				else if (nSubItem == SUB_ITEM_INSP_WIDTH)			// width offset
				{
					str.Format("%.5f", setupPara.InspPara.dInspWidth[nItem]);
				}
				else if (nSubItem == SUB_ITEM_INSP_HEIGHT)			// height
				{
					str.Format("%.5f", setupPara.InspPara.dInspHeight[nItem]);
				}
				else if (nSubItem == SUB_ITEM_INSP_AREA)			// height offset
				{
					str.Format("%.5f", setupPara.InspPara.dInspArea[nItem]);
				}
				else if (nSubItem == SUB_ITEM_INSP_LENGTH_OFFSET)			// Area
				{
					str.Format("%.5f", setupPara.InspPara.dInspOffSetZ[nItem]);
				}
				else if (nSubItem == SUB_ITEM_INSP_WIDTH_OFFSET)			// Area
				{
					str.Format("%.5f", setupPara.InspPara.dInspOffSetX[nItem]);
				}
				else if (nSubItem == SUB_ITEM_INSP_HEIGHT_OFFSET)			// Area
				{
					str.Format("%.5f", setupPara.InspPara.dInspOffSetY[nItem]);
				}
				else if (nSubItem == SUB_ITEM_INSP_AREA_OFFSET)			// Area
				{
					str.Format("%.5f", setupPara.InspPara.dInspOffSetA[nItem]);
				}

				if (nSubItem == 0)
					pList->InsertItem(nItem, str);
				else
					pList->SetItemText(nItem, nSubItem, str);

				if (nSubItem == 2 || nSubItem == 4 || nSubItem == 6 || nSubItem == 8 || nSubItem == 9 || nSubItem == 10 || nSubItem == 11 || nSubItem == 12)
					pList->SetEdit(nItem, nSubItem);
				else if (nSubItem == 1 || nSubItem == 3 || nSubItem == 5 || nSubItem == 7 )
				{
					switch(nSubItem)
					{
						case 1 :
							pList->SetCheckbox(nItem, nSubItem, setupPara.InspPara.bInspLength[nItem]);
							break;
						case 3 :
							pList->SetCheckbox(nItem, nSubItem, setupPara.InspPara.bInspWidth[nItem]);
							break;
						case 5 :
							pList->SetCheckbox(nItem, nSubItem, setupPara.InspPara.bInspHeight[nItem]);
							break;
						case 7 :
							pList->SetCheckbox(nItem, nSubItem, setupPara.InspPara.bInspArea[nItem]);
							break;
					}
				}
				// show background coloring
				//pList->SetItemText(nItem, nSubItem, str, RGB(0,0,0), RGB(210,221,244));
				//pList->SetCheckbox(nItem, nSubItem, 1);
				//pList->SetComboBox(nItem, nSubItem, 
				//					   TRUE,		// enable combobox
				//					   &m_sa1,		// pointer to persistent CStringArray
				//					   5,			// size of dropdown
				//					   0,			// initial combo selection
				//					   TRUE);		// sort CStringArray
			}
		//}
		//idx--;
	}

#ifndef NO_XLISTCTRL_TOOL_TIPS
	// set tool tips in second row
	nItem = 1;
	for (nSubItem = 0; nSubItem < 6; nSubItem++)
	{
		str.Format(_T("This is %d,%d"), nItem, nSubItem);
		pList->SetItemToolTipText(nItem, nSubItem, str);
	}
#endif

	pList->UnlockWindowUpdate();	// ***** unlock window updates *****
}


void VIEWCLASS::GetListCtrl(CXListCtrl * pList)
{
	CString str = _T("");
	int nItem, nSubItem;
	CBearingInsDlg *pdlg = ((CBearingInsDlg *)AfxGetApp()->m_pMainWnd);
	typeSetupPara setupPara;
	int i = 0;
	int idx = 0;
	setupPara = pdlg->m_VisMod->GetSetupPara();

	for(i = 0; i < m_nCnt; i++)//setupPara.ModelPara.nModelCnt; i++)
	{
		for (nItem = (i * MAX_ITEM); nItem < (MAX_ITEM + (i * MAX_ITEM)); nItem++)
		{
			for (nSubItem = 0; nSubItem < MAX_SUB_ITEM; nSubItem++)
			{
				if (nSubItem == 1 || nSubItem == 3 || nSubItem == 5 || nSubItem == 7 )			// inspection	
				{
					switch(nSubItem)
					{
						case 1 :
							setupPara.InspPara.bInspLength[i] = pList->GetCheckbox((nItem), nSubItem);
							break;
						case 3 :
							setupPara.InspPara.bInspWidth[i] = pList->GetCheckbox((nItem), nSubItem);
							break;
						case 5 :
							setupPara.InspPara.bInspHeight[i] = pList->GetCheckbox((nItem), nSubItem);
							break;
						case 7 :
							setupPara.InspPara.bInspArea[i] = pList->GetCheckbox((nItem), nSubItem);
							break;
					}
				}
				else if (nSubItem == SUB_ITEM_INSP_LENGTH)			// width
				{
					str = pList->GetItemText((nItem), nSubItem);
					setupPara.InspPara.dInspLength[i] = atof(str);
				}
				else if (nSubItem == SUB_ITEM_INSP_WIDTH)			// width offset
				{
					str = pList->GetItemText((nItem), nSubItem);
					setupPara.InspPara.dInspWidth[i] = atof(str);
				}
				else if (nSubItem == SUB_ITEM_INSP_HEIGHT)			// height
				{
					str = pList->GetItemText((nItem), nSubItem);
					setupPara.InspPara.dInspHeight[i] = atof(str);
				}
				else if (nSubItem == SUB_ITEM_INSP_AREA)			// height offset
				{
					str = pList->GetItemText((nItem), nSubItem);
					setupPara.InspPara.dInspArea[i] = atof(str);
				}
				else if (nSubItem == SUB_ITEM_INSP_LENGTH_OFFSET)			// Area
				{
					str = pList->GetItemText((nItem), nSubItem);
					setupPara.InspPara.dInspOffSetZ[i] = atof(str);
				}
				else if (nSubItem == SUB_ITEM_INSP_WIDTH_OFFSET)			// Area
				{
					str = pList->GetItemText((nItem), nSubItem);
					setupPara.InspPara.dInspOffSetX[i] = atof(str);
				}
				else if (nSubItem == SUB_ITEM_INSP_HEIGHT_OFFSET)			// Area
				{
					str = pList->GetItemText((nItem), nSubItem);
					setupPara.InspPara.dInspOffSetY[i] = atof(str);
				}
				else if (nSubItem == SUB_ITEM_INSP_AREA_OFFSET)			// Area
				{
					str = pList->GetItemText((nItem), nSubItem);
					setupPara.InspPara.dInspOffSetA[i] = atof(str);
				}
			}
		}
		idx++;
	}

	pdlg->m_VisMod->SetSetupPara(setupPara);
}

///////////////////////////////////////////////////////////////////////////////
// OnClick
//
// This method shows how to handle NM_CLICK messages from XListCtrl
//
void VIEWCLASS::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;
	}

	if (nItem >= 0 && nSubItem >= 0)
	{
		CString strText = m_List.GetItemText(nItem, nSubItem);
		//Log(_T("OnClick at (%d,%d):  '%s'"), nItem, nSubItem, strText);
	}

	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnDblClick
//
// This method shows how to handle NM_DBLCLK messages from XListCtrl
//
void VIEWCLASS::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;
	}

	if (nItem >= 0 && nSubItem >= 0)
	{
		CString strText = m_List.GetItemText(nItem, nSubItem);
		//Log(_T("OnDblClick at (%d,%d):  '%s'"), nItem, nSubItem, strText);
	}

	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnColumnClick
//
// This method shows how to handle LVN_COLUMNCLICK messages from XListCtrl
//
void VIEWCLASS::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMLV)
	{
		nItem = pNMLV->iItem;
		nSubItem = pNMLV->iSubItem;
	}

	//Log(_T("OnColumnClick on column %d"), nSubItem);

	CString strCaption = _T("");
	strCaption.LoadString(AFX_IDS_APP_TITLE);
	strCaption += _T(" *");
	SetWindowText(strCaption);

	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnItemChanged
//
// This method shows how to handle LVN_ITEMCHANGED messages from XListCtrl
//
void VIEWCLASS::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMLV)
	{
		nItem = pNMLV->iItem;
		nSubItem = pNMLV->iSubItem;
	}
	//TRACE(_T("in VIEWCLASS::OnItemChanged:  %d, %d\n"), nItem, nSubItem);

	if (pNMLV && (pNMLV->uNewState == (UINT)(LVIS_FOCUSED|LVIS_SELECTED)))
	{

		CString strText = m_List.GetItemText(nItem, nSubItem);
		//Log(_T("Selection changed to item %d"), nItem);
	}

	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnEditEnd
//
// This method shows how to handle WM_XLISTCTRL_EDIT_END messages 
// from XListCtrl
//
LRESULT VIEWCLASS::OnEditEnd(WPARAM wParam, LPARAM lParam)
{
	int nItem = (int)wParam;
	int nSubItem = (int)lParam;
	if (nItem >= 0 && nSubItem >= 0)
	{
		CString strText = m_List.GetItemText(nItem, nSubItem);
		//Log(_T("OnEditEnd at (%d,%d): '%s'"), nItem, nSubItem, strText);

		COLORREF crText, crBackground;
		m_List.GetItemColors(nItem, nSubItem, crText, crBackground);
		if (m_List.GetModified(nItem, nSubItem))
		{
			// subitem was modified - color it red
			m_List.SetItemText(nItem, nSubItem, strText, 
				RGB(255,0,0), crBackground);
			CString strCaption = _T("");
			strCaption.LoadString(AFX_IDS_APP_TITLE);
			strCaption += _T(" *");
			SetWindowText(strCaption);
		}
		else
		{
			// subitem not modified - color it black -
			// note that once modified, a subitem will remain
			// marked as modified
			m_List.SetItemText(nItem, nSubItem, strText, 
				RGB(0,0,0), crBackground);
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnComboSelection
//
// This method shows how to handle WM_XLISTCTRL_COMBO_SELECTION messages 
// from XListCtrl
//
LRESULT VIEWCLASS::OnComboSelection(WPARAM wParam, LPARAM lParam)
{
	int nItem = (int)wParam;
	int nSubItem = (int)lParam;
	if (nItem >= 0 && nSubItem >= 0)
	{
		/*CString strText = m_List.GetItemText(nItem, nSubItem);
		//Log(_T("OnComboSelection at (%d,%d): '%s'"), nItem, nSubItem, strText);

		COLORREF crText, crBackground;
		m_List.GetItemColors(nItem, nSubItem, crText, crBackground);
		if (m_List.GetModified(nItem, nSubItem))
		{
			// subitem was modified - color it red
			m_List.SetItemText(nItem, nSubItem, strText, 
				RGB(255,0,0), crBackground);
			CString strCaption = _T("");
			strCaption.LoadString(AFX_IDS_APP_TITLE);
			strCaption += _T(" *");
			SetWindowText(strCaption);
		}
		else
		{
			// subitem not modified - color it black -
			// note that once modified, a subitem will remain
			// marked as modified
			m_List.SetItemText(nItem, nSubItem, strText, 
				RGB(0,0,0), crBackground);
		}*/
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnCheckbox
//
// This method shows how to handle WM_XLISTCTRL_CHECKBOX_CLICKED messages 
// from XListCtrl
//
LRESULT VIEWCLASS::OnCheckbox(WPARAM wParam, LPARAM lParam)
{
	int nItem = (int)wParam;
	int nSubItem = (int)lParam;
	if (nItem >= 0 && nSubItem >= 0)
	{
		//Log(_T("OnCheckbox at (%d,%d)"), nItem, nSubItem);
/*
		COLORREF crText, crBackground;
		m_List.GetItemColors(nItem, nSubItem, crText, crBackground);
		if (m_List.GetModified(nItem, nSubItem))
		{
			// subitem was modified - color it red
			if (nSubItem == 0)
				m_List.SetItemText(nItem, nSubItem, _T("*"), 
					RGB(255,0,0), crBackground);
			else
				m_List.SetItemText(nItem, nSubItem, _T("Error"), 
					RGB(255,0,0), crBackground);

			CString strCaption = _T("");
			strCaption.LoadString(AFX_IDS_APP_TITLE);
			strCaption += _T(" *");
			SetWindowText(strCaption);
		}
		else
		{
			// subitem not modified - color it black -
			// note that once modified, a subitem will remain
			// marked as modified
			if (nSubItem == 0)
				m_List.SetItemText(nItem, nSubItem, _T(" "), 
					RGB(0,0,0), crBackground);
			else
				m_List.SetItemText(nItem, nSubItem, _T("OK"), 
					RGB(255,0,0), crBackground);
		}*/
	}

	return 0;
}
