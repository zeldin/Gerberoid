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
#include "Toast.h"
#include "Context.h"
#include "inithook.h"
#include "localframe.h"

namespace android
{

class Toast::Native : InitHook
{
  friend class Toast;
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_Toast;
  static jmethodID method_show;
  static jmethodID method_makeText;
};
Toast::Native Toast::Native::hook;

jclass Toast::Native::class_Toast = 0;
jmethodID Toast::Native::method_show = 0;
jmethodID Toast::Native::method_makeText = 0;

const int Toast::LENGTH_SHORT = 0;
const int Toast::LENGTH_LONG = 1;

void Toast::show()
{
  LocalFrame env;
  if (!env || !get()) return;
  env->CallVoidMethod(*this, Native::method_show);
}

Toast Toast::makeText(const Context& context, const char *text, int duration)
{
  LocalFrame env;
  if (!env || !text) return 0;
  return env->CallStaticObjectMethod(Native::class_Toast, Native::method_makeText,
				     static_cast<jobject>(context),
				     env->NewStringUTF(text),
				     static_cast<jint>(duration));
}

bool Toast::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/widget/Toast");
  if (!loc_class)
    return false;
  class_Toast = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_Toast)
    return false;
  method_show = env->GetMethodID(class_Toast, "show", "()V");
  method_makeText = env->GetStaticMethodID(class_Toast, "makeText", "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
  return method_show && method_makeText;
}

void Toast::Native::deinit(JNIEnv *env)
{
  method_show = 0;
  method_makeText = 0;
  if (class_Toast) {
    env->DeleteGlobalRef(class_Toast);
    class_Toast = 0;
  }
}

}
