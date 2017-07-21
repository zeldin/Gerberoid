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

#ifndef _WX_BITMAP_H_
#define _WX_BITMAP_H_

class wxBitmap;

class wxMask
{
 public:
  wxMask(const wxBitmap& bitmap, const wxColour& colour);
};

class wxBitmap
{
 public:
  wxBitmap(int width, int height);
  void SetMask(wxMask *mask);
};

extern wxBitmap wxNullBitmap;

#endif // _WX_BITMAP_H_
