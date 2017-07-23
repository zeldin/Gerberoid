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

#include <fctsys.h>
#include <common.h>
#include <gerbview.h>
#include <gerbview_frame.h>

#include <jni.h>
#include "inithook.h"
#include "threadscopedcontext.h"

class GerbviewFrame {

private:
  static jclass class_GerbviewFrame;

  static const JNINativeMethod methods[];

  static jlong ToHandle(GERBVIEW_FRAME *frame)
  {
    return reinterpret_cast<jlong>(frame);
  }
  static GERBVIEW_FRAME *FromHandle(jlong handle)
  {
    return reinterpret_cast<GERBVIEW_FRAME *>(handle);
  }

  static jlong NativeCreate(JNIEnv *env, jobject objectOrClass);
  static void NativeDestroy(JNIEnv *env, jobject objectOrClass, jlong handle);
  static jboolean NativeRead_GERBER_File(JNIEnv *env, jobject objectOrClass, jlong handle, jstring GERBER_FullFileName, jstring D_Code_FullFileName);
  static void NativeRedrawActiveWindow(JNIEnv *env, jobject objectOrClass, jlong handle, jobject canvas, jboolean eraseBg);

  class FrameHolder {
  private:
    GERBVIEW_FRAME *frame;
    ContextProvider *oldprovider;
  public:
    FrameHolder(jlong handle);
    ~FrameHolder();
    GERBVIEW_FRAME* operator->() { return frame; }
    operator bool() { return frame != 0; }
  };

  static class Hook : InitHook {
    virtual bool init(JNIEnv *env);
    virtual void deinit(JNIEnv *env);
  } hook;
};

GerbviewFrame::Hook GerbviewFrame::hook;

jclass GerbviewFrame::class_GerbviewFrame = 0;

const JNINativeMethod GerbviewFrame::methods[] = {
  { "NativeCreate", "()J", (void *)&NativeCreate },
  { "NativeDestroy", "(J)V", (void *)&NativeDestroy },
  { "NativeRead_GERBER_File", "(JLjava/lang/String;Ljava/lang/String;)Z", (void*)&NativeRead_GERBER_File },
  { "NativeRedrawActiveWindow", "(JLandroid/graphics/Canvas;Z)V", (void*)&NativeRedrawActiveWindow },
};

GerbviewFrame::FrameHolder::FrameHolder(jlong handle)
  : frame(FromHandle(handle))
{
  oldprovider = ThreadScopedContext::Swap(frame);
}

GerbviewFrame::FrameHolder::~FrameHolder()
{
  ThreadScopedContext::Swap(oldprovider);
}

jlong GerbviewFrame::NativeCreate(JNIEnv *env, jobject objectOrClass)
{
  return ToHandle(new(std::nothrow) GERBVIEW_FRAME(objectOrClass));
}

void GerbviewFrame::NativeDestroy(JNIEnv *env, jobject objectOrClass, jlong handle)
{
  GERBVIEW_FRAME *frame = FromHandle(handle);
  if (frame)
    delete frame;
}

jboolean GerbviewFrame::NativeRead_GERBER_File(JNIEnv *env, jobject objectOrClass, jlong handle, jstring GERBER_FullFileName, jstring D_Code_FullFileName)
{
  FrameHolder frame(handle);
  if (frame && GERBER_FullFileName && D_Code_FullFileName) {
    const char *utf_GERBER_FullFileName = env->GetStringUTFChars(GERBER_FullFileName, NULL);
    const char *utf_D_Code_FullFileName = env->GetStringUTFChars(D_Code_FullFileName, NULL);
    jboolean result = frame->Read_GERBER_File(utf_GERBER_FullFileName, utf_D_Code_FullFileName);
    env->ReleaseStringUTFChars(GERBER_FullFileName, utf_GERBER_FullFileName);
    env->ReleaseStringUTFChars(D_Code_FullFileName, utf_D_Code_FullFileName);
    return result;
  } else {
    return false;
  }
}

void GerbviewFrame::NativeRedrawActiveWindow(JNIEnv *env, jobject objectOrClass, jlong handle, jobject canvas, jboolean eraseBg)
{
  FrameHolder frame(handle);
  if (frame && canvas) {
    wxDC dc(canvas);
    frame->RedrawActiveWindow(&dc, eraseBg);
  }
}

bool GerbviewFrame::Hook::init(JNIEnv *env)
{
  jclass gfclass = env->FindClass("se/pp/mc/android/Gerberoid/GerbviewFrame");
  if (!gfclass)
    return false;
  class_GerbviewFrame = (jclass)env->NewGlobalRef(gfclass);
  env->DeleteLocalRef(gfclass);
  env->RegisterNatives(class_GerbviewFrame, methods, sizeof(methods)/sizeof(methods[0]));
  return true;
}

void GerbviewFrame::Hook::deinit(JNIEnv *env)
{
  if (class_GerbviewFrame) {
    env->UnregisterNatives(class_GerbviewFrame);
    env->DeleteGlobalRef(class_GerbviewFrame);
    class_GerbviewFrame = 0;
  }
}
