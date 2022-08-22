////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022  
// 
// The following code is supplied "as-is" and 
// is given without warranty of any kind. 
// It may be freely distributed and used. 
// The user holds author blameless from any complications or 
// problems that may arise from the use of this code.
// 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Author: steveb
//
// History:
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DlgBanner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Dialogs;

/////////////////////////////////////////////////////////////////////////////
// CDlgBanner

CDlgBanner::CDlgBanner()
{
}

CDlgBanner::~CDlgBanner()
{
}

BEGIN_MESSAGE_MAP(CDlgBanner, CStatic)
	//{{AFX_MSG_MAP(CDlgBanner)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBanner message handlers

void CDlgBanner::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(rect);

	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontBold);
	ASSERT(pOldFont != NULL);

	dc.SetTextColor(CMFCVisualManager::GetInstance()->OnDrawMenuLabel(&dc, rect));
	dc.SetBkMode(TRANSPARENT);

	rect.DeflateRect(5, 0);

	CString strText;
	GetWindowText(strText);

	dc.DrawText(strText, rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	dc.SelectObject(pOldFont);
}

