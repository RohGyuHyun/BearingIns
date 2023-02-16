// _CtrlModelManageDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BearingIns.h"
#include "_CtrlModelManageDlg.h"


//20130524 수정사항 lby
// 1. 레지스트리에 모델정보가 기록되지 않던 문제.
//		- 현재 로그인 중인 윈도우 계정이 레지스트리에 접근권한이 없기 때문에 발생함. 접근권한은 레지스트리 편집기에서 접근하고자 하는 트리에서 우클릭하고 팝업메뉴에서 사용권한 선택.
//		현재 윈도우 계정의 권한을 설정함.
//
// 2. 레지스트리에 모델정보를 정상적으로 읽어오지 못하던 문제.
//		- 레지스트리에서 얻어올 문자열 버퍼의 길이가 미리 명시된 길이보다 길었기 때문에 발생함.
//		얻어올 문자열 버퍼의 길이를 크게 확보하여 해결함.
//
// 3.1137 line 아래 코드에서 mainpath 초기화 코드에 오타가 있던것을 수정.
//	20130201
//	리스트에 존재 하지 않는 모델 폴더 삭제
//	mainpath.Format(_T("%s", m_szMainPath)) -> mainpath.Format(_T("%S"), m_szMainPath)


// C_CtrlModelManageDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(C_CtrlModelManageDlg, CDialog)


C_CtrlModelManageDlg::C_CtrlModelManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C_CtrlModelManageDlg::IDD, pParent)
	, m_edit_name(_T(""))
	, m_edit_desc(_T(""))
{
	m_iDlgType = 0;
	m_iModelIdx = -1;

	
	//1107
	memset(m_szMainPath, NULL, sizeof(m_szMainPath));
	//sprintf(m_szMainPath, MOD_MODEL_PATH);
	sprintf_s(m_szMainPath, sizeof(m_szMainPath), MOD_MODEL_PATH);

	memset(m_szMainRegi, NULL, sizeof(m_szMainRegi));
	//sprintf(m_szMainRegi, MOD_MODEL_REGI);
	sprintf_s(m_szMainRegi, sizeof(m_szMainRegi), MOD_MODEL_REGI);

	memset(&m_CurrentModel, NULL, sizeof(m_CurrentModel));
	memset(&m_ConversionModel, NULL, sizeof(m_ConversionModel));

	for(int i = 0; i < MAX_MODEL_NUM; i++)
	{
		memset(&m_AllModelData.Model[i], NULL, sizeof(m_AllModelData.Model[i]));
	}
	
}

C_CtrlModelManageDlg::~C_CtrlModelManageDlg()
{
}

void C_CtrlModelManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_edit_name);
	DDX_Text(pDX, IDC_EDIT_DESC, m_edit_desc);
	DDX_Control(pDX, IDC_LIST_MODEL, m_list_model);
}


BEGIN_MESSAGE_MAP(C_CtrlModelManageDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MODEL, &C_CtrlModelManageDlg::OnNMDblclkListModel)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &C_CtrlModelManageDlg::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &C_CtrlModelManageDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &C_CtrlModelManageDlg::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_OK, &C_CtrlModelManageDlg::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &C_CtrlModelManageDlg::OnBnClickedButtonCancel)
	ON_EN_CHANGE(IDC_EDIT_NAME, &C_CtrlModelManageDlg::OnEnChangeEditName)
END_MESSAGE_MAP()


// C_CtrlModelManageDlg message handlers

void C_CtrlModelManageDlg::AllModelLoad()
{
	typeModel mod;
	CString path, str0, str1;
	int idx = 0;
	while(1)
	{
		str0.Empty();
		str0.Format(_T("%d"), idx);

		memset(&mod, NULL, sizeof(mod));

		//제품명
		path.Empty();
		path.Format(_T("%s\\NAME"), m_szMainRegi);
		if(!GetRegString(path, str0, (LPBYTE)mod.szName))
		{
			break;
		}

		//설명
		path.Empty();
		path.Format(_T("%s\\DESCRIPTION"), m_szMainRegi);
		if(!GetRegString(path, str0, (LPBYTE)mod.szDesc))
		{
			break;
		}

		//경로명
		path.Empty();
		path.Format(_T("%s\\PATH"), m_szMainRegi);
		if(!GetRegString(path, str0, (LPBYTE)mod.szPath))
		{
			break;
		}

		sprintf_s(m_AllModelData.Model[idx].szName, sizeof(mod.szName), "%s", mod.szName);
		sprintf_s(m_AllModelData.Model[idx].szDesc, sizeof(mod.szDesc), "%s", mod.szDesc);
		sprintf_s(m_AllModelData.Model[idx].szPath, sizeof(mod.szPath), "%s", mod.szPath);

		idx++;
	}
	m_AllModelData.nMaxModelCnt = idx;
}

BOOL C_CtrlModelManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int i;
	long setstyle;
	LV_COLUMN column;
	CImageList *img_list_slct;
	CBitmap bitmap;

	SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE); 

	img_list_slct = new CImageList;
	img_list_slct->Create(16, 16, ILC_COLOR4, 3, 3);

	bitmap.LoadBitmap(IDB_BITMAP_SELECTED);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BITMAP_NOTSELECTED);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	m_list_model.SetImageList(img_list_slct, LVSIL_SMALL);

	setstyle = GetWindowLong(m_list_model.m_hWnd, GWL_STYLE);
	setstyle |= LVS_EDITLABELS;
	SetWindowLong(m_list_model.m_hWnd, GWL_STYLE, setstyle);

	m_list_model.RedrawWindow();


	//mod lby
	//wchar_t* list[4]={_T("제품명"), _T("설명"), _T("경로명")};
	char* list[4]={_T("제품명"), _T("설명"), _T("경로명")};
	int width[4]={133, 300, 500};
	for(i=0; i<3; i++)
	{
		column.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		column.fmt=LVCFMT_LEFT;
		column.pszText=list[i];
		column.iSubItem=i;
		column.cx=width[i];
		m_list_model.InsertColumn(i, &column);
	}
	m_list_model.DeleteAllItems();

	//데이타 불러오기////////////////////////////
	CString str0, str1, path;

	typeModel mod;
	//memset(&mod, NULL, sizeof(mod));

	BOOL name_not_null = TRUE;
	BOOL name_not_dupl = TRUE;
	int list_cnt, dupl_check;

	path.Empty();
	path.Format(_T("%s\\NAME"), m_szMainRegi);

	//모든데이타 불러오기
	int idx = 0;
	while(1)
	{
		str0.Empty();
		str0.Format(_T("%d"), idx);

		memset(&mod, NULL, sizeof(mod));

		//제품명
		path.Empty();
		path.Format(_T("%s\\NAME"), m_szMainRegi);
		if(!GetRegString(path, str0, (LPBYTE)mod.szName))
		{
			break;
		}

		//설명
		path.Empty();
		path.Format(_T("%s\\DESCRIPTION"), m_szMainRegi);
		if(!GetRegString(path, str0, (LPBYTE)mod.szDesc))
		{
			break;
		}

		//경로명
		path.Empty();
		path.Format(_T("%s\\PATH"), m_szMainRegi);
		if(!GetRegString(path, str0, (LPBYTE)mod.szPath))
		{
			break;
		}

		//Null 및 중복 확인
		str0.Empty();
		str0 = mod.szName;
		str1.Empty();
		str1 = mod.szPath;
		if(!(str0.IsEmpty() || str1.IsEmpty()))
		{
			name_not_dupl = TRUE;
			//리스트에 같은 이름이 존재하는지 확인
			dupl_check = 0;
			list_cnt = 0;
			list_cnt = m_list_model.GetItemCount();
			for(i = 0; i < list_cnt; i++)
			{
				str1.Empty();
				str1 = m_list_model.GetItemText(i,0);
				dupl_check = str0.Compare(str1);
				if(0 == dupl_check)
				{
					name_not_dupl = FALSE;
					break;
				}
			}
			if(name_not_dupl)
			{
				//리스트 구성
				AddList(&mod);
			}
		}

		sprintf_s(m_AllModelData.Model[idx].szName, sizeof(mod.szName), "%s", mod.szName);
		sprintf_s(m_AllModelData.Model[idx].szDesc, sizeof(mod.szDesc), "%s", mod.szDesc);
		sprintf_s(m_AllModelData.Model[idx].szPath, sizeof(mod.szPath), "%s", mod.szPath);

		idx++;
	}

	path.Empty();
	path.Format(_T("%s\\FULLPATH"), m_szMainRegi);
	str0 = _T("0");
	GetRegString(path, str0, (LPBYTE)m_szMainPath);

	
	//현재생산데이타 불러오기
	memset(&m_CurrentModel, NULL, sizeof(m_CurrentModel));


	//제품명
	path.Empty();
	path.Format(_T("%s\\CURRENT"), m_szMainRegi);
	if(GetRegString(path, _T("NAME"), (LPBYTE)m_CurrentModel.szName))
	{
		//설명
		if(!GetRegString(path, _T("DESCRIPTION"), (LPBYTE)m_CurrentModel.szDesc))
		{
			memset(m_CurrentModel.szDesc, NULL, sizeof(m_CurrentModel.szDesc));
		}

		//경로명
		if(!GetRegString(path, _T("PATH"), (LPBYTE)m_CurrentModel.szPath))
		{
			memset(m_CurrentModel.szPath, NULL, sizeof(m_CurrentModel.szPath));
		}
	}
	else
	{
		memset(&m_CurrentModel, NULL, sizeof(m_CurrentModel));
	}

	//현재생산데이타(null, 리스트에 존재하는지)
	str0.Empty();
	str0 = m_CurrentModel.szName;
	if(!str0.IsEmpty())
	{
		list_cnt = 0;
		list_cnt = m_list_model.GetItemCount();
		for(i = 0; i < list_cnt; i++)
		{
			str1.Empty();
			str1 = m_list_model.GetItemText(i, 0);
			if(0 == str0.Compare(str1))
			{
				str0.Empty();
				str0 = m_list_model.GetItemText(i, 1);
				sprintf_s(m_CurrentModel.szDesc, sizeof(m_CurrentModel.szDesc), ("%s"), (LPCTSTR)str0);

				str0.Empty();
				str0 = m_list_model.GetItemText(i, 2);

				sprintf_s(m_CurrentModel.szPath, sizeof(m_CurrentModel.szPath), ("%s"), (LPCTSTR)str0);
				SelectList(i, TRUE);
				break;
			}
		}
	}
	else
	{
		memset(&m_CurrentModel, NULL, sizeof(m_CurrentModel));
	}

	/////////////////////////////////////////////
	int list_num = 0;
	char szTime[30]; 
	SYSTEMTIME	lpSystemTime;

	GetLocalTime(&lpSystemTime);
	memset(szTime, NULL, sizeof(szTime));


	switch(m_iDlgType)
	{
		//신규
		case 0:
			SetWindowText(_T("Model-New"));
			GetDlgItem(IDC_EDIT_NAME)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_DESC)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BUTTON_NEW)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_SELECT)->ShowWindow(SW_HIDE);
			break;
		//삭제 
		case 1:
			SetWindowText(_T("Model-Delete"));
			GetDlgItem(IDC_EDIT_NAME)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_DESC)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BUTTON_NEW)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON_SELECT)->ShowWindow(SW_HIDE);
			break;
		//변경
		case 2:
			SetWindowText(_T("Model-Select"));
			GetDlgItem(IDC_EDIT_NAME)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_DESC)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BUTTON_NEW)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_SELECT)->ShowWindow(SW_SHOW);

			m_edit_name = m_CurrentModel.szName;
			m_edit_desc = m_CurrentModel.szDesc;
			break;
		//Conversion
		case 3:
			SetWindowText(_T("Conversion Model-Select"));
			GetDlgItem(IDC_EDIT_NAME)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_DESC)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BUTTON_NEW)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_SELECT)->ShowWindow(SW_SHOW);

			m_edit_name = m_CurrentModel.szName;
			m_edit_desc = m_CurrentModel.szDesc;
			break;
		default:
			AfxMessageBox(_T("Error : invalid index!"));
	}

	UpdateData(FALSE);


	return TRUE;  
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void C_CtrlModelManageDlg::Open()
{
	CString path;

	//현재생산데이타 불러오기
	memset(&m_CurrentModel, NULL, sizeof(m_CurrentModel));

	//제품명
	path.Empty();
	path.Format(_T("%s\\CURRENT"), m_szMainRegi);
	if(GetRegString(path, _T("NAME"), (LPBYTE)m_CurrentModel.szName))
	{
		Sleep(0);
		//설명
		if(!GetRegString(path, _T("DESCRIPTION"), (LPBYTE)m_CurrentModel.szDesc))
		{
			memset(m_CurrentModel.szDesc, NULL, sizeof(m_CurrentModel.szDesc));
		}
		Sleep(0);
		//경로명
		if(!GetRegString(path, _T("PATH"), (LPBYTE)m_CurrentModel.szPath))
		{
			memset(m_CurrentModel.szPath, NULL, sizeof(m_CurrentModel.szPath));
		}
	}
	else
	{
		memset(&m_CurrentModel, NULL, sizeof(m_CurrentModel));
	}
}


//모델리스트에 추가
//20120925 lby
//VC 2005 으로 컨버젼하면서 발생한 문제 1
// : AddList 함수 인자로 넘어온 구조체 'mod'의 값이 제대로 복사되지 않아 멤버 값이 쓰레기 값으로 변함.
//해결 : AddList 함수 인자를 포인터로 받아서 처리함.
//VC 2005 으로 컨버젼하면서 발생한 문제 2
// : LV_ITEM::pszText 에 값을 넣을 땐 캐스팅 문제로 대입이 정상적으로 안됨.
//해결 : char* 형의 데이터를 CString 으로 변환한 후 다시 BSTR 형으로 변한하여 (LPWSTR)형으로 캐스팅하여 대입.
BOOL C_CtrlModelManageDlg::AddList(typeModel* mod)
{
	LV_ITEM lvitem;
	
	lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
	lvitem.iItem=0;
	lvitem.iSubItem=0;
	lvitem.iImage = 1;
	lvitem.pszText = mod->szName;
	m_list_model.InsertItem(&lvitem);


	lvitem.mask=LVIF_TEXT;
	lvitem.iItem=0;
	lvitem.iSubItem=1;
	lvitem.pszText= mod->szDesc;
	m_list_model.SetItem(&lvitem);

	lvitem.mask=LVIF_TEXT;
	lvitem.iItem=0;
	lvitem.iSubItem=2;
	lvitem.pszText = mod->szPath;
	m_list_model.SetItem(&lvitem);

	//mod lby
	/*
	LV_ITEM lvitem;
	BSTR bstr;
	CString temp;

//	temp.Format(_T("%S"), mod->szName);
	temp = mod->szName;
	bstr = temp.AllocSysString();

	lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
	lvitem.iItem=0;
	lvitem.iSubItem=0;
	lvitem.iImage = 1;
	lvitem.pszText=(LPWSTR)bstr;
	m_list_model.InsertItem(&lvitem);
	SysFreeString(bstr);

//	temp.Format(_T("%S"), mod->szDesc);
	temp.Empty();
	temp = mod->szDesc;
	bstr = temp.AllocSysString();

	lvitem.mask=LVIF_TEXT;
	lvitem.iItem=0;
	lvitem.iSubItem=1;
	lvitem.pszText=(LPWSTR)bstr;//mod->szDesc;
	m_list_model.SetItem(&lvitem);
	SysFreeString(bstr);

//	temp.Format(_T("%S"), mod->szPath);
	temp.Empty();
	temp = mod->szPath;
	bstr = temp.AllocSysString();

	lvitem.mask=LVIF_TEXT;
	lvitem.iItem=0;
	lvitem.iSubItem=2;
	lvitem.pszText=(LPWSTR)bstr;//mod->szPath;
	m_list_model.SetItem(&lvitem);
	SysFreeString(bstr);
	*/

	return TRUE;
}

BOOL C_CtrlModelManageDlg::SelectList(int idx, BOOL on_off)
{
	CString str;
	LV_ITEM lvitem;
	BSTR bstr;

	str.Empty();
	str = m_list_model.GetItemText(idx, 0);
	bstr = str.AllocSysString();

	lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
	lvitem.iItem=idx;
	lvitem.iSubItem=0;
	lvitem.iImage = !(on_off);
	//mod lby
	//lvitem.pszText=(LPWSTR)bstr;
	lvitem.pszText = (char*)(LPCTSTR)str;
	m_list_model.SetItem(&lvitem);

	SysFreeString(bstr);

	return TRUE;
}

void C_CtrlModelManageDlg::SetModelPath(CString _path)
{
	_path += MOD_MODEL_PATH;
	
	sprintf_s(m_szMainPath, sizeof(m_szMainPath), ("%s"), _path);

	
	CString path, str0;
	
	path.Empty();
	path.Format(_T("%s\\FULLPATH"), m_szMainRegi);

	str0 = _T("0");

	SetRegString(path, str0, _path);

}

//현재생산제품 제품명얻기
char *C_CtrlModelManageDlg::GetName()
{
	return m_CurrentModel.szName;
}

//현재생산제품 설명얻기
char *C_CtrlModelManageDlg::GetDesc()
{

	return m_CurrentModel.szDesc;
}

//현재생산제품 경로명얻기
char *C_CtrlModelManageDlg::GetPath()
{
	return m_CurrentModel.szPath;
}

//Conversion 제품명얻기
char *C_CtrlModelManageDlg::GetConversionName()
{

	return m_ConversionModel.szName;
}

//Conversion 설명얻기
char *C_CtrlModelManageDlg::GetConversionDesc()
{

	return m_ConversionModel.szDesc;
}

//Conversion 경로명얻기
char *C_CtrlModelManageDlg::GetConversionPath()
{
	return m_ConversionModel.szPath;
}

//신규등록
void C_CtrlModelManageDlg::Add()
{
	UpdateData(TRUE);

	BOOL name_not_null = TRUE;
	BOOL name_not_dupl = TRUE;

	typeModel mod;
	memset(&mod, NULL, sizeof(mod));

	CString str0, str1;
	int list_cnt, dupl_check;
	int i;

	UpdateData(TRUE);

	

	sprintf_s(mod.szName, m_edit_name.GetLength() + 1, "%s", m_edit_name);
	sprintf_s(mod.szDesc, m_edit_desc.GetLength() + 1, "%s", m_edit_desc);
	//mod lby
	//WideCharToMultiByte(CP_ACP, 0, m_edit_name.GetBuffer(), -1, mod.szName, 100, NULL, NULL);
	//WideCharToMultiByte(CP_ACP, 0, m_edit_desc.GetBuffer(), -1, mod.szDesc, 100, NULL, NULL);
	
	//1107
	//sprintf(mod.szPath, "%s\\%s", m_szMainPath, mod.szName);	//이름이 Null인지 확인 
	sprintf_s(mod.szPath, sizeof(mod.szPath), "%s\\%s", m_szMainPath, mod.szName);

	str0 = mod.szName;
	if(str0.IsEmpty())
	{
		name_not_null = FALSE;
	}

	//리스트에 같은 이름이 존재하는지 확인
	dupl_check = 0;
	list_cnt = 0;
	list_cnt = m_list_model.GetItemCount();
	for(i = 0; i < list_cnt; i++)
	{
		str1.Empty();
		str1 = m_list_model.GetItemText(i,0);
		dupl_check = str0.Compare(str1);
		if(0 == dupl_check)
		{
			name_not_dupl = FALSE;
			break;
		}
	}


	if(name_not_null && name_not_dupl)
	{
		RemoveFileSRecurse((LPCTSTR)mod.szPath, -1);
		RemoveDirectory((LPCTSTR)mod.szPath);
		AddList(&mod);//1. Null이면 에러, 2. 같은 이름 있는지 확인 후 있으면 등록 불허
	}

	if(!name_not_null)
	{
		MessageBox(_T("Error : 제품명이 입력되지 않았습니다!"), _T("경고"), MB_OK | MB_ICONEXCLAMATION);
	}

	if(!name_not_dupl)
	{
		MessageBox(_T("Error : 신규등록 제품명이 이미 존재 합니다!"), _T("경고"), MB_OK | MB_ICONEXCLAMATION);
	}

}

//제품삭제
void C_CtrlModelManageDlg::Remove()
{
	if(0 > m_iModelIdx)
	{	
		MessageBox(_T("Error : 제품이 선택되지 않았습니다!"), _T("경고"), MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		m_list_model.DeleteItem(m_iModelIdx);
	}
}

//제품변경
void C_CtrlModelManageDlg::Select()
{
	UpdateData(TRUE);

	typeModel mod;
	memset(&mod, NULL, sizeof(mod));

	memset(&m_CurrentModel, NULL, sizeof(m_CurrentModel));

	CString str0, str1;
	int list_cnt, dupl_check;
	int i;

	char szName[100] = {0x00};
	char szDesc[100] = {0x00};
	char szPath[100] = {0x00};

	sprintf_s(mod.szName, m_edit_name.GetLength() + 1, "%s", m_edit_name);
	sprintf_s(mod.szDesc, m_edit_desc.GetLength() + 1, "%s", m_edit_desc);
//mod lby
//	WideCharToMultiByte(CP_ACP, 0, m_edit_name.GetBuffer(), -1, mod.szName, 100, NULL, NULL);
//	WideCharToMultiByte(CP_ACP, 0, m_edit_desc.GetBuffer(), -1, mod.szDesc, 100, NULL, NULL);

	sprintf_s(mod.szPath, sizeof(mod.szPath), "%s\\%s", m_szMainPath, mod.szName);

	//이름이 Null인지 확인 
	str0 = mod.szName;
	if(str0.IsEmpty())
	{
		MessageBox(_T("Error : 제품명이 입력되지 않았습니다!"), _T("경고"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	//리스트에 같은 이름 찾기
	BOOL find_check = FALSE;
	dupl_check = 0;
	list_cnt = 0;
	list_cnt = m_list_model.GetItemCount();
	for(i = 0; i < list_cnt; i++)
	{
		str1.Empty();
		str1 = m_list_model.GetItemText(i,0);
		dupl_check = str0.Compare(str1);
		if( (0 == dupl_check) && !find_check)
		{
			//mod lby
			//제품명
			sprintf_s(m_CurrentModel.szName, m_list_model.GetItemText(i,0).GetLength() + 1, "%s", m_list_model.GetItemText(i,0));
//			WideCharToMultiByte(CP_ACP, 0, m_list_model.GetItemText(i,0).GetBuffer(), -1, m_CurrentModel.szName, 100, NULL, NULL);

			//설명
			sprintf_s(m_CurrentModel.szDesc, m_list_model.GetItemText(i,1).GetLength() + 1, "%s", m_list_model.GetItemText(i,1));
//			WideCharToMultiByte(CP_ACP, 0, m_list_model.GetItemText(i,1).GetBuffer(), -1, m_CurrentModel.szDesc, 100, NULL, NULL);

			//경로
			sprintf_s(m_CurrentModel.szPath, m_list_model.GetItemText(i,2).GetLength() + 1, "%s", m_list_model.GetItemText(i,2));
//			WideCharToMultiByte(CP_ACP, 0, m_list_model.GetItemText(i,2).GetBuffer(), -1, m_CurrentModel.szPath, 100, NULL, NULL);

			SelectList(i, TRUE);

			find_check = TRUE;
		}
		else
		{
			SelectList(i, FALSE);
		}
	}

	if(!find_check)
	{
		MessageBox(_T("Error : 변경할 제품명이 존재하지 않습니다!"), _T("경고"), MB_OK | MB_ICONEXCLAMATION);
	}
}

//Conversion
/*
void C_CtrlModelManageDlg::Conversion()
{
	UpdateData(TRUE);

	typeModel mod;
	CString str0, str1;
	int list_cnt, dupl_check;
	int i;

	UpdateData(TRUE);

	//1107
	//sprintf(mod.szName, "%S", (LPCTSTR)m_edit_name);
	sprintf_s(mod.szName, sizeof(mod.szName), "%S", (LPCTSTR)m_edit_name);
	//sprintf(mod.szDesc, "%S", (LPCTSTR)m_edit_desc);
	sprintf_s(mod.szDesc, sizeof(mod.szDesc), "%S", (LPCTSTR)m_edit_desc);
	//sprintf(mod.szPath, "%S\\%S", m_szMainPath, mod.szName);
	sprintf_s(mod.szPath, sizeof(mod.szPath), "%s\\%s", m_szMainPath, mod.szName);

	//이름이 Null인지 확인 
	str0 = mod.szName;
	if(str0.IsEmpty())
	{
		MessageBox(_T("Error : 제품명이 입력되지 않았습니다!"), _T("경고"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	//리스트에 같은 이름 찾기
	BOOL find_check = FALSE;
	dupl_check = 0;
	list_cnt = 0;
	list_cnt = m_list_model.GetItemCount();
	for(i = 0; i < list_cnt; i++)
	{
		str1.Empty();
		str1 = m_list_model.GetItemText(i,0);
		dupl_check = str0.Compare(str1);
		if( (0 == dupl_check) && !find_check)
		{
			//1107
			//제품명
			//sprintf(m_ConversionModel.szName, "%S", m_list_model.GetItemText(i,0));
			sprintf_s(m_ConversionModel.szName, sizeof(m_ConversionModel.szName), "%S", m_list_model.GetItemText(i,0));
			//설명
			//sprintf(m_ConversionModel.szDesc, "%S", m_list_model.GetItemText(i,1));
			sprintf_s(m_ConversionModel.szDesc, sizeof(m_ConversionModel.szDesc), "%S", m_list_model.GetItemText(i,1));
			//경로
			//sprintf(m_ConversionModel.szPath, "%S", m_list_model.GetItemText(i,2));
			sprintf_s(m_ConversionModel.szPath, sizeof(m_ConversionModel.szPath), "%S", m_list_model.GetItemText(i,2));
			

			SelectList(i, TRUE);

			find_check = TRUE;
		}
		else
		{
			SelectList(i, FALSE);
		}
	}

	if(!find_check)
	{
		MessageBox(_T("Error : 제품명이 존재하지 않습니다!"), _T("경고"), MB_OK | MB_ICONEXCLAMATION);
	}
}
*/


void C_CtrlModelManageDlg::Model()
{
	switch(m_iDlgType)
	{
		//신규
		case 0:
			Add();
			break;
		//삭제 
		case 1:
			Remove();
			break;
		//변경
		case 2:
			Select();
			break;
		//Conversion
		case 3:
			//Conversion();

			break;
		default:
			AfxMessageBox(_T("Error : invalid index!"));
	}
}


//20120925 lby
//VS 2005 으로 컨버젼하면서 발생한 문제 1
// : RegSetValueEx 함수의 5 번째 인자로 BYTE* 형으로 캐스팅하면 문자열의 첫글자만 얻게 됨. 
// 해결 : SetRegString 함수의 3번째 인자를 CString 형으로 받아 함수내 에서 별도의 캐스팅을 거쳐 문자열을 획득함.
BOOL C_CtrlModelManageDlg::SetRegString(LPCTSTR lpstrKey, LPCTSTR lpstrEntry, CString lpintValue)//LPCTSTR lpintValue)
{

	TCHAR ClassName[MAX_PATH] = {0x00}; // Buffer for class name.
    HKEY hkey;
    DWORD dwDesc = 0;
	DWORD temp;

	//long rslt = RegCreateKeyEx(HKEY_LOCAL_MACHINE,_T("Software\\Test\\test"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_CREATE_SUB_KEY, NULL, &hkey, &dwDesc);

	temp = RegCreateKeyEx(HKEY_LOCAL_MACHINE, 
                                        lpstrKey, 
                                          0,
                                          ClassName, 
                                          REG_OPTION_NON_VOLATILE, 
                                          KEY_ALL_ACCESS, 
                                          NULL, 
                                          &hkey,
                                          &dwDesc);

    // Set Registry Key & Value
    if (ERROR_SUCCESS == temp)
	{

		//New Code (VC 2005)
		RegSetValueEx(hkey, lpstrEntry, 0, REG_SZ, (LPBYTE)lpintValue.GetBuffer(), lpintValue.GetLength()*sizeof(TCHAR));
		
        RegCloseKey(hkey);
 
        return TRUE;
    }
 
    return FALSE;
}

//20120925 lby
//VS 2005 으로 컨버젼하면서 발생한 문제 1
// : GetRegString 함수의 3 번째 인자 BYTE* 는 문자열 첫 번째 글자만 획득함.
// 해결 : RegQueryValueEx 함수의 5번재 인자로 얻어온 문자열을 1바이트씩 이동하여 문자열을 획득하여 재복사 함.
//20121107 lby
//DEBUG 모드일 때 정상작동 했지만 RELEASE 모드일 때 오작동 했던 문제1.
// : RegQueryValueEx() 함수의 6 번째 인자를 초기화 하지 않게 되면 값을 정상적으로 읽어오지 못하는 경우 발생.
// 해결 : DWORD dwLength 변수를 0 또는 버퍼의 길이로 초기화로 해결.
//Get registry
BOOL C_CtrlModelManageDlg::GetRegString(LPCTSTR lpstrKey, LPCTSTR lpstrEntry, BYTE *lpintValue)
{
    HKEY hkResult;
    DWORD dwLength = 512;// = DAT_BUF_LEN;
    DWORD dwType;// = 0;
	//char strtemp[100];
	//memset(strtemp, NULL, sizeof(100));

	TCHAR sval[100];
	memset(sval, NULL, sizeof(100));
 
    // Get Registry Key handle
    if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                                        lpstrKey,
                                        0, 
                                        KEY_ALL_ACCESS,
                                        &hkResult)) {

        return FALSE;
    }
	else
	{
 
		// Get Entry Value
		if (ERROR_SUCCESS != RegQueryValueEx(hkResult,
											  lpstrEntry,                                           
											   NULL,
											   &dwType,                                        
											   (LPBYTE)lpintValue,
											   &dwLength)) 
		{
			RegCloseKey(hkResult);
			return FALSE;
		}

		//mod lby
		/*
		int nLengs = (int)(wcslen(sval)+1)*2;
		WideCharToMultiByte(CP_ACP, 0, sval, -1, (LPSTR)strtemp, nLengs, NULL, NULL);
		
		sprintf((char*)lpintValue, "%s", strtemp);
		*/
		RegCloseKey(hkResult);
		
	}
    return TRUE;
}

void C_CtrlModelManageDlg::RemoveFileSRecurse(LPCTSTR pstr, long period)
{
	CFileFind finder;
	//FILETIME prevT, file_creationtime;
	CTime current_time, file_creationtime;
	CTimeSpan cur_spantime, file_spantime;
	LONGLONG cur_days, file_creationdays, chk_file, max_file;
	max_file = LONG_MIN;
	
	// build a string with wildcards
	CString strWildcard(pstr), OldFName, path;
	strWildcard += _T("\\*.*");
	
	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);
	
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		
		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if (finder.IsDots())
			continue;
		
		// if it's a directory, recursively search it
		if (finder.IsDirectory() != 0)
		{
			current_time  = CTime::GetCurrentTime();
			cur_spantime = current_time.GetTime();
			cur_days = cur_spantime.GetDays();
			
			//File creation days
			finder.GetCreationTime(file_creationtime);
			file_spantime = file_creationtime.GetTime();
			file_creationdays = file_spantime.GetDays();
			
			chk_file = cur_days - file_creationdays;
			
			CString str = finder.GetFilePath();
			//			AfxMessageBox(str);
			
			if( chk_file > ((LONG)period) )
			{
				CString str = finder.GetFilePath();
				RemoveFileSRecurse(str, period);
				//				str += "(Directory 지우기 동작)";
				//				AfxMessageBox(str);
				RemoveDirectory(str);
			}
			continue;
		}
		else
		{
			//Current days
			current_time  = CTime::GetCurrentTime();
			cur_spantime = current_time.GetTime();
			cur_days = cur_spantime.GetDays();
			
			//File creation days
			finder.GetCreationTime(file_creationtime);
			file_spantime = file_creationtime.GetTime();
			file_creationdays = file_spantime.GetDays();
			
		/////060209
			chk_file = cur_days - file_creationdays;
			if( chk_file > ((LONG)period) )
			{
				path.Format(_T("%s"), finder.GetFilePath());
				if(path != """")
				{
					CFile::Remove((LPCTSTR)path);
				}
			}
		}
	}

}


//레지스트리 전부삭제(하위 레지스트리까지 삭제)
int C_CtrlModelManageDlg::RegDeleteSubKey(HKEY hKey, LPCTSTR lpSubKey)
{
    // RegDeleteKey() 함수는 NT이후 서브키가 있으면 삭제가 안된다.
    // 그래서 서브키까지 모두 삭제해주는 함수를 만들었다.

    HKEY newKey;
    char newSubKey[MAX_PATH];
    LONG Result;
    DWORD Size; 
    FILETIME FileTime;

	//mod lby
    RegOpenKeyEx(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &newKey);
	//RegOpenKeyEx(hKey, _T("SOFTWARE\\CANINS\\MODEL"), 0, KEY_ALL_ACCESS, &newKey);
    Result = ERROR_SUCCESS;

    while(TRUE) 
	{
        Size = MAX_PATH;
        // 계속 키가 삭제 되므로 dwIndex는 항상 0을 넣어주어야 한다.
        // 만약 for문으로 i를 증가시며 사용하면 하나지우고 하나 뛰어넘어 반이 남는다.
        Result = RegEnumKeyEx(newKey, 0, newSubKey, &Size, NULL, NULL, NULL, &FileTime);
        if (Result != ERROR_SUCCESS) break;
        Result = RegDeleteSubKey(newKey, newSubKey);
        if (Result  != ERROR_SUCCESS) break;
    }
    RegCloseKey(newKey);

    return RegDeleteKey(hKey, lpSubKey);
}



int C_CtrlModelManageDlg::DoModal(int type) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_iDlgType = type;

	return (int)CDialog::DoModal();
}



void C_CtrlModelManageDlg::OnNMDblclkListModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	m_iModelIdx = 0;
	CString str;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_iModelIdx = pNMListView->iItem;

	str.Empty();

	//제품명
	str = m_list_model.GetItemText(m_iModelIdx, 0);
	m_edit_name = str;

	//설명
	str = m_list_model.GetItemText(m_iModelIdx, 1);
	m_edit_desc = str;

	UpdateData(FALSE);

	*pResult = 0;
}

void C_CtrlModelManageDlg::OnBnClickedButtonNew()
{
	// TODO: Add your control notification handler code here
	if(IDYES == MessageBox(_T("새로운 모델을 생성하겠습니까?"), NULL, MB_YESNO))
	{
		Model();
	}
}

void C_CtrlModelManageDlg::OnBnClickedButtonDelete()
{
	// TODO: Add your control notification handler code here
	if(IDYES == MessageBox(_T("선택된 모델을 지우겠습니까?"), NULL, MB_YESNO | MB_ICONEXCLAMATION))
	{
		Model();
	}
}

void C_CtrlModelManageDlg::OnBnClickedButtonSelect()
{
	// TODO: Add your control notification handler code here
	if(IDYES == MessageBox(_T("선택된 모델로 변경하겠습니까?"), NULL, MB_YESNO))
	{
		Model();
	}
}



BOOL C_CtrlModelManageDlg::DeleteDirectory(CString lpDirPath)
{
    BOOL bRval = FALSE;
    int nRval = 0; 
    CString szNextDirPath   = _T("");  
    CString szRoot = _T("");   
    CFileFind find;
         
    // 폴더가 존재 하는 지 확인 검사   
    bRval = find.FindFile( lpDirPath );
     
    if( bRval == FALSE )       
    {      
        return bRval;      
    }
     
    while( bRval )     
    {      
        bRval = find.FindNextFile();
        // . or .. 인 경우 무시 한다.     
        if( find.IsDots() == TRUE )        
        {          
            continue;          
        }
        // Directory 일 경우      
        if( find.IsDirectory() )           
        {          
            szNextDirPath.Format(_T("%s\\*.*") , find.GetFilePath() );
 
            // Recursion function 호 출          
            DeleteDirectory( szNextDirPath );          
        }          
        // file일 경우        
        else           
        {          
            // 파일 삭제           
            ::DeleteFile( find.GetFilePath() );        
        }      
    }  

    szRoot = find.GetRoot();   
    find.Close();
    bRval = RemoveDirectory( szRoot );         

    return bRval;
}


void C_CtrlModelManageDlg::OnBnClickedButtonOk()
{
	// TODO: Add your control notification handler code here
	CString path, str0, str1;
	int list_cnt, dupl_check;
	int i, idx;
	
	RegDeleteSubKey(HKEY_LOCAL_MACHINE, (LPCTSTR)m_szMainRegi);

	str1 = m_szMainPath;
	path.Empty();
	path.Format(_T("%s\\FULLPATH"), m_szMainRegi);
	str0 = _T("0");
	SetRegString(path, str0, str1);


	//레지스트리 적용하기
	//리스트 데이타
	dupl_check = 0;
	list_cnt = 0;
	list_cnt = m_list_model.GetItemCount();

	idx = list_cnt;
	for(i = 0; i < list_cnt; i++)
	{
		idx--;
		str0.Empty();
		str0.Format(_T("%d"), i);

		//제품명
		path.Empty();
		path.Format(_T("%s\\NAME"), m_szMainRegi);
		str1.Empty();
		str1 = m_list_model.GetItemText(idx,0);
		if(!str1.IsEmpty())
		{
			SetRegString(path, str0, str1);
		}

		//설명
		path.Empty();
		path.Format(_T("%s\\DESCRIPTION"), m_szMainRegi);
		str1.Empty();
		str1 = m_list_model.GetItemText(idx,1);
		SetRegString(path, str0, str1);

		//경로명
		path.Empty();
		path.Format(_T("%s\\PATH"), m_szMainRegi);
		str1.Empty();
		str1 = m_list_model.GetItemText(idx,2);
		if(!str1.IsEmpty())
		{
			SetRegString(path, str0, str1);
		}
	}

	//리스트에 존재 하지 않는 모델 폴더 삭제
	typeModel mod;
	memset(&mod, NULL, sizeof(mod));
	BOOL bsame = FALSE;

	CFileFind finder;

	CString mainpath;
	mainpath.Format(_T("%s"), m_szMainPath);
	CString nextpath, temppath;
	
	mainpath += _T("\\*.*");

	BOOL bWorking = finder.FindFile(mainpath);

	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(!bWorking)
			break; // mainpath 내에 폴더 또는 파일이 없다면 검색중지.

		if(finder.IsDots())
			continue;
	
		if(finder.IsDirectory()) // 디렉토리 삭제
		{
			nextpath = finder.GetFilePath();
			
			for(i = 0; i < list_cnt; i++)
			{
				str0.Empty();
				str0.Format(_T("%d"), i);

				//제품명
				path.Empty();
				path.Format(_T("%s\\PATH"), m_szMainRegi);
				if(!GetRegString(path, str0, (LPBYTE)mod.szPath))
				{
					break;
				}

				temppath = mod.szPath;

				if(nextpath.Compare(temppath) == 0)
				{
					bsame = TRUE;
				}
			
			}

			if(!bsame)
			{	
				DeleteDirectory(nextpath);
			}
		}
		else // 파일 삭제
		{

		}

		bsame = FALSE;
	}

	//현재생산데이타(null, 리스트에 존재하는지)
	str0.Empty();
	str0 = m_CurrentModel.szName;
	BOOL chk = FALSE;
	if(!str0.IsEmpty())
	{
		list_cnt = 0;
		list_cnt = m_list_model.GetItemCount();
		for(i = 0; i < list_cnt; i++)
		{
			str1.Empty();
			str1 = m_list_model.GetItemText(i, 0);
			if(0 == str0.Compare(str1))
			{
				str0.Empty();
				str0 = m_list_model.GetItemText(i, 1);

				sprintf_s(m_CurrentModel.szDesc, str0.GetLength() + 1, "%s", str0);
				//mod lby
				//WideCharToMultiByte(CP_ACP, 0, str0.GetBuffer(), -1, m_CurrentModel.szDesc, 100, NULL, NULL);

				str0.Empty();
				str0 = m_list_model.GetItemText(i, 2);

				sprintf_s(m_CurrentModel.szPath, str0.GetLength() + 1, "%s", str0);
				//mod lby
				//WideCharToMultiByte(CP_ACP, 0, str0.GetBuffer(), -1, m_CurrentModel.szPath, 100, NULL, NULL);
				SelectList(i, TRUE);
				chk = TRUE;
				break;
			}
		}
	}
	else
	{
		memset(&m_CurrentModel, NULL, sizeof(m_CurrentModel));
	}
	if(!chk)
	{
		memset(&m_CurrentModel, NULL, sizeof(m_CurrentModel));		
	}

	//현생산모델(인덱스 = -1)
	//제품명
	path.Empty();
	path.Format(_T("%s\\CURRENT"), m_szMainRegi);
	str1.Empty();
	str1 = m_CurrentModel.szName;
	SetRegString(path, _T("NAME"), str1);

	//설명
	str1.Empty();
	str1 = m_CurrentModel.szDesc;
	SetRegString(path, _T("DESCRIPTION"), str1);

	//경로명
	str1.Empty();
	str1 = m_CurrentModel.szPath;
	SetRegString(path, _T("PATH"), str1);

	switch(m_iDlgType)
	{
		//신규
		case 0:
			SetWindowText(_T("Model-New"));

			break;
		//삭제 
		case 1:
			SetWindowText(_T("Model-Delete"));

			break;
		//변경
		case 2:
			SetWindowText(_T("Model-Select"));
			m_edit_name.Format(_T("%s"), m_CurrentModel.szName);
			m_edit_desc.Format(_T("%s"), m_CurrentModel.szDesc);
			break;
		//Conversion
		case 3:
			SetWindowText(_T("Conversion Model-Select"));
			m_edit_name.Format(_T("%s"), m_CurrentModel.szName);
			m_edit_desc.Format(_T("%s"), m_CurrentModel.szDesc);
			break;
		default:
			AfxMessageBox(_T("Error : invalid index!"));
	}


	EndDialog(1);	
}

void C_CtrlModelManageDlg::OnBnClickedButtonCancel()
{
	// TODO: Add your control notification handler code here
	
	EndDialog(0);
}

typeModel C_CtrlModelManageDlg::GetModelData(int idx)
{
	return m_AllModelData.Model[idx];
}

int C_CtrlModelManageDlg::GetMaxModel()
{
	return m_AllModelData.nMaxModelCnt;
}

void C_CtrlModelManageDlg::OnEnChangeEditName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);

	int invalid_cnt;
	invalid_cnt = 0;
	CString name;

	name = m_edit_name;

	UpdateData(TRUE);

	if(0 <= name.Find('\\', 1))
	{
		invalid_cnt++;
	}

	if(0 <= name.Find('/', 1))
	{
		invalid_cnt++;
	}

	if(0 <= name.Find(':', 1))
	{
		invalid_cnt++;
	}

	if(0 <= name.Find('*', 1))
	{
		invalid_cnt++;
	}

	if(0 <= name.Find('?', 1))
	{
		invalid_cnt++;
	}

	if(0 <= name.Find('"', 1))
	{
		invalid_cnt++;
	}

	if(0 <= name.Find('<', 1))
	{
		invalid_cnt++;
	}

	if(0 <= name.Find('>', 1))
	{
		invalid_cnt++;
	}

	if(0 <= name.Find('|', 1))
	{
		invalid_cnt++;
	}


	int leng = 0;
	if(0 < invalid_cnt)
	{
		MessageBox(_T("Error : 제품명에 다음 문자를 사용할 수 없습니다! \n \\ / : * ? \" < > |"), _T("경고"), MB_OK | MB_ICONEXCLAMATION);
		leng = name.GetLength();
		name.Delete(leng - 1, 1);
		UpdateData(FALSE);
	}

}

