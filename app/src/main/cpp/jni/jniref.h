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

#ifndef JNIREF_H__
#define JNIREF_H__ 1

#include <jni.h>
#include <utility>

class JNIRef
{
private:
  jobject ref;

protected:
  jobject get() const { return ref; }

 public:
  JNIRef() : ref(0) {}
  JNIRef(jobject aRef);
  JNIRef(JNIRef&& other) : ref(other.ref) { other.ref=0; }
  JNIRef(const JNIRef& other) : JNIRef(other.ref) {}
  ~JNIRef();

  void swap(JNIRef& other) { std::swap(ref, other.ref); }

  operator jobject() const { return get(); }
  operator bool() { return ref != 0; }
};

#endif // JNIREF_H__
