// DirDialog.h: interface for the CDirDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRDIALOG_H__34475EDD_DBF9_4305_A7C7_CFA497829F0A__INCLUDED_)
#define AFX_DIRDIALOG_H__34475EDD_DBF9_4305_A7C7_CFA497829F0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDirDialog  
{
  public:
    CDirDialog();
    virtual ~CDirDialog();
    int DoBrowse(CWnd *pwndParent);

    CString m_strPath;
    CString m_strInitDir;
    CString m_strSelDir;
    CString m_strWindowTitle;
    int  m_iImageIndex;

};

#endif // !defined(AFX_DIRDIALOG_H__34475EDD_DBF9_4305_A7C7_CFA497829F0A__INCLUDED_)
