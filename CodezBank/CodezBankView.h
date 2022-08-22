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
//  $Workfile: $   created: 04/05/2008 13:32
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
using namespace Document;

namespace Views{

class CCodezBankView : public CListView
{
   DECLARE_DYNCREATE(CCodezBankView)

public:
   static bool m_bSortOrder;

   static int CALLBACK CompareFunc (LPARAM, LPARAM, LPARAM);

	CCodezBankDoc* GetDocument() const;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~CCodezBankView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   CImageList  m_Image;
   CImageList  m_ImageSm;
   UINT        m_nMask;

   CCodezBankView();
   virtual void OnInitialUpdate(); // called first time after construct
   virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
   void Refresh(CCodeNode* pNode);

   DECLARE_MESSAGE_MAP()
   afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);   
   afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnEditDelete();
   afx_msg void OnUpdateEditDelete(CCmdUI *pCmdUI);
   afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
   afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnEditRename();
};

#ifndef _DEBUG  // debug version in CodezBankView.cpp
inline CCodezBankDoc* CCodezBankView::GetDocument() const
   { return reinterpret_cast<CCodezBankDoc*>(m_pDocument); }
#endif

};
