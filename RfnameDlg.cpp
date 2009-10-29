// RfnameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Rfname.h"
#include "RfnameDlg.h"
#include "DirDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CheckStr(CString strCheckData)
{
	int iQesCnt,iChrCnt;
	TCHAR c;

	iQesCnt = iChrCnt = 0;
	for(int i=0;i<strCheckData.GetLength();i++)
	{
		if ((c = strCheckData.GetAt(i))=='*') return 0;

		if (c=='?')
			iQesCnt ++;
		else
			iChrCnt ++;
	}

	//既有问号又有字符
	if (iQesCnt && iChrCnt) return 0;

	//没有问号
	if (!iQesCnt && iChrCnt) return 1;

	//没有字符
	if (iQesCnt && !iChrCnt)
	{
		if (iQesCnt==4 || iQesCnt==6 || iQesCnt==8) return 2;
	}

	return 0;
}

CString FormatDateStr(CString strDate)
{
	CString strYear,strMonth,strDay;
	TCHAR c;

	strYear = strDate.Left(4);
	c = strDate.GetAt(6);
	if (c=='-')
	{
		strMonth = "0" + strDate.Mid(5,1);
	}
	else
	{
		strMonth = strDate.Mid(5,2);
	}

	int l = strDate.GetLength();
	c = strDate.GetAt(l-2);
	if (c=='-')
	{
		strDay = "0" + strDate.Right(1);
	}
	else
	{
		strDay = strDate.Right(2);
	}

	return strYear+strMonth+strDay;
}

CString ExtractFNameSubStr(CString strFile,int iflag)
{
	CString cstr = "";
	int i=0,ifnd;
	
	ifnd = -1;
	while(i>=0)
	{
		i = strFile.Find(".",i);
		if (i>=0) {ifnd = i;i++;}	
	}
	if (ifnd>=0)
	{
		if (iflag==0)		//取文件名
		{
			cstr = strFile.Left(ifnd);
		}
		if (iflag==1)		//取扩展名
		{
			cstr = strFile.Mid(ifnd+1);
		}
	}

	return cstr;
}

/////////////////////////////////////////////////////////////////////////////
// CRfnameDlg dialog

CRfnameDlg::CRfnameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRfnameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRfnameDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bitmapQes.LoadBitmap(IDB_BITMAPQES);
}

void CRfnameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRfnameDlg)
	DDX_Control(pDX, IDC_BUTTONQES, m_btnQes);
	DDX_Control(pDX, IDC_EDITEXT, m_EditExt);
	DDX_Control(pDX, IDC_LIST1, m_ResultList);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_DateEnd);
	DDX_Control(pDX, IDC_EDITFORMAT, m_EditFmt);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateBegin);
	DDX_Control(pDX, IDC_CHECK3, m_DateSpan);
	DDX_Control(pDX, IDC_CHECK2, m_FNameSave);
	DDX_Control(pDX, IDC_CHECK1, m_SubDir);
	DDX_Control(pDX, IDC_TREE1, m_FileTree);
	//}}AFX_DATA_MAP
}

BOOL CRfnameDlg::SyntaxAnalyze()
{
	CString strInput;
	CString strTitle,strExt;
	BOOL IsIns;

	TCHAR c;

	m_EditFmt.GetWindowText(strInput);

	if (((CButton*)GetDlgItem(IDC_RADIOREPL))->GetCheck()==BST_CHECKED)
	{
		IsIns = FALSE;
	}
	if (((CButton*)GetDlgItem(IDC_RADIOINS))->GetCheck()==BST_CHECKED)
	{
		IsIns = TRUE;
	}

	for(int i=0;i<strInput.GetLength();i++)
	{
		c = strInput.GetAt(i);
		if (i==0 && c=='.') return FALSE;
		if (c=='\\' || c=='/' || c==':' ||
			c=='>' || c=='<' || c=='"' ||
			c=='|')
			return FALSE;
	}
	strTitle = ExtractFNameSubStr(strInput,0);
	strExt = ExtractFNameSubStr(strInput,1);
	
	if (strTitle.GetLength()==1 && 	strExt.GetLength()==1) return FALSE;

	CString strTmp;
	int l,ret;
	//修改文件名title情况分析
	if (strExt=="*")
	{
		l = strTitle.GetLength();
		if (strTitle.Right(1)=="*")
		{
			strTmp = strTitle.Left(l-1);
			ret = CheckStr(strTmp); 
			if (!ret) return FALSE;
			if (IsIns)
			{
				if (ret==1) m_iCmd = INS_HEAD;
				if (ret==2) m_iCmd = INS_HEAD_DATE;
			}
			else
			{
				if (ret==1) m_iCmd = REPL_HEAD;
				if (ret==2) m_iCmd = REPL_HEAD_DATE;
			}
			m_strConTent = strTmp;
			return TRUE;
		}
		if (strTitle.Left(1)=="*")
		{
			strTmp = strTitle.Right(l-1);
			ret = CheckStr(strTmp); 
			if (!ret) return FALSE;
			if (IsIns)
			{
				if (ret==1) m_iCmd = INS_TAIL;
				if (ret==2) m_iCmd = INS_TAIL_DATE;
			}
			else
			{
				if (ret==1) m_iCmd = REPL_TAIL;
				if (ret==2) m_iCmd = REPL_TAIL_DATE;
			}
			m_strConTent = strTmp;
			return TRUE;
		}
	}
	//修改文件名扩展名情况分析
	if (strTitle=="*")
	{
		l = strExt.GetLength();
		if (strExt.Right(1)=="*")
		{
			strTmp = strExt.Left(l-1);
			ret = CheckStr(strTmp); 
			if (!ret) return FALSE;
			if (IsIns)
			{
				if (ret==1) m_iCmd = INS_EXTHEAD;
				if (ret==2) m_iCmd = INS_EXTHEAD_DATE;
			}
			else
			{
				if (ret==1) m_iCmd = REPL_EXTHEAD;
				if (ret==2) m_iCmd = REPL_EXTHEAD_DATE;
			}
			m_strConTent = strTmp;
			return TRUE;
		}
		if (strExt.Left(1)=="*")
		{
			strTmp = strExt.Right(l-1);
			ret = CheckStr(strTmp); 
			if (!ret) return FALSE;
			if (IsIns)
			{
				if (ret==1) m_iCmd = INS_EXTTAIL;
				if (ret==2) m_iCmd = INS_EXTTAIL_DATE;
			}
			else
			{
				if (ret==1) m_iCmd = REPL_EXTTAIL;
				if (ret==2) m_iCmd = REPL_EXTTAIL_DATE;
			}
			m_strConTent = strTmp;
			return TRUE;
		}
	}

	return FALSE;
	
}

void CRfnameDlg::SaveBuffer()
{
	FILE *fp;
	char *fname = "rfname.log";
	int iRowCnt;
	CString cstr;

	fp = fopen(fname,"w");
	iRowCnt = m_ResultList.GetItemCount();
	for (int i=0;i<iRowCnt;i++)
	{
		cstr = m_ResultList.GetItemText(i,0);
		cstr += "\n";
		fwrite(cstr,cstr.GetLength(),1,fp);
	}

	fclose(fp);
}

void CRfnameDlg::ProcRename(CString strFile,CString strCreateDate)
{
	CString strDir,strName;
	CString strTitle,strExt;
	int iStrLen = strFile.GetLength();
	int i=0,ifnd;
	ifnd = -1;
	
	//查找最后一个'\'
	while(i>=0)
	{
		i = strFile.Find("\\",i);
		if (i>=0) {ifnd = i;i++;}	
	}
	strDir = strFile.Left(ifnd+1);
	strName = strFile.Mid(ifnd+1);
	strTitle = ExtractFNameSubStr(strName,0);	//文件名称title
	strExt = ExtractFNameSubStr(strName,1);		//扩展名
	
	//不匹配的扩展名不修改
	if (m_strExt!="*" && strExt!=m_strExt) return;
	//创建时间不在选定的范围内不修改
	if (strCreateDate<m_strBeginDate || strCreateDate>m_strEndDate) return;

	CString strTmp=strCreateDate.Left(m_strConTent.GetLength());

	//根据命令条件作改名操作
	switch(m_iCmd)
	{
	case REPL_HEAD:		//从头替换
		strName.Delete(0,m_strConTent.GetLength());
		strName = m_strConTent + strName;
		break;
	case REPL_HEAD_DATE:
		strName.Delete(0,strTmp.GetLength());
		strName = strTmp + strName;
		break;
	case INS_HEAD:		//从头插入
		strName = m_strConTent + strName;
		break;
	case INS_HEAD_DATE:
		strName = strTmp + strName;
		break;
	case REPL_TAIL:		//从尾替换
		strName = strTitle.Left(strTitle.GetLength() - 
				  m_strConTent.GetLength()) + m_strConTent +
				  "." + strExt;
		break;
	case REPL_TAIL_DATE:
		strName = strTitle.Left(strTitle.GetLength() - 
				  strTmp.GetLength()) + strTmp +
				  "." + strExt;
		break;
	case INS_TAIL:		//从尾插入
		strName = strTitle + m_strConTent + "." + strExt;
		break;
	case INS_TAIL_DATE:
		strName = strTitle + strTmp + "." + strExt;
		break;
	/////////////////////////////////////////////////////////////
	case REPL_EXTHEAD:
		strExt.Delete(0,m_strConTent.GetLength());
		strName = strTitle + "." + m_strConTent + strExt;
		break;
	case REPL_EXTHEAD_DATE:
		strExt.Delete(0,strTmp.GetLength());
		strName = strTitle + "." + strTmp + strExt;
		break;
	case INS_EXTHEAD:
		strName = strTitle + "." + m_strConTent + strExt;
		break;
	case INS_EXTHEAD_DATE:
		strName = strTitle + "." + strTmp + strExt;
		break;
	case REPL_EXTTAIL:
		strName = strTitle + "." + strExt.Left(strExt.GetLength() - 
				  m_strConTent.GetLength()) + m_strConTent;
		break;
	case REPL_EXTTAIL_DATE:
		strName = strTitle + "." + strExt.Left(strExt.GetLength() - 
				  strTmp.GetLength()) + strTmp;
		break;
	case INS_EXTTAIL:
		strName = strName + m_strConTent;
		break;
	case INS_EXTTAIL_DATE:
		strName = strName + strTmp;
		break;

	default:
		break;
	}
	strDir +=strName;

	SetDlgItemText(IDC_STATICPATH,strFile);
	
	m_ResultList.InsertItem(0,strFile);
	m_ResultList.SetItemText(0,1,strDir);
}

void CRfnameDlg::BeginRename(CString strFile,BOOL blSub)
{
	RenameFile(strFile,blSub);
	
	CString sFname,dFname;
	 
	int iRowCnt = m_ResultList.GetItemCount();
	for (int i=0;i<iRowCnt;i++)
	{
		sFname = m_ResultList.GetItemText(i,0);
		dFname = m_ResultList.GetItemText(i,1);
	
		TRY
		{
			CFile::Rename(sFname,dFname);
		}
		CATCH( CFileException, e )
		{
			#ifdef _DEBUG
				afxDump << "File " << strFile << " not found, cause = "
				<< e->m_cause << "\n";
			#endif
		}
		END_CATCH
	}
}

void CRfnameDlg::RenameFile(CString strFile,BOOL blSub)
{
	CFileFind ff;
	CString szDir = strFile;

	if(szDir.Right(1) != "\\")
		szDir += "\\";
	
	szDir += "*.*";
	
	BOOL res = ff.FindFile(szDir);
	while(res)
	{
		res = ff.FindNextFile();
		if(ff.IsDirectory() && !ff.IsDots())
		{
			//如果是一个子目录，并且子目录文件也改名
			//用递归继续往深一层找
			CString strPath = ff.GetFilePath();
			if (blSub) RenameFile(strPath,blSub);
		}
		else if(!ff.IsDirectory() && !ff.IsDots())
		{
			//显示当前访问的文件
			CString strPath = ff.GetFilePath();
			CTime refTime ;
			ff.GetLastWriteTime(refTime);
			CString strCDate = refTime.Format("%Y%m%d");
			ProcRename(strPath,strCDate);
		}
	}
	ff.Close();//关闭
}

void CRfnameDlg::BrowseFile(int CallNum, CString strFile)
{
	CallNum++;
	CFileFind ff;
	CString szDir = strFile;
	
	if(szDir.Right(1) != "\\")
		szDir += "\\";
	
	szDir += "*.*";
	
	BOOL res = ff.FindFile(szDir);
	while(res)
	{
		res = ff.FindNextFile();
		if(ff.IsDirectory() && !ff.IsDots())
		{
			//如果是一个子目录，用递归继续往深一层找
			CString strPath = ff.GetFilePath();
			CString strTitle = ff.GetFileName();//ff.GetFileTitle();
//			int i =0;
			
			switch(CallNum)
			{
				case 1:
					strHTFir = m_FileTree.InsertItem(strTitle,0,0,NULL);						
					break;
				case 2:
					strHTSec = m_FileTree.InsertItem(strTitle,0,0,strHTFir);											
					break;
				case 3:
					strHtThi = m_FileTree.InsertItem(strTitle,0,0,strHTSec);
					break;
				case 4:
					strHtFor = m_FileTree.InsertItem(strTitle,0,0,strHtThi);
					break;
				default:
					strHtFif = m_FileTree.InsertItem(strTitle,0,0,strHtFor);
					break;					
			}
			BrowseFile(CallNum,strPath);
		}
		else if(!ff.IsDirectory() && !ff.IsDots())
		{
			//显示当前访问的文件
			CString strPath;
			CString strTitle;
			strPath = ff.GetFilePath();
			strTitle = ff.GetFileName();//ff.GetFileTitle();
			switch(CallNum)
			{
			case 1:
				strRoot = m_FileTree.InsertItem(strTitle,1,1,NULL);
				break;
			case 2:
				strHtEnd = m_FileTree.InsertItem(strTitle,1,1,strHTFir);
				break;
			case 3:
				strHtEnd = m_FileTree.InsertItem(strTitle,1,1,strHTSec);				
				break;
			case 4:
				strHtEnd = m_FileTree.InsertItem(strTitle,1,1,strHtThi);
				break;
			case 5:
				strHtEnd = m_FileTree.InsertItem(strTitle,1,1,strHtFor);
				break;
			default:
				strHtEnd = m_FileTree.InsertItem(strTitle,1,1,strHtFif);
				break;
			}
		}
	}
	ff.Close();//关闭
}

BEGIN_MESSAGE_MAP(CRfnameDlg, CDialog)
	//{{AFX_MSG_MAP(CRfnameDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONSELDIR, OnButtonseldir)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_BUTTONQES, OnButtonqes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRfnameDlg message handlers

BOOL CRfnameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_btnQes.SetBitmap(m_bitmapQes);

	m_iImageList.Create(16, 16, TRUE, 2,0);

	HICON hIcon = NULL;
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDI_FOLDER), IMAGE_ICON, 16, 16, 0);
	m_iImageList.Add(hIcon);

	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDI_DOC), IMAGE_ICON, 16, 16, 0);
	m_iImageList.Add(hIcon);

	m_FileTree.SetImageList ( &m_iImageList,TVSIL_NORMAL );	

	((CButton*)GetDlgItem(IDC_RADIOREPL))->SetCheck(TRUE);
	m_EditExt.SetWindowText("*");
	m_SubDir.SetCheck(BST_CHECKED);
	
	m_ResultList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ResultList.InsertColumn(0,_T("修改前文件名"),LVCFMT_CENTER,240);
	m_ResultList.InsertColumn(1,_T("修改后文件名"),LVCFMT_LEFT,240);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRfnameDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRfnameDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRfnameDlg::OnButtonseldir() 
{
	// TODO: Add your control notification handler code here
	CDirDialog	dlg;		
	if (dlg.DoBrowse(this) == IDOK)
	{
		m_strDirPath = dlg.m_strPath;
		SetDlgItemText(IDC_STATICPATH,m_strDirPath);
		m_FileTree.DeleteAllItems();
		BrowseFile(0,m_strDirPath);
	}
}

void CRfnameDlg::OnCheck3() 
{
	// TODO: Add your control notification handler code here
	CWnd* pWndParent = GetParent();
	CWnd* pWnd1 = GetDlgItem(IDC_DATETIMEPICKER1);
	CWnd* pWnd2 = GetDlgItem(IDC_DATETIMEPICKER2);

	if (m_DateSpan.GetCheck() ==BST_CHECKED)
	{
		pWnd1->EnableWindow(TRUE);
		pWnd2->EnableWindow(TRUE);
	}
	else
	{
		pWnd1->EnableWindow(FALSE);
		pWnd2->EnableWindow(FALSE);
	}
}

void CRfnameDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_EditExt.GetWindowText(m_strExt);

	m_strExt.TrimLeft();
	m_strExt.TrimRight();
	if (m_strExt=="")  m_strExt = "*";

	if (m_DateSpan.GetCheck()==1)
	{
		m_DateBegin.GetWindowText(m_strBeginDate);
		m_strBeginDate = FormatDateStr(m_strBeginDate);
		m_DateEnd.GetWindowText(m_strEndDate);
		m_strEndDate = FormatDateStr(m_strEndDate);
	}
	else
	{
		m_strBeginDate = "00000000";
		m_strEndDate = "99999999";
	}

	if (!SyntaxAnalyze())
	{
		MessageBox("输入文件格式错","提示",MB_OK);
		m_EditFmt.SetFocus();
		return;
	}

	m_ResultList.DeleteAllItems();
	if (m_SubDir.GetCheck()==1)
		BeginRename(m_strDirPath,TRUE);
	else
		BeginRename(m_strDirPath,FALSE);

	int iRowCnt = m_ResultList.GetItemCount();
	CString cstr;
	cstr.Format("%d",iRowCnt);
	cstr = "共"+ cstr;
	cstr += "个文件改名";
	m_ResultList.InsertItem(iRowCnt,cstr);

	if (m_FNameSave.GetCheck()==1)
	{
		SaveBuffer();
	}
	
	SetDlgItemText(IDC_STATICPATH,m_strDirPath);
	//CDialog::OnOK();
}

void CRfnameDlg::OnButtonqes() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,"open","rule.txt",NULL,NULL,SW_SHOWNORMAL);	
}
