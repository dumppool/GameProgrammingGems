// HLADemo.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CHLADemoApp:
// �� Ŭ������ ������ ���ؼ��� HLADemo.cpp�� �����Ͻʽÿ�.
//

class CHLADemoApp : public CWinApp
{
public:
	CHLADemoApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CHLADemoApp theApp;
