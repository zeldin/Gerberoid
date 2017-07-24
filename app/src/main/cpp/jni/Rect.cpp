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
#include "Rect.h"
#include "inithook.h"
#include "localframe.h"

namespace android
{

class Rect::Native : InitHook
{
  friend class Rect;
  friend class RectF;
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_Rect;
  static jclass class_RectF;
  static jmethodID method_init;
  static jmethodID method_initF;

  static JNIRef createRect(int left, int top, int right, int bottom);
  static JNIRef createRectF(float left, float top, float right, float bottom);
};
Rect::Native Rect::Native::hook;

jclass Rect::Native::class_Rect = 0;
jclass Rect::Native::class_RectF = 0;
jmethodID Rect::Native::method_init = 0;
jmethodID Rect::Native::method_initF = 0;

Rect::Rect(int left, int top, int right, int bottom) : JNIRef(Native::createRect(left, top, right, bottom)) {}

JNIRef Rect::Native::createRect(int left, int top, int right, int bottom)
{
  LocalFrame env;
  if (!env) return JNIRef();
  return env->NewObject(class_Rect, method_init,
			static_cast<jint>(left), static_cast<jint>(top),
			static_cast<jint>(right), static_cast<jint>(bottom));
}

RectF::RectF(float left, float top, float right, float bottom) : JNIRef(Rect::Native::createRectF(left, top, right, bottom)) {}

JNIRef Rect::Native::createRectF(float left, float top, float right, float bottom)
{
  LocalFrame env;
  if (!env) return JNIRef();
  return env->NewObject(class_RectF, method_initF,
			static_cast<jfloat>(left), static_cast<jfloat>(top),
			static_cast<jfloat>(right), static_cast<jfloat>(bottom));
}

bool Rect::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/graphics/Rect");
  if (!loc_class)
    return false;
  class_Rect = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  loc_class = env->FindClass("android/graphics/RectF");
  if (!loc_class)
    return false;
  class_RectF = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_Rect || !class_RectF)
    return false;
  method_init = env->GetMethodID(class_Rect, "<init>", "(IIII)V");
  method_initF = env->GetMethodID(class_RectF, "<init>", "(FFFF)V");
  return method_init && method_initF;
}

void Rect::Native::deinit(JNIEnv *env)
{
  method_init = 0;
  method_initF = 0;
  if (class_Rect) {
    env->DeleteGlobalRef(class_Rect);
    class_Rect = 0;
  }
  if (class_RectF) {
    env->DeleteGlobalRef(class_RectF);
    class_RectF = 0;
  }
}

}
