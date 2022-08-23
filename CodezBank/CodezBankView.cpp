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
//  $Workfile: $   created: 04/05/2008 13:33
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
#include "CodezBankView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Views;

bool CCodezBankView::m_bSortOrder = false;

IMPLEMENT_DYNCREATE(CCodezBankView, CListView)

BEGIN_MESSAGE_MAP(CCodezBankView, CListView)
	ON_WM_STYLECHANGED()
   ON_WM_CREATE()
   ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CCodezBankView::OnLvnGetdispinfo)
   ON_NOTIFY_REFLECT(NM_DBLCLK, &CCodezBankView::OnNMDblclk)
   ON_NOTIFY_REFLECT(NM_CLICK, &CCodezBankView::OnNMClick)
   ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CCodezBankView::OnLvnItemchanged)
   ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &CCodezBankView::OnLvnEndlabeledit)
   ON_COMMAND(ID_EDIT_DELETE, &CCodezBankView::OnEditDelete)
   ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, &CCodezBankView::OnUpdateEditDelete)
   ON_WM_CONTEXTMENU()
   ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CCodezBankView::OnLvnColumnclick)
   ON_COMMAND(ID_EDIT_RENAME, &CCodezBankView::OnEditRename)
END_MESSAGE_MAP()

CCodezBankView::CCodezBankView()
{
   m_nMask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
}

CCodezBankView::~CCodezBankView()
{
}

BOOL CCodezBankView::PreCreateWindow(CREATESTRUCT& cs)
{
   cs.style |= LVS_EDITLABELS | LVS_SHOWSELALWAYS;

	return CListView::PreCreateWindow(cs);
}

void CCodezBankView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
   Refresh(NULL);
}

#ifdef _DEBUG
void CCodezBankView::AssertValid() const
{
	CListView::AssertValid();
}

void CCodezBankView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CCodezBankDoc* CCodezBankView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCodezBankDoc)));
	return (CCodezBankDoc*)m_pDocument;
}
#endif //_DEBUG

void CCodezBankView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window	
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);	
}

int CCodezBankView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CListView::OnCreate(lpCreateStruct) == -1)
      return -1;

 
   DWORD dwExStyles = ListView_GetExtendedListViewStyle(GetListCtrl().GetSafeHwnd());
   ListView_SetExtendedListViewStyle(GetListCtrl().GetSafeHwnd(),
      LVS_EX_FULLROWSELECT |
      LVS_EX_HEADERDRAGDROP | dwExStyles);

   m_Image.Create(32, 32, ILC_MASK|ILC_COLOR24, 0, 1);

   m_Image.Add(AfxGetApp()->LoadIcon(IDI_FOLDER));
   m_Image.Add(AfxGetApp()->LoadIcon(IDI_CODE2));

   m_ImageSm.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1);

   m_ImageSm.Add(AfxGetApp()->LoadIcon(IDI_FOLDER));
   m_ImageSm.Add(AfxGetApp()->LoadIcon(IDI_CODE2));

   GetListCtrl().SetImageList(&m_Image, LVSIL_NORMAL);
   GetListCtrl().SetImageList(&m_ImageSm, LVSIL_SMALL);

   GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP);

   GetListCtrl().InsertColumn(0, "Name", LVCFMT_LEFT, 300, 0);
   GetListCtrl().InsertColumn(1, "Created", LVCFMT_LEFT, 120, 0);
   GetListCtrl().InsertColumn(2, "Modified", LVCFMT_LEFT, 120, 0);
   
   DWORD dwStyle = AfxGetApp()->GetProfileInt(SECTION_SETTINGS, KEY_LISTSTYLE, LVS_ICON);
   ModifyStyle(LVS_TYPEMASK, dwStyle);

   m_wndHeader.SubclassDlgItem(0, this);

   m_clrSortedColumn = CDrawingManager::PixelAlpha(GetListCtrl().GetBkColor(), .97, .97, .97);
   return 0;
}

void CCodezBankView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
   CCodeNode* pNodeIn = reinterpret_cast<CCodeNode*>(pHint);

   switch(lHint)
   {
   case hintRefreshListView:
      Refresh(pNodeIn);
      break;
   case hintClearDocument:
      GetListCtrl().DeleteAllItems();
      break;
   }
}

void CCodezBankView::Refresh(CCodeNode* pNode)
{
   GetListCtrl().DeleteAllItems();

   int nIndex = 0;

   if(pNode)
   {
      for(POSITION pos = pNode->m_lstChildren.GetHeadPosition(); pos != NULL; nIndex++)
      {
         CCodeNode* pItem = pNode->m_lstChildren.GetNext(pos);

         LV_ITEM lvi;
         lvi.mask = m_nMask; 
         lvi.iItem = nIndex; 
         lvi.iSubItem = 0; 
         lvi.iImage = pItem->m_nFolderType == nodeFolder ? 0:1;
         lvi.pszText = LPSTR_TEXTCALLBACK; 
         lvi.lParam = (LPARAM)pItem;

         GetListCtrl().InsertItem(&lvi);
      }
   }
   else
   {
      // topmost
      for(POSITION pos = GetDocument()->m_lstNodes.GetHeadPosition(); pos != NULL; nIndex++)
      {
         CCodeNode* pItem = GetDocument()->m_lstNodes.GetNext(pos);

         LV_ITEM lvi;
         lvi.mask = m_nMask; 
         lvi.iItem = nIndex; 
         lvi.iSubItem = 0; 
         lvi.iImage = 0;
         lvi.pszText = LPSTR_TEXTCALLBACK; 
         lvi.lParam = (LPARAM)pItem;

         GetListCtrl().InsertItem(&lvi);
      }
   }
}

void CCodezBankView::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
   NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

   if(pDispInfo->item.mask & LVIF_TEXT)
   {
      CCodeNode* pNode = reinterpret_cast<CCodeNode*>(pDispInfo->item.lParam);
      
      switch(pDispInfo->item.iSubItem)
      {
      case 0:
         lstrcpy (pDispInfo->item.pszText, pNode->m_strName);
         break;
      case 1:
         lstrcpy (pDispInfo->item.pszText, pNode->GetCreated());
         break;
      case 2:
         lstrcpy (pDispInfo->item.pszText, pNode->GetModified());
         break;
      }
   }
   *pResult = 0;
}

void CCodezBankView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
   *pResult = 0;

   if(pNMLV->iItem == -1)
      return;

   CCodeNode* pNode = reinterpret_cast<CCodeNode*>(GetListCtrl().GetItemData(pNMLV->iItem));
   Refresh(pNode);
   GetDocument()->UpdateAllViews(this, hintSelTreeItem, pNode);
}

void CCodezBankView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
   // TODO: Add your control notification handler code here
   *pResult = 0;
}

void CCodezBankView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
   // TODO: Add your control notification handler code here
   *pResult = 0;
}

void CCodezBankView::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
   NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
   CCodeNode* pNode = reinterpret_cast<CCodeNode*>(pDispInfo->item.lParam);

   if(pDispInfo->item.pszText)
   {
      pNode->m_strName = pDispInfo->item.pszText;
      pNode->Modify();
      GetDocument()->SetModifiedFlag();
      GetDocument()->UpdateAllViews(this, hintRepaintTree, pNode);
   }
   *pResult = 0;
}

void CCodezBankView::OnEditDelete()
{
   if(Ask(MB_YESNO, "Delete selected item(s)?") == IDNO)
      return;

   for(POSITION pos = GetListCtrl().GetFirstSelectedItemPosition(); pos != NULL;)
   {
      int nItem = GetListCtrl().GetNextSelectedItem(pos);
  
      CCodeNode* pNode = reinterpret_cast<CCodeNode*>(GetListCtrl().GetItemData(nItem));
      GetDocument()->UpdateAllViews(this, hintRemoveItemFromTree, pNode);
      GetListCtrl().DeleteItem(nItem);
      GetDocument()->DeleteNode(pNode);
      pos = GetListCtrl().GetFirstSelectedItemPosition();
   }
}

void CCodezBankView::OnUpdateEditDelete(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(GetListCtrl().GetFirstSelectedItemPosition() != NULL);
}

void CCodezBankView::OnContextMenu(CWnd* pWnd, CPoint point)
{
   theApp.GetContextMenuManager()->ShowPopupMenu(IDR_TREE_CONTEXT, point.x, point.y, this, TRUE);
}

int CALLBACK CCodezBankView::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   CCodeNode* pItem1 = reinterpret_cast<CCodeNode*>(lParam1);
   CCodeNode* pItem2 = reinterpret_cast<CCodeNode*>(lParam2);
   int nResult;

   switch (lParamSort) 
   {
   case 0: // name
      if(m_bSortOrder)
         nResult = pItem1->m_strName.CompareNoCase (pItem2->m_strName);
      else
         nResult = pItem2->m_strName.CompareNoCase (pItem1->m_strName);
      break;

   case 1: // created
      if(m_bSortOrder)
         nResult = pItem1->m_tCreated > pItem2->m_tCreated;
      else
         nResult = pItem1->m_tCreated < pItem2->m_tCreated;
      break;

   case 2: // modified
      if(m_bSortOrder)
         nResult = pItem1->m_tModified > pItem2->m_tModified;
      else
         nResult = pItem1->m_tModified < pItem2->m_tModified;
      break;
   }
   return nResult;

}

void CCodezBankView::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
   m_bSortOrder = !m_bSortOrder;
   GetListCtrl().SortItems (CompareFunc, pNMLV->iSubItem);
   m_iSortedColumn = pNMLV->iSubItem;
   m_wndHeader.SetSortColumn(pNMLV->iSubItem, m_bSortOrder);
   *pResult = 0;
}

void CCodezBankView::OnEditRename()
{
   POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();
   if(!pos)
      return;

   int iItem = GetListCtrl().GetNextSelectedItem(pos);
   GetListCtrl().EditLabel(iItem);
}
