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
{
  m_gerberLayout = NULL;
  m_canvas = new EDA_DRAW_PANEL(this, std::move(view));
  m_displayMode = 0;
  m_DisplayOptions.m_DisplayDCodes = true;
  m_DisplayOptions.m_DisplayFlashedItemsFill = true;
  m_DisplayOptions.m_DisplayLinesFill = true;
  m_DisplayOptions.m_DisplayNegativeObjects = false;
  m_DisplayOptions.m_DisplayPolarCood = false;
  m_DisplayOptions.m_DisplayPolygonsFill = true;
  m_DisplayOptions.m_IsPrinting = false;
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
  return true;
}

EDA_COLOR_T GERBVIEW_FRAME::GetLayerColor( int aLayer ) const
{
  /* Stub */
  return BLACK;
}

EDA_COLOR_T GERBVIEW_FRAME::GetVisibleElementColor( GERBER_VISIBLE_ID aItemIdVisible ) const
{
  /* Stub */
  return BLACK;
}

void GERBVIEW_FRAME::SetLayerColor( int aLayer, EDA_COLOR_T aColor )
{
  /* Stub */
}

EDA_COLOR_T GERBVIEW_FRAME::GetDrawBgColor() const
{
  /* Stub */
  return BLACK;
}

EDA_COLOR_T GERBVIEW_FRAME::GetNegativeItemsColor() const
{
  /* Stub */
  return BLACK;
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

bool GERBVIEW_FRAME::IsElementVisible( GERBER_VISIBLE_ID aItemIdVisible ) const
{
  /* Stub */
  return true;
}

void GERBVIEW_FRAME::DrawWorkSheet( wxDC* aDC, BASE_SCREEN* aScreen, int aLineWidth,
				    double aScale, const wxString &aFilename )
{
  /* Stub */
}

void GERBVIEW_FRAME::UpdateTitleAndInfo()
{
  /* Stub */
}

void GERBVIEW_FRAME::onDraw(wxDC& DC, bool eraseBg)
{
  m_canvas->DoPrepareDC(DC);
  RedrawActiveWindow(&DC, eraseBg);
}
