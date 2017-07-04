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
#include "Html.h"
#include "inithook.h"
#include "localframe.h"

namespace android
{

class Html::Native : InitHook
{
  friend class Html;
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_Html;
  static jmethodID method_fromHtml;
};
Html::Native Html::Native::hook;

jclass Html::Native::class_Html = 0;
jmethodID Html::Native::method_fromHtml = 0;

Spanned Html::fromHtml(const char *source)
{
  LocalFrame env;
  if (!env || !source) return Spanned();
  return env->CallStaticObjectMethod(Native::class_Html, Native::method_fromHtml,
				     env->NewStringUTF(source));
}

bool Html::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/text/Html");
  if (!loc_class)
    return false;
  class_Html = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_Html)
    return false;
  method_fromHtml = env->GetStaticMethodID(class_Html, "fromHtml", "(Ljava/lang/String;)Landroid/text/Spanned;");
  return method_fromHtml;
}

void Html::Native::deinit(JNIEnv *env)
{
  method_fromHtml = 0;
  if (class_Html) {
    env->DeleteGlobalRef(class_Html);
    class_Html = 0;
  }
}

}
