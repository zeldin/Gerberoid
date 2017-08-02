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

#ifndef CANVAS_H__
#define CANVAS_H__ 1

#include "jniref.h"

namespace android
{

class Bitmap;
class Path;
class Paint;
class Rect;
class RectF;

class Canvas : public JNIRef
{
  using JNIRef::JNIRef;

 private:
  class Native;

 public:
  Canvas();
  void setBitmap(Bitmap &bitmap);
  int save();
  void restore();
  void scale(float sx, float sy);
  void translate(float dx, float dy);
  bool clipRect(int left, int top, int right, int bottom);
  void drawCircle(float cx, float cy, float radius, const Paint& paint);
  void drawOval(const RectF &rect, const Paint& paint);
  void drawArc(const RectF &rect, float startAngle, float sweepAngle, bool useCenter, const Paint& paint);
  void drawBitmap(const Bitmap &bitmap, const Rect &src, const Rect &dst, const Paint &paint);
  void drawLine(float startX, float startY, float stopX, float stopY, const Paint &paint);
  void drawPath(const Path &path, const Paint &paint);
  void drawRect(float left, float top, float right, float bottom, const Paint &paint);
  void drawPoint(float x, float y, const Paint &paint);
};

}

#endif // CANVAS_H__
