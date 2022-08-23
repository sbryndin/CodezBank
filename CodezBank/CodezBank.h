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
#define KEY_KEYWORDCOLOR      "KeywordColor"
#define KEY_COMMENTCOLOR      "CommentColor"
#define KEY_STRINGCOLOR       "StringColor"
#define KEY_FONTNAME          "FontName"
#define KEY_FONTHEIGHT        "FontHeight"

// Default syntax colors
#define clr_Blue     RGB(0, 0, 0xFF)
#define clr_DkGreen  RGB(0, 0x80, 0)
#define clr_Brown    RGB(0xA5, 0, 0)


#define EMPTY_PTR_LIST(list) \
   while(list.GetCount() > 0) \
      delete list.RemoveHead(); 

enum EAppMessages
{
   WM_UPDATEEDITORCOLORS = WM_USER + 1,
   WM_UPDATEEDITORFONT,
};

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

   void SetLoadLastFile(BOOL val);
   BOOL GetLoadLastFile() const { return m_bLoadLastFile; }
   void SetKeywordColor(COLORREF val);
   COLORREF GetKeywordColor() const { return m_clrKeyword; }
   void SetCommentColor(COLORREF val);
   COLORREF GetCommentColor() const { return m_clrComment; }
   void SetStringColor(COLORREF val);
   COLORREF GetStringColor() const { return m_clrString; }
   void SetFontName(LPCTSTR val);
   CString GetFontName() const { return m_strFont; }
   void SetFontHeight(int val);
   int GetFontHeight() const { return m_nFontHeight; }

   UINT  m_nAppLook;
   BOOL  m_bHiColorIcons;
   
protected:
   BOOL  m_bLoadLastFile;
   COLORREF m_clrKeyword;
   COLORREF m_clrComment;
   COLORREF m_clrString;
   CString  m_strFont;
   int      m_nFontHeight;
	
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
