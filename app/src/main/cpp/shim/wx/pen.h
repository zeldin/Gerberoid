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

#ifndef _WX_PEN_H_
#define _WX_PEN_H_

typedef enum {
  wxPENSTYLE_INVALID = -1,
  wxPENSTYLE_SOLID = wxSOLID,
  wxPENSTYLE_SHORT_DASH = wxSHORT_DASH,
  wxPENSTYLE_DOT_DASH = wxDOT_DASH
} wxPenStyle;

class wxPen
{
 private:
  int width;
  wxPenStyle style;
  wxColour colour;

 public:
  wxPen() : width(0), style(wxPENSTYLE_INVALID), colour(wxColour(0,0,0,0)) {}
  bool IsOk() const { return style != wxPENSTYLE_INVALID; }
  void SetColour(const wxColour& col) { colour = col; }
  void SetWidth(int w) { width = w; }
  void SetStyle(wxPenStyle sty) { style = sty; }
  wxColour GetColour() const { return colour; }
  int GetWidth() const { return width; }
  wxPenStyle GetStyle() const { return style; }
};

#endif // _WX_PEN_H_
