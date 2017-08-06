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
#include "gr_basic.h"
#include "gerbview_frame.h"
#include "convert_to_biu.h"

#define MIN_GRID_SIZE 20

void EDA_DRAW_PANEL::SetPrintMirrored(bool mode)
{
  /* Stub */
}

void EDA_DRAW_PANEL::DoPrepareDC(wxDC& dc)
{
  dc.SetUserScale(m_userScale, m_userScale);
  dc.SetLogicalOrigin(m_logicalOriginX, m_logicalOriginY);
  EDA_RECT clipBox;
  clipBox.SetSize( GetClientSize() );
  clipBox.Inflate( 2 );
  m_ClipBox.SetOrigin( wxPoint( dc.DeviceToLogicalX( clipBox.GetX() ),
				dc.DeviceToLogicalY( clipBox.GetY() ) ) );
  m_ClipBox.SetSize( wxSize( dc.DeviceToLogicalXRel( clipBox.GetWidth() ),
			     dc.DeviceToLogicalYRel( clipBox.GetHeight() ) ) );
  GRResetPenAndBrush( &dc );
  dc.SetBackgroundMode(wxTRANSPARENT);
}

void EDA_DRAW_PANEL::DrawBackGround(wxDC* DC)
{
  GRSetDrawMode( DC, GR_COPY );
  if( GetParent()->IsElementVisible(GERBER_GRID_VISIBLE) )
    DrawGrid( DC );
}

void EDA_DRAW_PANEL::DrawGrid(wxDC* DC)
{
  wxRealPoint  gridSize(1000*IU_PER_DECIMILS, 1000*IU_PER_DECIMILS);
  wxSize       screenSize;
  wxPoint      org;
  wxRealPoint  screenGridSize;

  screenSize = GetClientSize();
  screenGridSize.x = DC->LogicalToDeviceXRel(KiROUND(gridSize.x));
  screenGridSize.y = DC->LogicalToDeviceYRel(KiROUND(gridSize.y));
  org = m_ClipBox.GetPosition();
  if (screenGridSize.x < MIN_GRID_SIZE || screenGridSize.y < MIN_GRID_SIZE)
    return;
  org.x = KiROUND(KiROUND(org.x/gridSize.x)*gridSize.x);
  org.y = KiROUND(KiROUND(org.y/gridSize.y)*gridSize.y);
  GRSetColorPen(DC, GetParent()->GetVisibleElementColor(GERBER_GRID_VISIBLE));
  double right = m_ClipBox.GetRight();
  double bottom = m_ClipBox.GetBottom();
  for (double x=org.x; x<=right; x+=gridSize.x) {
    int xpos = KiROUND( x );
    for (double y=org.y; y<=bottom; y+=gridSize.y)
      DC->DrawPoint( xpos, KiROUND( y )  );
  }
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

void EDA_DRAW_PANEL::SetOriginAndScale(int logicalOriginX, int logicalOriginY, float userScale)
{
  m_logicalOriginX = logicalOriginX;
  m_logicalOriginY = logicalOriginY;
  m_userScale = userScale;
}
