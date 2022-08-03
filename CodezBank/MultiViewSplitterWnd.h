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
#pragma once

namespace Utils{

class CMultiViewSplitterWnd : public CSplitterWnd
{
	DECLARE_DYNAMIC(CMultiViewSplitterWnd)

public:
	CMultiViewSplitterWnd();
	virtual ~CMultiViewSplitterWnd();
   CView* AddView(int nRow, int nCol, CRuntimeClass* pViewClass, LPCTSTR lpID);
   CView* ReplaceView(int nRow, int nCol, CView* pNewView);
   void SwitchToView(int row, int col, LPCTSTR lpID = NULL);

protected:
   CMapStringToPtr m_mapViews;

	DECLARE_MESSAGE_MAP()
};

};