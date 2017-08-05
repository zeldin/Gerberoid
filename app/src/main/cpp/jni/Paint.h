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

class Xfermode;
class PathEffect;
class ColorFilter;

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

  class Cap {
    friend class Paint;
  private:
    const jfieldID &field;
    Cap(const jfieldID& field) : field(field) {}
  public:
    static const Cap BUTT;
    static const Cap ROUND;
    static const Cap SQUARE;
  };

  class Join {
    friend class Paint;
  private:
    const jfieldID &field;
    Join(const jfieldID& field) : field(field) {}
  public:
    static const Join BEVEL;
    static const Join MITER;
    static const Join ROUND;
  };

  Paint();
  void setColor(uint32_t color);
  void setStrokeWidth(float width);
  void setStyle(const Style &style);
  void setStrokeCap(const Cap &style);
  void setStrokeJoin(const Join &style);
  const Xfermode &setXfermode(const Xfermode &xfermode);
  const PathEffect &setPathEffect(const PathEffect &effect);
  const ColorFilter &setColorFilter(const ColorFilter &filter);
};

}

#endif // PAINT_H__
