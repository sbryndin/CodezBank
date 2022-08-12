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
#pragma once

namespace Data       { class CCodeNode;};

namespace Views{
class CNotesView : public CEditView
{
	DECLARE_DYNCREATE(CNotesView)

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
   CFont m_font;
   Data::CCodeNode* m_pData;

   CNotesView();           // protected constructor used by dynamic creation
	virtual ~CNotesView();
   virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	DECLARE_MESSAGE_MAP()
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnEnChange();
};

};
