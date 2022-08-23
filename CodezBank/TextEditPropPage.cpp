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
// 08/22/2022	Created
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CodezBank.h"
#include "TextEditPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Dialogs;

// CTextEditPropPage dialog
IMPLEMENT_DYNAMIC(CTextEditPropPage, CMFCPropertyPage)

CTextEditPropPage::CTextEditPropPage()
	: CMFCPropertyPage(CTextEditPropPage::IDD)
{

}

CTextEditPropPage::~CTextEditPropPage()
{
}

void CTextEditPropPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_KEYWORDS, m_Keyword);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_COMMENTS, m_Comments);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_STRINGS, m_Strings);
	DDX_Control(pDX, IDC_MFCFONTCOMBO_FONT, m_Font);
	DDX_Control(pDX, IDC_COMBO_FONT_SIZE, m_cboSize);
}


BEGIN_MESSAGE_MAP(CTextEditPropPage, CMFCPropertyPage)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON_KEYWORDS, &CTextEditPropPage::OnClickedMfccolorbuttonKeywords)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON_COMMENTS, &CTextEditPropPage::OnClickedMfccolorbuttonComments)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON_STRINGS, &CTextEditPropPage::OnClickedMfccolorbuttonStrings)
	ON_CBN_SELCHANGE(IDC_MFCFONTCOMBO_FONT, &CTextEditPropPage::OnSelchangeMfcfontcomboFont)
	ON_CBN_SELCHANGE(IDC_COMBO_FONT_SIZE, &CTextEditPropPage::OnSelchangeComboFontSize)
END_MESSAGE_MAP()


// CTextEditPropPage message handlers
BOOL CTextEditPropPage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	m_Keyword.SetColor(theApp.GetKeywordColor());
	m_Comments.SetColor(theApp.GetCommentColor());
	m_Strings.SetColor(theApp.GetStringColor());
	m_Font.SelectFont(theApp.GetFontName());

	CString strFontHeigt;
	strFontHeigt.Format(_T("%d"), theApp.GetFontHeight());

	for (int i = 6; i <= 24; i++)
	{
		CString str;
		str.Format(_T("%d"), i);
		m_cboSize.AddString(str);
	}

	m_cboSize.SelectString(0, strFontHeigt);

	UpdateData(FALSE);
	return TRUE;
}

void CTextEditPropPage::OnClickedMfccolorbuttonKeywords()
{
	SetModified();
}

void CTextEditPropPage::OnClickedMfccolorbuttonComments()
{
	SetModified();
}

void CTextEditPropPage::OnClickedMfccolorbuttonStrings()
{
	SetModified();
}

void CTextEditPropPage::OnSelchangeMfcfontcomboFont()
{
	SetModified();
}

void CTextEditPropPage::OnSelchangeComboFontSize()
{
	SetModified();
}

BOOL CTextEditPropPage::OnApply()
{
	UpdateData();

	if (IsButtonEnabled(ID_APPLY_NOW))
	{
		theApp.SetKeywordColor(m_Keyword.GetColor());
		theApp.SetCommentColor(m_Comments.GetColor());
		theApp.SetStringColor(m_Strings.GetColor());

		CMFCFontInfo* pFontInfo = m_Font.GetSelFont();
		theApp.SetFontName(pFontInfo->m_strName);

		int nSel = m_cboSize.GetCurSel();
		nSel += 6;
		theApp.SetFontHeight(nSel);
	}

	return CMFCPropertyPage::OnApply();
}
