/*
 * This file is part of the Gerberoid project.
 *
 * Copyright (C) 2017 Marcus Comstedt <marcus@mc.pp.se>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _WX_WXSTRINGH__
#define _WX_WXSTRINGH__

#include "wx/defs.h"

#include <string>
#include <vector>
#include <cstdlib>

typedef char wxChar;

typedef std::string wxStdString;
typedef wxStdString wxStringBase;

class wxMBConv;

class wxString : public wxStringBase
{
 public:
  wxString() : wxStringBase() {}
  wxString(const wxChar *str) : wxStringBase(str ? str : wxT("")) { }
  inline wxString(const char *str, const wxMBConv &conv);
  wxString(const wxChar *psz, size_t nLength) : wxStringBase(psz, nLength) { }
  wxString(const wxString& str) : wxStringBase(str) {}
  wxString(const wxString& str, size_t pos, size_t count = npos) : wxStringBase(str, pos, count) {}

  bool IsEmpty() const { return empty(); }
  void Empty() { clear(); }
  size_t Length() const { return length(); }
  int Cmp(const wxString &str) const;
  int CmpNoCase(const wxString& s) const;
  int Printf(const wxChar *pszFormat, ...) __attribute__((__format__(__printf__, 2, 3)));
  int PrintfV(const wxChar *pszFormat, va_list argptr) __attribute__((__format__(__printf__, 2, 0)));
  bool IsSameAs(const wxChar *psz, bool compareWithCase = true) const
    { return (compareWithCase ? Cmp(psz) : CmpNoCase(psz)) == 0; }
  wxString Mid(size_t nFirst, size_t nCount = npos) const;
  wxString& Append(wxChar ch, size_t count = 1u) { append(count, ch); return *this; }
  bool ToLong(long *val, int base = 10) const;
  wxString& MakeUpper();
  const wxChar* GetData() const { return c_str(); }
  const char *utf8_str() const { return c_str(); }
  wxChar& operator[](int n) { return wxStringBase::at(n); }
  wxChar GetChar(size_t n) const { return wxStringBase::at(n); }
  size_t Replace(const wxString& strOld,
                 const wxString& strNew,
                 bool bReplaceAll = true);
  wxString Left(size_t nCount) { return wxString(*this, 0, nCount); }

  operator const wxChar*() const { return c_str(); }

  wxString& operator=(const wxStringBase& stringSrc)
    { return (wxString&)wxStringBase::operator=(stringSrc); }
  wxString& operator=(const wxChar *psz)
    { if(psz) wxStringBase::operator=(psz); else clear(); return *this; }

  wxString& operator<<(const wxString& s)
  {
    append(s);
    return *this;
  }

  static wxString FromUTF8(const char *str) { return wxString(str); }
};

extern const wxChar* wxEmptyString;

class wxMBConv {
 public:
  const wxString cMB2WC(const char *in) const {
    return wxString::FromUTF8(in);
  }
  int WC2MB(char *ptr, const wchar_t *txt, int n) const {
    return std::wcstombs(ptr, txt, n);
  }
};

inline wxString::wxString(const char *str, const wxMBConv &conv) : wxStringBase(conv.cMB2WC(str? str : "")) {}

extern wxMBConv wxConvUTF8, *wxConvCurrent;

#endif  // _WX_WXSTRINGH__
