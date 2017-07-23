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

#ifndef PAINT_H__
#define PAINT_H__ 1

#include "jniref.h"
#include <cstdint>

namespace android
{

class Paint : public JNIRef
{
  using JNIRef::JNIRef;

 private:
  class Native;

 public:
  class Style {
    friend class Paint;
  private:
    const jfieldID &field;
    Style(const jfieldID& field) : field(field) {}
  public:
    static const Style FILL;
    static const Style FILL_AND_STROKE;
    static const Style STROKE;
  };

  Paint();
  void setColor(uint32_t color);
  void setStyle(const Style &style);
};

}

#endif // PAINT_H__
