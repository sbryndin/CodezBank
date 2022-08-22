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
#pragma once
#include "wndpos.h"
#include "Titlebar.h"
#include "MultiViewSplitterWnd.h"

namespace Views
{
   class CCodezBankView;
   class CLeftView;
};
using namespace Views;
using namespace Utils;


class CMainFrame : public CFrameWndEx
{
	DECLARE_DYNCREATE(CMainFrame)
  
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);
	virtual ~CMainFrame();
	CCodezBankView* GetRightPane();
   CLeftView* GetLeftPane();
   virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
   void SwitchView(LPCTSTR lpNewView);
   void UpdateIndSnipCount(CCmdUI* pCmdUI, INT_PTR nCount);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
   CMFCMenuBar             m_wndMenuBar;
   CMFCToolBar             m_wndToolBar;
   CMFCStatusBar           m_wndStatusBar;
   CMultiViewSplitterWnd   m_wndSplitter;
   CTitleBar               m_titleBar;
   CFindReplaceDialog*     m_pFindDlg;

   CMainFrame();

   DECLARE_MESSAGE_MAP()
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
   afx_msg void OnViewStyle(UINT nCommandID);
   afx_msg void OnDestroy();
   afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
   afx_msg LRESULT OnFindReplace(WPARAM wparam, LPARAM lparam);
   afx_msg void OnEditFind();
   afx_msg void OnApplicationLook(UINT id);
   afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
   afx_msg void OnViewCustomize();
   afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
   afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnFileOptions();
};


