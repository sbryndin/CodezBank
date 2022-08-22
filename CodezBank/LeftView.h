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
//  $Workfile: $   created: 04/05/2008 11:51
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

namespace Document{class CCodezBankDoc;};
namespace Data { class CCodeNode;};

using namespace Document;
using namespace Data;

namespace Views{

class CLeftView : public CTreeView
{
   DECLARE_DYNCREATE(CLeftView)

public:
	CCodezBankDoc* GetDocument();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~CLeftView();
   void FindString(LPCTSTR lpString, BOOL bDown, BOOL bCaseSensitve, BOOL bWholeWord);
   
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   CImageList  m_Image;
   UINT        m_nMask;
   BOOL		   m_bLDragging;
	HTREEITEM	m_hDrag,m_hDrop;
   CImageList*	m_pDragImage;

   CLeftView();
   virtual void OnInitialUpdate(); // called first time after construct
   virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
   void Populate(HTREEITEM hParent, CCodeNode* pNode);
   void AddNewFolder(CCodeNode* pNodeIn);
   void AddNewTopFolder(CCodeNode* pNodeIn);
   HTREEITEM DoSelect(HTREEITEM hStart, CCodeNode* pNode);
   HTREEITEM DoRemove(HTREEITEM hStart, CCodeNode* pNode);
   void MoveUp(void);
   HTREEITEM CopyBranch( HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter = TVI_LAST);
   HTREEITEM CopyItem( HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter = TVI_LAST);
   virtual void OnItemCopied(HTREEITEM hItem, HTREEITEM hNewItem);
   void SaveTreeState(HTREEITEM hItem);

	DECLARE_MESSAGE_MAP()
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnTvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnEditDelete();
   afx_msg void OnUpdateEditDelete(CCmdUI *pCmdUI);
   afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnEditRename();
   afx_msg void OnUpdateEditRename(CCmdUI *pCmdUI);
   afx_msg void OnUpdateViewUponelevel(CCmdUI *pCmdUI);
   afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CCodezBankDoc* CLeftView::GetDocument()
   { return reinterpret_cast<CCodezBankDoc*>(m_pDocument); }
#endif

};