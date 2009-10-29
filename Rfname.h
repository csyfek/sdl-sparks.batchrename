// Rfname.h : main header file for the RFNAME application
//

#if !defined(AFX_RFNAME_H__DE6347F3_C6B5_4E9A_B49E_38E23301A901__INCLUDED_)
#define AFX_RFNAME_H__DE6347F3_C6B5_4E9A_B49E_38E23301A901__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRfnameApp:
// See Rfname.cpp for the implementation of this class
//

class CRfnameApp : public CWinApp
{
public:
	CRfnameApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRfnameApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRfnameApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RFNAME_H__DE6347F3_C6B5_4E9A_B49E_38E23301A901__INCLUDED_)
