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
// 8/13/22 Updated CSplitterWnd
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace Views{
class CCodeContainer : public CView
{
	DECLARE_DYNCREATE(CCodeContainer)

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
   CSplitterWndEx m_wndSplitter;

   CCodeContainer();           // protected constructor used by dynamic creation
	virtual ~CCodeContainer();

	DECLARE_MESSAGE_MAP()
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnSize(UINT nType, int cx, int cy);
};

};

