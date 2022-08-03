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
//  $Workfile: $   created: 04/05/2008 11:12
////////////////////////////////////////////////////////////////////////////////
//  $Revision: $  $Modtime: $
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Author: steveb
//
// History:
//
////////////////////////////////////////////////////////////////////////////////
#pragma once 

namespace Utils{

class CWindowPosition
{
public:

   CWindowPosition(LPCTSTR lpSection, LPCTSTR lpKey):
      m_strSection(lpSection)
         , m_strKey(lpKey)
      {
      }

      ~CWindowPosition(void)
      {
      }

      void Load(CWnd* pWnd)
      {
         ASSERT(pWnd);

         LPBYTE pbtData = 0;
         UINT nSize = 0;
         WINDOWPLACEMENT wp;
         if(!AfxGetApp()->GetProfileBinary(m_strSection, m_strKey, &pbtData, &nSize))
            return;

         memcpy(&wp, pbtData, sizeof(WINDOWPLACEMENT));

         if(wp.flags & WPF_RESTORETOMAXIMIZED)
            wp.showCmd = SW_SHOWMAXIMIZED;
         else
            wp.showCmd = AfxGetApp ()->m_nCmdShow;

         pWnd->SetWindowPlacement(&wp);

         delete [] pbtData;
      }

      void Save(CWnd* pWnd)
      {
         ASSERT(pWnd);

         WINDOWPLACEMENT wp;
         if(!pWnd->GetWindowPlacement(&wp))
            return;

         AfxGetApp()->WriteProfileBinary(m_strSection, m_strKey, 
            reinterpret_cast<LPBYTE>((LPWINDOWPLACEMENT)&wp), 
            sizeof(WINDOWPLACEMENT));
      }

protected:
   CString m_strSection;
   CString m_strKey;
};
};
