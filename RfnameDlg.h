// RfnameDlg.h : header file
//

#if !defined(AFX_RFNAMEDLG_H__702AF3FD_A5DB_4A24_98BB_605B10711CB5__INCLUDED_)
#define AFX_RFNAMEDLG_H__702AF3FD_A5DB_4A24_98BB_605B10711CB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	REPL_HEAD			100
#define REPL_HEAD_DATE		101
#define	REPL_TAIL			102
#define	REPL_TAIL_DATE		103
#define	REPL_EXTHEAD		104
#define	REPL_EXTHEAD_DATE	105
#define	REPL_EXTTAIL		106
#define	REPL_EXTTAIL_DATE	107
#define	INS_HEAD			200
#define INS_HEAD_DATE		201
#define	INS_TAIL			202
#define	INS_TAIL_DATE		203
#define	INS_EXTHEAD			204
#define	INS_EXTHEAD_DATE	205
#define	INS_EXTTAIL			206
#define	INS_EXTTAIL_DATE	207


/////////////////////////////////////////////////////////////////////////////
// CRfnameDlg dialog

class CRfnameDlg : public CDialog
{
// Construction
public:
	CRfnameDlg(CWnd* pParent = NULL);	// standard constructor
	
	CString m_strDirPath;
// Dialog Data
	//{{AFX_DATA(CRfnameDlg)
	enum { IDD = IDD_RFNAME_DIALOG };
	CButton	m_btnQes;
	CEdit	m_EditExt;
	CListCtrl	m_ResultList;
	CDateTimeCtrl	m_DateEnd;
	CEdit	m_EditFmt;
	CDateTimeCtrl	m_DateBegin;
	CButton	m_DateSpan;
	CButton	m_FNameSave;
	CButton	m_SubDir;
	CTreeCtrl	m_FileTree;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRfnameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	void BrowseFile(int CallNum,CString strFile);
	void RenameFile(CString strFile,BOOL blSub);
	void BeginRename(CString strFile,BOOL blSub);
	void ProcRename(CString strFile,CString strCreateDate);
	BOOL SyntaxAnalyze();	//”Ô∑®∑÷Œˆ
	void SaveBuffer();
	
	CBitmap m_bitmapQes;
	CImageList m_iImageList;//Õº±Í¡¥
	HTREEITEM strRoot;
	HTREEITEM strHTFir;
	HTREEITEM strHTSec;
	HTREEITEM strHtThi;
	HTREEITEM strHtFor;
	HTREEITEM strHtFif;
	HTREEITEM strHtEnd;

	int m_iCmd;
	CString m_strConTent;
	CString m_strExt;
	CString m_strBeginDate,m_strEndDate;
	// Generated message map functions
	//{{AFX_MSG(CRfnameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonseldir();
	afx_msg void OnCheck3();
	virtual void OnOK();
	afx_msg void OnButtonqes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RFNAMEDLG_H__702AF3FD_A5DB_4A24_98BB_605B10711CB5__INCLUDED_)
