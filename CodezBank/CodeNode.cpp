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
//  $Workfile: $   created: 04/05/2008 11:37
////////////////////////////////////////////////////////////////////////////////
//  $Revision: $  $Modtime: $
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Author: steveb
//
// History:
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CodezBank.h"
#include "CodeNode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Data;

IMPLEMENT_SERIAL(CCodeNode, CObject, VERSIONABLE_SCHEMA | 2)

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
CCodeNode::CCodeNode():
  m_bExpanded(false)
, m_nFolderType(nodeFolder)
, m_tCreated(COleDateTime::GetCurrentTime())
, m_tModified(COleDateTime::GetCurrentTime())
, m_bMatchCase(FALSE)
, m_bWholeWord(FALSE)
{
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
CCodeNode::~CCodeNode()
{
   EMPTY_PTR_LIST(m_lstChildren);
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
void Data::CCodeNode::Serialize(CArchive& ar)
{
   if (ar.IsStoring())
   {	// storing code

      // New members
      ar << m_strNote;

      ar << m_strName;
      ar << m_bExpanded;
      ar << m_nFolderType;
      ar << m_tCreated;
      ar << m_tModified;
      ar << m_strCode;
   }
   else
   {	// loading code
      UINT nSchema = ar.GetObjectSchema();

      switch(nSchema)
      {
      case 2:
         ar >> m_strNote;
         // let fall through (binary compatibility with older files)
      case 1:
         ar >> m_strName;
         ar >> m_bExpanded;
         ar >> m_nFolderType;
         ar >> m_tCreated;
         ar >> m_tModified;
         ar >> m_strCode;
      break;
      }
   }

   m_lstChildren.Serialize(ar);
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
CString CCodeNode::GetModified(void) const
{
   return m_tModified.Format("%m/%d/%y %I:%M %p");
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
CString CCodeNode::GetCreated(void) const
{
   return m_tCreated.Format("%m/%d/%y %I:%M %p");
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
void CCodeNode::Modify(void)
{
   m_tModified = COleDateTime::GetCurrentTime();
}
