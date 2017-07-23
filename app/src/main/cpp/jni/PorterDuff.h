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

#ifndef PORTERDUFF_H__
#define PORTERDUFF_H__ 1

#include "jniref.h"
#include <cstdint>
#include "Xfermode.h"

namespace android
{

class PorterDuff
{
 public:
  class Mode {
    friend class PorterDuffXfermode;
  private:
    const jfieldID &field;
    Mode(const jfieldID& field) : field(field) {}
  public:
    static const Mode ADD;
    static const Mode CLEAR;
    static const Mode DARKEN;
    static const Mode DST;
    static const Mode DST_ATOP;
    static const Mode DST_IN;
    static const Mode DST_OUT;
    static const Mode DST_OVER;
    static const Mode LIGHTEN;
    static const Mode MULTIPLY;
    static const Mode OVERLAY;
    static const Mode SCREEN;
    static const Mode SRC;
    static const Mode SRC_ATOP;
    static const Mode SRC_IN;
    static const Mode SRC_OUT;
    static const Mode SRC_OVER;
    static const Mode XOR;
  };
};

class PorterDuffXfermode : public Xfermode
{
  friend class PorterDuff::Mode;

  using Xfermode::Xfermode;

 private:
  class Native;

 public:
  PorterDuffXfermode(const PorterDuff::Mode &mode);
};

}

#endif // PORTERDUFF_H__
