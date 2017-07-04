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

#ifndef _SHIM_BASE_STRUCT_H_
#define _SHIM_BASE_STRUCT_H_

#include <dlist.h>
#include <colors.h>
#include <class_eda_rect.h>

#include <ctime>

enum KICAD_T
{
  TYPE_GERBER_DRAW_ITEM,
};

typedef unsigned STATUS_FLAGS;

class EDA_ITEM
{
 private:
  STATUS_FLAGS  m_Status;

 protected:
  EDA_ITEM *Pnext, *Pback;
  DHEAD* m_List;
  EDA_ITEM*     m_Parent;       ///< Linked list: Link (parent struct)
  time_t        m_TimeStamp;
  STATUS_FLAGS  m_Flags;

  EDA_ITEM( EDA_ITEM* parent, KICAD_T idType );

 public:
  void SetTimeStamp( time_t aNewTimeStamp ) { m_TimeStamp = aNewTimeStamp; }
  EDA_ITEM* Next() const { return Pnext; }
  EDA_ITEM* Back() const { return Pback; }
  DHEAD* GetList() const { return m_List; }
  void SetNext( EDA_ITEM* aNext )       { Pnext = aNext; }
  void SetBack( EDA_ITEM* aBack )       { Pback = aBack; }
  void SetParent( EDA_ITEM* aParent )   { m_Parent = aParent; }
  void SetList( DHEAD* aList )          { m_List = aList; }
  STATUS_FLAGS GetStatus() const { return m_Status; }
  void SetStatus( STATUS_FLAGS aStatus ) { m_Status = aStatus; }
};

#endif // _SHIM_BASE_STRUCT_H_
