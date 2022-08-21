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
//  $Workfile: $   created: 04/06/2008 14:38
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

#include <TOM.h>
#include <Richole.h>

namespace ColorSyntax{
class CSyntaxColorView : public CRichEditView
{
	DECLARE_DYNCREATE(CSyntaxColorView)

public:
   void AddKeyword(LPCTSTR lpKeyword);
 
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
   CComQIPtr<ITextDocument> m_pTomDoc;
   CMapStringToString m_mapKeywords;
   

   CSyntaxColorView();           // protected constructor used by dynamic creation
   virtual ~CSyntaxColorView();
   virtual void OnTextChanged(LPCTSTR lpText) {} // override in your code
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

   void SetEditText(LPCTSTR lpText);
   void SetDefaultStyle(void);   
   void Scan(CString lpText);
   void ResetToNormal(void);
   void ColorItem(int nStart, int nEnd, COLORREF clr);
   void DoComments(int nStart, int nEnd);
   void DoKeywords(CString strWord, int nStart, int nEnd);
   void DoStringLiterals(int nStart, int nEnd);   

	DECLARE_MESSAGE_MAP()
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnEnChange();
};
};
