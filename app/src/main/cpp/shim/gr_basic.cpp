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

#include <gr_basic.h>
#include <class_eda_rect.h>

#include <Canvas.h>
#include <Paint.h>

void GRSetDrawMode( wxDC* DC, GR_DRAWMODE mode )
{
  /* Stub */
}

void GRLine( EDA_RECT* ClipBox, wxDC* DC, int x1, int y1, int x2, int y2, int width, EDA_COLOR_T Color)
{
  /* Stub */
}

void GRCircle( EDA_RECT* aClipBox, wxDC* aDC, wxPoint aPos, int aRadius, int aWidth, EDA_COLOR_T aColor )
{
  /* Stub */
}

void GRFilledCircle( EDA_RECT* aClipBox, wxDC* aDC, wxPoint aPos, int aRadius, EDA_COLOR_T aColor )
{
  android::Canvas &canvas = aDC->GetCanvas();
  android::Paint paint;
  paint.setColor(MakeColour(aColor).ARGB());
  paint.setStyle(android::Paint::Style::FILL);
  canvas.save();
  if (aClipBox)
    canvas.clipRect(aClipBox->GetX(), aClipBox->GetY(), aClipBox->GetRight(), aClipBox->GetBottom());
  canvas.drawCircle(aPos.x, aPos.y, aRadius, paint);
  canvas.restore();
}

void GRArc1( EDA_RECT* aClipBox, wxDC* aDC, wxPoint aStart, wxPoint aEnd,
             wxPoint aCenter, int aWidth, EDA_COLOR_T aColor )
{
  /* Stub */
}

void GRFilledSegment( EDA_RECT* aClipBox, wxDC* aDC, wxPoint aStart, wxPoint aEnd,
                      int aWidth, EDA_COLOR_T aColor )
{
  /* Stub */
}

void GRCSegm( EDA_RECT* ClipBox, wxDC* DC, int x1, int y1, int x2, int y2, int width, EDA_COLOR_T Color )
{
  /* Stub */
}

void GRFillCSegm( EDA_RECT* ClipBox, wxDC* DC, int x1, int y1, int x2, int y2,
                  int width, EDA_COLOR_T Color )
{
  /* Stub */
}

void GRCSegm( EDA_RECT* aClipBox, wxDC* aDC, wxPoint aStart, wxPoint aEnd,
              int aWidth, EDA_COLOR_T aColor )
{
  /* Stub */
}

void GRPoly( EDA_RECT* ClipBox, wxDC* DC, int n, wxPoint Points[],
             bool Fill, int width, EDA_COLOR_T Color, EDA_COLOR_T BgColor )
{
  /* Stub */
}

void GRClosedPoly( EDA_RECT* ClipBox,
                   wxDC  *   aDC,
                   int       aPointCount,
                   wxPoint   aPoints[],
                   bool      doFill,
                   EDA_COLOR_T aPenColor,
                   EDA_COLOR_T aFillColor )
{
  /* Stub */
}

void GRRect( EDA_RECT* ClipBox, wxDC* DC, int x1, int y1,
             int x2, int y2, int width, EDA_COLOR_T Color )
{
  /* Stub */
}

void GRFilledRect( EDA_RECT* ClipBox, wxDC* DC, int x1, int y1,
                   int x2, int y2, int width, EDA_COLOR_T Color, EDA_COLOR_T BgColor )
{
  /* Stub */
}

void GRDrawAnchor( EDA_RECT *aClipBox, wxDC *aDC, int x, int y,
                   int aSize, EDA_COLOR_T aColor )
{
  /* Stub */
}

void GRResetPenAndBrush(wxDC*)
{
  /* Stub */
}
