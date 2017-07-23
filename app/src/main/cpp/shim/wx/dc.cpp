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

#include <wx/wx.h>

wxDC::wxDC(android::Canvas&& canvas) : android::Canvas(std::move(canvas))
{
  scalex = scaley = 1.0;
  logorgx = logorgy = 0.0;
}

wxPoint wxDC::GetDeviceOrigin() const
{
  /* Stub */
  return wxPoint();
}

wxPoint wxDC::GetLogicalOrigin() const
{
  return wxPoint(logorgx, logorgy);
}

void wxDC::SetDeviceOrigin(wxCoord x, wxCoord y)
{
  /* Stub */
}

void wxDC::SetLogicalOrigin(wxCoord x, wxCoord y)
{
  if (x != logorgx || y != logorgy) {
    translate(logorgx-x, logorgy-y);
    logorgx = x;
    logorgy = y;
  }
}

void wxDC::SetUserScale(double x, double y)
{
  if (x != scalex || y != scaley) {
    logorgx *= x/scalex;
    logorgy *= y/scaley;
    scale(x/scalex, y/scaley);
    scalex = x;
    scaley = y;
  }
}

void wxDC::GetUserScale(double *x, double *y) const
{
  *x = scalex;
  *y = scaley;
}

void wxDC::SetBackground(const wxBrush& brush)
{
  /* Stub */
}

void wxDC::SetBackgroundMode(int mode)
{
  /* Stub */
}

void wxDC::Clear()
{
  /* Stub */
}

bool wxDC::Blit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
		wxDC *source, wxCoord xsrc, wxCoord ysrc,
		wxRasterOperationMode rop, bool useMask)
{
  /* Stub */
  return true;
}

wxCoord wxDC::LogicalToDeviceYRel(wxCoord y) const
{
  return y * scaley;
}

wxCoord wxDC::DeviceToLogicalX(wxCoord x) const
{
  return x / scalex + logorgx;
}

wxCoord wxDC::DeviceToLogicalY(wxCoord y) const
{
  return y / scaley + logorgy;
}

wxCoord wxDC::DeviceToLogicalXRel(wxCoord x) const
{
  return x / scalex;
}

wxCoord wxDC::DeviceToLogicalYRel(wxCoord y) const
{
  return y / scaley;
}

void wxMemoryDC::SelectObject(wxBitmap& bmp)
{
  setBitmap(bmp);
}
