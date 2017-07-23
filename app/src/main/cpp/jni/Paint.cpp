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
#include "Paint.h"
#include "inithook.h"
#include "localframe.h"

namespace android
{

class Paint::Native : InitHook
{
  friend class Paint;
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_Paint;
  static jclass class_Paint_Style;
  static jmethodID method_init;
  static jmethodID method_setColor;
  static jmethodID method_setStyle;
  static jfieldID field_FILL;
  static jfieldID field_FILL_AND_STROKE;
  static jfieldID field_STROKE;

  static JNIRef createPaint();
};
Paint::Native Paint::Native::hook;

jclass Paint::Native::class_Paint = 0;
jclass Paint::Native::class_Paint_Style = 0;
jmethodID Paint::Native::method_init = 0;
jmethodID Paint::Native::method_setColor = 0;
jmethodID Paint::Native::method_setStyle = 0;
jfieldID Paint::Native::field_FILL = 0;
jfieldID Paint::Native::field_FILL_AND_STROKE = 0;
jfieldID Paint::Native::field_STROKE = 0;

const Paint::Style Paint::Style::FILL(Native::field_FILL);
const Paint::Style Paint::Style::FILL_AND_STROKE(Native::field_FILL_AND_STROKE);
const Paint::Style Paint::Style::STROKE(Native::field_STROKE);

Paint::Paint() : JNIRef(Native::createPaint()) {}

void Paint::setColor(uint32_t color)
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_setColor,
		      static_cast<jint>(color));
}

void Paint::setStyle(const Style &style)
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_setStyle,
		      env->GetStaticObjectField(Native::class_Paint_Style, style.field));
}

JNIRef Paint::Native::createPaint()
{
  LocalFrame env;
  if (!env) return JNIRef();
  return env->NewObject(class_Paint, method_init);
}

bool Paint::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/graphics/Paint");
  if (!loc_class)
    return false;
  class_Paint = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  loc_class = env->FindClass("android/graphics/Paint$Style");
  if (!loc_class)
    return false;
  class_Paint_Style = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_Paint || !class_Paint_Style)
    return false;
  method_init = env->GetMethodID(class_Paint, "<init>", "()V");
  method_setColor = env->GetMethodID(class_Paint, "setColor", "(I)V");
  method_setStyle = env->GetMethodID(class_Paint, "setStyle", "(Landroid/graphics/Paint$Style;)V");
  field_FILL = env->GetStaticFieldID(class_Paint_Style, "FILL", "Landroid/graphics/Paint$Style;");
  field_FILL_AND_STROKE = env->GetStaticFieldID(class_Paint_Style, "FILL_AND_STROKE", "Landroid/graphics/Paint$Style;");
  field_STROKE = env->GetStaticFieldID(class_Paint_Style, "STROKE", "Landroid/graphics/Paint$Style;");
  return method_init && method_setColor && method_setStyle && field_FILL &&
    field_FILL_AND_STROKE && field_STROKE;
}

void Paint::Native::deinit(JNIEnv *env)
{
  method_init = 0;
  method_setColor = 0;
  method_setStyle = 0;
  field_FILL = 0;
  field_FILL_AND_STROKE = 0;
  field_STROKE = 0;
  if (class_Paint_Style) {
    env->DeleteGlobalRef(class_Paint_Style);
    class_Paint_Style = 0;
  }
  if (class_Paint) {
    env->DeleteGlobalRef(class_Paint);
    class_Paint = 0;
  }
}

}
