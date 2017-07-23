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

#include <fctsys.h>
#include <common.h>
#include <base_struct.h>

bool EDA_RECT::Contains( const wxPoint& aPoint ) const
{
  if (m_Size.x<0) {
    if (aPoint.x < m_Pos.x+m_Size.x || aPoint.x > m_Pos.x)
      return false;
  } else {
    if (aPoint.x < m_Pos.x || aPoint.x > m_Pos.x+m_Size.x)
      return false;
  }
  if (m_Size.y<0) {
    if (aPoint.y < m_Pos.y+m_Size.y || aPoint.y > m_Pos.y)
      return false;
  } else {
    if (aPoint.y < m_Pos.y || aPoint.y > m_Pos.y+m_Size.y)
      return false;
  }
  return true;
}

bool EDA_RECT::Contains( const EDA_RECT& aRect ) const
{
  return Contains(aRect.GetOrigin()) && Contains(aRect.GetEnd());
}

namespace {
  inline void InflateCoord(wxCoord &size, wxCoord &pos, wxCoord delta)
  {
    pos -= delta;
    if ((size += 2*delta) < 0) {
      pos += size/2;
      size = 0;
    }
  }
}

EDA_RECT& EDA_RECT::Inflate( wxCoord dx, wxCoord dy )
{
  InflateCoord(m_Size.x, m_Pos.x, dx);
  InflateCoord(m_Size.y, m_Pos.y, dy);
  return *this;
}

EDA_RECT& EDA_RECT::Inflate(int aDelta)
{
  return Inflate(aDelta, aDelta);
}

void EDA_RECT::Merge(const EDA_RECT& aRect)
{
  wxCoord x1 = m_Pos.x;
  wxCoord x2 = x1 + m_Size.x;
  wxCoord x3 = aRect.m_Pos.x;
  wxCoord x4 = x3 + aRect.m_Size.x;
  wxCoord y1 = m_Pos.y;
  wxCoord y2 = y1 + m_Size.y;
  wxCoord y3 = aRect.m_Pos.y;
  wxCoord y4 = y3 + aRect.m_Size.y;
  m_Pos.x = std::min(std::min(x1, x2), std::min(x3, x4));
  m_Pos.y = std::min(std::min(y1, y2), std::min(y3, y4));
  m_Size.x = std::max(std::max(x1, x2), std::max(x3, x4)) - m_Pos.x;
  m_Size.y = std::max(std::max(y1, y2), std::max(y3, y4)) - m_Pos.y;
}

void EDA_RECT::Normalize()
{
  if (m_Size.y < 0) {
    m_Size.y = -m_Size.y;
    m_Pos.y -= m_Size.y;
  }
  if (m_Size.x < 0)
  {
    m_Size.x = -m_Size.x;
    m_Pos.x -= m_Size.x;
  }
}

void EDA_RECT::Move( const wxPoint& aMoveVector )
{
    m_Pos += aMoveVector;
}

bool EDA_RECT::Intersects( const EDA_RECT& aRect ) const
{
  EDA_RECT self(*this);
  EDA_RECT other(aRect);
  self.Normalize();
  other.Normalize();
  return std::max(self.m_Pos.x, other.m_Pos.x) <=
    std::min(self.m_Pos.x + self.m_Size.x, other.m_Pos.x + other.m_Size.x) &&
    std::max(self.m_Pos.y, other.m_Pos.y) <=
    std::min(self.m_Pos.y + self.m_Size.y, other.m_Pos.y + other.m_Size.y);
}

EDA_ITEM::EDA_ITEM( EDA_ITEM* parent, KICAD_T idType )
  : Pnext(NULL), Pback(NULL), m_List(NULL), m_Parent(parent),
    m_TimeStamp(0), m_Flags(0)
{
}
