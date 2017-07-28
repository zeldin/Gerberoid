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
  static void NativeSetLayerColor(JNIEnv *env, jobject objectOrClass, jlong handle, jint layer, jint color);
  static jint NativeGetLayerColor(JNIEnv *env, jobject objectOrClass, jlong handle, jint layer);
  static void NativeSetVisibleElementColor(JNIEnv *env, jobject objectOrClass, jlong handle, jint item, jint color);
  static jint NativeGetVisibleElementColor(JNIEnv *env, jobject objectOrClass, jlong handle, jint item);
  static jboolean NativeRead_GERBER_File(JNIEnv *env, jobject objectOrClass, jlong handle, jstring GERBER_FullFileName, jstring D_Code_FullFileName);
  static jboolean NativeRead_EXCELLON_File(JNIEnv *env, jobject objectOrClass, jlong handle, jstring EXCELLON_FullFileName);
  static jboolean NativeClear_DrawLayers(JNIEnv *env, jobject objectOrClass, jlong handle);
  static void NativeErase_Current_DrawLayer(JNIEnv *env, jobject objectOrClass, jlong handle);
  static void NativeOnDraw(JNIEnv *env, jobject objectOrClass, jlong handle, jobject canvas, jboolean eraseBg);
  static void NativeSetOriginAndScale(JNIEnv *env, jobject objectOrClass, jlong handle, jint logicalOriginX, jint logicalOriginY, jfloat userScale);
  static jboolean NativeIsLayerVisible(JNIEnv *env, jobject objectOrClass, jlong handle, jint layer);
  static void NativeSetLayerVisible(JNIEnv *env, jobject objectOrClass, jlong handle, jint layer, jboolean visible);
  static void NativesetActiveLayer(JNIEnv *env, jobject objectOrClass, jlong handle, jint layer);
  static jint NativeMakeColour(JNIEnv *env, jobject objectOrClass, jint color);
  static jstring NativeColorGetName(JNIEnv *env, jobject objectOrClass, jint color);

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
  { "NativeSetLayerColor", "(JII)V", (void*)&NativeSetLayerColor },
  { "NativeGetLayerColor", "(JI)I", (void*)&NativeGetLayerColor },
  { "NativeSetVisibleElementColor", "(JII)V", (void*)&NativeSetVisibleElementColor },
  { "NativeGetVisibleElementColor", "(JI)I", (void*)&NativeGetVisibleElementColor },
  { "NativeRead_GERBER_File", "(JLjava/lang/String;Ljava/lang/String;)Z", (void*)&NativeRead_GERBER_File },
  { "NativeRead_EXCELLON_File", "(JLjava/lang/String;)Z", (void*)&NativeRead_EXCELLON_File },
  { "NativeClear_DrawLayers", "(J)Z", (void*)&NativeClear_DrawLayers },
  { "NativeErase_Current_DrawLayer", "(J)V", (void*)&NativeErase_Current_DrawLayer },
  { "NativeOnDraw", "(JLandroid/graphics/Canvas;Z)V", (void*)&NativeOnDraw },
  { "NativeSetOriginAndScale", "(JIIF)V", (void*)&NativeSetOriginAndScale },
  { "NativeIsLayerVisible", "(JI)Z", (void*)&NativeIsLayerVisible },
  { "NativeSetLayerVisible", "(JIZ)V", (void*)&NativeSetLayerVisible },
  { "NativesetActiveLayer", "(JI)V", (void*)&NativesetActiveLayer },
  { "NativeMakeColour", "(I)I", (void*)&NativeMakeColour },
  { "NativeColorGetName", "(I)Ljava/lang/String;", (void*)&NativeColorGetName },
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

void GerbviewFrame::NativeSetLayerColor(JNIEnv *env, jobject objectOrClass, jlong handle, jint layer, jint color)
{
  FrameHolder frame(handle);
  if (frame)
    frame->SetLayerColor(layer, static_cast<EDA_COLOR_T>(color));
}

jint GerbviewFrame::NativeGetLayerColor(JNIEnv *env, jobject objectOrClass, jlong handle, jint layer)
{
  FrameHolder frame(handle);
  if (frame)
    return frame->GetLayerColor(layer);
  else
    return -1;
}

void GerbviewFrame::NativeSetVisibleElementColor(JNIEnv *env, jobject objectOrClass, jlong handle, jint item, jint color)
{
  FrameHolder frame(handle);
  if (frame)
    frame->SetVisibleElementColor(static_cast<GERBER_VISIBLE_ID>(item), static_cast<EDA_COLOR_T>(color));
}

jint GerbviewFrame::NativeGetVisibleElementColor(JNIEnv *env, jobject objectOrClass, jlong handle, jint item)
{
  FrameHolder frame(handle);
  if (frame)
    return frame->GetVisibleElementColor(static_cast<GERBER_VISIBLE_ID>(item));
  else
    return -1;
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

jboolean GerbviewFrame::NativeRead_EXCELLON_File(JNIEnv *env, jobject objectOrClass, jlong handle, jstring EXCELLON_FullFileName)
{
  FrameHolder frame(handle);
  if (frame && EXCELLON_FullFileName) {
    const char *utf_EXCELLON_FullFileName = env->GetStringUTFChars(EXCELLON_FullFileName, NULL);
    jboolean result = frame->Read_EXCELLON_File(utf_EXCELLON_FullFileName);
    env->ReleaseStringUTFChars(EXCELLON_FullFileName, utf_EXCELLON_FullFileName);
    return result;
  } else {
    return false;
  }
}

jboolean GerbviewFrame::NativeClear_DrawLayers(JNIEnv *env, jobject objectOrClass, jlong handle)
{
  FrameHolder frame(handle);
  if (frame) {
    return frame->Clear_DrawLayers();
  } else {
    return false;
  }
}

void GerbviewFrame::NativeErase_Current_DrawLayer(JNIEnv *env, jobject objectOrClass, jlong handle)
{
  FrameHolder frame(handle);
  if (frame) {
    frame->Erase_Current_DrawLayer();
  }
}

void GerbviewFrame::NativeOnDraw(JNIEnv *env, jobject objectOrClass, jlong handle, jobject canvas, jboolean eraseBg)
{
  FrameHolder frame(handle);
  if (frame && canvas) {
    wxDC dc(canvas);
    frame->onDraw(dc, eraseBg);
  }
}

void GerbviewFrame::NativeSetOriginAndScale(JNIEnv *env, jobject objectOrClass, jlong handle, jint logicalOriginX, jint logicalOriginY, jfloat userScale)
{
  FrameHolder frame(handle);
  if (frame) {
    frame->m_canvas->SetOriginAndScale(logicalOriginX, logicalOriginY, userScale);
  }
}

jboolean GerbviewFrame::NativeIsLayerVisible(JNIEnv *env, jobject objectOrClass, jlong handle, jint layer)
{
  FrameHolder frame(handle);
  if (frame) {
    return frame->IsLayerVisible(layer);
  } else {
    return false;
  }
}

void GerbviewFrame::NativeSetLayerVisible(JNIEnv *env, jobject objectOrClass, jlong handle, jint layer, jboolean visible)
{
  FrameHolder frame(handle);
  if (frame) {
    frame->SetLayerVisible(layer, visible);
  }
}

void GerbviewFrame::NativesetActiveLayer(JNIEnv *env, jobject objectOrClass, jlong handle, jint layer)
{
  FrameHolder frame(handle);
  if (frame) {
    frame->setActiveLayer(layer);
  }
}

jint GerbviewFrame::NativeMakeColour(JNIEnv *env, jobject objectOrClass, jint color)
{
  return MakeColour(static_cast<EDA_COLOR_T>(color)).ARGB();
}

jstring GerbviewFrame::NativeColorGetName(JNIEnv *env, jobject objectOrClass, jint color)
{
  return env->NewStringUTF(ColorGetName(static_cast<EDA_COLOR_T>(color)));
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
