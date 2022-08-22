////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022  
// 
// The following code is supplied "as-is" and 
// is given without warranty of any kind. 
// It may be freely distributed and used. 
// The user holds author blameless from any complications or 
// problems that may arise from the use of this code.
// 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Author: steveb
//
// History:
// 
// 08/22/2022	Created as per TODO list
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace ColorSyntax {
   struct SyntaxIdentifier
   {
      CString  strWord;
      int      nStart = -1;   // starting position in the text
      int      nEnd = -1;     // Ending position in the text
   };

   typedef std::vector<SyntaxIdentifier> VecIdentifiers;

   class CSyntaxParser
   {
   public:
      bool GetWords(CString strText, VecIdentifiers& vecMatches);
      bool GetQuotedStrings(CString strText, VecIdentifiers& vecMatches);

   private:
      bool ParsePattern(const char* szPattern, CString strText, VecIdentifiers& vecMatches);
   };
}
