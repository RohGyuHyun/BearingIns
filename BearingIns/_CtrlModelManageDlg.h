#pragma once

#define MAX_MOD_NAME					100
#define MAX_MOD_DESC					100

#define MAX_MODEL_NUM					1024

#define MOD_MODEL_PATH					("D:\\DATABASE\\MODEL")
#define MOD_MODEL_REGI   				("SOFTWARE\\BearingIns\\MODEL")


//제품관리
typedef struct Model
{
	char szName[MAX_MOD_NAME];
	char szDesc[MAX_MOD_DESC];
	char szPath[MAX_PATH];
}typeModel;

typedef struct AllModel
{
	typeModel Model[MAX_MODEL_NUM];
	int nMaxModelCnt;
}typeAllModel;

// C_CtrlModelManageDlg 대화 상자입니다.

class C_CtrlModelManageDlg : public CDialog
{
	DECLARE_DYNAMIC(C_CtrlModelManageDlg)

public:
	C_CtrlModelManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~C_CtrlModelManageDlg();

	virtual int DoModal(int type);

	void Open();
	void Model();
	
	void SetModelPath(CString path);

	char *GetName();
	char *GetDesc();
	char *GetPath();
	char *GetConversionName();
	char *GetConversionDesc();
	char *GetConversionPath();

	void AllModelLoad();
	int GetMaxModel();
	typeModel GetModelData(int idx);

protected:

	BOOL DeleteDirectory(CString lpDirPath);
	//void Conversion();
	void Select();
	void Remove();
	void Add();
	BOOL AddList(typeModel *mod);
	BOOL SelectList(int idx, BOOL on_off);
	BOOL SetRegString(LPCTSTR lpstrKey, LPCTSTR lpstrEntry, CString lpintValue);//LPCTSTR lpintValue);
	BOOL GetRegString(LPCTSTR lpstrKey, LPCTSTR lpstrEntry, BYTE *lpintValue);
	int RegDeleteSubKey(HKEY hKey, LPCTSTR lpSubKey);
	void RemoveFileSRecurse(LPCTSTR pstr, long period);

	

	int m_iDlgType;
	int m_iModelIdx;
	char m_szMainPath[MAX_PATH];
	char m_szMainRegi[MAX_PATH];
	typeModel m_CurrentModel;
	typeModel m_ConversionModel;

	typeAllModel m_AllModelData;

// Dialog Data
	enum { IDD = IDD_CTRLMODELMANAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_edit_name;
	CString m_edit_desc;
	CListCtrl m_list_model;
	afx_msg void OnNMDblclkListModel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditName();
};
