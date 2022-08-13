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
// Main data tree
// 
// History:
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

enum ENodeType
{
   nodeFolder,
   nodeCode,
};

namespace Data{

class CCodeNode;
typedef CTypedPtrList<CObList, CCodeNode*> CCodeNodeList;

class CCodeNode : public CObject
{
   DECLARE_SERIAL(CCodeNode)

public:
   CString        m_strName;
   bool           m_bExpanded;
   UINT           m_nFolderType;
   CString        m_strCode;
   COleDateTime   m_tCreated;
   COleDateTime   m_tModified;
   CCodeNodeList  m_lstChildren;

   CString        m_strFound;
   BOOL           m_bMatchCase;
   BOOL           m_bWholeWord;

   CString        m_strNote; // new member in Schema #2

	CCodeNode();
	virtual ~CCodeNode();
   virtual void Serialize(CArchive& ar);
   CString GetModified(void) const;
   CString GetCreated(void) const;
   void Modify(void);
};

};
