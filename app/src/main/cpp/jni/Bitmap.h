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

#ifndef BITMAP_H__
#define BITMAP_H__ 1

#include "jniref.h"
#include <Context.h>

namespace android
{

class Bitmap : public JNIRef
{
  using JNIRef::JNIRef;

 private:
  class Native;

 public:
  class Config {
    friend class Bitmap;
  private:
    const jfieldID& field;
    Config(const jfieldID& field) : field(field) {}
  public:
    static const Config ALPHA_8;
    static const Config ARGB_4444;
    static const Config ARGB_8888;
    static const Config RGB_565;
  };

  static Bitmap createBitmap(int width, int height, const Config& config);
};

}

#endif // BITMAP_H__
