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

#include "wx/string.h"
#include "wx/log.h"

void wxOnAssert(const char *szFile,
		int nLine,
		const char *szFunc,
		const wxChar *szCond,
		const wxChar *szMsg)
{
  if (szMsg)
    __android_log_assert(szCond, WX_ANDROID_TAG, "%s:%d:%s: %s",
			 szFile, nLine, szFunc, szMsg);
  else
    __android_log_assert(szCond, WX_ANDROID_TAG, "%s:%d:%s",
			 szFile, nLine, szFunc);
}
