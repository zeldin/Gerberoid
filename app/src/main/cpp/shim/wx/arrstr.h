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

#ifndef _WX_ARRSTR_H
#define _WX_ARRSTR_H

#include <vector>

#include "wx/string.h"

class wxArrayString : public std::vector<wxString>
{
 public:
  void Add(const wxString &aItem, size_t aNInsert = 1) { insert(end(), aNInsert, aItem); }
  size_t GetCount() const { return size(); }
  size_t Count() const { return size(); }
  void Clear() { clear(); }
  wxString& Item(size_t aIndex) const { return (wxString&)vector::operator[](aIndex); }
};

#endif
