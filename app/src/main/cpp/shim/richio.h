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

struct IO_ERROR
{
    IO_ERROR() {}
};

struct OUTPUTFORMATTER
{
};

inline int StrPrintf( std::string* aResult, const char* aFormat, ...)
  __attribute__((__format__(__printf__, 2, 3)));
inline int StrPrintf( std::string* aResult, const char* aFormat, ...)
{
  va_list va;
  va_start(va, aFormat);
  wxString tmp;
  int result = tmp.PrintfV(aFormat, va);
  if (aResult) *aResult = tmp;
  va_end(va);
  return result;
}
