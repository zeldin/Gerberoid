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

#ifndef _WX_DEFS_H_
#define _WX_DEFS_H_

#define wxT(s) s
#define _(s) (wxT(s))

#define wxFopen fopen

#define wxALPHA_OPAQUE 0xff

#define wxEXEC_ASYNC 0

#define wxDefaultDateTimeFormat "%c"

#define wxSOLID 100

#endif // _WX_DEFS_H_
