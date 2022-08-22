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
// 08/22/2022	Created as per TODO list.
//					Added logic to load last file on the startup.
//
////////////////////////////////////////////////////////////////////////////////

// GeneralPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "CodezBank.h"
#include "GeneralPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Dialogs;

// CGeneralPropPage dialog
IMPLEMENT_DYNAMIC(CGeneralPropPage, CMFCPropertyPage)

CGeneralPropPage::CGeneralPropPage()
	: CMFCPropertyPage(CGeneralPropPage::IDD)
	, m_bLoadLastFile(FALSE)
{

}

CGeneralPropPage::~CGeneralPropPage()
{
}

void CGeneralPropPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_LOAD_LAST_FILE, m_bLoadLastFile);
}


BEGIN_MESSAGE_MAP(CGeneralPropPage, CMFCPropertyPage)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_LOAD_LAST_FILE, &CGeneralPropPage::OnClickedCheckLoadLastFile)
END_MESSAGE_MAP()


// CGeneralPropPage message handlers
void CGeneralPropPage::OnSize(UINT nType, int cx, int cy)
{
	CMFCPropertyPage::OnSize(nType, cx, cy);

	if (m_wndBanner.GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectBanner;
	m_wndBanner.GetWindowRect(rectBanner);

	CRect rectParent;
	GetParent()->GetWindowRect(rectParent);

	m_wndBanner.SetWindowPos(NULL, -1, -1, rectParent.right - rectBanner.left - 10, rectBanner.Height(), SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CGeneralPropPage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	m_bLoadLastFile = theApp.GetLoadLastFile();

	UpdateData(FALSE);
	return TRUE;
}

BOOL CGeneralPropPage::OnApply()
{
	UpdateData();

	if (IsButtonEnabled(ID_APPLY_NOW))
	{
		theApp.UpdateLoadLastFile(m_bLoadLastFile);
	}

	return CMFCPropertyPage::OnApply();
}

void CGeneralPropPage::OnClickedCheckLoadLastFile()
{
	SetModified();
}
