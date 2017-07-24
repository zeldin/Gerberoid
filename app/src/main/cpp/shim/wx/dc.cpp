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

#include <Rect.h>
#include <Canvas.h>
#include <Paint.h>
#include <PorterDuff.h>
#include <PathEffect.h>

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
  if (source->bitmap) {
    android::Paint paint;
    paint.setXfermode(android::PorterDuffXfermode(rop == wxOR?
						  android::PorterDuff::Mode::LIGHTEN :
						  (useMask? android::PorterDuff::Mode::SRC_OVER :
						   android::PorterDuff::Mode::SRC)));
    drawBitmap(source->bitmap,
	       android::Rect(xsrc, ysrc, xsrc+width, ysrc+height),
	       android::Rect(xdest, ydest, xdest+width, ydest+height),
	       paint);
    return true;
  }
  return false;
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

void wxDC::DrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2)
{
  /* Stub */
}

void wxDC::DrawLine(const wxPoint& pt1, const wxPoint& pt2)
{
  /* Stub */
}

void wxDC::DrawPoint(wxCoord x, wxCoord y)
{
  /* Stub */
}

void wxDC::DrawArc(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2,
		   wxCoord xc, wxCoord yc)
{
  /* Stub */
}

void wxDC::DrawArc(const wxPoint& pt1, const wxPoint& pt2, const wxPoint& centre)
{
  /* Stub */
}

void wxDC::DrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
  drawOval(android::RectF(x, y, x+width, y+height), paint);
}

void wxDC::DrawPolygon(int n, const wxPoint points[])
{
  /* Stub */
}

void wxDC::SetPen(const wxPen& pen)
{
  if (&pen == &this->pen)
    return;

  paint.setColor(pen.GetColour().ARGB());
  float w = pen.GetWidth();
  paint.setStrokeWidth(w);
  if (pen.GetStyle() != this->pen.GetStyle())
    switch(pen.GetStyle()) {
    case wxPENSTYLE_INVALID:
    case wxPENSTYLE_SOLID:
      paint.setPathEffect(0);
      break;
    case wxPENSTYLE_SHORT_DASH:
      paint.setPathEffect(android::DashPathEffect({w*10, w*10}, 0));
      break;
    case wxPENSTYLE_DOT_DASH:
      paint.setPathEffect(android::DashPathEffect({w, w*10}, 0));
      break;
    }
  this->pen = pen;
}

void wxDC::SetBrush(const wxBrush& brush)
{
  paint.setColor(brush.GetColour().ARGB());
  paint.setStyle((brush.GetStyle() == wxBRUSHSTYLE_SOLID?
		  android::Paint::Style::FILL_AND_STROKE :
		  android::Paint::Style::STROKE));
}

void wxDC::SetLogicalFunction(wxRasterOperationMode function)
{
  /* Stub */
}

void wxMemoryDC::SelectObject(wxBitmap& bmp)
{
  android::Bitmap tmpbmp(bmp);
  bitmap.swap(tmpbmp);
  setBitmap(bmp);
}
