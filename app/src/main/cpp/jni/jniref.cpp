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
#include "jniref.h"
#include "inithook.h"

JNIRef::JNIRef(jobject aRef)
{
  JNIEnv *env;
  JavaVM *vm = InitHook::GetVM();
  if (!vm || !aRef ||
      vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
    ref = 0;
  else
    ref = env->NewGlobalRef(aRef);
}

JNIRef::~JNIRef()
{
  if (ref) {
    JNIEnv *env;
    JavaVM *vm = InitHook::GetVM();
    if (vm && vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK)
      env->DeleteGlobalRef(ref);
  }
}
