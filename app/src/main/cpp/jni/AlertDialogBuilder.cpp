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
#include "AlertDialogBuilder.h"
#include "Context.h"
#include "inithook.h"
#include "localframe.h"

namespace android
{

class AlertDialogBuilder::Native : InitHook
{
  friend class AlertDialogBuilder;
  virtual bool init(JNIEnv *env);
  virtual void deinit(JNIEnv *env);
private:
  static Native hook;

  static jclass class_AlertDialogBuilder;
  static jmethodID method_init;
  static jmethodID method_setTitle;
  static jmethodID method_setMessage;
  static jmethodID method_setNegativeButton;
  static jmethodID method_show;

  static JNIRef Create(const Context& context);
};
AlertDialogBuilder::Native AlertDialogBuilder::Native::hook;

jclass AlertDialogBuilder::Native::class_AlertDialogBuilder = 0;
jmethodID AlertDialogBuilder::Native::method_init = 0;
jmethodID AlertDialogBuilder::Native::method_setTitle = 0;
jmethodID AlertDialogBuilder::Native::method_setMessage = 0;
jmethodID AlertDialogBuilder::Native::method_setNegativeButton = 0;
jmethodID AlertDialogBuilder::Native::method_show = 0;

AlertDialogBuilder::AlertDialogBuilder(const Context& context)
  : JNIRef(Native::Create(context))
{
}

JNIRef AlertDialogBuilder::Native::Create(const Context& context)
{
  LocalFrame env;
  if (!env) return JNIRef();
  return env->NewObject(Native::class_AlertDialogBuilder, Native::method_init,
			static_cast<jobject>(context));
}

AlertDialogBuilder& AlertDialogBuilder::setTitle(const char *title)
{
  LocalFrame env;
  if (env && get() && title)
    env->CallObjectMethod(*this, Native::method_setTitle,
			  env->NewStringUTF(title));
  return *this;
}

AlertDialogBuilder& AlertDialogBuilder::setMessage(const Spanned& message)
{
  LocalFrame env;
  if (env && get() && message)
    env->CallObjectMethod(*this, Native::method_setMessage,
			  static_cast<jobject>(message));
  return *this;
}

AlertDialogBuilder& AlertDialogBuilder::setNegativeButton(const char *title, const DialogInterfaceOnClickListener& listener)
{
  LocalFrame env;
  if (env && get() && title)
    env->CallObjectMethod(*this, Native::method_setNegativeButton,
			  env->NewStringUTF(title),
			  static_cast<jobject>(listener));
  return *this;
}

AlertDialog AlertDialogBuilder::show()
{
  LocalFrame env;
  if (!env || !get()) return AlertDialog();
  return env->CallObjectMethod(*this, Native::method_show);
}

bool AlertDialogBuilder::Native::init(JNIEnv *env)
{
  jclass loc_class = env->FindClass("android/app/AlertDialog$Builder");
  if (!loc_class)
    return false;
  class_AlertDialogBuilder = (jclass)env->NewGlobalRef(loc_class);
  env->DeleteLocalRef(loc_class);
  if (!class_AlertDialogBuilder)
    return false;
  method_init = env->GetMethodID(class_AlertDialogBuilder, "<init>", "(Landroid/content/Context;)V");
  method_setTitle = env->GetMethodID(class_AlertDialogBuilder, "setTitle", "(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;");
  method_setMessage = env->GetMethodID(class_AlertDialogBuilder, "setMessage", "(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;");
  method_setNegativeButton = env->GetMethodID(class_AlertDialogBuilder, "setNegativeButton", "(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;");
  method_show = env->GetMethodID(class_AlertDialogBuilder, "show", "()Landroid/app/AlertDialog;");
  return method_init && method_setTitle && method_setMessage &&
    method_setNegativeButton && method_show;
}

void AlertDialogBuilder::Native::deinit(JNIEnv *env)
{
  method_init = 0;
  method_setTitle = 0;
  method_setMessage = 0;
  method_setNegativeButton = 0;
  method_show = 0;
  if (class_AlertDialogBuilder) {
    env->DeleteGlobalRef(class_AlertDialogBuilder);
    class_AlertDialogBuilder = 0;
  }
}

}
