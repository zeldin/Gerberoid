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
#include "Path.h"
#include "inithook.h"
#include "localframe.h"

namespace android
{

class Path::Native : InitHook
{
  friend class Path;
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_Path;
  static jmethodID method_init;
  static jmethodID method_moveTo;
  static jmethodID method_lineTo;
  static jmethodID method_close;

  static JNIRef createPath();
};
Path::Native Path::Native::hook;

jclass Path::Native::class_Path = 0;
jmethodID Path::Native::method_init = 0;
jmethodID Path::Native::method_moveTo = 0;
jmethodID Path::Native::method_lineTo = 0;
jmethodID Path::Native::method_close = 0;

Path::Path() : JNIRef(Native::createPath()) {}

void Path::moveTo(float x, float y)
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_moveTo,
		      static_cast<jfloat>(x), static_cast<jfloat>(y));
}

void Path::lineTo(float x, float y)
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_lineTo,
		      static_cast<jfloat>(x), static_cast<jfloat>(y));
}

void Path::close()
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_close);
}

JNIRef Path::Native::createPath()
{
  LocalFrame env;
  if (!env) return JNIRef();
  return env->NewObject(class_Path, method_init);
}

bool Path::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/graphics/Path");
  if (!loc_class)
    return false;
  class_Path = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_Path)
    return false;
  method_init = env->GetMethodID(class_Path, "<init>", "()V");
  method_moveTo = env->GetMethodID(class_Path, "moveTo", "(FF)V");
  method_lineTo = env->GetMethodID(class_Path, "lineTo", "(FF)V");
  method_close = env->GetMethodID(class_Path, "close", "()V");
  return method_init && method_moveTo && method_lineTo && method_close;
}

void Path::Native::deinit(JNIEnv *env)
{
  method_init = 0;
  method_moveTo = 0;
  method_lineTo = 0;
  method_close = 0;
  if (class_Path) {
    env->DeleteGlobalRef(class_Path);
    class_Path = 0;
  }
}

}
