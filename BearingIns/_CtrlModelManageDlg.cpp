// _CtrlModelManageDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "BearingIns.h"
#include "_CtrlModelManageDlg.h"


//20130524 �������� lby
// 1. ������Ʈ���� �������� ��ϵ��� �ʴ� ����.
//		- ���� �α��� ���� ������ ������ ������Ʈ���� ���ٱ����� ���� ������ �߻���. ���ٱ����� ������Ʈ�� �����⿡�� �����ϰ��� �ϴ� Ʈ������ ��Ŭ���ϰ� �˾��޴����� ������ ����.
//		���� ������ ������ ������ ������.
//
// 2. ������Ʈ���� �������� ���������� �о���� ���ϴ� ����.
//		- ������Ʈ������ ���� ���ڿ� ������ ���̰� �̸� ��õ� ���̺��� ����� ������ �߻���.
//		���� ���ڿ� ������ ���̸� ũ�� Ȯ���Ͽ� �ذ���.
//
// 3.1137 line �Ʒ� �ڵ忡�� mainpath �ʱ�ȭ �ڵ忡 ��Ÿ�� �ִ����� ����.
//	20130201
//	����Ʈ�� ���� ���� �ʴ� �� ���� ����
//	mainpath.Format(_T("%s", m_szMainPath)) -> mainpath.Format(_T("%S"), m_szMainPath)


// C_CtrlModelManageDlg ��ȭ �����Դϴ�.

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

		//��ǰ��
		path.Empty();
		path.Format(_T("%s\\NAME"), m_szMainRegi);
		if(!GetRegString(path, str0, (LPBYTE)mod.szName))
		{
			break;
		}

		//����
		path.Empty();
		path.Format(_T("%s\\DESCRIPTION"), m_szMainRegi);
		if(!GetRegString(path, str0, (LPBYTE)mod.szDesc))
		{
			break;
		}

		//��θ�
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
	//wchar_t* list[4]={_T("��ǰ��"), _T("����"), _T("��θ�")};
	char* list[4]={_T("��ǰ��"), _T("����"), _T("��θ�")};
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

	//����Ÿ �ҷ�����////////////////////////////
	CString str0, str1, path;

	typeModel mod;
	//memset(&mod, NULL, sizeof(mod));

	BOOL name_not_null = TRUE;
	BOOL name_not_dupl = TRUE;
	int list_cnt, dupl_check;

	path.Empty();
	path.Format(_T("%s\\NAME"), m_szMainRegi);

	//��絥��Ÿ �ҷ�����
	int idx = 0;
	while(1)
	{
		str0.Empty();
		str0.Format(_T("%d"), idx);

		memset(&mod, NULL, sizeof(mod));

		//��ǰ��
		path.Empty();
		path.Format(_T("%s\\NAME"), m_szMainRegi);
		if(!GetRegString(path, str0, (LPBYTE)mod.szName))
		{
			break;
		}

		//����
		path.Empty();
		path.Format(_T("%s\\DESCRIPTION"), m_szMainRegi);
		if(!GetRegString(path, str0, (LPBYTE)mod.szDesc))
		{
			break;
		}

		//��θ�
		path.Empty();
		path.Format(_T("%s\\PATH"), m_szMainRegi);
		if(!GetRegString(path, str0, (LPBYTE)mod.szPath))
		{
			break;
		}

		//Null �� �ߺ� Ȯ��
		str0.Empty();
		str0 = mod.szName;
		str1.Empty();
		str1 = mod.szPath;
		if(!(str0.IsEmpty() || str1.IsEmpty()))
		{
			name_not_dupl = TRUE;
			//����Ʈ�� ���� �̸��� �����ϴ��� Ȯ��
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
				//����Ʈ ����
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

	
	//������굥��Ÿ �ҷ�����
	memset(&m_CurrentModel, NULL, sizeof(m_CurrentModel));


	//��ǰ��
	path.Empty();
	path.Format(_T("%s\\CURRENT"), m_szMainRegi);
	if(GetRegString(path, _T("NAME"), (LPBYTE)m_CurrentModel.szName))
	{
		//����
		if(!GetRegString(path, _T("DESCRIPTION"), (LPBYTE)m_CurrentModel.szDesc))
		{
			memset(m_CurrentModel.szDesc, NULL, sizeof(m_CurrentModel.szDesc));
		}

		//��θ�
		if(!GetRegString(path, _T("PATH"), (LPBYTE)m_CurrentModel.szPath))
		{
			memset(m_CurrentModel.szPath, NULL, sizeof(m_CurrentModel.szPath));
		}
	}
	else
	{
		memset(&m_CurrentModel, NULL, sizeof(m_CurrentModel));
	}

	//������굥��Ÿ(null, ����Ʈ�� �����ϴ���)
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
		//�ű�
		case 0:
			SetWindowText(_T("Model-New"));
			GetDlgItem(IDC_EDIT_NAME)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_DESC)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BUTTON_NEW)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_SELECT)->ShowWindow(SW_HIDE);
			break;
		//���� 
		case 1:
			SetWindowText(_T("Model-Delete"));
			GetDlgItem(IDC_EDIT_NAME)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_DESC)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BUTTON_NEW)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON_SELECT)->ShowWindow(SW_HIDE);
			break;
		//����
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

	//������굥��Ÿ �ҷ�����
	memset(&m_CurrentModel, NULL, sizeof(m_CurrentModel));

	//��ǰ��
	path.Empty();
	path.Format(_T("%s\\CURRENT"), m_szMainRegi);
	if(GetRegString(path, _T("NAME"), (LPBYTE)m_CurrentModel.szName))
	{
		Sleep(0);
		//����
		if(!GetRegString(path, _T("DESCRIPTION"), (LPBYTE)m_CurrentModel.szDesc))
		{
			memset(m_CurrentModel.szDesc, NULL, sizeof(m_CurrentModel.szDesc));
		}
		Sleep(0);
		//��θ�
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


//�𵨸���Ʈ�� �߰�
//20120925 lby
//VC 2005 ���� �������ϸ鼭 �߻��� ���� 1
// : AddList �Լ� ���ڷ� �Ѿ�� ����ü 'mod'�� ���� ����� ������� �ʾ� ��� ���� ������ ������ ����.
//�ذ� : AddList �Լ� ���ڸ� �����ͷ� �޾Ƽ� ó����.
//VC 2005 ���� �������ϸ鼭 �߻��� ���� 2
// : LV_ITEM::pszText �� ���� ���� �� ĳ���� ������ ������ ���������� �ȵ�.
//�ذ� : char* ���� �����͸� CString ���� ��ȯ�� �� �ٽ� BSTR ������ �����Ͽ� (LPWSTR)������ ĳ�����Ͽ� ����.
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

//���������ǰ ��ǰ����
char *C_CtrlModelManageDlg::GetName()
{
	return m_CurrentModel.szName;
}

//���������ǰ ������
char *C_CtrlModelManageDlg::GetDesc()
{

	return m_CurrentModel.szDesc;
}

//���������ǰ ��θ���
char *C_CtrlModelManageDlg::GetPath()
{
	return m_CurrentModel.szPath;
}

//Conversion ��ǰ����
char *C_CtrlModelManageDlg::GetConversionName()
{

	return m_ConversionModel.szName;
}

//Conversion ������
char *C_CtrlModelManageDlg::GetConversionDesc()
{

	return m_ConversionModel.szDesc;
}

//Conversion ��θ���
char *C_CtrlModelManageDlg::GetConversionPath()
{
	return m_ConversionModel.szPath;
}

//�űԵ��
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
	//sprintf(mod.szPath, "%s\\%s", m_szMainPath, mod.szName);	//�̸��� Null���� Ȯ�� 
	sprintf_s(mod.szPath, sizeof(mod.szPath), "%s\\%s", m_szMainPath, mod.szName);

	str0 = mod.szName;
	if(str0.IsEmpty())
	{
		name_not_null = FALSE;
	}

	//����Ʈ�� ���� �̸��� �����ϴ��� Ȯ��
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
		AddList(&mod);//1. Null�̸� ����, 2. ���� �̸� �ִ��� Ȯ�� �� ������ ��� ����
	}

	if(!name_not_null)
	{
		MessageBox(_T("Error : ��ǰ���� �Էµ��� �ʾҽ��ϴ�!"), _T("���"), MB_OK | MB_ICONEXCLAMATION);
	}

	if(!name_not_dupl)
	{
		MessageBox(_T("Error : �űԵ�� ��ǰ���� �̹� ���� �մϴ�!"), _T("���"), MB_OK | MB_ICONEXCLAMATION);
	}

}

//��ǰ����
void C_CtrlModelManageDlg::Remove()
{
	if(0 > m_iModelIdx)
	{	
		MessageBox(_T("Error : ��ǰ�� ���õ��� �ʾҽ��ϴ�!"), _T("���"), MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		m_list_model.DeleteItem(m_iModelIdx);
	}
}

//��ǰ����
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

	//�̸��� Null���� Ȯ�� 
	str0 = mod.szName;
	if(str0.IsEmpty())
	{
		MessageBox(_T("Error : ��ǰ���� �Էµ��� �ʾҽ��ϴ�!"), _T("���"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	//����Ʈ�� ���� �̸� ã��
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
			//��ǰ��
			sprintf_s(m_CurrentModel.szName, m_list_model.GetItemText(i,0).GetLength() + 1, "%s", m_list_model.GetItemText(i,0));
//			WideCharToMultiByte(CP_ACP, 0, m_list_model.GetItemText(i,0).GetBuffer(), -1, m_CurrentModel.szName, 100, NULL, NULL);

			//����
			sprintf_s(m_CurrentModel.szDesc, m_list_model.GetItemText(i,1).GetLength() + 1, "%s", m_list_model.GetItemText(i,1));
//			WideCharToMultiByte(CP_ACP, 0, m_list_model.GetItemText(i,1).GetBuffer(), -1, m_CurrentModel.szDesc, 100, NULL, NULL);

			//���
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
		MessageBox(_T("Error : ������ ��ǰ���� �������� �ʽ��ϴ�!"), _T("���"), MB_OK | MB_ICONEXCLAMATION);
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

	//�̸��� Null���� Ȯ�� 
	str0 = mod.szName;
	if(str0.IsEmpty())
	{
		MessageBox(_T("Error : ��ǰ���� �Էµ��� �ʾҽ��ϴ�!"), _T("���"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	//����Ʈ�� ���� �̸� ã��
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
			//��ǰ��
			//sprintf(m_ConversionModel.szName, "%S", m_list_model.GetItemText(i,0));
			sprintf_s(m_ConversionModel.szName, sizeof(m_ConversionModel.szName), "%S", m_list_model.GetItemText(i,0));
			//����
			//sprintf(m_ConversionModel.szDesc, "%S", m_list_model.GetItemText(i,1));
			sprintf_s(m_ConversionModel.szDesc, sizeof(m_ConversionModel.szDesc), "%S", m_list_model.GetItemText(i,1));
			//���
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
		MessageBox(_T("Error : ��ǰ���� �������� �ʽ��ϴ�!"), _T("���"), MB_OK | MB_ICONEXCLAMATION);
	}
}
*/


void C_CtrlModelManageDlg::Model()
{
	switch(m_iDlgType)
	{
		//�ű�
		case 0:
			Add();
			break;
		//���� 
		case 1:
			Remove();
			break;
		//����
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
//VS 2005 ���� �������ϸ鼭 �߻��� ���� 1
// : RegSetValueEx �Լ��� 5 ��° ���ڷ� BYTE* ������ ĳ�����ϸ� ���ڿ��� ù���ڸ� ��� ��. 
// �ذ� : SetRegString �Լ��� 3��° ���ڸ� CString ������ �޾� �Լ��� ���� ������ ĳ������ ���� ���ڿ��� ȹ����.
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
//VS 2005 ���� �������ϸ鼭 �߻��� ���� 1
// : GetRegString �Լ��� 3 ��° ���� BYTE* �� ���ڿ� ù ��° ���ڸ� ȹ����.
// �ذ� : RegQueryValueEx �Լ��� 5���� ���ڷ� ���� ���ڿ��� 1����Ʈ�� �̵��Ͽ� ���ڿ��� ȹ���Ͽ� �纹�� ��.
//20121107 lby
//DEBUG ����� �� �����۵� ������ RELEASE ����� �� ���۵� �ߴ� ����1.
// : RegQueryValueEx() �Լ��� 6 ��° ���ڸ� �ʱ�ȭ ���� �ʰ� �Ǹ� ���� ���������� �о���� ���ϴ� ��� �߻�.
// �ذ� : DWORD dwLength ������ 0 �Ǵ� ������ ���̷� �ʱ�ȭ�� �ذ�.
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
				//				str += "(Directory ����� ����)";
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


//������Ʈ�� ���λ���(���� ������Ʈ������ ����)
int C_CtrlModelManageDlg::RegDeleteSubKey(HKEY hKey, LPCTSTR lpSubKey)
{
    // RegDeleteKey() �Լ��� NT���� ����Ű�� ������ ������ �ȵȴ�.
    // �׷��� ����Ű���� ��� �������ִ� �Լ��� �������.

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
        // ��� Ű�� ���� �ǹǷ� dwIndex�� �׻� 0�� �־��־�� �Ѵ�.
        // ���� for������ i�� �����ø� ����ϸ� �ϳ������ �ϳ� �پ�Ѿ� ���� ���´�.
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

	//��ǰ��
	str = m_list_model.GetItemText(m_iModelIdx, 0);
	m_edit_name = str;

	//����
	str = m_list_model.GetItemText(m_iModelIdx, 1);
	m_edit_desc = str;

	UpdateData(FALSE);

	*pResult = 0;
}

void C_CtrlModelManageDlg::OnBnClickedButtonNew()
{
	// TODO: Add your control notification handler code here
	if(IDYES == MessageBox(_T("���ο� ���� �����ϰڽ��ϱ�?"), NULL, MB_YESNO))
	{
		Model();
	}
}

void C_CtrlModelManageDlg::OnBnClickedButtonDelete()
{
	// TODO: Add your control notification handler code here
	if(IDYES == MessageBox(_T("���õ� ���� ����ڽ��ϱ�?"), NULL, MB_YESNO | MB_ICONEXCLAMATION))
	{
		Model();
	}
}

void C_CtrlModelManageDlg::OnBnClickedButtonSelect()
{
	// TODO: Add your control notification handler code here
	if(IDYES == MessageBox(_T("���õ� �𵨷� �����ϰڽ��ϱ�?"), NULL, MB_YESNO))
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
         
    // ������ ���� �ϴ� �� Ȯ�� �˻�   
    bRval = find.FindFile( lpDirPath );
     
    if( bRval == FALSE )       
    {      
        return bRval;      
    }
     
    while( bRval )     
    {      
        bRval = find.FindNextFile();
        // . or .. �� ��� ���� �Ѵ�.     
        if( find.IsDots() == TRUE )        
        {          
            continue;          
        }
        // Directory �� ���      
        if( find.IsDirectory() )           
        {          
            szNextDirPath.Format(_T("%s\\*.*") , find.GetFilePath() );
 
            // Recursion function ȣ ��          
            DeleteDirectory( szNextDirPath );          
        }          
        // file�� ���        
        else           
        {          
            // ���� ����           
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


	//������Ʈ�� �����ϱ�
	//����Ʈ ����Ÿ
	dupl_check = 0;
	list_cnt = 0;
	list_cnt = m_list_model.GetItemCount();

	idx = list_cnt;
	for(i = 0; i < list_cnt; i++)
	{
		idx--;
		str0.Empty();
		str0.Format(_T("%d"), i);

		//��ǰ��
		path.Empty();
		path.Format(_T("%s\\NAME"), m_szMainRegi);
		str1.Empty();
		str1 = m_list_model.GetItemText(idx,0);
		if(!str1.IsEmpty())
		{
			SetRegString(path, str0, str1);
		}

		//����
		path.Empty();
		path.Format(_T("%s\\DESCRIPTION"), m_szMainRegi);
		str1.Empty();
		str1 = m_list_model.GetItemText(idx,1);
		SetRegString(path, str0, str1);

		//��θ�
		path.Empty();
		path.Format(_T("%s\\PATH"), m_szMainRegi);
		str1.Empty();
		str1 = m_list_model.GetItemText(idx,2);
		if(!str1.IsEmpty())
		{
			SetRegString(path, str0, str1);
		}
	}

	//����Ʈ�� ���� ���� �ʴ� �� ���� ����
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
			break; // mainpath ���� ���� �Ǵ� ������ ���ٸ� �˻�����.

		if(finder.IsDots())
			continue;
	
		if(finder.IsDirectory()) // ���丮 ����
		{
			nextpath = finder.GetFilePath();
			
			for(i = 0; i < list_cnt; i++)
			{
				str0.Empty();
				str0.Format(_T("%d"), i);

				//��ǰ��
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
		else // ���� ����
		{

		}

		bsame = FALSE;
	}

	//������굥��Ÿ(null, ����Ʈ�� �����ϴ���)
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

	//�������(�ε��� = -1)
	//��ǰ��
	path.Empty();
	path.Format(_T("%s\\CURRENT"), m_szMainRegi);
	str1.Empty();
	str1 = m_CurrentModel.szName;
	SetRegString(path, _T("NAME"), str1);

	//����
	str1.Empty();
	str1 = m_CurrentModel.szDesc;
	SetRegString(path, _T("DESCRIPTION"), str1);

	//��θ�
	str1.Empty();
	str1 = m_CurrentModel.szPath;
	SetRegString(path, _T("PATH"), str1);

	switch(m_iDlgType)
	{
		//�ű�
		case 0:
			SetWindowText(_T("Model-New"));

			break;
		//���� 
		case 1:
			SetWindowText(_T("Model-Delete"));

			break;
		//����
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
		MessageBox(_T("Error : ��ǰ�� ���� ���ڸ� ����� �� �����ϴ�! \n \\ / : * ? \" < > |"), _T("���"), MB_OK | MB_ICONEXCLAMATION);
		leng = name.GetLength();
		name.Delete(leng - 1, 1);
		UpdateData(FALSE);
	}

}

