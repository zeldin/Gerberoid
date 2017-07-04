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

#include <gerbview.h>
#include <class_gbr_layout.h>

#include <map>

class GERBVIEW_FRAME : public wxWindow
{
private:
    GBR_LAYOUT*     m_gerberLayout;
    wxArrayString   m_Messages;

public:
    GERBVIEW_FRAME(android::View&& view);

    int getActiveLayer();
    bool IsLayerVisible( int aLayer ) const;
    EDA_COLOR_T GetLayerColor( int aLayer ) const;
    EDA_COLOR_T GetNegativeItemsColor() const;
    bool DisplayLinesSolidMode() const;
    bool DisplayPolygonsSolidMode() const;
    bool DisplayFlashedItemsSolidMode() const;

    void ReportMessage( const wxString aMessage )
    {
        m_Messages.Add( aMessage );
    }
    void ClearMessageList()
    {
        m_Messages.Clear();
    }

    void SetLayout( GBR_LAYOUT* aLayout )
    {
        delete m_gerberLayout;
        m_gerberLayout = aLayout;
    }

    GBR_LAYOUT* GetGerberLayout() const
    {
        wxASSERT( m_gerberLayout );
        return m_gerberLayout;
    }

    GERBER_DRAW_ITEM* GetItemsList()
    {
        GERBER_DRAW_ITEM* item = GetGerberLayout()->m_Drawings;

        return (GERBER_DRAW_ITEM*) item;
    }

    void AppendMsgPanel( const wxString& textUpper, const wxString& textLower,
                         EDA_COLOR_T color );
    void ClearMsgPanel( void );

    bool                Read_GERBER_File( const wxString&   GERBER_FullFileName,
                                          const wxString&   D_Code_FullFileName );

    void                CopyDCodesSizeToItems();
};
