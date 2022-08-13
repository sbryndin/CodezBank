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
//  $Workfile: $   created: 04/05/2008 12:39
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
#include "MultiViewSplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Utils;

IMPLEMENT_DYNAMIC(CMultiViewSplitterWnd, CSplitterWndEx)

CMultiViewSplitterWnd::CMultiViewSplitterWnd()
{
}

CMultiViewSplitterWnd::~CMultiViewSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CMultiViewSplitterWnd, CSplitterWndEx)
END_MESSAGE_MAP()

CView* CMultiViewSplitterWnd::AddView(int nRow, int nCol, CRuntimeClass* pViewClass, LPCTSTR lpID)
{
   CView* pTemp=NULL;

   // can't have the same id!
   ASSERT(!m_mapViews.Lookup(lpID, (void*&)pTemp));

   CView* pOldView = DYNAMIC_DOWNCAST(CView, GetPane (nRow, nCol));

   if(!m_mapViews.Lookup("0", (void*&)pTemp))
      m_mapViews["0"] = pOldView; // very first view 

   CDocument* pDocument = pOldView->GetDocument();

   int nCtrlID = pOldView->GetDlgCtrlID();

   // Create new view
   CCreateContext contextT;
   contextT.m_pLastView = NULL;
   contextT.m_pCurrentDoc = pDocument;
   contextT.m_pNewViewClass = pViewClass;
   contextT.m_pNewDocTemplate = pDocument ? pDocument->GetDocTemplate() : NULL;
   contextT.m_pCurrentFrame = NULL;

   CWnd* pWnd = NULL;
   TRY
   {
      pWnd = (CWnd*)pViewClass->CreateObject();
      if (pWnd == NULL)
         AfxThrowMemoryException();
   }
   CATCH_ALL(e)
   {
      TRACE0("Out of memory creating a view.\n");
      // Note: DELETE_EXCEPTION(e) not required
      return NULL;
   }
   END_CATCH_ALL

      ASSERT_KINDOF(CWnd, pWnd);
   ASSERT(pWnd->m_hWnd == NULL); // not yet created.

   // Create with the right size (wrong position)
   if (!pWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
      CRect(0, 0, 0, 0), this, nCtrlID, &contextT))
   {
      TRACE0("Warning: couldn't create new view.\n");
      // pWnd will be cleaned up by PostNcDestroy
      return NULL;
   }

   CView* pNewView = DYNAMIC_DOWNCAST(CView, pWnd);
   ASSERT_KINDOF (CView, pNewView);

   m_mapViews[lpID] = pNewView;
  
   pNewView->OnInitialUpdate();

   if(m_mapViews.Lookup("0", (void*&)pTemp))
   {
      ReplaceView(nRow, nCol, pTemp);
   }

   return pNewView;
}

CView* CMultiViewSplitterWnd::ReplaceView(int nRow, int nCol, CView* pNewView)
{
   CView* pOldView = DYNAMIC_DOWNCAST(CView, GetPane (nRow, nCol));
   ASSERT_KINDOF (CView, pOldView);

   if (pOldView == pNewView)
      return NULL;

   int nCtrlID = pOldView->GetDlgCtrlID();

   // swap ids.
   pOldView->SetDlgCtrlID(0);
   pNewView->SetDlgCtrlID(nCtrlID);

   // show the views.
   pOldView->ShowWindow(SW_HIDE);
   pNewView->ShowWindow(SW_SHOW);
   //pNewView->SetFocus();

   RecalcLayout();

   return pOldView;
}

void CMultiViewSplitterWnd::SwitchToView(int row, int col, LPCTSTR lpID)
{
   CView* pTemp=NULL;

   if(lpID == NULL)
      lpID = "0";

   if(m_mapViews.Lookup(lpID, (void*&)pTemp))
   {
      ReplaceView(row, col, pTemp);
   }
}
