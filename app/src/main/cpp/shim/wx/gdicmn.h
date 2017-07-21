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

#ifndef _WX_GDICMNH__
#define _WX_GDICMNH__

typedef int wxCoord;
enum {  wxDefaultCoord = -1 };

class wxSize
{
 public:
  int x, y;

  wxSize() : x(0), y(0) { }
  wxSize(int aX, int aY) : x(aX), y(aY) { }

  int GetWidth() const { return x; }
  int GetHeight() const { return y; }
};

class wxPoint
{
 public:
  int x, y;

  wxPoint() : x(0), y(0) { }
  wxPoint(int aX, int aY) : x(aX), y(aY) { }

  wxPoint operator+(const wxPoint& p) const { return wxPoint(x + p.x, y + p.y); }
  wxPoint operator-(const wxPoint& p) const { return wxPoint(x - p.x, y - p.y); }
  wxPoint& operator+=(const wxPoint& p) { x += p.x; y += p.y; return *this; }
  wxPoint& operator-=(const wxPoint& p) { x -= p.x; y -= p.y; return *this; }
  wxPoint operator-() const { return wxPoint(-x, -y); }

  wxPoint operator+(const wxSize& s) const { return wxPoint(x + s.GetWidth(), y + s.GetHeight()); }
  wxPoint operator-(const wxSize& s) const { return wxPoint(x - s.GetWidth(), y - s.GetHeight()); }
};

class wxRealPoint
{
 public:
  double x, y;

  wxRealPoint() : x(0), y(0) { }
  wxRealPoint(double aX, double aY) : x(aX), y(aY) { }
};

class wxRect
{
 public:
  int x, y, width, height;

  wxRect() : x(0), y(0), width(0), height(0) { }
  wxRect(const wxPoint& aPoint, const wxSize& aSize)
    : x(aPoint.x), y(aPoint.y), width(aSize.x), height(aSize.y) { }
};

extern wxPoint wxDefaultPosition;

#endif // _WX_GDICMNH__
