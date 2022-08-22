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
//  $Workfile: $   created: 04/05/2008 11:25
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
#include "CodeNode.h"
#include "undo.h"

enum EViewHints
{
   hintNone,
   hintNewFolder,
   hintNewRootFolder,
   hintRefreshListView,
   hintRefreshCodeView,
   hintSelTreeItem,
   hintSelFoundTreeItem,
   hintSelFoundCodeItem,
   hintUpOneLevel,
   hintNewSnippet,
   hintRepaintTree,
   hintRemoveItemFromTree,
   hintClearDocument,
   hintReloadTree,
   hintSaveTreeState,
};

using namespace Data;
using namespace Utils;

namespace Document{

class CCodezBankDoc : public COleDocument//, public CUndo
{
   DECLARE_DYNCREATE(CCodezBankDoc)
public:
   CCodeNodeList m_lstNodes;

	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual ~CCodezBankDoc();
   virtual void DeleteContents();
   //virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;
   virtual void SetModifiedFlag(BOOL bModified = TRUE);
   virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
   virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

   void DeleteNode(CCodeNode* ptr, bool bDeletePtr = true);
   void FindString(CCodeNode* pFrom, LPCTSTR lpString, BOOL bDown, BOOL bCaseSensitve, BOOL bWholeWord);
   void SetCheckpoint(void);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   bool m_bBeginSearch;
   bool m_bFoundMatch;

   CCodezBankDoc();
   void DeleteNodeRecursive(CCodeNodeList& list, CCodeNode* ptr, bool bDeletePtr);
   void SearchNodeRecursive(CCodeNodeList& list, CCodeNode* ptr, LPCTSTR lpString, BOOL bDown, BOOL bCaseSensitve, BOOL bWholeWord);
   bool Match(CCodeNode* ptr, LPCTSTR lpString, BOOL bCaseSensitve, BOOL bWholeWord);
   INT_PTR CountSnippets(void);
   void CountSnippetsRecursive(CCodeNodeList& list, INT_PTR& nCount);

   DECLARE_MESSAGE_MAP()
   afx_msg void OnUpdateMod(CCmdUI* pCmdUI);
   afx_msg void OnCodeNewfolder();
   afx_msg void OnViewUponelevel();
   afx_msg void OnCodeNewtopmostfolder();
   afx_msg void OnCodeNewcodesnippet();
   afx_msg void OnUpdateSnipCount(CCmdUI* pCmdUI);
   afx_msg void OnEditUndo();
   afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
   afx_msg void OnEditRedo();
   afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
};

};
