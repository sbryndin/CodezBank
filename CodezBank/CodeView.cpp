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
//  $Workfile: $   created: 04/05/2008 17:38
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
#include "CodezBankDoc.h"
#include "CodeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Views;

IMPLEMENT_DYNCREATE(CCodeView, CSyntaxColorView)

CCodeView::CCodeView():
  m_pData(NULL)
{
}

CCodeView::~CCodeView()
{
}

BEGIN_MESSAGE_MAP(CCodeView, CSyntaxColorView)
   ON_COMMAND(ID_EDIT_UNDO, &CCodeView::OnEditUndo)
   ON_COMMAND(ID_EDIT_REDO, &CCodeView::OnEditRedo)
   ON_WM_SIZE()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CCodeView::AssertValid() const
{
	CSyntaxColorView::AssertValid();
}

#ifndef _WIN32_WCE
void CCodeView::Dump(CDumpContext& dc) const
{
	CSyntaxColorView::Dump(dc);
}
#endif

CCodezBankDoc* CCodeView::GetDocument() // non-debug version is inline
{
   ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCodezBankDoc)));
   return (CCodezBankDoc*)m_pDocument;
}
#endif //_DEBUG

void Views::CCodeView::OnInitialUpdate()
{
   CSyntaxColorView::OnInitialUpdate();

   AddKeyword("auto");
   AddKeyword("break");
   AddKeyword("case");
   AddKeyword("char");
   AddKeyword("class");
   AddKeyword("const");
   AddKeyword("continue");
   AddKeyword("default");
   AddKeyword("do");
   AddKeyword("double");
   AddKeyword("else");
   AddKeyword("enum");
   AddKeyword("extern");
   AddKeyword("float");
   AddKeyword("for");
   AddKeyword("goto");
   AddKeyword("if");
   AddKeyword("int");
   AddKeyword("long");
   AddKeyword("register");
   AddKeyword("return");
   AddKeyword("short");
   AddKeyword("signed");
   AddKeyword("sizeof");
   AddKeyword("static");
   AddKeyword("struct");
   AddKeyword("switch");
   AddKeyword("typedef");
   AddKeyword("template");
   AddKeyword("union");
   AddKeyword("unsigned");
   AddKeyword("void");
   AddKeyword("volatile");
   AddKeyword("while");
   AddKeyword("#include");
   AddKeyword("#define");
   AddKeyword("delete");
   AddKeyword("new");
   AddKeyword("public");
   AddKeyword("protected");
   AddKeyword("private");
   AddKeyword("#pragma");
   AddKeyword("once");
   AddKeyword("namespace");
   AddKeyword("#ifdef");
   AddKeyword("#else");
   AddKeyword("#elif");
   AddKeyword("#endif");
   AddKeyword("#if");
   AddKeyword("defined");
   AddKeyword("inline");
   AddKeyword("virtual");

   AddKeyword("#ifndef");
   AddKeyword("asm");
   AddKeyword("bool"); 
   AddKeyword("catch"); 
   AddKeyword("const_cast"); 
   AddKeyword("dynamic_cast"); 
   AddKeyword("explicit");
   AddKeyword("export");
   AddKeyword("false");
   AddKeyword("friend");
   AddKeyword("mutable"); 
   AddKeyword("operator");
   AddKeyword("reinterpret_cast");
   AddKeyword("static_cast");
   AddKeyword("this"); 
   AddKeyword("throw"); 
   AddKeyword("true"); 
   AddKeyword("try"); 
   AddKeyword("typeid"); 
   AddKeyword("typename"); 
   AddKeyword("using"); 
   AddKeyword("wchar_t");
}

BOOL Views::CCodeView::PreCreateWindow(CREATESTRUCT& cs)
{
   return CSyntaxColorView::PreCreateWindow(cs);
}

void Views::CCodeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
      //SetWindowText(m_pData->m_strCode);
      SetEditText(m_pData->m_strCode);
      break;
   case hintSelFoundCodeItem:
      FindText(m_pData->m_strFound, m_pData->m_bMatchCase, m_pData->m_bWholeWord);
      break;
   case hintClearDocument:
      SetWindowText("");
      break;
   }
}

void Views::CCodeView::OnTextChanged(LPCTSTR lpText)
{
   if(!m_pData)
      return;

   CString strText = lpText;

   if(m_pData->m_strCode != strText)
   {
      GetDocument()->SetCheckpoint();

      m_pData->m_strCode = strText;
      m_pData->Modify();
      GetDocument()->SetModifiedFlag();
   }
}

void Views::CCodeView::OnEditUndo()
{
   GetRichEditCtrl().Undo();
}

void Views::CCodeView::OnEditRedo()
{
   GetRichEditCtrl().Redo();
}

HMENU Views::CCodeView::GetContextMenu(WORD, LPOLEOBJECT, CHARRANGE* )
{
	CMenu menuText;
	menuText.LoadMenu(IDR_TEXT_POPUP);
	CMenu* pMenuPopup = menuText.GetSubMenu(0);
	menuText.RemoveMenu(0, MF_BYPOSITION);
	return pMenuPopup->Detach();
}

void Views::CCodeView::OnSize(UINT nType, int cx, int cy)
{
   CSyntaxColorView::OnSize(nType, cx, cy);

   AfxGetApp()->WriteProfileInt(SECTION_SETTINGS, KEY_CODEVIEW, cy);
}
