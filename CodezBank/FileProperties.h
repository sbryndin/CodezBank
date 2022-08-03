#pragma once

namespace Utils{
class CFileProps
{
public:
   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   CFileProps(LPCTSTR lpFile, bool bNTFSStream=true):
     m_pPropSetStg(NULL)
   , m_pPropStg(NULL)
   , m_pPropDoc(NULL)
   {
      CString strFile(lpFile);

      HRESULT hr = S_OK;

      DWORD dwFmt = STGFMT_FILE;

      if(bNTFSStream == false)
         dwFmt = STGFMT_STORAGE;

      hr = ::StgOpenStorageEx(strFile.AllocSysString(), 
                              STGM_READWRITE| STGM_SHARE_EXCLUSIVE,
                              dwFmt,
                              // STGFMT_STORAGE => Structured Storage 
                                                // property sets
                              // STGFMT_FILE    => NTFS file system 
                                                 // property sets
                              0, NULL, NULL,
                              IID_IPropertySetStorage,
                              reinterpret_cast<void**>(&m_pPropSetStg) );

      if(FAILED(hr))
         throw new CFileException();

      hr = m_pPropSetStg->Open(FMTID_SummaryInformation,
                               STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 
                               &m_pPropStg);
      if(FAILED(hr))
      {
         if(hr == STG_E_FILENOTFOUND)
         {
            hr = m_pPropSetStg->Create(FMTID_SummaryInformation, 
                                       NULL, PROPSETFLAG_DEFAULT, 
                                       STGM_CREATE|STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                                       &m_pPropStg);
         }
         else
         {
            m_pPropSetStg->Release();
            throw new CFileException();
         }
      }

      hr = m_pPropSetStg->Open(FMTID_DocSummaryInformation,
                               STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 
                               &m_pPropDoc);

      if(FAILED(hr))
      {
         if(hr == STG_E_FILENOTFOUND || hr == STG_E_INVALIDHEADER)
         {
            hr = m_pPropSetStg->Create(FMTID_DocSummaryInformation, 
                                       NULL, PROPSETFLAG_DEFAULT, 
                                       STGM_CREATE|STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                                       &m_pPropDoc);
         }
         else
         {
            m_pPropSetStg->Release();
            throw new CFileException();
         }
      }
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   virtual ~CFileProps(void)
   {
      if(m_pPropStg)
         m_pPropStg->Release();

      if(m_pPropDoc)
         m_pPropDoc->Release();

      if(m_pPropSetStg)
         m_pPropSetStg->Release();
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   CString GetTitle(void)
   {
      return GetStringProperty(PIDSI_TITLE);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   bool SetTitle(LPCTSTR lpVal)
   {
      return SetStringProperty(PIDSI_TITLE, lpVal);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   CString GetSubject(void)
   {
      return GetStringProperty(PIDSI_SUBJECT);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   bool SetSubject(LPCTSTR lpVal)
   {
      return SetStringProperty(PIDSI_SUBJECT, lpVal);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   CString GetAuthor(void)
   {
      return GetStringProperty(PIDSI_AUTHOR);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   bool SetAuthor(LPCTSTR lpVal)
   {
      return SetStringProperty(PIDSI_AUTHOR, lpVal);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   CString GetKeywords(void)
   {
      return GetStringProperty(PIDSI_KEYWORDS);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   bool SetKeywords(LPCTSTR lpVal)
   {
      return SetStringProperty(PIDSI_KEYWORDS, lpVal);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   CString GetComments(void)
   {
      return GetStringProperty(PIDSI_COMMENTS);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   bool SetComments(LPCTSTR lpVal)
   {
      return SetStringProperty(PIDSI_COMMENTS, lpVal);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   CString GetCategory(void)
   {
      return GetStringPropertyDoc(PIDDSI_CATEGORY);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   bool SetCategory(LPCTSTR lpVal)
   {
      return SetStringPropertyDoc(PIDDSI_CATEGORY, lpVal);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   CString GetVersion(void)
   {
      return GetStringProperty(PIDSI_REVNUMBER);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   bool SetVersion(LPCTSTR lpVal)
   {
      return SetStringProperty(PIDSI_REVNUMBER, lpVal);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   CString GetTemplate(void)
   {
      return GetStringProperty(PIDSI_TEMPLATE);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   bool SetTemplate(LPCTSTR lpVal)
   {
      return SetStringProperty(PIDSI_TEMPLATE, lpVal);
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   CString GetLastAuthor(void)
   {
      return GetStringProperty(PIDSI_LASTAUTHOR);
   }

   bool SetLastAuthor(LPCTSTR lpVal)
   {
      return SetStringProperty(PIDSI_LASTAUTHOR, lpVal);
   }

protected:
   IPropertySetStorage* m_pPropSetStg;
   IPropertyStorage *   m_pPropStg;
   IPropertyStorage *   m_pPropDoc;

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   CString GetStringProperty(PROPID pid)
   {
      PROPSPEC propspec; 
      PROPVARIANT propvarRead; 

      propspec.ulKind = PRSPEC_PROPID;
      propspec.propid = pid;
      HRESULT hr = m_pPropStg->ReadMultiple( 1, &propspec, &propvarRead );

      CString sRet;
      
      switch(propvarRead.vt)
      {
      case VT_LPWSTR:
         sRet = CString(propvarRead.pwszVal);
         break;
      case VT_LPSTR:
         sRet = propvarRead.pszVal;
         break;
   }
   
   return sRet;
   }

   ///////////////////////////////////////////////
   ///
   /// 
   ///////////////////////////////////////////////
   bool SetStringProperty(PROPID pid, LPCTSTR lpVal)
   {
      CString strVal(lpVal);
      PROPSPEC propspec; 
      PROPVARIANT propvarWrite; 

      propspec.ulKind = PRSPEC_PROPID;
      propspec.propid = pid;

      propvarWrite.vt = VT_LPWSTR;
      propvarWrite.pwszVal =  strVal.AllocSysString();

      HRESULT hr = m_pPropStg->WriteMultiple( 1, &propspec, &propvarWrite, PID_FIRST_USABLE);
      if(FAILED(hr)) 
         return false;

      return true;
   }

   CString GetStringPropertyDoc(PROPID pid)
   {
      PROPSPEC propspec; 
      PROPVARIANT propvarRead; 

      propspec.ulKind = PRSPEC_PROPID;
      propspec.propid = pid;
      HRESULT hr = m_pPropDoc->ReadMultiple( 1, &propspec, &propvarRead );

      CString sRet;
      
      switch(propvarRead.vt)
      {
      case VT_LPWSTR:
         sRet = CString(propvarRead.pwszVal);
         break;
      case VT_LPSTR:
         sRet = propvarRead.pszVal;
         break;
      }
      
      return sRet;
   }

   bool SetStringPropertyDoc(PROPID pid, LPCTSTR lpVal)
   {
      CString strVal(lpVal);
      PROPSPEC propspec; 
      PROPVARIANT propvarWrite; 

      propspec.ulKind = PRSPEC_PROPID;
      propspec.propid = pid;

      propvarWrite.vt = VT_LPWSTR;
      propvarWrite.pwszVal =  strVal.AllocSysString();

      HRESULT hr = m_pPropDoc->WriteMultiple( 1, &propspec, &propvarWrite, PID_FIRST_USABLE);
      if(FAILED(hr)) 
         return false;

      return true;
   }
};
};
