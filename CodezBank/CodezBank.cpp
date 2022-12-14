////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2008  
// 
// The following code is supplied "as-is" and 
// is given without warranty of any kind. 
// It may be freely distributed and used. 
// The user holds author blameless from any complications or 
// problems that may arise from the use of this code.
// 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  $Workfile: $   created: 04/05/2008 11:35
////////////////////////////////////////////////////////////////////////////////
//  $Revision: $  $Modtime: $
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Author: steveb
//
// History:
// 
// 08/22/2022	Added logic to load last file on the startup
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CodezBank.h"
#include "MainFrm.h"
#include "CodezBankDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Views;
using namespace Document;

BEGIN_MESSAGE_MAP(CCodezBankApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CCodezBankApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()

CCodezBankApp::CCodezBankApp()
{
	m_bHiColorIcons = TRUE;
	m_nAppLook = 0;
	m_bLoadLastFile = TRUE;
	m_clrKeyword = clr_Blue;
	m_clrComment = clr_DkGreen;
	m_clrString = clr_Brown;
	m_strFont = _T("Courier");
	m_nFontHeight = 10;
}

// The one and only CCodezBankApp object
CCodezBankApp theApp;

BOOL CCodezBankApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

   // Initialize RichEdit control
	if (!AfxInitRichEdit())
	{
		AfxMessageBox(IDS_RICHED_LOAD_FAIL, MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	EnableTaskbarInteraction();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("ObjectiveMarkets"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCodezBankDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeftView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Load last file on startup if the option is selected
	if (m_bLoadLastFile && m_pRecentFileList)
	{		
		if (m_pRecentFileList->GetSize() > 0)
		{
			CString strLastFile = (*m_pRecentFileList)[0];
			// Can be empty
			if(!strLastFile.IsEmpty())
				OpenDocumentFile(strLastFile);
		}			
	}

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

int CCodezBankApp::ExitInstance()
{
	AfxOleTerm(FALSE);
	return CWinAppEx::ExitInstance();
}

void CCodezBankApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_TEXT_POPUP);
	bNameValid = strName.LoadString(IDS_TREE_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_TREE_CONTEXT);
}

void CCodezBankApp::LoadCustomState()
{
	// Load values from registry
	m_bLoadLastFile = GetInt(KEY_LOADLASTFILE, TRUE);
	m_clrKeyword = GetInt(KEY_KEYWORDCOLOR, clr_Blue);
	m_clrComment = GetInt(KEY_COMMENTCOLOR, clr_DkGreen);
	m_clrString = GetInt(KEY_STRINGCOLOR, clr_Brown);
	m_strFont = GetString(KEY_FONTNAME, _T("Courier"));
	m_nFontHeight = GetInt(KEY_FONTHEIGHT, 10);
}

void CCodezBankApp::SaveCustomState()
{
	// Save value into registry
	WriteInt(KEY_LOADLASTFILE, (int)m_bLoadLastFile);
	WriteInt(KEY_KEYWORDCOLOR, (int)m_clrKeyword);
	WriteInt(KEY_COMMENTCOLOR, (int)m_clrComment);
	WriteInt(KEY_STRINGCOLOR, (int)m_clrString);
	WriteString(KEY_FONTNAME, m_strFont);
	WriteInt(KEY_FONTHEIGHT, m_nFontHeight);
}

void CCodezBankApp::SetLoadLastFile(BOOL val)
{
	if (m_bLoadLastFile == val)
		return;

	m_bLoadLastFile = val;

	// Save value into registry
	WriteInt(KEY_LOADLASTFILE, (int)m_bLoadLastFile);
}

void CCodezBankApp::SetKeywordColor(COLORREF val)
{
	if (m_clrKeyword == val)
		return;

	m_clrKeyword = val;
	WriteInt(KEY_KEYWORDCOLOR, (int)m_clrKeyword);
	m_pMainWnd->SendMessageToDescendants(WM_UPDATEEDITORCOLORS);
}

void CCodezBankApp::SetCommentColor(COLORREF val)
{
	if (m_clrComment == val)
		return;

	m_clrComment = val;
	WriteInt(KEY_COMMENTCOLOR, (int)m_clrComment);
	m_pMainWnd->SendMessageToDescendants(WM_UPDATEEDITORCOLORS);
}

void CCodezBankApp::SetStringColor(COLORREF val)
{
	if (m_clrString == val)
		return;

	m_clrString = val;
	WriteInt(KEY_STRINGCOLOR, (int)m_clrString);
	m_pMainWnd->SendMessageToDescendants(WM_UPDATEEDITORCOLORS);
}

void CCodezBankApp::SetFontName(LPCTSTR val)
{
	if (m_strFont.Compare(val) == 0)
		return;

	m_strFont = val;
	WriteString(KEY_FONTNAME, m_strFont);
	m_pMainWnd->SendMessageToDescendants(WM_UPDATEEDITORFONT);
}

void CCodezBankApp::SetFontHeight(int val)
{
	if (m_nFontHeight == val)
		return;

	m_nFontHeight = val;
	WriteInt(KEY_FONTHEIGHT, m_nFontHeight);
	m_pMainWnd->SendMessageToDescendants(WM_UPDATEEDITORFONT);
}

namespace Dialogs
{

	// CAboutDlg dialog used for App About
	class CAboutDlg : public CDialog
	{
	public:
		CAboutDlg();

		// Dialog Data
		enum { IDD = IDD_ABOUTBOX };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		// Implementation
	protected:
		DECLARE_MESSAGE_MAP()
	};

	CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
	{
	}

	void CAboutDlg::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
	}

	BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	END_MESSAGE_MAP()
}

// App command to run the dialog
void CCodezBankApp::OnAppAbout()
{
	using namespace Dialogs;

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
