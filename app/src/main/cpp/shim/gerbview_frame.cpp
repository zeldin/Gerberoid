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
#include <class_gbr_layout.h>

GERBVIEW_FRAME::GERBVIEW_FRAME(android::View&& view)
  : wxWindow(std::move(view))
{
  m_gerberLayout = NULL;
  SetLayout( new GBR_LAYOUT() );
}

int GERBVIEW_FRAME::getActiveLayer()
{
  /* Stub */
  return 0;
}

bool GERBVIEW_FRAME::IsLayerVisible( int aLayer ) const
{
  /* Stub */
  return false;
}

EDA_COLOR_T GERBVIEW_FRAME::GetLayerColor( int aLayer ) const
{
  /* Stub */
  return BLACK;
}

EDA_COLOR_T GERBVIEW_FRAME::GetNegativeItemsColor() const
{
  /* Stub */
  return BLACK;
}

bool GERBVIEW_FRAME::DisplayLinesSolidMode() const
{
  /* Stub */
  return false;
}

bool GERBVIEW_FRAME::DisplayPolygonsSolidMode() const
{
  /* Stub */
  return false;
}

bool GERBVIEW_FRAME::DisplayFlashedItemsSolidMode() const
{
  /* Stub */
  return false;
}

void GERBVIEW_FRAME::AppendMsgPanel( const wxString& textUpper, const wxString& textLower,
		     EDA_COLOR_T color )
{
  /* Stub */
}

void GERBVIEW_FRAME::ClearMsgPanel( void )
{
  /* Stub */
}
