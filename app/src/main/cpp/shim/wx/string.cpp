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

#include "wx/string.h"

#include <string>
#include <locale>
#include <cstdlib>
#include <cstdarg>

const wxChar* wxEmptyString = "";

int wxString::Cmp(const wxString &str) const
{
  return strcmp(c_str(), str.c_str());
}

int wxString::CmpNoCase(const wxString& s) const
{
  return strcasecmp(c_str(), s.c_str());
}

int wxString::PrintfV(const wxChar *pszFormat, va_list va)
{
  for(size_t sz = 1024; ; )
  {
    char buf[sz];
    int len = vsnprintf(buf, sz, pszFormat, va);
    if (len>=0 && len<sz) {
      assign(buf, len);
      break;
    }
    if (len > 0)
      sz = len+4;
    else
      sz <<= 1;
  }
  return size();
}

int wxString::Printf(const wxChar *pszFormat, ...)
{
  va_list va;
  va_start(va, pszFormat);
  int result = PrintfV(pszFormat, va);
  va_end(va);
  return result;
}

wxString wxString::Mid(size_t nFirst, size_t nCount) const
{
  size_t len = length();
  if (nFirst > len) return "";
  if (nCount == npos) nCount = len - nFirst;
  if (nCount > len - nFirst) nCount = len - nFirst;
  return wxString(*this, nFirst, nCount);
}

bool wxString::ToLong(long *val, int base) const
{
  char *end;
  long v = strtol(c_str(), &end, base);
  if (*end)
    return false;
  if (val)
    *val = v;
  return true;
}

wxString& wxString::MakeUpper()
{
  auto & f = std::use_facet<std::ctype<char>>(std::locale());
  f.toupper(&*begin(), &*end());
  return *this;
}

namespace {
  wxMBConv conv;
};

wxMBConv *wxConvCurrent = &conv;
