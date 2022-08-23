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

#pragma once

namespace Dialogs
{
	// CTextEditPropPage dialog
	class CTextEditPropPage : public CMFCPropertyPage
	{
		DECLARE_DYNAMIC(CTextEditPropPage)

	public:
		CTextEditPropPage();   // standard constructor
		virtual ~CTextEditPropPage();

		virtual BOOL OnInitDialog();
		virtual BOOL OnApply();

		// Dialog Data
		enum { IDD = IDD_PROPPAGE_EDITOR };
		CMFCColorButton	m_Keyword;
		CMFCColorButton	m_Comments;
		CMFCColorButton	m_Strings;
		CMFCFontComboBox	m_Font;
		CComboBox			m_cboSize;

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		DECLARE_MESSAGE_MAP()
			afx_msg void OnClickedMfccolorbuttonKeywords();
		afx_msg void OnClickedMfccolorbuttonComments();
		afx_msg void OnClickedMfccolorbuttonStrings();
		afx_msg void OnSelchangeMfcfontcomboFont();
		afx_msg void OnSelchangeComboFontSize();
	};
}