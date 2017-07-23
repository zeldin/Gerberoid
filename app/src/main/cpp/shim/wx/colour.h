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

#ifndef _WX_COLOUR_H_
#define _WX_COLOUR_H_

class wxColour
{
 private:
  uint32_t m_ARGB;

 public:
  wxColour(unsigned char aRed, unsigned char aGreen, unsigned char aBlue, unsigned char aAlpha = wxALPHA_OPAQUE) :
    m_ARGB(aAlpha << 24 | aRed << 16 | aGreen << 8 | aBlue) {}
    unsigned char Red() const { return (m_ARGB >> 16) & 0xff; }
    unsigned char Green() const { return (m_ARGB >> 8) & 0xff; }
    unsigned char Blue() const { return m_ARGB & 0xff; }
    unsigned char Alpha() const { return (m_ARGB >> 24) & 0xff; }
    uint32_t ARGB() const { return m_ARGB; }
};

#endif // _WX_COLOUR_H_
