
// MSG Package and Unpack System.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMSGPackageandUnpackSystemApp:
// �йش����ʵ�֣������ MSG Package and Unpack System.cpp
//

class CMSGPackageandUnpackSystemApp : public CWinApp
{
public:
	CMSGPackageandUnpackSystemApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMSGPackageandUnpackSystemApp theApp;