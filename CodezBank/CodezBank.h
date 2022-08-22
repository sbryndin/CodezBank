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
//  $Workfile: $   created: 04/05/2008 11:34
////////////////////////////////////////////////////////////////////////////////
//  $Revision: $  $Modtime: $
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Author: steveb
//
// History:
// 
// 08/12/22 Changed base app class from CWinApp to CWinAppEx to take advantage
//          of new MFC features.
// 08/22/2022	Added logic to load last file on the startup
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

// Registry names
#define SECTION_SETTINGS      "Settings"
#define KEY_MAIN_WND          "MainWnd"
#define KEY_CTRLBARS          "CtrlBars"
#define KEY_TREEVIEW          "TreeView"
#define KEY_LISTSTYLE         "ListStyle"
#define KEY_CODEVIEW          "CodeViewHeight"
#define KEY_LOADLASTFILE      "LoadLastFile"

#define EMPTY_PTR_LIST(list) \
   while(list.GetCount() > 0) \
      delete list.RemoveHead(); 

// CCodezBankApp:
// See CodezBank.cpp for the implementation of this class
//

class CCodezBankApp : public CWinAppEx
{
public:
	CCodezBankApp();

	virtual BOOL InitInstance();
   virtual int ExitInstance();
   virtual void PreLoadState();
   virtual void LoadCustomState();
   virtual void SaveCustomState();

   void UpdateLoadLastFile(BOOL val);
   BOOL GetLoadLastFile() const { return m_bLoadLastFile; }

   UINT  m_nAppLook;
   BOOL  m_bHiColorIcons;
   
protected:
   BOOL  m_bLoadLastFile;
	
	DECLARE_MESSAGE_MAP()
   afx_msg void OnAppAbout();
};

extern CCodezBankApp theApp;

class CMainFrame;

///////////////////////////////////////////////
/// gets pointer to mainframe without
/// need to cast it every time
///////////////////////////////////////////////
inline CMainFrame* Frame(void)
{
   return reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
}

///////////////////////////////////////////////
/// Message box with variable argument list
/// 
///////////////////////////////////////////////
inline int Ask(UINT nMBStyle, LPCTSTR szTemplate, ...)
{
   CString strBuffer;

   // Combine arguments using the template
   if (szTemplate)
   {
      va_list arg_ptr;
      va_start(arg_ptr, szTemplate);
      strBuffer.FormatV(szTemplate, arg_ptr);
      va_end(arg_ptr);
   }
   return AfxMessageBox(strBuffer, nMBStyle);
}
