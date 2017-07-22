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
#include "Bitmap.h"
#include "inithook.h"
#include "localframe.h"

namespace android
{

class Bitmap::Native : InitHook
{
  friend class Bitmap;
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_Bitmap;
  static jclass class_Bitmap_Config;
  static jmethodID method_createBitmap;
  static jfieldID field_ALPHA_8;
  static jfieldID field_ARGB_4444;
  static jfieldID field_ARGB_8888;
  static jfieldID field_RGB_565;
};
Bitmap::Native Bitmap::Native::hook;

jclass Bitmap::Native::class_Bitmap = 0;
jclass Bitmap::Native::class_Bitmap_Config = 0;
jmethodID Bitmap::Native::method_createBitmap = 0;
jfieldID Bitmap::Native::field_ALPHA_8 = 0;
jfieldID Bitmap::Native::field_ARGB_4444 = 0;
jfieldID Bitmap::Native::field_ARGB_8888 = 0;
jfieldID Bitmap::Native::field_RGB_565 = 0;

const Bitmap::Config Bitmap::Config::ALPHA_8(Native::field_ALPHA_8);
const Bitmap::Config Bitmap::Config::ARGB_4444(Native::field_ARGB_4444);
const Bitmap::Config Bitmap::Config::ARGB_8888(Native::field_ARGB_8888);
const Bitmap::Config Bitmap::Config::RGB_565(Native::field_RGB_565);

Bitmap Bitmap::createBitmap(int width, int height, const Config& config)
{
  LocalFrame env;
  if (!env) return 0;
  return env->CallStaticObjectMethod(Native::class_Bitmap, Native::method_createBitmap,
				     static_cast<jint>(width),
				     static_cast<jint>(height),
				     env->GetStaticObjectField(Native::class_Bitmap_Config, config.field));
}

bool Bitmap::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/graphics/Bitmap");
  if (!loc_class)
    return false;
  class_Bitmap = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  loc_class = env->FindClass("android/graphics/Bitmap$Config");
  if (!loc_class)
    return false;
  class_Bitmap_Config = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_Bitmap || !class_Bitmap_Config)
    return false;
  method_createBitmap = env->GetStaticMethodID(class_Bitmap, "createBitmap", "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
  field_ALPHA_8 = env->GetStaticFieldID(class_Bitmap_Config, "ALPHA_8", "Landroid/graphics/Bitmap$Config;");
  field_ARGB_4444 = env->GetStaticFieldID(class_Bitmap_Config, "ARGB_4444", "Landroid/graphics/Bitmap$Config;");
  field_ARGB_8888 = env->GetStaticFieldID(class_Bitmap_Config, "ARGB_8888", "Landroid/graphics/Bitmap$Config;");
  field_RGB_565 = env->GetStaticFieldID(class_Bitmap_Config, "RGB_565", "Landroid/graphics/Bitmap$Config;");
  return method_createBitmap && field_ALPHA_8 && field_ARGB_4444 && field_ARGB_8888 && field_RGB_565;
}

void Bitmap::Native::deinit(JNIEnv *env)
{
  method_createBitmap = 0;
  field_ALPHA_8 = 0;
  field_ARGB_4444 = 0;
  field_ARGB_8888 = 0;
  field_RGB_565 = 0;
  if (class_Bitmap_Config) {
    env->DeleteGlobalRef(class_Bitmap_Config);
    class_Bitmap_Config = 0;
  }
  if (class_Bitmap) {
    env->DeleteGlobalRef(class_Bitmap);
    class_Bitmap = 0;
  }
}

}
