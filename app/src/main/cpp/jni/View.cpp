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
#include "View.h"
#include "inithook.h"
#include "localframe.h"

namespace android
{

class View::Native : InitHook
{
  friend class View;
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_View;
  static jmethodID method_getContext;
};
View::Native View::Native::hook;

jclass View::Native::class_View = 0;
jmethodID View::Native::method_getContext = 0;

Context View::getContext()
{
  LocalFrame env;
  if (!env || !get()) return Context();
  return env->CallObjectMethod(*this, Native::method_getContext);
}

bool View::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/view/View");
  if (!loc_class)
    return false;
  class_View = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_View)
    return false;
  method_getContext = env->GetMethodID(class_View, "getContext", "()Landroid/content/Context;");
  return method_getContext;
}

void View::Native::deinit(JNIEnv *env)
{
  method_getContext = 0;
  if (class_View) {
    env->DeleteGlobalRef(class_View);
    class_View = 0;
  }
}

}
