
// BearingIns.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CBearingInsApp:
// �� Ŭ������ ������ ���ؼ��� BearingIns.cpp�� �����Ͻʽÿ�.
//

class CBearingInsApp : public CWinAppEx
{
public:
	CBearingInsApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CBearingInsApp theApp;