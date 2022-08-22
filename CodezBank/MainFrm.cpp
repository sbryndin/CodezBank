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
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CodezBank.h"
#include "MainFrm.h"
#include "LeftView.h"
#include "CodezBankView.h"
#include "CodeView.h"
#include "CodeContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Views;

static int WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, &CMainFrame::OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, &CMainFrame::OnViewStyle)
   ON_WM_DESTROY()
   ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
   ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplace)
   ON_COMMAND(ID_EDIT_FIND, &CMainFrame::OnEditFind)
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
  m_pos(SECTION_SETTINGS, KEY_MAIN_WND)
, m_pFindDlg(NULL)
{
   static UINT indicators[] = { ID_TITLE_MOD };
   m_titleBar.SetIndicators(indicators, 
      sizeof(indicators)/sizeof(indicators[0]));
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
CMainFrame::~CMainFrame()
{
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		! m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

   // Set up hot bar image lists.
   CImageList imageList;
   CBitmap    bitmap;

   // Create and set the normal toolbar image list.
   bitmap.LoadBitmap(IDB_TOOLBAR_COLD);
   imageList.Create(16, 15, ILC_COLORDDB|ILC_MASK, 16, 1);
   imageList.Add(&bitmap, RGB(255,0,255));
   m_wndToolBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
   imageList.Detach();
   bitmap.Detach();

   // Create and set the hot toolbar image list.
   bitmap.LoadBitmap(IDB_TOOLBAR_HOT);
   imageList.Create(16, 15, ILC_COLORDDB|ILC_MASK, 16, 1);
   imageList.Add(&bitmap, RGB(255,0,255));
   m_wndToolBar.SendMessage(TB_SETHOTIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
   imageList.Detach();
   bitmap.Detach();

	if(!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

   m_pos.Load(this);
   LoadBarState(KEY_CTRLBARS);
	return 0;
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
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

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

#ifdef _DEBUG
///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
CCodezBankView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CCodezBankView* pView = DYNAMIC_DOWNCAST(CCodezBankView, pWnd);
	return pView;
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
CLeftView* CMainFrame::GetLeftPane()
{
   CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	CLeftView* pView = DYNAMIC_DOWNCAST(CLeftView, pWnd);
	return pView;
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
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

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
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

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
void CMainFrame::OnDestroy()
{
   m_pos.Save(this);
   SaveBarState(KEY_CTRLBARS);

   CFrameWnd::OnDestroy();
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
LRESULT CMainFrame::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
   m_titleBar.OnIdleUpdate(this, (BOOL)wParam);	// pass to title bar
   CFrameWnd::OnIdleUpdateCmdUI();				// pass to default handler
   return 0L;
}

///////////////////////////////////////////////
/// forward command messages to CTitleBar
/// 
///////////////////////////////////////////////
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
   return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) ?
            TRUE : m_titleBar.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
void CMainFrame::SwitchView(LPCTSTR lpNewView)
{
   m_wndSplitter.SwitchToView(0, 1, lpNewView);
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
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

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
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

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
void CMainFrame::UpdateIndSnipCount(CCmdUI* pCmdUI, INT_PTR nCount)
{
   CString str;
   str.Format("Snippet Count: %d", nCount);

   CClientDC dc(&m_wndStatusBar);
	CFont* pOldFont = dc.SelectObject(m_wndStatusBar.GetFont());
	CSize szExtent = dc.GetTextExtent(str, str.GetLength());
	dc.SelectObject(pOldFont);

	UINT nStyle, nID;
	int nWidth;
	 
	m_wndStatusBar.GetPaneInfo(2, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(2, nID, nStyle, szExtent.cx/* + 18*/);

	pCmdUI->SetText(str);
}
