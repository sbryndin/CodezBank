#pragma once
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
//					Added logic to load last file on the startup
//
////////////////////////////////////////////////////////////////////////////////

// CGeneralPropPage dialog

#include "DlgBanner.h"

namespace Dialogs
{

	class CGeneralPropPage : public CMFCPropertyPage
	{
		DECLARE_DYNAMIC(CGeneralPropPage)

	public:
		CGeneralPropPage();   // standard constructor
		virtual ~CGeneralPropPage();

		virtual BOOL OnInitDialog();
		virtual BOOL OnApply();

		// Dialog Data
		enum { IDD = IDD_PROPPAGE_GENERAL };
		CDlgBanner m_wndBanner;
		BOOL m_bLoadLastFile;

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		DECLARE_MESSAGE_MAP()
			afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnClickedCheckLoadLastFile();

	};

}