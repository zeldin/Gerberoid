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

#ifndef PANEL_WXSTRUCT_H
#define PANEL_WXSTRUCT_H

#include <base_struct.h>
#include <class_base_screen.h>

class GERBVIEW_FRAME;

class EDA_DRAW_PANEL : public wxWindow
{
  friend class GERBVIEW_FRAME;
 private:
  EDA_RECT        m_ClipBox;
  GERBVIEW_FRAME *m_Parent;
  int             m_logicalOriginX;
  int             m_logicalOriginY;
  float           m_userScale;

 public:
  EDA_DRAW_PANEL(GERBVIEW_FRAME *parent, android::View&& view) : wxWindow(std::move(view)), m_Parent(parent), m_logicalOriginX(0), m_logicalOriginY(0), m_userScale(1.0) {}
  GERBVIEW_FRAME* GetParent() const { return m_Parent; }
  EDA_RECT* GetClipBox() { return &m_ClipBox; }
  void SetClipBox( const EDA_RECT& aRect ) { m_ClipBox = aRect; }
  void SetPrintMirrored(bool mode);
  void DoPrepareDC(wxDC& DC);
  void DrawBackGround(wxDC* DC);
  void DrawCrossHair(wxDC* DC);
  bool IsMouseCaptured();
  void CallMouseCapture( wxDC* aDC, const wxPoint& aPosition, bool aErase );
  void SetOriginAndScale(int logicalOriginX, int logicalOriginY, float userScale);
};

#endif // PANEL_WXSTRUCT_H
