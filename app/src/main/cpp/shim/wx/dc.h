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

#ifndef _WX_DC_H_
#define _WX_DC_H_

#include <Canvas.h>

enum wxRasterOperationMode
{
  wxCOPY,
  wxOR
};

class wxDC : protected android::Canvas
{
 private:
  float scalex, scaley;
  float logorgx, logorgy;

 public:
  wxDC(android::Canvas&& canvas);
  android::Canvas &GetCanvas() { return *this; }

  wxPoint GetDeviceOrigin() const;
  wxPoint GetLogicalOrigin() const;
  void SetDeviceOrigin(wxCoord x, wxCoord y);
  void SetLogicalOrigin(wxCoord x, wxCoord y);
  void SetUserScale(double x, double y);
  void GetUserScale(double *x, double *y) const;
  void SetBackground(const wxBrush& brush);
  void SetBackgroundMode(int mode);
  void Clear();
  bool Blit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
	    wxDC *source, wxCoord xsrc, wxCoord ysrc,
	    wxRasterOperationMode rop = wxCOPY, bool useMask = false);
  wxCoord LogicalToDeviceYRel(wxCoord y) const;
  wxCoord DeviceToLogicalX(wxCoord x) const;
  wxCoord DeviceToLogicalY(wxCoord y) const;
  wxCoord DeviceToLogicalXRel(wxCoord x) const;
  wxCoord DeviceToLogicalYRel(wxCoord y) const;
};

class wxMemoryDC : public wxDC
{
 public:
  wxMemoryDC() : wxDC(android::Canvas()) {}

  void SelectObject(wxBitmap& bmp);
};

#endif // _WX_DC_H_
