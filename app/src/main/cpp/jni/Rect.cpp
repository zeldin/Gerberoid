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
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_Rect;
  static jmethodID method_init;

  static JNIRef createRect(int left, int top, int right, int bottom);
};
Rect::Native Rect::Native::hook;

jclass Rect::Native::class_Rect = 0;
jmethodID Rect::Native::method_init = 0;

Rect::Rect(int left, int top, int right, int bottom) : JNIRef(Native::createRect(left, top, right, bottom)) {}

  JNIRef Rect::Native::createRect(int left, int top, int right, int bottom)
{
  LocalFrame env;
  if (!env) return JNIRef();
  return env->NewObject(class_Rect, method_init,
			static_cast<jint>(left), static_cast<jint>(top),
			static_cast<jint>(right), static_cast<jint>(bottom));
}

bool Rect::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/graphics/Rect");
  if (!loc_class)
    return false;
  class_Rect = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_Rect)
    return false;
  method_init = env->GetMethodID(class_Rect, "<init>", "(IIII)V");
  return method_init;
}

void Rect::Native::deinit(JNIEnv *env)
{
  method_init = 0;
  if (class_Rect) {
    env->DeleteGlobalRef(class_Rect);
    class_Rect = 0;
  }
}

}
