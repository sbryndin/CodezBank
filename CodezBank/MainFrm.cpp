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
//  $Workfile: $   created: 04/05/2008 11:11
////////////////////////////////////////////////////////////////////////////////
//  $Revision: $  $Modtime: $
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Author: steveb
//
// History:
// 
// 08/22/2022	Added Options property sheets for configuring the app settings
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CodezBank.h"
#include "MainFrm.h"
#include "LeftView.h"
#include "CodezBankView.h"
#include "CodeView.h"
#include "CodeContainer.h"
#include "AppPropSheet.h"
#include "GeneralPropPage.h"
#include "TextEditPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Views;

static int WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, &CMainFrame::OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, &CMainFrame::OnViewStyle)
   ON_WM_DESTROY()
   ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
   ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplace)
   ON_COMMAND(ID_EDIT_FIND, &CMainFrame::OnEditFind)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_FILE_OPTIONS, &CMainFrame::OnFileOptions)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
   ID_INDICATOR_SNIP_COUNT,
};

///////////////////////////////////////////////
/// setup '*' indicator for title
/// 
///////////////////////////////////////////////
CMainFrame::CMainFrame():
  //m_pos(SECTION_SETTINGS, KEY_MAIN_WND)
  m_pFindDlg(NULL)
{
   static UINT indicators[] = { ID_TITLE_MOD };
   m_titleBar.SetIndicators(indicators, 
      sizeof(indicators)/sizeof(indicators[0]));

	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	//CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();
	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if(!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

   int nTreeWidth = AfxGetApp()->GetProfileInt(SECTION_SETTINGS, KEY_TREEVIEW, 250);

	if(!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(nTreeWidth, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CCodezBankView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

   m_wndSplitter.AddView(0, 1, RUNTIME_CLASS(/*CCodeView*/CCodeContainer), "Code");
  	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}

#endif //_DEBUG

CCodezBankView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CCodezBankView* pView = DYNAMIC_DOWNCAST(CCodezBankView, pWnd);
	return pView;
}

CLeftView* CMainFrame::GetLeftPane()
{
   CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	CLeftView* pView = DYNAMIC_DOWNCAST(CLeftView, pWnd);
	return pView;
}

void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	if (!pCmdUI)
		return;

	// TODO: customize or extend this code to handle choices on the View menu

	CCodezBankView* pView = GetRightPane(); 

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
}

void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the View menu
	CCodezBankView* pView = GetRightPane();

	// if the right-hand pane has been created and is a CCodezBankView,
	// process the menu commands...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

		// other commands change the style on the list control
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1)
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);

      AfxGetApp()->WriteProfileInt(SECTION_SETTINGS, KEY_LISTSTYLE, dwStyle);
	}
}

void CMainFrame::OnDestroy()
{

   CFrameWndEx::OnDestroy();
}

LRESULT CMainFrame::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
   m_titleBar.OnIdleUpdate(this, (BOOL)wParam);	// pass to title bar
   CFrameWndEx::OnIdleUpdateCmdUI();				// pass to default handler
   return 0L;
}

///////////////////////////////////////////////
/// forward command messages to CTitleBar
/// 
///////////////////////////////////////////////
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
   return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) ?
            TRUE : m_titleBar.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::SwitchView(LPCTSTR lpNewView)
{
   m_wndSplitter.SwitchToView(0, 1, lpNewView);
}

LRESULT CMainFrame::OnFindReplace(WPARAM wparam, LPARAM lparam)
{
   if(NULL != m_pFindDlg )
   {
      // Use pDlg as a pointer to the existing FindReplace dlg to 
      // call CFindReplaceDialog member functions
      if( m_pFindDlg->IsTerminating() )
      {
         CString   csFindString;
         CString   csReplaceString;
         
         csFindString = m_pFindDlg->GetFindString();
         csReplaceString = m_pFindDlg->GetReplaceString();

         VERIFY( AfxGetApp()->WriteProfileString(SECTION_SETTINGS,
            "FindString", csFindString ) );
         VERIFY( AfxGetApp()->WriteProfileString(SECTION_SETTINGS,
            "ReplaceString", csReplaceString ));
         
         m_pFindDlg = NULL;
         return 0L;
      }

      GetLeftPane()->FindString(m_pFindDlg->GetFindString(), 
         m_pFindDlg->SearchDown(), m_pFindDlg->MatchCase(), m_pFindDlg->MatchWholeWord());
   }

   return 0L;
}

void CMainFrame::OnEditFind()
{
   if(m_pFindDlg == NULL)
   {
      CString csFindString = AfxGetApp()->GetProfileString(SECTION_SETTINGS, "FindString", "");
      CString csReplaceString = AfxGetApp()->GetProfileString(SECTION_SETTINGS, "ReplaceString", "");

      m_pFindDlg = new CFindReplaceDialog;
      m_pFindDlg->Create( TRUE, csFindString, csReplaceString, FR_DOWN, Frame() ); 

      m_pFindDlg->m_fr.lStructSize = sizeof(FINDREPLACE);
      m_pFindDlg->m_fr.hwndOwner = Frame()->m_hWnd;
   }
}

void CMainFrame::UpdateIndSnipCount(CCmdUI* pCmdUI, INT_PTR nCount)
{
   CString str;
   str.Format("Snippet Count: %d", (UINT)nCount);

   CClientDC dc(&m_wndStatusBar);
	CFont* pOldFont = dc.SelectObject(m_wndStatusBar.GetFont());
	CSize szExtent = dc.GetTextExtent(str, str.GetLength());
	dc.SelectObject(pOldFont);

	UINT nStyle, nID;
	int nWidth;
	 
	m_wndStatusBar.GetPaneInfo(2, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(2, nID, nStyle, szExtent.cx/* + 18*/);

	pCmdUI->SetText(str);
	pCmdUI->Enable();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	//m_wndOutput.UpdateFonts();
	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// base class does the real work
	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}
	return TRUE;
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp, LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp, lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
//	m_wndOutput.UpdateFonts();
}

void CMainFrame::OnFileOptions()
{
	using namespace Dialogs;

	CAppPropSheet		propSheet(this, 0);
	CGeneralPropPage	pageGeneral;
	CTextEditPropPage pageText;

	propSheet.EnablePageHeader(max(60, afxGlobalData.GetTextHeight() * 3));
	//propSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	propSheet.SetLook(CMFCPropertySheet::PropSheetLook_List, 124);

	propSheet.AddPage(&pageGeneral);
	propSheet.AddPage(&pageText);

	if (propSheet.DoModal() != IDOK)
	{
		return;
	}
}
