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

#include <cstddef>
#include <jni.h>
#include "inithook.h"

InitHook *InitHook::head = nullptr;
JavaVM *InitHook::vm = nullptr;

InitHook::InitHook()
{
  link = head;
  head = this;
}

InitHook::~InitHook()
{
  if (head == this)
    head = link;
  else
    for (InitHook *h = head; h; h = h->link)
      if (h->link == this) {
	h->link = this->link;
	break;
      }
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void * reserved)
{
  JNIEnv* env;
  InitHook::vm = vm;
  if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) {
    for (InitHook *hook = InitHook::head; hook; hook = hook->link)
      if (!hook->init(env))
	return -1;
    return JNI_VERSION_1_6;
  } else {
    return -1;
  }
}
 
void JNI_OnUnload(JavaVM *vm, void *reserved)
{
  JNIEnv* env;
  if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) {
    for (InitHook *hook = InitHook::head; hook; hook = hook->link)
      hook->deinit(env);
  }
  InitHook::vm = nullptr;
}
