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
#include <Path.h>
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
    translate(logorgx, logorgy);
    scale(x/scalex, y/scaley);
    translate(-logorgx, -logorgy);
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
  if (x1 == x2 && y1 == y2)
    DrawPoint(x1, y1);
  else
    drawLine(x1, y1, x2, y2, paint);
}

void wxDC::DrawLine(const wxPoint& pt1, const wxPoint& pt2)
{
  DrawLine(pt1.x, pt1.y, pt2.x, pt2.y);
}

void wxDC::DrawPoint(wxCoord x, wxCoord y)
{
  drawPoint(x, y, paint);
}

void wxDC::DrawArc(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2,
		   wxCoord xc, wxCoord yc)
{
  double dx = x1-xc;
  double dy = y1-yc;
  int r = sqrt(dx*dx+dy*dy);
  float angle1 = atan2f(y1-yc, x1-xc)*(float)(180/M_PI);
  float angle2 = atan2f(y2-yc, x2-xc)*(float)(180/M_PI);
  float start = angle2;
  float sweep = angle1-angle2;
  if (start < 0)
    start += 360;
  if (sweep <= 0)
    sweep += 360;
  drawArc(android::RectF(xc-r, yc-r, xc+r, yc+r), start, sweep, false, paint);
}

void wxDC::DrawArc(const wxPoint& pt1, const wxPoint& pt2, const wxPoint& centre)
{
  DrawArc(pt1.x, pt1.y, pt2.x, pt2.y, centre.x, centre.y);
}

void wxDC::DrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
  drawOval(android::RectF(x, y, x+width, y+height), paint);
}

void wxDC::DrawPolygon(int n, const wxPoint points[])
{
  if (n > 3 && points[0] == points[1]) {
    --n;
    points++;
  }
  if (n == 4 &&
      points[0].x == points[1].x && points[1].y == points[2].y &&
      points[2].x == points[3].x && points[0].y == points[3].y &&
      points[3].x >= points[1].x && points[3].y >= points[1].y) {
    /* Actually a rectangle... */
    drawRect(points[1].x, points[1].y, points[3].x, points[3].y, paint);
  } else if (n >= 2) {
    android::Path path;
    path.moveTo(points[0].x, points[0].y);
    for(int i=1; i<n; i++)
      path.lineTo(points[i].x, points[i].y);
    path.close();
    drawPath(path, paint);
  }
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
      paint.setPathEffect(android::DashPathEffect({w/10, w*10}, 0));
      break;
    }
  if (!this->pen.IsOk()) {
    paint.setStrokeCap(android::Paint::Cap::ROUND);
    paint.setStrokeJoin(android::Paint::Join::ROUND);
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
  switch(function) {
  case wxCOPY:
    paint.setXfermode(0);
    break;
  case wxOR:
    paint.setXfermode(android::PorterDuffXfermode(android::PorterDuff::Mode::LIGHTEN));
    break;
  case wxXOR:
    paint.setXfermode(android::PorterDuffXfermode(android::PorterDuff::Mode::XOR));
    break;
  case wxEQUIV:
    paint.setXfermode(android::PorterDuffXfermode(android::PorterDuff::Mode::SRC_IN));
    break;
  case wxINVERT:
    paint.setXfermode(android::PorterDuffXfermode(android::PorterDuff::Mode::DST_OUT));
    break;
  }
}

void wxMemoryDC::SelectObject(wxBitmap& bmp)
{
  android::Bitmap tmpbmp(bmp);
  bitmap.swap(tmpbmp);
  setBitmap(bmp);
}
