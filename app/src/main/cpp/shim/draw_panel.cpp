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

#include "class_drawpanel.h"

void EDA_DRAW_PANEL::SetPrintMirrored(bool mode)
{
  /* Stub */
}

void EDA_DRAW_PANEL::DoPrepareDC(wxDC& DC)
{
  /* Stub */
}

void EDA_DRAW_PANEL::DrawBackGround(wxDC* DC)
{
  /* Stub */
}

void EDA_DRAW_PANEL::DrawCrossHair(wxDC* DC)
{
  /* Stub */
}

bool EDA_DRAW_PANEL::IsMouseCaptured()
{
  /* Stub */
  return false;
}

void EDA_DRAW_PANEL::CallMouseCapture( wxDC* aDC, const wxPoint& aPosition, bool aErase )
{
  /* Stub */
}

void EDA_DRAW_PANEL::GetClientSize( int *w, int *h ) const
{
  /* Stub */
  *w = 0;
  *h = 0;
}

