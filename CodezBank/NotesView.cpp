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
//  $Workfile: $   created: 06/30/2008 12:34
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
#include "NotesView.h"
#include "CodeNode.h"
#include "CodezBankDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Views;

IMPLEMENT_DYNCREATE(CNotesView, CEditView)

CNotesView::CNotesView():
  m_pData(NULL)
{
}

CNotesView::~CNotesView()
{
   m_font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CNotesView, CEditView)
   ON_WM_CREATE()
   ON_CONTROL_REFLECT(EN_CHANGE, &CNotesView::OnEnChange)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CNotesView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CNotesView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG

int Views::CNotesView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CEditView::OnCreate(lpCreateStruct) == -1)
      return -1;

   m_font.CreatePointFont(110, "Arial");
   SetFont(&m_font);
   return 0;
}

void Views::CNotesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
   m_pData = reinterpret_cast<CCodeNode*>(pHint);
   if(!m_pData)
   {
      SetWindowText("");
      return;
   }

   switch(lHint)
   {
   case hintRefreshCodeView:
      SetWindowText(m_pData->m_strNote);
      break;
   case hintClearDocument:
      SetWindowText("");
      break;
   }
}

void Views::CNotesView::OnEnChange()
{
   if(!m_pData)
      return;

   GetWindowText(m_pData->m_strNote);
   GetDocument()->SetModifiedFlag();
}
