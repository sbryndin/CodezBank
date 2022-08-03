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
#pragma once
#include "SyntaxColorView.h"

// Forward decls
namespace Document   { class CCodezBankDoc;};
namespace Data       { class CCodeNode;};

using namespace Document;
using namespace Data;
using namespace ColorSyntax;

namespace Views{
class CCodeView : public CSyntaxColorView
{
	DECLARE_DYNCREATE(CCodeView)

public:
   CCodezBankDoc* GetDocument();
   virtual void OnInitialUpdate();
   virtual HMENU GetContextMenu(WORD seltype, LPOLEOBJECT lpoleobj,
		CHARRANGE* lpchrg);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
   Data::CCodeNode* m_pData;
   
   CCodeView();           // protected constructor used by dynamic creation
   virtual ~CCodeView();
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
   virtual void OnTextChanged(LPCTSTR lpText);

	DECLARE_MESSAGE_MAP()
   afx_msg void OnEditUndo();
   afx_msg void OnEditRedo();
   afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CCodezBankDoc* CCodeView::GetDocument()
{ return reinterpret_cast<CCodezBankDoc*>(m_pDocument); }
#endif

};
