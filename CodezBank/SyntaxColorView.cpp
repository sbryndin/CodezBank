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
//  $Workfile: SyntaxColorView.cpp $   created: 04/06/2008 14:39
////////////////////////////////////////////////////////////////////////////////
//  $Revision: 1 $  $Modtime: 4/20/08 9:47a $
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Author: steveb
//
// Sections:
// ---------
// Comments.
// Keywords.
// String literals.
// Normal text.
//
// History:
// 
// 08/21/2022 Removed VBScript regular expressions and replaced it with
// std::regex
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CodezBank.h"
#include "SyntaxColorView.h"
#include "SyntaxParser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace ColorSyntax;

IMPLEMENT_DYNCREATE(CSyntaxColorView, CRichEditView)

CSyntaxColorView::CSyntaxColorView()
{
   m_nPasteType = 0;
   m_bSyncCharFormat = m_bSyncParaFormat = TRUE;
}

CSyntaxColorView::~CSyntaxColorView()
{
}

BEGIN_MESSAGE_MAP(CSyntaxColorView, CRichEditView)
   ON_WM_CREATE()
   ON_CONTROL_REFLECT(EN_CHANGE, &CSyntaxColorView::OnEnChange)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CSyntaxColorView::AssertValid() const
{
	CRichEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CSyntaxColorView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif
#endif //_DEBUG

int ColorSyntax::CSyntaxColorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CRichEditView::OnCreate(lpCreateStruct) == -1)
      return -1;

   IRichEditOle*pOle =  GetRichEditCtrl().GetIRichEditOle();
   if(pOle)
   {
      m_pTomDoc = pOle;
      pOle->Release();
   }

   // Set the printing margins (720 twips = 1/2 inch)
   SetMargins(CRect(720, 720, 720, 720));

   SetDefaultStyle();
   
   long lEventMask = GetRichEditCtrl().GetEventMask();
   lEventMask |= ENM_SELCHANGE;
   GetRichEditCtrl().SetEventMask(lEventMask);

   m_nWordWrap = WrapNone;
   WrapChanged();

   GetRichEditCtrl().SetOptions(ECOOP_OR, ECO_AUTOWORDSELECTION);

   GetRichEditCtrl().SetUndoLimit(100);

   return 0;
}

void ColorSyntax::CSyntaxColorView::SetDefaultStyle(void)
{
   CHARFORMAT2 cf;
   memset (&cf, '\0', sizeof (CHARFORMAT2));
   cf.cbSize = sizeof (CHARFORMAT2);
   GetRichEditCtrl().GetDefaultCharFormat (cf);
   cf.dwMask = CFM_FACE | CFM_SIZE | CFM_UNDERLINE | CFM_COLOR | CFM_BOLD;
   cf.yHeight = 10 * 20;	// 10 points in twips
   cf.dwEffects &= ~(CFE_AUTOCOLOR | CFE_BOLD);

   cf.dwEffects |= CFE_AUTOCOLOR;

   cf.crTextColor = RGB(0x00, 0x00, 0x00);
   strcpy_s(cf.szFaceName, "Courier");
   GetRichEditCtrl().SetDefaultCharFormat (cf);
}

void ColorSyntax::CSyntaxColorView::AddKeyword(LPCTSTR lpKeyword)
{
   m_mapKeywords[lpKeyword] = "Keyword";
}

void ColorSyntax::CSyntaxColorView::SetEditText(LPCTSTR lpText)
{
   CComPtr<ITextRange> pRange;
   m_pTomDoc->Range(0, GetRichEditCtrl().GetTextLength(), &pRange);

   CString strText = lpText;
   BSTR bstrText = strText.AllocSysString();
   pRange->SetText(bstrText);

   GetRichEditCtrl().EmptyUndoBuffer();
}

void ColorSyntax::CSyntaxColorView::OnEnChange()
{
   CComPtr<ITextRange> pRange;
   m_pTomDoc->Range(0, GetRichEditCtrl().GetTextLength(), &pRange);

   long count;
   m_pTomDoc->Freeze(&count);
   m_pTomDoc->Undo( tomSuspend, NULL);

   SetDefaultStyle();
   ResetToNormal();

   CComBSTR   str;
   pRange->GetText(&str);

   CString strText = CString(str);

   Scan(strText);

   m_pTomDoc->Unfreeze(&count);
   m_pTomDoc->Undo( tomResume,NULL);

   GetWindowText(strText);
   OnTextChanged(strText);
}

void ColorSyntax::CSyntaxColorView::Scan(CString strText)
{
   // Happens if the code is empty
   CString strTemp = strText;
   strTemp.Trim();

   if (strTemp.IsEmpty())
      return;

   VecIdentifiers vecMatches;
   CSyntaxParser parser;

   if (parser.GetWords(strText, vecMatches))
   {
      for (SyntaxIdentifier identif : vecMatches)
      {
         DoKeywords(identif.strWord, identif.nStart, identif.nEnd);
      }
   }
   vecMatches.clear();

   if (parser.GetQuotedStrings(strText, vecMatches))
   {
      for (SyntaxIdentifier identif : vecMatches)
      {
         DoStringLiterals(identif.nStart, identif.nEnd);
      }
   }
  
   // Comments C and C++
   int nPos = strText.Find("/*", 0);
   while(nPos > -1)
   {
      int nEnd = strText.Find("*/", nPos);
      if(nEnd != -1)
      {
         DoComments(nPos, nEnd+2);
         nPos = strText.Find("/*", nEnd);
      } 
      else
         break;
   }

   nPos = strText.Find("//", 0);
   while(nPos > -1)
   {
      int nEnd = strText.Find("\r", nPos);
      if(nEnd != -1)
      {
         DoComments(nPos, nEnd);
         nPos = strText.Find("//", nEnd);
      } 
      else
         break;
   }
}

void ColorSyntax::CSyntaxColorView::ColorItem(int nStart, int nEnd, COLORREF clr)
{
   CComPtr<ITextRange> pRange;
   m_pTomDoc->Range(nStart,nEnd, &pRange);

   CComPtr<ITextFont> pFont;
   pRange->GetFont(&pFont);
   pFont->SetForeColor(clr);
}

void ColorSyntax::CSyntaxColorView::DoComments(int nStart, int nEnd)
{
   ColorItem(nStart, nEnd, RGB(0, 0x80, 0));
}

void ColorSyntax::CSyntaxColorView::ResetToNormal(void)
{
   ColorItem(0, GetRichEditCtrl().GetTextLength(), RGB(0,0,0));
}

void ColorSyntax::CSyntaxColorView::DoKeywords(CString strWord, int nStart, int nEnd)
{
   CString strValue;
   if(m_mapKeywords.Lookup(strWord, strValue))
   {
      ColorItem(nStart, nEnd, RGB(0,0,255));
   }
}

void ColorSyntax::CSyntaxColorView::DoStringLiterals(int nStart, int nEnd)
{
   ColorItem(nStart, nEnd, RGB(0xA5, 0, 0));
}

BOOL ColorSyntax::CSyntaxColorView::PreCreateWindow(CREATESTRUCT& cs)
{
   BOOL bRes = CRichEditView::PreCreateWindow(cs);
   cs.style |= ES_AUTOHSCROLL | ES_AUTOVSCROLL |	ES_MULTILINE | ES_NOHIDESEL | ES_SAVESEL | ES_SELECTIONBAR;

   return bRes;
}
