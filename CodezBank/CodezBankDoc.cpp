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
#include "stdafx.h"
#include "CodezBank.h"
#include "CodezBankDoc.h"
#include "MainFrm.h"
#include "FileProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Document;

IMPLEMENT_DYNCREATE(CCodezBankDoc, COleDocument)

BEGIN_MESSAGE_MAP(CCodezBankDoc, COleDocument)
   ON_UPDATE_COMMAND_UI(ID_TITLE_MOD, OnUpdateMod)
   ON_COMMAND(ID_CODE_NEWFOLDER, &CCodezBankDoc::OnCodeNewfolder)
   ON_COMMAND(ID_VIEW_UPONELEVEL, &CCodezBankDoc::OnViewUponelevel)
   ON_COMMAND(ID_CODE_NEWTOPMOSTFOLDER, &CCodezBankDoc::OnCodeNewtopmostfolder)
   ON_COMMAND(ID_CODE_NEWCODESNIPPET, &CCodezBankDoc::OnCodeNewcodesnippet)
   ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
   ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
   ON_UPDATE_COMMAND_UI(ID_INDICATOR_SNIP_COUNT, OnUpdateSnipCount)
   ON_COMMAND(ID_EDIT_UNDO, &CCodezBankDoc::OnEditUndo)
   ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CCodezBankDoc::OnUpdateEditUndo)
   ON_COMMAND(ID_EDIT_REDO, &CCodezBankDoc::OnEditRedo)
   ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CCodezBankDoc::OnUpdateEditRedo)
END_MESSAGE_MAP()

CCodezBankDoc::CCodezBankDoc()://:CUndo(100)
  m_bBeginSearch(false)
, m_bFoundMatch(false)
{
   EnableCompoundFile(TRUE);
}

CCodezBankDoc::~CCodezBankDoc()
{
}

//CRichEditCntrItem* CCodezBankDoc::CreateClientItem(REOBJECT* preo) const
//{
//	return new CRichEditCntrItem(preo, const_cast<CCodezBankDoc*>(this));
//}

BOOL CCodezBankDoc::OnNewDocument()
{
	if (!COleDocument::OnNewDocument())
		return FALSE;

	SetCheckpoint();

	return TRUE;
}

void CCodezBankDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}

   m_lstNodes.Serialize(ar);
}

#ifdef _DEBUG
void CCodezBankDoc::AssertValid() const
{
	COleDocument::AssertValid();
}

void CCodezBankDoc::Dump(CDumpContext& dc) const
{
	COleDocument::Dump(dc);
}
#endif //_DEBUG

///////////////////////////////////////////////
/// Append '*' to the title if dec was
/// modified
///////////////////////////////////////////////
void CCodezBankDoc::OnUpdateMod(CCmdUI* pCmdUI)
{
   pCmdUI->SetText(CDocument::IsModified() ? "*" : "");
   pCmdUI->m_bContinueRouting = TRUE;
}

void Document::CCodezBankDoc::DeleteContents()
{
   UpdateAllViews(NULL, hintClearDocument);
   EMPTY_PTR_LIST(m_lstNodes);
   COleDocument::DeleteContents();
}

void Document::CCodezBankDoc::OnCodeNewfolder()
{
   CCodeNode* pNode = new CCodeNode;
   SetModifiedFlag();
   UpdateAllViews(NULL, hintNewFolder, pNode);
}

void Document::CCodezBankDoc::OnViewUponelevel()
{
   UpdateAllViews(NULL, hintUpOneLevel);
}

void Document::CCodezBankDoc::OnCodeNewtopmostfolder()
{
   CCodeNode* pNode = new CCodeNode;
   SetModifiedFlag();
   UpdateAllViews(NULL, hintNewRootFolder, pNode);
}

void Document::CCodezBankDoc::OnCodeNewcodesnippet()
{
   CCodeNode* pNode = new CCodeNode;
   pNode->m_nFolderType = nodeCode,
   SetModifiedFlag();
   UpdateAllViews(NULL, hintNewSnippet, pNode);
}

void Document::CCodezBankDoc::DeleteNode(CCodeNode* ptr, bool bDeletePtr)
{
   SetCheckpoint();
   DeleteNodeRecursive(m_lstNodes, ptr, bDeletePtr);
}

void Document::CCodezBankDoc::DeleteNodeRecursive(CCodeNodeList& list, 
                                                  CCodeNode* ptr, bool bDeletePtr)
{
   POSITION pos2 = NULL;
   for(POSITION pos = list.GetHeadPosition(); (pos2 = pos) != NULL; )
   {  
      CCodeNode* pNode = list.GetNext(pos);

      if(pNode == ptr)
      {
         list.RemoveAt(pos2);

         if(bDeletePtr)
            delete pNode;

         SetModifiedFlag();
         return;
      }

      if(pNode->m_lstChildren.GetCount() > 0)
         DeleteNodeRecursive(pNode->m_lstChildren, ptr, bDeletePtr);
   }   
}

void CCodezBankDoc::FindString(CCodeNode* pFrom, LPCTSTR lpString, BOOL bDown, BOOL bCaseSensitve, BOOL bWholeWord)
{
   m_bFoundMatch = false;
   if(pFrom == NULL)
      m_bBeginSearch = true;
   else
      m_bBeginSearch = false;

   SearchNodeRecursive(m_lstNodes, pFrom, lpString, bDown, bCaseSensitve, bWholeWord);
}

void CCodezBankDoc::SearchNodeRecursive(CCodeNodeList& list, 
                                        CCodeNode* ptr, 
                                        LPCTSTR lpString, 
                                        BOOL bDown, 
                                        BOOL bCaseSensitve, 
                                        BOOL bWholeWord)
{
   if(m_bFoundMatch)
      return;

   if(bDown)
   {
      for(POSITION pos = list.GetHeadPosition(); pos != NULL; )
      {
         CCodeNode* pNode = list.GetNext(pos);

         if(Match(pNode, lpString, bCaseSensitve, bWholeWord))
         {
            m_bFoundMatch = true;
            return;
         }
   
         if(pNode->m_lstChildren.GetCount() > 0)
            SearchNodeRecursive(pNode->m_lstChildren, ptr, lpString, bDown, bCaseSensitve, bWholeWord);
      }
   }
   else
   {
      for(POSITION pos = list.GetTailPosition(); pos != NULL; )
      {
         CCodeNode* pNode = list.GetPrev(pos);

         if(Match(pNode, lpString, bCaseSensitve, bWholeWord))
         {
            m_bFoundMatch = true;
            return;
         }

         if(pNode->m_lstChildren.GetCount() > 0)
            SearchNodeRecursive(pNode->m_lstChildren, ptr, lpString, bDown, bCaseSensitve, bWholeWord);
      }
   }
}

bool CCodezBankDoc::Match(CCodeNode* ptr, LPCTSTR lpString, 
                          BOOL bCaseSensitve, BOOL bWholeWord)
{
   if(ptr->m_nFolderType == nodeFolder)
   {
      if(bCaseSensitve == FALSE && bWholeWord == TRUE)
      {
         if(ptr->m_strName.CompareNoCase(lpString) == 0)
         {
            UpdateAllViews(NULL, hintSelFoundTreeItem, ptr);
            return true;
         }
      }
      else if(bCaseSensitve == TRUE && bWholeWord == TRUE)
      {
         if(ptr->m_strName.Compare(lpString) == 0)
         {
            UpdateAllViews(NULL, hintSelFoundTreeItem, ptr);
            return true;
         }
      }
      else if(bCaseSensitve == FALSE && bWholeWord == FALSE)
      {
         CString strCompareTo = lpString;
         CString strThis = ptr->m_strName;
         strCompareTo.MakeLower();
         strThis.MakeLower();

         if(strThis.Find(strCompareTo, 0) > -1)
         {
            UpdateAllViews(NULL, hintSelFoundTreeItem, ptr);
            return true;
         }
      }
      else if(bCaseSensitve == TRUE && bWholeWord == FALSE)
      {
         CString strCompareTo = lpString;
         CString strThis = ptr->m_strName;
   
         if(strThis.Find(strCompareTo, 0) > -1)
         {
            UpdateAllViews(NULL, hintSelFoundTreeItem, ptr);
            return true;
         }
      }
   }
   else
   {
      if(bCaseSensitve == FALSE && bWholeWord == TRUE)
      {
         if(ptr->m_strName.CompareNoCase(lpString) == 0)
         {
            UpdateAllViews(NULL, hintSelFoundTreeItem, ptr);
            return true;
         }
      }
      else if(bCaseSensitve == TRUE && bWholeWord == TRUE)
      {
         if(ptr->m_strName.Compare(lpString) == 0)
         {
            UpdateAllViews(NULL, hintSelFoundTreeItem, ptr);
            return true;
         }
      }
      else if(bCaseSensitve == FALSE && bWholeWord == FALSE)
      {
         CString strCompareTo = lpString;
         CString strThis = ptr->m_strName;
         strCompareTo.MakeLower();
         strThis.MakeLower();

         if(strThis.Find(strCompareTo, 0) > -1)
         {
            UpdateAllViews(NULL, hintSelFoundTreeItem, ptr);
            return true;
         }
      }
      else if(bCaseSensitve == TRUE && bWholeWord == FALSE)
      {
         CString strCompareTo = lpString;
         CString strThis = ptr->m_strName;
   
         if(strThis.Find(strCompareTo, 0) > -1)
         {
            UpdateAllViews(NULL, hintSelFoundTreeItem, ptr);
            return true;
         }
      }
      if(bCaseSensitve == FALSE)
      {
         CString strThis = ptr->m_strCode;
         CString strFind = lpString;

         strThis.MakeLower();
         strFind.MakeLower();

         if(strThis.Find(strFind, 0) > -1)
         {
            ptr->m_strFound = lpString;
            ptr->m_bMatchCase = bCaseSensitve;
            ptr->m_bWholeWord = bWholeWord;
            UpdateAllViews(NULL, hintSelFoundTreeItem, ptr);
            UpdateAllViews(NULL, hintSelFoundCodeItem, ptr);
            return true;
         }
      }
      else if(bCaseSensitve == TRUE)
      {
         CString strThis = ptr->m_strCode;
         CString strFind = lpString;

         if(strThis.Find(strFind, 0) > -1)
         {
            ptr->m_strFound = lpString;
            ptr->m_bMatchCase = bCaseSensitve;
            ptr->m_bWholeWord = bWholeWord;
            UpdateAllViews(NULL, hintSelFoundTreeItem, ptr);
            UpdateAllViews(NULL, hintSelFoundCodeItem, ptr);
            return true;
         }
      }
   }

   return false;
}

///////////////////////////////////////////////
/// do not allow call to CDocument
///  because we dont care
///////////////////////////////////////////////
void CCodezBankDoc::SetModifiedFlag(BOOL bModified /*= TRUE*/)
{
   CDocument::SetModifiedFlag(bModified);
}

INT_PTR CCodezBankDoc::CountSnippets(void)
{
   INT_PTR nCount = 0;
   CountSnippetsRecursive(m_lstNodes, nCount);
   return nCount;
}

void CCodezBankDoc::CountSnippetsRecursive(CCodeNodeList& list, INT_PTR& nCount)
{
   for(POSITION pos = list.GetHeadPosition(); pos != NULL; )
   {  
      CCodeNode* pNode = list.GetNext(pos);

      if(pNode->m_nFolderType == nodeCode)
         nCount++;

      if(pNode->m_lstChildren.GetCount() > 0)
         CountSnippetsRecursive(pNode->m_lstChildren, nCount);
   }
}

void CCodezBankDoc::OnUpdateSnipCount(CCmdUI* pCmdUI)
{
   INT_PTR nCount = CountSnippets();
   Frame()->UpdateIndSnipCount(pCmdUI, nCount);
}

void CCodezBankDoc::SetCheckpoint(void)
{
//   CheckPoint();
}

BOOL Document::CCodezBankDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
   if(!COleDocument::OnOpenDocument(lpszPathName))
      return FALSE;

   //SetCheckpoint();

   return TRUE;
}

void Document::CCodezBankDoc::OnEditUndo()
{
//   Undo();
//   UpdateAllViews(NULL, hintReloadTree);
}

void Document::CCodezBankDoc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(FALSE);
}

void Document::CCodezBankDoc::OnEditRedo()
{
   //Redo();
   //UpdateAllViews(NULL, hintReloadTree);
}

void Document::CCodezBankDoc::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(FALSE);
}

BOOL Document::CCodezBankDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
   //CFileProps propFile(lpszPathName, false);

   return COleDocument::OnSaveDocument(lpszPathName);
}
