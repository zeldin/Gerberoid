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

#include <jni.h>
#include "PorterDuff.h"
#include "inithook.h"
#include "localframe.h"

namespace android
{

class PorterDuffXfermode::Native : InitHook
{
  friend class PorterDuff::Mode;
  friend class PorterDuffXfermode;
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_PorterDuffXfermode;
  static jclass class_PorterDuff_Mode;
  static jmethodID method_init;
  static jfieldID field_ADD;
  static jfieldID field_CLEAR;
  static jfieldID field_DARKEN;
  static jfieldID field_DST;
  static jfieldID field_DST_ATOP;
  static jfieldID field_DST_IN;
  static jfieldID field_DST_OUT;
  static jfieldID field_DST_OVER;
  static jfieldID field_LIGHTEN;
  static jfieldID field_MULTIPLY;
  static jfieldID field_OVERLAY;
  static jfieldID field_SCREEN;
  static jfieldID field_SRC;
  static jfieldID field_SRC_ATOP;
  static jfieldID field_SRC_IN;
  static jfieldID field_SRC_OUT;
  static jfieldID field_SRC_OVER;
  static jfieldID field_XOR;

  static JNIRef createPorterDuffXfermode(const PorterDuff::Mode &mode);
};
PorterDuffXfermode::Native PorterDuffXfermode::Native::hook;

jclass PorterDuffXfermode::Native::class_PorterDuffXfermode = 0;
jclass PorterDuffXfermode::Native::class_PorterDuff_Mode = 0;
jmethodID PorterDuffXfermode::Native::method_init = 0;
jfieldID PorterDuffXfermode::Native::field_ADD = 0;
jfieldID PorterDuffXfermode::Native::field_CLEAR = 0;
jfieldID PorterDuffXfermode::Native::field_DARKEN = 0;
jfieldID PorterDuffXfermode::Native::field_DST = 0;
jfieldID PorterDuffXfermode::Native::field_DST_ATOP = 0;
jfieldID PorterDuffXfermode::Native::field_DST_IN = 0;
jfieldID PorterDuffXfermode::Native::field_DST_OUT = 0;
jfieldID PorterDuffXfermode::Native::field_DST_OVER = 0;
jfieldID PorterDuffXfermode::Native::field_LIGHTEN = 0;
jfieldID PorterDuffXfermode::Native::field_MULTIPLY = 0;
jfieldID PorterDuffXfermode::Native::field_OVERLAY = 0;
jfieldID PorterDuffXfermode::Native::field_SCREEN = 0;
jfieldID PorterDuffXfermode::Native::field_SRC = 0;
jfieldID PorterDuffXfermode::Native::field_SRC_ATOP = 0;
jfieldID PorterDuffXfermode::Native::field_SRC_IN = 0;
jfieldID PorterDuffXfermode::Native::field_SRC_OUT = 0;
jfieldID PorterDuffXfermode::Native::field_SRC_OVER = 0;
jfieldID PorterDuffXfermode::Native::field_XOR = 0;

const PorterDuff::Mode PorterDuff::Mode::ADD(PorterDuffXfermode::Native::field_ADD);
const PorterDuff::Mode PorterDuff::Mode::CLEAR(PorterDuffXfermode::Native::field_CLEAR);
const PorterDuff::Mode PorterDuff::Mode::DARKEN(PorterDuffXfermode::Native::field_DARKEN);
const PorterDuff::Mode PorterDuff::Mode::DST(PorterDuffXfermode::Native::field_DST);
const PorterDuff::Mode PorterDuff::Mode::DST_ATOP(PorterDuffXfermode::Native::field_DST_ATOP);
const PorterDuff::Mode PorterDuff::Mode::DST_IN(PorterDuffXfermode::Native::field_DST_IN);
const PorterDuff::Mode PorterDuff::Mode::DST_OUT(PorterDuffXfermode::Native::field_DST_OUT);
const PorterDuff::Mode PorterDuff::Mode::DST_OVER(PorterDuffXfermode::Native::field_DST_OVER);
const PorterDuff::Mode PorterDuff::Mode::LIGHTEN(PorterDuffXfermode::Native::field_LIGHTEN);
const PorterDuff::Mode PorterDuff::Mode::MULTIPLY(PorterDuffXfermode::Native::field_MULTIPLY);
const PorterDuff::Mode PorterDuff::Mode::OVERLAY(PorterDuffXfermode::Native::field_OVERLAY);
const PorterDuff::Mode PorterDuff::Mode::SCREEN(PorterDuffXfermode::Native::field_SCREEN);
const PorterDuff::Mode PorterDuff::Mode::SRC(PorterDuffXfermode::Native::field_SRC);
const PorterDuff::Mode PorterDuff::Mode::SRC_ATOP(PorterDuffXfermode::Native::field_SRC_ATOP);
const PorterDuff::Mode PorterDuff::Mode::SRC_IN(PorterDuffXfermode::Native::field_SRC_IN);
const PorterDuff::Mode PorterDuff::Mode::SRC_OUT(PorterDuffXfermode::Native::field_SRC_OUT);
const PorterDuff::Mode PorterDuff::Mode::SRC_OVER(PorterDuffXfermode::Native::field_SRC_OVER);
const PorterDuff::Mode PorterDuff::Mode::XOR(PorterDuffXfermode::Native::field_XOR);

PorterDuffXfermode::PorterDuffXfermode(const PorterDuff::Mode &mode) : Xfermode(Native::createPorterDuffXfermode(mode)) {}

JNIRef PorterDuffXfermode::Native::createPorterDuffXfermode(const PorterDuff::Mode &mode)
{
  LocalFrame env;
  if (!env) return JNIRef();
  return env->NewObject(class_PorterDuffXfermode, method_init,
			env->GetStaticObjectField(Native::class_PorterDuff_Mode, mode.field));
}

bool PorterDuffXfermode::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/graphics/PorterDuffXfermode");
  if (!loc_class)
    return false;
  class_PorterDuffXfermode = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  loc_class = env->FindClass("android/graphics/PorterDuff$Mode");
  if (!loc_class)
    return false;
  class_PorterDuff_Mode = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_PorterDuffXfermode || !class_PorterDuff_Mode)
    return false;
  method_init = env->GetMethodID(class_PorterDuffXfermode, "<init>", "(Landroid/graphics/PorterDuff$Mode;)V");
  field_ADD = env->GetStaticFieldID(class_PorterDuff_Mode, "ADD", "Landroid/graphics/PorterDuff$Mode;");
  field_CLEAR = env->GetStaticFieldID(class_PorterDuff_Mode, "CLEAR", "Landroid/graphics/PorterDuff$Mode;");
  field_DARKEN = env->GetStaticFieldID(class_PorterDuff_Mode, "DARKEN", "Landroid/graphics/PorterDuff$Mode;");
  field_DST = env->GetStaticFieldID(class_PorterDuff_Mode, "DST", "Landroid/graphics/PorterDuff$Mode;");
  field_DST_ATOP = env->GetStaticFieldID(class_PorterDuff_Mode, "DST_ATOP", "Landroid/graphics/PorterDuff$Mode;");
  field_DST_IN = env->GetStaticFieldID(class_PorterDuff_Mode, "DST_IN", "Landroid/graphics/PorterDuff$Mode;");
  field_DST_OUT = env->GetStaticFieldID(class_PorterDuff_Mode, "DST_OUT", "Landroid/graphics/PorterDuff$Mode;");
  field_DST_OVER = env->GetStaticFieldID(class_PorterDuff_Mode, "DST_OVER", "Landroid/graphics/PorterDuff$Mode;");
  field_LIGHTEN = env->GetStaticFieldID(class_PorterDuff_Mode, "LIGHTEN", "Landroid/graphics/PorterDuff$Mode;");
  field_MULTIPLY = env->GetStaticFieldID(class_PorterDuff_Mode, "MULTIPLY", "Landroid/graphics/PorterDuff$Mode;");
  field_OVERLAY = env->GetStaticFieldID(class_PorterDuff_Mode, "OVERLAY", "Landroid/graphics/PorterDuff$Mode;");
  field_SCREEN = env->GetStaticFieldID(class_PorterDuff_Mode, "SCREEN", "Landroid/graphics/PorterDuff$Mode;");
  field_SRC = env->GetStaticFieldID(class_PorterDuff_Mode, "SRC", "Landroid/graphics/PorterDuff$Mode;");
  field_SRC_ATOP = env->GetStaticFieldID(class_PorterDuff_Mode, "SRC_ATOP", "Landroid/graphics/PorterDuff$Mode;");
  field_SRC_IN = env->GetStaticFieldID(class_PorterDuff_Mode, "SRC_IN", "Landroid/graphics/PorterDuff$Mode;");
  field_SRC_OUT = env->GetStaticFieldID(class_PorterDuff_Mode, "SRC_OUT", "Landroid/graphics/PorterDuff$Mode;");
  field_SRC_OVER = env->GetStaticFieldID(class_PorterDuff_Mode, "SRC_OVER", "Landroid/graphics/PorterDuff$Mode;");
  field_XOR = env->GetStaticFieldID(class_PorterDuff_Mode, "XOR", "Landroid/graphics/PorterDuff$Mode;");
  return method_init && field_ADD && field_CLEAR && field_DARKEN && field_DST
    && field_DST_ATOP && field_DST_IN && field_DST_OUT && field_DST_OVER
    && field_LIGHTEN && field_MULTIPLY && field_OVERLAY && field_SCREEN
    && field_SRC && field_SRC_ATOP && field_SRC_IN && field_SRC_OUT
    && field_SRC_OVER && field_XOR;
}

void PorterDuffXfermode::Native::deinit(JNIEnv *env)
{
  method_init = 0;
  field_ADD = 0;
  field_CLEAR = 0;
  field_DARKEN = 0;
  field_DST = 0;
  field_DST_ATOP = 0;
  field_DST_IN = 0;
  field_DST_OUT = 0;
  field_DST_OVER = 0;
  field_LIGHTEN = 0;
  field_MULTIPLY = 0;
  field_OVERLAY = 0;
  field_SCREEN = 0;
  field_SRC = 0;
  field_SRC_ATOP = 0;
  field_SRC_IN = 0;
  field_SRC_OUT = 0;
  field_SRC_OVER = 0;
  field_XOR = 0;
  if (class_PorterDuff_Mode) {
    env->DeleteGlobalRef(class_PorterDuff_Mode);
    class_PorterDuff_Mode = 0;
  }
  if (class_PorterDuffXfermode) {
    env->DeleteGlobalRef(class_PorterDuffXfermode);
    class_PorterDuffXfermode = 0;
  }
}

}
