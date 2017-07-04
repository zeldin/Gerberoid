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

#ifndef _WX_LOG_H_
#define _WX_LOG_H_

#include <android/log.h>

#ifndef WX_ANDROID_TAG
#define WX_ANDROID_TAG "wx"
#endif

inline void wxLOG(int prio, const char *msg)
{
  __android_log_write(prio, WX_ANDROID_TAG, msg);
}

template<typename T, typename ...Us>
inline void wxLOG(int prio, const char *fmt, T arg1, Us... argsrest)
{
  __android_log_print(prio, WX_ANDROID_TAG, fmt, arg1, argsrest...);
}

#define wxLogMessage(...) wxLOG(ANDROID_LOG_INFO, __VA_ARGS__)
#define wxLogDebug(...)   wxLOG(ANDROID_LOG_DEBUG, __VA_ARGS__)

#endif  // _WX_LOG_H_
