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

#ifndef   _WX_DEBUG_H_
#define   _WX_DEBUG_H_

#define wxFAIL_COND_MSG(cond, msg)					\
  wxOnAssert(__FILE__, __LINE__,  __FUNCTION__, wxT(cond), msg)

#define wxASSERT_MSG(cond, msg)						\
  do {									\
    if ( cond )								\
      ;									\
    else								\
      wxOnAssert(__FILE__, __LINE__, __FUNCTION__, wxT(#cond), msg);	\
  } while(0)

#define wxCHECK2_MSG(cond, op, msg)			\
  do {							\
    if ( cond )						\
      ;							\
    else						\
      {							\
	wxFAIL_COND_MSG(#cond, msg);			\
      op;						\
    }							\
  } while(0)

#define wxASSERT(cond) wxASSERT_MSG(cond, NULL)

#define wxCHECK_RET(cond, msg)       wxCHECK2_MSG(cond, return, msg)

extern void wxOnAssert(const char *szFile,
		       int nLine,
		       const char *szFunc,
		       const wxChar *szCond,
		       const wxChar *szMsg = NULL);

#endif  /*  _WX_DEBUG_H_ */
