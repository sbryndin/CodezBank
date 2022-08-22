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
//  $Workfile: $   created: 04/05/2008 11:52
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
#include "LeftView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Views;

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
   ON_WM_SIZE()
   ON_WM_CREATE()
   ON_NOTIFY_REFLECT(TVN_GETDISPINFO, &CLeftView::OnTvnGetdispinfo)
   ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CLeftView::OnTvnEndlabeledit)
   ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, &CLeftView::OnTvnItemexpanded)
   ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CLeftView::OnTvnSelchanged)
   ON_COMMAND(ID_EDIT_DELETE, &CLeftView::OnEditDelete)
   ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, &CLeftView::OnUpdateEditDelete)
   ON_WM_CONTEXTMENU()
   ON_WM_RBUTTONDOWN()
   ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CLeftView::OnTvnBegindrag)
   ON_WM_MOUSEMOVE()
   ON_WM_LBUTTONUP()
   ON_COMMAND(ID_EDIT_RENAME, &CLeftView::OnEditRename)
   ON_UPDATE_COMMAND_UI(ID_EDIT_RENAME, &CLeftView::OnUpdateEditRename)
   ON_UPDATE_COMMAND_UI(ID_VIEW_UPONELEVEL, &CLeftView::OnUpdateViewUponelevel)
   ON_NOTIFY_REFLECT(NM_DBLCLK, &CLeftView::OnNMDblclk)
END_MESSAGE_MAP()

CLeftView::CLeftView():
  m_bLDragging(FALSE)
, m_hDrag(NULL)
, m_hDrop(NULL)
, m_pDragImage(NULL)
{
   m_nMask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_SHOWSELALWAYS|TVS_EDITLABELS;
	return CTreeView::PreCreateWindow(cs);
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

   for(POSITION pos = GetDocument()->m_lstNodes.GetHeadPosition(); pos != NULL; )
      Populate(NULL, GetDocument()->m_lstNodes.GetNext(pos));
}

///////////////////////////////////////////////
/// recursive populate function
/// 
///////////////////////////////////////////////
void CLeftView::Populate(HTREEITEM hParent, CCodeNode* pNode)
{
   int nImage = 0;
   if(pNode->m_nFolderType == nodeCode)
      nImage = 1;

   HTREEITEM hItem = GetTreeCtrl().InsertItem(m_nMask, LPSTR_TEXTCALLBACK, nImage, nImage, 0, 0, (LPARAM)pNode,  hParent, TVI_LAST);
   if(pNode->m_bExpanded)
      GetTreeCtrl().Expand(hParent, TVE_EXPAND);

   for(POSITION pos = pNode->m_lstChildren.GetHeadPosition(); pos != NULL; )
      Populate(hItem, pNode->m_lstChildren.GetNext(pos));
}

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CCodezBankDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCodezBankDoc)));
	return (CCodezBankDoc*)m_pDocument;
}
#endif //_DEBUG

void Views::CLeftView::OnSize(UINT nType, int cx, int cy)
{
   CTreeView::OnSize(nType, cx, cy);

   AfxGetApp()->WriteProfileInt(SECTION_SETTINGS, KEY_TREEVIEW, cx);
}

int Views::CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CTreeView::OnCreate(lpCreateStruct) == -1)
      return -1;

   m_Image.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1);

   m_Image.Add(AfxGetApp()->LoadIcon(IDI_FOLDER));
   m_Image.Add(AfxGetApp()->LoadIcon(IDI_CODE2));

   GetTreeCtrl().SetImageList(&m_Image, TVSIL_NORMAL);
   return 0;
}

void Views::CLeftView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
   CCodeNode* pNodeIn = reinterpret_cast<CCodeNode*>(pHint);

   switch(lHint)
   {
   case hintNewFolder:
   case hintNewSnippet:
      AddNewFolder(pNodeIn);
      break;
   case hintSelTreeItem:
      {
         HTREEITEM hSel = GetTreeCtrl().GetSelectedItem();
         if(hSel == NULL)
         {
            hSel = GetTreeCtrl().GetRootItem();
            while(hSel)
            {
               DoSelect(hSel, pNodeIn);
               hSel = GetTreeCtrl().GetNextSiblingItem(hSel);
            }
         }
         else
         {
            DoSelect(hSel, pNodeIn);
         }
      }
      break;
   case hintSelFoundTreeItem:
      {
         HTREEITEM hSel = GetTreeCtrl().GetRootItem();
         while(hSel)
         {
            DoSelect(hSel, pNodeIn);
            hSel = GetTreeCtrl().GetNextSiblingItem(hSel);
         }
      }
      break;
   case hintUpOneLevel:
      MoveUp();
      break;
   case hintNewRootFolder:
      AddNewTopFolder(pNodeIn);
      break;
   case hintRepaintTree:
      Invalidate();
      break;
   case hintRemoveItemFromTree:
      {
         HTREEITEM hSel = GetTreeCtrl().GetSelectedItem();
         if(hSel == NULL)
         {
            hSel = GetTreeCtrl().GetRootItem();
            while(hSel)
            {
               DoRemove(hSel, pNodeIn);
               hSel = GetTreeCtrl().GetNextSiblingItem(hSel);
            }
         }
         else
         {
            DoRemove(hSel, pNodeIn);
         }
      }
      break;
   case hintClearDocument:
      GetTreeCtrl().DeleteAllItems();
      break;
   case hintReloadTree:
      GetTreeCtrl().DeleteAllItems();
      for(POSITION pos = GetDocument()->m_lstNodes.GetHeadPosition(); pos != NULL; )
         Populate(NULL, GetDocument()->m_lstNodes.GetNext(pos));
      break;
   case hintSaveTreeState:
      SaveTreeState(nullptr);
      break;
   }
}

void CLeftView::AddNewFolder(CCodeNode* pNodeIn)
{
   HTREEITEM hSel = GetTreeCtrl().GetSelectedItem();
   if(hSel)
   {
      DWORD_PTR dwData = GetTreeCtrl().GetItemData(hSel);
      CCodeNode* pNode = reinterpret_cast<CCodeNode*>(dwData);
      pNode->m_lstChildren.AddTail(pNodeIn);
      GetDocument()->UpdateAllViews(this, hintRefreshListView, pNode);
   }
   else
   {
      GetDocument()->m_lstNodes.AddTail(pNodeIn);
      GetDocument()->UpdateAllViews(this, hintRefreshListView, NULL);
   }

   int nImage = 0;
   if(pNodeIn->m_nFolderType == nodeCode)
      nImage = 1;

   HTREEITEM hItem = GetTreeCtrl().InsertItem(m_nMask, LPSTR_TEXTCALLBACK, nImage, nImage, 0, 0, (LPARAM)pNodeIn,  hSel, TVI_LAST);
   GetTreeCtrl().Expand(hSel, TVE_EXPAND);
   GetTreeCtrl().Invalidate();
}

void CLeftView::AddNewTopFolder(CCodeNode* pNodeIn)
{
   int nImage = 0;
   if(pNodeIn->m_nFolderType == nodeCode)
      nImage = 1;

   GetDocument()->m_lstNodes.AddTail(pNodeIn);
   HTREEITEM hItem = GetTreeCtrl().InsertItem(m_nMask, LPSTR_TEXTCALLBACK, nImage, nImage, 0, 0, (LPARAM)pNodeIn,  NULL, TVI_ROOT);
   GetTreeCtrl().Invalidate();
}

void Views::CLeftView::OnTvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
   CCodeNode* pNode = reinterpret_cast<CCodeNode*>(pTVDispInfo->item.lParam);
   strcpy_s(pTVDispInfo->item.pszText, 260, pNode->m_strName);
   *pResult = 0;
}

void Views::CLeftView::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
   CCodeNode* pNode = reinterpret_cast<CCodeNode*>(pTVDispInfo->item.lParam);

   if(pTVDispInfo->item.pszText)
   {
      pNode->m_strName = pTVDispInfo->item.pszText;
      pNode->Modify();
      GetDocument()->SetModifiedFlag();
   }
   *pResult = 0;
}

void Views::CLeftView::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
   CCodeNode* pNode = reinterpret_cast<CCodeNode*>(pNMTreeView->itemNew.lParam);
   if(pNMTreeView->action == 1)
      pNode->m_bExpanded = false;
   else if(pNMTreeView->action == 2)
      pNode->m_bExpanded = true;
      
   *pResult = 0;
}

void Views::CLeftView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
   *pResult = 0;
   CCodeNode* pNode = reinterpret_cast<CCodeNode*>(pNMTreeView->itemNew.lParam);
   if(!pNode)
      return;

   switch(pNode->m_nFolderType)
   {
   case nodeFolder:
      Frame()->SwitchView(NULL);
      GetDocument()->UpdateAllViews(this, hintRefreshListView, pNode);
      break;
   case nodeCode:
      Frame()->SwitchView("Code");
      GetDocument()->UpdateAllViews(this, hintRefreshCodeView, pNode);
      break;
   }
}

///////////////////////////////////////////////
/// recursive select function
/// 
///////////////////////////////////////////////
HTREEITEM CLeftView::DoSelect(HTREEITEM hStart, CCodeNode* pNode)
{
   CTreeCtrl& tree = GetTreeCtrl();

   if(hStart == NULL)
      return NULL;

   if(tree.GetItemData(hStart) == (DWORD_PTR)pNode)
   {
      tree.Select(hStart, TVGN_CARET);
      tree.EnsureVisible(hStart);
      return hStart;
   }

   hStart = tree.GetChildItem(hStart);
   do 
   {
      HTREEITEM hti;

      if((hti = DoSelect(hStart, pNode)) != NULL)
         return hti;

   } while((hStart = tree.GetNextItem(hStart, TVGN_NEXT)) != NULL);

   return NULL;
}

HTREEITEM CLeftView::DoRemove(HTREEITEM hStart, CCodeNode* pNode)
{
   CTreeCtrl& tree = GetTreeCtrl();

   if(hStart == NULL)
      return NULL;

   if(tree.GetItemData(hStart) == (DWORD_PTR)pNode)
   {
      tree.DeleteItem(hStart);
      return hStart;
   }

   hStart = tree.GetChildItem(hStart);
   do 
   {
      HTREEITEM hti;

      if((hti = DoRemove(hStart, pNode)) != NULL)
         return hti;

   } while((hStart = tree.GetNextItem(hStart, TVGN_NEXT)) != NULL);

   return NULL;
}

void CLeftView::MoveUp(void)
{
   HTREEITEM hSel = GetTreeCtrl().GetSelectedItem();
   HTREEITEM hParent = GetTreeCtrl().GetParentItem(hSel);
   if(hParent)
   {
      GetTreeCtrl().Select(hParent, TVGN_CARET);
      GetTreeCtrl().EnsureVisible(hParent);
   }
   else
   {
      GetTreeCtrl().SelectItem(NULL);
      GetDocument()->UpdateAllViews(this, hintRefreshListView);
   }
}

void CLeftView::OnEditDelete()
{
   HTREEITEM hSel = GetTreeCtrl().GetSelectedItem();
   DWORD_PTR dwData = GetTreeCtrl().GetItemData(hSel);
   CCodeNode* pNode = reinterpret_cast<CCodeNode*>(dwData);
   
   if(Ask(MB_YESNO, "Are you sure you want to delete '%s' and all subitems?", pNode->m_strName) == IDNO)
      return;

   GetDocument()->DeleteNode(pNode);
   GetTreeCtrl().DeleteItem(hSel);
}

void CLeftView::OnUpdateEditDelete(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(GetTreeCtrl().GetSelectedItem() != NULL);
}

void CLeftView::OnContextMenu(CWnd* pWnd, CPoint point)
{
   theApp.GetContextMenuManager()->ShowPopupMenu(IDR_TREE_CONTEXT, point.x, point.y, this, TRUE);
}

void Views::CLeftView::OnRButtonDown(UINT nFlags, CPoint point)
{
   CTreeView::OnRButtonDown(nFlags, point);

   ClientToScreen(&point);
   OnContextMenu(this, point);
}

void CLeftView::FindString(LPCTSTR lpString, BOOL bDown, BOOL bCaseSensitve, BOOL bWholeWord)
{
   HTREEITEM hSel = GetTreeCtrl().GetSelectedItem();
   CCodeNode* pNode = NULL;

   if(hSel)
      pNode = reinterpret_cast<CCodeNode*>(GetTreeCtrl().GetItemData(hSel));

   GetDocument()->FindString(pNode, lpString, bDown, bCaseSensitve, bWholeWord);  
}

void Views::CLeftView::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
   *pResult = 0;

   m_hDrag = pNMTreeView->itemNew.hItem;
	m_hDrop = NULL;

	m_pDragImage = GetTreeCtrl().CreateDragImage(m_hDrag);  // get the image list for dragging
	// CreateDragImage() returns NULL if no image list
	// associated with the tree view control
	if( !m_pDragImage )
		return;
   
   m_bLDragging = TRUE;
	m_pDragImage->BeginDrag(0, CPoint(0,-15));
	POINT pt = pNMTreeView->ptDrag;
	ClientToScreen( &pt );
	m_pDragImage->DragEnter(NULL, pt);
	SetCapture();

}

void Views::CLeftView::OnMouseMove(UINT nFlags, CPoint point)
{
   HTREEITEM	hitem;
	UINT		flags;

	if (m_bLDragging)
	{
		POINT pt = point;
		ClientToScreen( &pt );
		CImageList::DragMove(pt);
		if ((hitem = GetTreeCtrl().HitTest(point, &flags)) != NULL)
		{
			CImageList::DragShowNolock(FALSE);
			GetTreeCtrl().SelectDropTarget(hitem);
			m_hDrop = hitem;
			CImageList::DragShowNolock(TRUE);
		}
	}

   CTreeView::OnMouseMove(nFlags, point);
}

void Views::CLeftView::OnLButtonUp(UINT nFlags, CPoint point)
{
   CTreeView::OnLButtonUp(nFlags, point);

   if (m_bLDragging)
	{
		m_bLDragging = FALSE;
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();

		delete m_pDragImage;

		// Remove drop target highlighting
		GetTreeCtrl().SelectDropTarget(NULL);

		if( m_hDrag == m_hDrop )
			return;

		// If Drag item is an ancestor of Drop item then return
		HTREEITEM htiParent = m_hDrop;
		while( (htiParent = GetTreeCtrl().GetParentItem( htiParent )) != NULL )
		{
			if( htiParent == m_hDrag ) 
            return;
		}

		GetTreeCtrl().Expand( m_hDrop, TVE_EXPAND ) ;

		HTREEITEM htiNew = CopyBranch( m_hDrag, m_hDrop, TVI_LAST );
		GetTreeCtrl().DeleteItem(m_hDrag);
		GetTreeCtrl().SelectItem( htiNew );
	}
}

///////////////////////////////////////////////
///
/// 
///////////////////////////////////////////////
HTREEITEM CLeftView::CopyBranch( HTREEITEM htiBranch, HTREEITEM htiNewParent, 
                                                HTREEITEM htiAfter /*= TVI_LAST*/ )
{
   HTREEITEM hChild;

   HTREEITEM hNewItem = CopyItem( htiBranch, htiNewParent, htiAfter );
   hChild = GetTreeCtrl().GetChildItem(htiBranch);
   while( hChild != NULL)
   {
      // recursively transfer all the items
      CopyBranch(hChild, hNewItem);  
      hChild = GetTreeCtrl().GetNextSiblingItem( hChild );
   }
   return hNewItem;
}

HTREEITEM CLeftView::CopyItem( HTREEITEM hItem, HTREEITEM htiNewParent, 
                                        HTREEITEM htiAfter /*= TVI_LAST*/ )

{
   TV_INSERTSTRUCT         tvstruct;
   HTREEITEM                       hNewItem;
   //CString                         sText;

   // get information of the source item
   tvstruct.item.hItem = hItem;
   tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE | 
                          TVIF_IMAGE | TVIF_SELECTEDIMAGE;
   GetTreeCtrl().GetItem(&tvstruct.item);  
   
//   sText = GetTreeCtrl().GetItemText( hItem );

   //tvstruct.item.cchTextMax = sText.GetLength();
   //tvstruct.item.pszText = sText.LockBuffer();

   tvstruct.item.pszText = LPSTR_TEXTCALLBACK;
   // Insert the item at proper location
   tvstruct.hParent = htiNewParent;
   tvstruct.hInsertAfter = htiAfter;
   tvstruct.item.mask = m_nMask;
   hNewItem = GetTreeCtrl().InsertItem(&tvstruct);
//   sText.ReleaseBuffer();

   // Now copy item data and item state.
   GetTreeCtrl().SetItemData( hNewItem, GetTreeCtrl().GetItemData( hItem ));
   GetTreeCtrl().SetItemState( hNewItem, GetTreeCtrl().GetItemState( hItem, TVIS_STATEIMAGEMASK ), 
                                                  TVIS_STATEIMAGEMASK );

   // Call virtual function to allow further processing in derived class
   OnItemCopied(hItem, hNewItem);

   return hNewItem;
}

void CLeftView::OnItemCopied(HTREEITEM hItem, HTREEITEM hNewItem)
{
   DWORD_PTR dwData = GetTreeCtrl().GetItemData(hItem);
   CCodeNode* pNode = reinterpret_cast<CCodeNode*>(dwData);

   // remove this node from list collection and keep the ptr
   GetDocument()->DeleteNode(pNode, false);

   HTREEITEM hParent = GetTreeCtrl().GetParentItem(hNewItem);
   DWORD_PTR dwDataNew = GetTreeCtrl().GetItemData(hParent);
   CCodeNode* pParentNode = reinterpret_cast<CCodeNode*>(dwDataNew);
   pParentNode->m_lstChildren.AddTail(pNode);
}

void Views::CLeftView::OnEditRename()
{
   HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();  
   if(hItem)
      GetTreeCtrl().EditLabel(hItem);
}

void Views::CLeftView::OnUpdateEditRename(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(GetTreeCtrl().GetSelectedItem() != NULL);
}

void Views::CLeftView::OnUpdateViewUponelevel(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(GetTreeCtrl().GetSelectedItem() != NULL);
}

void Views::CLeftView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
   // TODO: Add your control notification handler code here
   *pResult = 0;
}

void CLeftView::SaveTreeState(HTREEITEM hItem)
{
   CTreeCtrl& tree = GetTreeCtrl();

   if(hItem == nullptr)
      hItem = tree.GetRootItem();

   while (hItem)
   {
      DWORD_PTR dwData = tree.GetItemData(hItem);
      CCodeNode* pNode = reinterpret_cast<CCodeNode*>(dwData);

      UINT nState = tree.GetItemState(hItem, TVIS_EXPANDED);
      if (nState)
         pNode->m_bExpanded = true;
      else
         pNode->m_bExpanded = false;

      //TRACE("state: %d %s\n", nState, pNode->m_strName);

      if (tree.ItemHasChildren(hItem))
      {
         HTREEITEM hChildItem = tree.GetChildItem(hItem);
         SaveTreeState(hChildItem);
      }
      hItem = tree.GetNextSiblingItem(hItem);
   }
}