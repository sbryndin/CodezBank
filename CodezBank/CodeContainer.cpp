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
//  $Workfile: $   created: 06/30/2008 12:08
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
#include "CodeContainer.h"
#include "CodeView.h"
#include "NotesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Views;

IMPLEMENT_DYNCREATE(CCodeContainer, CView)

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
CCodeContainer::CCodeContainer()
{
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
CCodeContainer::~CCodeContainer()
{
}

BEGIN_MESSAGE_MAP(CCodeContainer, CView)
   ON_WM_CREATE()
   ON_WM_SIZE()
END_MESSAGE_MAP()

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
void CCodeContainer::OnDraw(CDC* pDC)
{
}

#ifdef _DEBUG
///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
void CCodeContainer::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
void CCodeContainer::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
int Views::CCodeContainer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CView::OnCreate(lpCreateStruct) == -1)
      return -1;

   CCreateContext* pContext = (CCreateContext*)lpCreateStruct->lpCreateParams;

   // create splitter window
	if(!m_wndSplitter.CreateStatic(this, 2, 1))
		return FALSE;

   int nPaneHeight = AfxGetApp()->GetProfileInt(SECTION_SETTINGS, KEY_CODEVIEW, 450);

   if(nPaneHeight <= 0)
      nPaneHeight = 450;

	if(!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CCodeView), CSize(100, nPaneHeight), pContext) ||
		!m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CNotesView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

   return 0;
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
void Views::CCodeContainer::OnSize(UINT nType, int cx, int cy)
{
   CView::OnSize(nType, cx, cy);

   if(::IsWindow(m_wndSplitter.m_hWnd))
      m_wndSplitter.MoveWindow(-2, -2, cx+3, cy+3);
}
