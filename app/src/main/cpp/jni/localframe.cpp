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
#include "localframe.h"
#include "inithook.h"

LocalFrame::LocalFrame(int aCapacity)
{
  JavaVM *vm = InitHook::GetVM();
  if (!vm || vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK ||
      env->PushLocalFrame(aCapacity) < 0)
    env = nullptr;
}

LocalFrame::LocalFrame(int aCapacity, JNIEnv *aEnv)
  : env(aEnv)
{
  if (env->PushLocalFrame(aCapacity) < 0)
    env = nullptr;
}

LocalFrame::LocalFrame(JNIEnv *aEnv) : LocalFrame(10, aEnv) {}
LocalFrame::LocalFrame() : LocalFrame(10) {}

LocalFrame::~LocalFrame()
{
  if (env)
    env->PopLocalFrame(0);
}
