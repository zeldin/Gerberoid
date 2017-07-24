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
#include "PathEffect.h"
#include "inithook.h"
#include "localframe.h"

namespace android
{

class DashPathEffect::Native : InitHook
{
  friend class DashPathEffect;
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_DashPathEffect;
  static jmethodID method_init;

  static JNIRef createDashPathEffect(const std::vector<float>& intervals, float phase);
};
DashPathEffect::Native DashPathEffect::Native::hook;

jclass DashPathEffect::Native::class_DashPathEffect = 0;
jmethodID DashPathEffect::Native::method_init = 0;

DashPathEffect::DashPathEffect(const std::vector<float>& intervals, float phase) : PathEffect(Native::createDashPathEffect(intervals, phase)) {}

JNIRef DashPathEffect::Native::createDashPathEffect(const std::vector<float>& intervals, float phase)
{
  LocalFrame env;
  if (!env) return JNIRef();
  jfloatArray intervals_arr = env->NewFloatArray(intervals.size());
  env->SetFloatArrayRegion(intervals_arr, 0, intervals.size(), intervals.data());
  return env->NewObject(class_DashPathEffect, method_init,
			intervals_arr, static_cast<jfloat>(phase));
}

bool DashPathEffect::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/graphics/DashPathEffect");
  if (!loc_class)
    return false;
  class_DashPathEffect = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_DashPathEffect)
    return false;
  method_init = env->GetMethodID(class_DashPathEffect, "<init>", "([FF)V");
  return method_init;
}

void DashPathEffect::Native::deinit(JNIEnv *env)
{
  method_init = 0;
  if (class_DashPathEffect) {
    env->DeleteGlobalRef(class_DashPathEffect);
    class_DashPathEffect = 0;
  }
}

}
