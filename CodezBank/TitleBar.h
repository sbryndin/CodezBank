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
//  $Workfile: $   created: 04/05/2008 11:12
////////////////////////////////////////////////////////////////////////////////
//  $Revision: $  $Modtime: $
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Author: steveb
// Note: Based on the code from article in MSJ magazine by Paul DiLascia
// History: 
//
////////////////////////////////////////////////////////////////////////////////
#pragma once 
#include <afxpriv.h>

namespace Utils{

struct TITLEPANE {
   CString	m_sText;			// current text
   UINT		m_nID;			// indicator ID
   BOOL		m_bEnabled;		// enabled?
};

//////////////////
// This class is private to this file.
//
class CTitleCmdUI : public CCmdUI {
public: 
   TITLEPANE*	m_pPane;		// ptr to associate title pane

   // re-implementations:
   virtual void Enable(BOOL bOn)
   {
      m_bEnableChanged = TRUE;	// used by MFC
      m_pPane->m_bEnabled = bOn;
   }

   virtual void SetCheck(int nCheck) {}
   virtual void SetText(LPCTSTR lpszText)
   {
      m_pPane->m_sText = lpszText;
   }
};


//////////////////
// A titlebar is not really a window, just an object that manages title 
// bar "inidcators", which are like panes in the status bar.
//
class CTitleBar : public CCmdTarget {
   //DECLARE_DYNAMIC(CTitleBar)
   TITLEPANE*	m_arPanes;		// array of "panes" (inidicators)
   UINT			m_nCount;		// #panes
   CString		m_sPrevTitle;	// previous title
   CString		m_sTitle;		// current title
public:
   CTitleBar()
   {
      m_nCount  = 0;
      m_arPanes = NULL;
   }

   ~CTitleBar()
   {
      delete [] m_arPanes;			// OK if NULL
   }

   BOOL SetIndicators(const UINT* lpIDs, int nCount)
   {
      // Allocate indicator array
      ASSERT(nCount>0);
      m_arPanes = new TITLEPANE[nCount];
      ASSERT(m_arPanes);

      // Initialize panes
      for (int i=0; i<nCount; i++) {
         TITLEPANE& pane = m_arPanes[i];
         pane.m_nID = lpIDs[i];
         pane.m_bEnabled = TRUE;
      }
      m_nCount = nCount;
      return TRUE;
   }

   void OnIdleUpdate(CFrameWnd* pFrame, BOOL bDisableIfNoHndler)
   {
      m_sTitle.Empty();

      CTitleCmdUI cmdui;
      cmdui.m_nIndexMax = m_nCount;
      for (UINT i=0; i<cmdui.m_nIndexMax; i++) 
      {
         TITLEPANE& pane = m_arPanes[i];
         cmdui.m_nIndex  = i;
         cmdui.m_pPane   = &pane;
         cmdui.m_nID     = pane.m_nID;
         cmdui.DoUpdate(pFrame, FALSE);
         if (pane.m_bEnabled)
            m_sTitle += pane.m_sText;
      }
      if (m_sPrevTitle != m_sTitle) 
      {
         pFrame->OnUpdateFrameTitle(TRUE);
         CString title;
         pFrame->GetWindowText(title);
         title += m_sTitle;
         pFrame->SetWindowText(title);
         m_sPrevTitle = m_sTitle;
      }
   }
};

};
