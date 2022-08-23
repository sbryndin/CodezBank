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
	DDX_Control(pDX, IDC_COMBO_APPSKINS, m_cboSkins);
}


BEGIN_MESSAGE_MAP(CGeneralPropPage, CMFCPropertyPage)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_LOAD_LAST_FILE, &CGeneralPropPage::OnClickedCheckLoadLastFile)
	ON_CBN_SELCHANGE(IDC_COMBO_APPSKINS, &CGeneralPropPage::OnSelchangeComboAppskins)
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

	m_cboSkins.AddString(_T("Windows 2000"));
	m_cboSkins.AddString(_T("Office XP"));
	m_cboSkins.AddString(_T("Windows XP"));
	m_cboSkins.AddString(_T("Office 2003"));
	m_cboSkins.AddString(_T("Visual Studio 2005"));
	m_cboSkins.AddString(_T("Visual Studio 2008"));
	m_cboSkins.AddString(_T("Office Blue"));
	m_cboSkins.AddString(_T("Office Black"));
	m_cboSkins.AddString(_T("Office Silver"));
	m_cboSkins.AddString(_T("Office Aqua"));
	m_cboSkins.AddString(_T("Windows 7"));
	
	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		m_cboSkins.SetCurSel(0);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		m_cboSkins.SetCurSel(1);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		m_cboSkins.SetCurSel(2);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		m_cboSkins.SetCurSel(3);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		m_cboSkins.SetCurSel(4);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		m_cboSkins.SetCurSel(5);
		break;

	case ID_VIEW_APPLOOK_OFF_2007_BLUE:
		m_cboSkins.SetCurSel(6);
		break;

	case ID_VIEW_APPLOOK_OFF_2007_BLACK:
		m_cboSkins.SetCurSel(7);
		break;

	case ID_VIEW_APPLOOK_OFF_2007_SILVER:
		m_cboSkins.SetCurSel(8);
		break;

	case ID_VIEW_APPLOOK_OFF_2007_AQUA:
		m_cboSkins.SetCurSel(9);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		m_cboSkins.SetCurSel(10);
		break;
	}

	m_nOldSel = m_cboSkins.GetCurSel();

	UpdateData(FALSE);
	return TRUE;
}

BOOL CGeneralPropPage::OnApply()
{
	UpdateData();

	if (IsButtonEnabled(ID_APPLY_NOW))
	{
		theApp.SetLoadLastFile(m_bLoadLastFile);

		int nCurSel = m_cboSkins.GetCurSel();
		// Do not bother sending message if nothing changed.
		if (nCurSel != m_nOldSel)
		{
			// Continous command range
			UINT nMenuID = ID_VIEW_APPLOOK_WIN_2000 + nCurSel;
			AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(nMenuID, 0));
			GetParent()->RedrawWindow(); // Redraw banner in the Property Sheet

			m_nOldSel = nCurSel;
		}
	}

	return CMFCPropertyPage::OnApply();
}

void CGeneralPropPage::OnClickedCheckLoadLastFile()
{
	SetModified();
}

void CGeneralPropPage::OnSelchangeComboAppskins()
{
	SetModified();
}
