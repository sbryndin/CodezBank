// AppPropSheet.cpp : implementation file

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
// 08/22/2022	Created as per TODO list
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CodezBank.h"
#include "AppPropSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Dialogs;

// CAppPropSheet
IMPLEMENT_DYNAMIC(CAppPropSheet, CMFCPropertySheet)

CAppPropSheet::CAppPropSheet(CWnd* pWndParent, UINT nSelectedPage) :
	CMFCPropertySheet(_T("Options"), pWndParent, nSelectedPage)
{
	m_Icons.SetImageSize(CSize(32, 32));
	m_Icons.Load(IDB_OPTIONS);

	CMFCControlRendererInfo params(_T(""), CLR_DEFAULT, 
		CRect(0, 0, 350, 60), CRect(83, 58, 266, 1), CRect(0, 0, 0, 0), CRect(0, 0, 0, 0), FALSE);

	params.m_uiBmpResID = IDB_HEADERPAT_1;
	m_Pat[0].Create(params);
	params.m_uiBmpResID = IDB_HEADERPAT_2;
	m_Pat[1].Create(params);
	params.m_uiBmpResID = IDB_HEADERPAT_3;
	m_Pat[2].Create(params);
	params.m_uiBmpResID = IDB_HEADERPAT_4;
	m_Pat[3].Create(params);
}

CAppPropSheet::~CAppPropSheet()
{
}


BEGIN_MESSAGE_MAP(CAppPropSheet, CMFCPropertySheet)
END_MESSAGE_MAP()

// CAppPropSheet message handlers
BOOL CAppPropSheet::OnInitDialog()
{
	BOOL bResult = CMFCPropertySheet::OnInitDialog();

	// Ensure that the Options dialog is fully visible on screen
	CRect rectDialog;
	GetWindowRect(&rectDialog);

	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYMAXIMIZED) - (GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYMAXIMIZED));

	if ((rectDialog.left < 0) || (rectDialog.top < 0))
	{
		SetWindowPos(NULL, rectDialog.left < 0 ? 0 : rectDialog.left, rectDialog.top < 0 ? 0 : rectDialog.top, 0, 0, SWP_NOSIZE);
	}
	else if ((rectDialog.right > cxScreen) || (rectDialog.bottom > cyScreen))
	{
		SetWindowPos(NULL, rectDialog.right > cxScreen ? cxScreen - rectDialog.Width() : rectDialog.left, rectDialog.bottom > cyScreen ? cyScreen - rectDialog.Height() : rectDialog.top, 0, 0, SWP_NOSIZE);
	}

	return bResult;
}

void CAppPropSheet::OnDrawPageHeader(CDC* pDC, int nPage, CRect rectHeader)
{
	CSize sizeIcon = m_Icons.GetImageSize();
	CDrawingManager dm(*pDC);

	COLORREF clrFill = afxGlobalData.clrBarFace;
	CMFCControlRenderer* pRenderer = NULL;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_OFF_2007_SILVER: 
		pRenderer = &m_Pat[0];
		break;
	case ID_VIEW_APPLOOK_OFF_2007_BLUE:
		pRenderer = &m_Pat[1];
		break;
	case ID_VIEW_APPLOOK_OFF_2007_BLACK:
		pRenderer = &m_Pat[2];
		break;
	case ID_VIEW_APPLOOK_OFF_2007_AQUA:
		pRenderer = &m_Pat[3];
		break;
	}

	if (pRenderer != NULL)
	{
		pRenderer->Draw(pDC, rectHeader);
	}
	else
	{
		dm.FillGradient(rectHeader, pDC->GetPixel(rectHeader.left, rectHeader.bottom), clrFill);
	}

	rectHeader.bottom -= 10;

	CRect rectIcon = rectHeader;
	rectIcon.left += 20;
	rectIcon.right = rectIcon.left + sizeIcon.cx;

	m_Icons.DrawEx(pDC, rectIcon, nPage, CMFCToolBarImages::ImageAlignHorzLeft, CMFCToolBarImages::ImageAlignVertCenter);

	CString strText;

	switch (nPage)
	{
	case 0:
		strText = _T("Configure most common application settings");
		break;

	case 1:
		strText = _T("Text editor common settings");
		break;

	case 2:
		strText = _T("Contact Microsoft, find online resources and obtain additional information regarding our products and services");
		break;
	}

	CRect rectText = rectHeader;
	rectText.left = rectIcon.right + 10;
	rectText.right -= 20;

	CFont* pOldFont = pDC->SelectObject(&afxGlobalData.fontBold);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(afxGlobalData.clrBarText);

	UINT uiFlags = DT_SINGLELINE | DT_VCENTER;

	CRect rectTextCalc = rectText;
	pDC->DrawText(strText, rectTextCalc, uiFlags | DT_CALCRECT);

	if (rectTextCalc.right > rectText.right)
	{
		rectText.DeflateRect(0, 10);
		uiFlags = DT_WORDBREAK;
	}

	pDC->DrawText(strText, rectText, uiFlags);

	pDC->SelectObject(pOldFont);
}
