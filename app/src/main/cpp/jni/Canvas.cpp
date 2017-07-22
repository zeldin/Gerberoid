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
#include "Canvas.h"
#include "Bitmap.h"
#include "inithook.h"
#include "localframe.h"

namespace android
{

class Canvas::Native : InitHook
{
  friend class Canvas;
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_Canvas;
  static jmethodID method_init;
  static jmethodID method_setBitmap;

  static JNIRef createCanvas();
};
Canvas::Native Canvas::Native::hook;

jclass Canvas::Native::class_Canvas = 0;
jmethodID Canvas::Native::method_init = 0;
jmethodID Canvas::Native::method_setBitmap = 0;

Canvas::Canvas() : JNIRef(Native::createCanvas()) {}

void Canvas::setBitmap(Bitmap &bitmap)
{
  LocalFrame env;
  if (!env || !get() || !bitmap) return;
  env->CallVoidMethod(*this, Native::method_setBitmap,
		      static_cast<jobject>(bitmap));
}

JNIRef Canvas::Native::createCanvas()
{
  LocalFrame env;
  if (!env) return JNIRef();
  return env->NewObject(class_Canvas, method_init);
}

bool Canvas::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/graphics/Canvas");
  if (!loc_class)
    return false;
  class_Canvas = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_Canvas)
    return false;
  method_init = env->GetMethodID(class_Canvas, "<init>", "()V");
  method_setBitmap = env->GetMethodID(class_Canvas, "setBitmap", "(Landroid/graphics/Bitmap;)V");
  return method_init && method_setBitmap;
}

void Canvas::Native::deinit(JNIEnv *env)
{
  method_init = 0;
  method_setBitmap = 0;
  if (class_Canvas) {
    env->DeleteGlobalRef(class_Canvas);
    class_Canvas = 0;
  }
}

}
