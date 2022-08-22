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
#include "stdafx.h"
#include "SyntaxParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace ColorSyntax;

bool CSyntaxParser::GetWords(CString strText, VecIdentifiers& vecMatches)
{
   // Identifiers: words and numbers or combinations of both
   return ParsePattern("[#a-zA-Z0-9_]*\\b", strText, vecMatches);
}

bool CSyntaxParser::GetQuotedStrings(CString strText, VecIdentifiers& vecMatches)
{
   // Strings withing the quotation marks
   return ParsePattern("\"(\\.|[^\\\"])*\"", strText, vecMatches);
}

bool CSyntaxParser::ParsePattern(const char* szPattern, CString strText, VecIdentifiers& vecMatches)
{
   std::regex rgx(szPattern);
   std::cmatch match;
   std::regex_constants::match_flag_type flags = std::regex_constants::match_default;
   const char* tgt = strText.GetBuffer();
   const char* first = tgt;
   const char* last = tgt + strlen(tgt);

   while (std::regex_search(first, last, match, rgx, flags))
   {
      SyntaxIdentifier identifier;

      // show match, move past it
      identifier.strWord = match.str().c_str();
      first += match.position() + match.length();
      flags = flags | std::regex_constants::match_not_bol;

      if (identifier.strWord.IsEmpty())
         return false;

      identifier.nEnd = (int)(first - tgt);
      identifier.nStart = identifier.nEnd - identifier.strWord.GetLength();

      if (identifier.nStart == identifier.nEnd)
         return false;

      vecMatches.push_back(identifier);
   }
   return true;
}
