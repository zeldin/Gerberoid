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

#ifndef _WX_BRUSH_H_
#define _WX_BRUSH_H_

enum wxBrushStyle
{
  wxBRUSHSTYLE_SOLID = wxSOLID,
  wxBRUSHSTYLE_TRANSPARENT = wxTRANSPARENT
};

class wxBrush
{
 private:
  wxColour colour;
  wxBrushStyle style;
 public:
  wxBrush(wxColour c, wxBrushStyle s) : colour(c), style(s) {}
  wxBrush() : colour(wxColour(0, 0, 0, 0)), style(wxBRUSHSTYLE_SOLID) {}
  void SetColour(wxColour c) { colour = c; }
  void SetColour(unsigned char aRed, unsigned char aGreen, unsigned char aBlue) { SetColour(wxColour(aRed, aGreen, aBlue)); }
  void SetStyle(wxBrushStyle s) { style = s; }
  wxColour GetColour() const { return colour; }
  wxBrushStyle GetStyle() const { return style; }
};

#endif // _WX_BRUSH_H_
