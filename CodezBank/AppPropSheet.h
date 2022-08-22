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

#pragma once

namespace Dialogs
{

	// CAppPropSheet
	class CAppPropSheet : public CMFCPropertySheet
	{
		DECLARE_DYNAMIC(CAppPropSheet)

	public:
		CAppPropSheet(CWnd* pWndParent, UINT nSelectedPage = 0);
		virtual ~CAppPropSheet();

		virtual BOOL OnInitDialog();

	protected:
		CMFCToolBarImages		m_Icons;
		CMFCControlRenderer	m_Pat[4];

		virtual void OnDrawPageHeader(CDC* pDC, int nPage, CRect rectHeader);

		DECLARE_MESSAGE_MAP()
	};

}
