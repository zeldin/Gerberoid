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
#include "ColorFilter.h"
#include "inithook.h"
#include "localframe.h"

namespace android
{

class ColorMatrixColorFilter::Native : InitHook
{
  friend class ColorMatrixColorFilter;
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_ColorMatrixColorFilter;
  static jmethodID method_init;

  static JNIRef createColorMatrixColorFilter(const float (&array)[20]);
};
ColorMatrixColorFilter::Native ColorMatrixColorFilter::Native::hook;

jclass ColorMatrixColorFilter::Native::class_ColorMatrixColorFilter = 0;
jmethodID ColorMatrixColorFilter::Native::method_init = 0;

ColorMatrixColorFilter::ColorMatrixColorFilter(const float (&array)[20]) : ColorFilter(Native::createColorMatrixColorFilter(array)) {}

JNIRef ColorMatrixColorFilter::Native::createColorMatrixColorFilter(const float (&array)[20])
{
  LocalFrame env;
  if (!env) return JNIRef();
  jfloatArray array_arr = env->NewFloatArray(20);
  env->SetFloatArrayRegion(array_arr, 0, 20, &array[0]);
  return env->NewObject(class_ColorMatrixColorFilter, method_init, array_arr);
}

bool ColorMatrixColorFilter::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/graphics/ColorMatrixColorFilter");
  if (!loc_class)
    return false;
  class_ColorMatrixColorFilter = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_ColorMatrixColorFilter)
    return false;
  method_init = env->GetMethodID(class_ColorMatrixColorFilter, "<init>", "([F)V");
  return method_init;
}

void ColorMatrixColorFilter::Native::deinit(JNIEnv *env)
{
  method_init = 0;
  if (class_ColorMatrixColorFilter) {
    env->DeleteGlobalRef(class_ColorMatrixColorFilter);
    class_ColorMatrixColorFilter = 0;
  }
}

}
