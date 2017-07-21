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

#ifndef BOX2_H_
#define BOX2_H_

#include <algorithm>
#include <deque>

struct VECTOR2D
{
  double x;
  double y;
  VECTOR2D(double x = 0.0, double y = 0.0) : x(x), y(y) {}
};

struct VECTOR2I
{
  int x;
  int y;
};

typedef VECTOR2D DPOINT;

class BOX2D
{
 private:
  VECTOR2D m_Pos;
  VECTOR2D m_Size;
 public:
  double GetRight() const { return m_Pos.x + m_Size.x; }
  double GetBottom() const { return m_Pos.y + m_Size.y; }
  const VECTOR2D GetEnd() const { return VECTOR2D( GetRight(), GetBottom() ); }
  void Compute(const std::deque<VECTOR2D> &aPointList)
  {
    m_Pos = aPointList[0];
    m_Size = VECTOR2D();
    for(std::deque<VECTOR2D>::const_iterator i = aPointList.begin();
	i != aPointList.end(); ++i ) {
      if ((*i).x < m_Pos.x) {
	m_Size.x += m_Pos.x - (*i).x;
	m_Pos.x = (*i).x;
      } else if ((*i).x > m_Pos.x + m_Size.x) {
	m_Size.x = (*i).x - m_Pos.x;
      }
      if ((*i).y < m_Pos.y) {
	m_Size.y += m_Pos.y - (*i).y;
	m_Pos.y = (*i).y;
      } else if ((*i).y > m_Pos.y + m_Size.y) {
	m_Size.y = (*i).y - m_Pos.y;
      }
    }
  }
};

#endif // BOX2_H_
