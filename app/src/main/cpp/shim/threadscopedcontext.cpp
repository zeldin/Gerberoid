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
#include "threadscopedcontext.h"

#include <pthread.h>

namespace {

template<class T> class ThreadLocal {
private:
  pthread_key_t key;
public:
  ThreadLocal() {
    pthread_key_create(&key, NULL);
  }
  ~ThreadLocal() {
    pthread_key_delete(key);
  }
  T* get() { return static_cast<T*>(pthread_getspecific(key)); }
  void set(T* value) { pthread_setspecific(key, value); }
};

static ThreadLocal<GERBVIEW_FRAME> ThreadLocalFrame;

}


GERBVIEW_FRAME *ThreadScopedContext::Swap(GERBVIEW_FRAME *frame)
{
  GERBVIEW_FRAME *otherframe = ThreadLocalFrame.get();
  ThreadLocalFrame.set(frame);
  return otherframe;
}

android::Context ThreadScopedContext::Get()
{
  GERBVIEW_FRAME *frame = ThreadLocalFrame.get();
  if (frame)
    return frame->getView().getContext();
  else
    return android::Context();
}
