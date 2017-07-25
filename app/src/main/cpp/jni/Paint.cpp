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
#include "Xfermode.h"
#include "PathEffect.h"
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
  static jclass class_Paint_Cap;
  static jclass class_Paint_Join;
  static jmethodID method_init;
  static jmethodID method_setColor;
  static jmethodID method_setStrokeWidth;
  static jmethodID method_setStyle;
  static jmethodID method_setStrokeCap;
  static jmethodID method_setStrokeJoin;
  static jmethodID method_setXfermode;
  static jmethodID method_setPathEffect;
  static jfieldID field_FILL;
  static jfieldID field_FILL_AND_STROKE;
  static jfieldID field_STROKE;
  static jfieldID field_cap_BUTT;
  static jfieldID field_cap_ROUND;
  static jfieldID field_cap_SQUARE;
  static jfieldID field_join_BEVEL;
  static jfieldID field_join_MITER;
  static jfieldID field_join_ROUND;

  static JNIRef createPaint();
};
Paint::Native Paint::Native::hook;

jclass Paint::Native::class_Paint = 0;
jclass Paint::Native::class_Paint_Style = 0;
jclass Paint::Native::class_Paint_Cap = 0;
jclass Paint::Native::class_Paint_Join = 0;
jmethodID Paint::Native::method_init = 0;
jmethodID Paint::Native::method_setColor = 0;
jmethodID Paint::Native::method_setStrokeWidth = 0;
jmethodID Paint::Native::method_setStyle = 0;
jmethodID Paint::Native::method_setStrokeCap = 0;
jmethodID Paint::Native::method_setStrokeJoin = 0;
jmethodID Paint::Native::method_setXfermode = 0;
jmethodID Paint::Native::method_setPathEffect = 0;
jfieldID Paint::Native::field_FILL = 0;
jfieldID Paint::Native::field_FILL_AND_STROKE = 0;
jfieldID Paint::Native::field_STROKE = 0;
jfieldID Paint::Native::field_cap_BUTT = 0;
jfieldID Paint::Native::field_cap_ROUND = 0;
jfieldID Paint::Native::field_cap_SQUARE = 0;
jfieldID Paint::Native::field_join_BEVEL = 0;
jfieldID Paint::Native::field_join_MITER = 0;
jfieldID Paint::Native::field_join_ROUND = 0;

const Paint::Style Paint::Style::FILL(Native::field_FILL);
const Paint::Style Paint::Style::FILL_AND_STROKE(Native::field_FILL_AND_STROKE);
const Paint::Style Paint::Style::STROKE(Native::field_STROKE);

const Paint::Cap Paint::Cap::BUTT(Native::field_cap_BUTT);
const Paint::Cap Paint::Cap::ROUND(Native::field_cap_ROUND);
const Paint::Cap Paint::Cap::SQUARE(Native::field_cap_SQUARE);

const Paint::Join Paint::Join::BEVEL(Native::field_join_BEVEL);
const Paint::Join Paint::Join::MITER(Native::field_join_MITER);
const Paint::Join Paint::Join::ROUND(Native::field_join_ROUND);

Paint::Paint() : JNIRef(Native::createPaint()) {}

void Paint::setColor(uint32_t color)
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_setColor,
		      static_cast<jint>(color));
}

void Paint::setStrokeWidth(float width)
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_setStrokeWidth,
		      static_cast<jfloat>(width));
}

void Paint::setStyle(const Style &style)
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_setStyle,
		      env->GetStaticObjectField(Native::class_Paint_Style, style.field));
}

void Paint::setStrokeCap(const Cap &cap)
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_setStrokeCap,
		      env->GetStaticObjectField(Native::class_Paint_Cap, cap.field));
}

void Paint::setStrokeJoin(const Join &join)
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_setStrokeJoin,
		      env->GetStaticObjectField(Native::class_Paint_Join, join.field));
}

const Xfermode &Paint::setXfermode(const Xfermode &xfermode)
{
  LocalFrame env;
  if (!env || !get()) return xfermode;
  env->CallObjectMethod(*this, Native::method_setXfermode,
			static_cast<jobject>(xfermode));
  return xfermode;
}

const PathEffect &Paint::setPathEffect(const PathEffect &effect)
{
  LocalFrame env;
  if (!env || !get()) return effect;
  env->CallObjectMethod(*this, Native::method_setPathEffect,
			static_cast<jobject>(effect));
  return effect;
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
  loc_class = env->FindClass("android/graphics/Paint$Cap");
  if (!loc_class)
    return false;
  class_Paint_Cap = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  loc_class = env->FindClass("android/graphics/Paint$Join");
  if (!loc_class)
    return false;
  class_Paint_Join = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_Paint || !class_Paint_Style || !class_Paint_Cap || !class_Paint_Join)
    return false;
  method_init = env->GetMethodID(class_Paint, "<init>", "()V");
  method_setColor = env->GetMethodID(class_Paint, "setColor", "(I)V");
  method_setStrokeWidth = env->GetMethodID(class_Paint, "setStrokeWidth", "(F)V");
  method_setStyle = env->GetMethodID(class_Paint, "setStyle", "(Landroid/graphics/Paint$Style;)V");
  method_setStrokeCap = env->GetMethodID(class_Paint, "setStrokeCap", "(Landroid/graphics/Paint$Cap;)V");
  method_setStrokeJoin = env->GetMethodID(class_Paint, "setStrokeJoin", "(Landroid/graphics/Paint$Join;)V");
  method_setXfermode = env->GetMethodID(class_Paint, "setXfermode", "(Landroid/graphics/Xfermode;)Landroid/graphics/Xfermode;");
  method_setPathEffect = env->GetMethodID(class_Paint, "setPathEffect", "(Landroid/graphics/PathEffect;)Landroid/graphics/PathEffect;");
  field_FILL = env->GetStaticFieldID(class_Paint_Style, "FILL", "Landroid/graphics/Paint$Style;");
  field_FILL_AND_STROKE = env->GetStaticFieldID(class_Paint_Style, "FILL_AND_STROKE", "Landroid/graphics/Paint$Style;");
  field_STROKE = env->GetStaticFieldID(class_Paint_Style, "STROKE", "Landroid/graphics/Paint$Style;");
  field_cap_BUTT = env->GetStaticFieldID(class_Paint_Cap, "BUTT", "Landroid/graphics/Paint$Cap;");
  field_cap_ROUND = env->GetStaticFieldID(class_Paint_Cap, "ROUND", "Landroid/graphics/Paint$Cap;");
  field_cap_SQUARE = env->GetStaticFieldID(class_Paint_Cap, "SQUARE", "Landroid/graphics/Paint$Cap;");
  field_join_BEVEL = env->GetStaticFieldID(class_Paint_Join, "BEVEL", "Landroid/graphics/Paint$Join;");
  field_join_MITER = env->GetStaticFieldID(class_Paint_Join, "MITER", "Landroid/graphics/Paint$Join;");
  field_join_ROUND = env->GetStaticFieldID(class_Paint_Join, "ROUND", "Landroid/graphics/Paint$Join;");
  return method_init && method_setColor && method_setStrokeWidth &&
    method_setStyle && method_setStrokeCap && method_setStrokeJoin &&
    method_setXfermode && method_setPathEffect &&
    field_FILL && field_FILL_AND_STROKE && field_STROKE &&
    field_cap_BUTT && field_cap_ROUND && field_cap_SQUARE &&
    field_join_BEVEL && field_join_MITER && field_join_ROUND;
}

void Paint::Native::deinit(JNIEnv *env)
{
  method_init = 0;
  method_setColor = 0;
  method_setStrokeWidth = 0;
  method_setStyle = 0;
  method_setStrokeCap = 0;
  method_setStrokeJoin = 0;
  method_setXfermode = 0;
  method_setPathEffect = 0;
  field_FILL = 0;
  field_FILL_AND_STROKE = 0;
  field_STROKE = 0;
  if (class_Paint_Join) {
    env->DeleteGlobalRef(class_Paint_Join);
    class_Paint_Join = 0;
  }
  if (class_Paint_Cap) {
    env->DeleteGlobalRef(class_Paint_Cap);
    class_Paint_Cap = 0;
  }
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
