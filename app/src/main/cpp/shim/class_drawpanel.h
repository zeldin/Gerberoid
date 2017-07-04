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

#include <base_struct.h>

class GERBVIEW_FRAME;

class EDA_DRAW_PANEL
{
 private:
  EDA_RECT        m_ClipBox;
  GERBVIEW_FRAME *m_Parent;

 public:
  EDA_DRAW_PANEL(GERBVIEW_FRAME *parent) : m_Parent(parent) {}
  GERBVIEW_FRAME* GetParent() const { return m_Parent; };
  EDA_RECT* GetClipBox() { return &m_ClipBox; }
};
