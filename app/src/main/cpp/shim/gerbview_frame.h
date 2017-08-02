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
#include <class_drawpanel.h>
#include <class_colors_design_settings.h>
#include <ContextProvider.h>

#include <map>

struct GBR_DISPLAY_OPTIONS
{
  bool m_DisplayDCodes;
  bool m_DisplayFlashedItemsFill;
  bool m_DisplayLinesFill;
  bool m_DisplayNegativeObjects;
  bool m_DisplayPolarCood;
  bool m_DisplayPolygonsFill;
  bool m_IsPrinting;
  bool m_DisplayGrid;
};

class GBR_SCREEN : public BASE_SCREEN {};

class GERBVIEW_FRAME : ContextProvider
{
  friend class GerbviewFrame;
private:
    GBR_LAYOUT*     m_gerberLayout;
    wxArrayString   m_Messages;
    int             m_displayMode;
    EDA_DRAW_PANEL* m_canvas;
    COLORS_DESIGN_SETTINGS m_colorsSettings;
    uint32_t        m_visibleLayers;
    int             m_Active_Layer;

    BASE_SCREEN* GetScreen() const { return NULL; }

    android::Context getContext() { return m_canvas->getView().getContext(); }

public:
    GERBVIEW_FRAME(android::View&& view);

    GBR_DISPLAY_OPTIONS m_DisplayOptions;

    void setActiveLayer( int aLayer );
    int getActiveLayer();
    int getNextAvailableLayer( int aLayer ) const;
    void UpdateTitleAndInfo();
    bool IsLayerVisible( int aLayer ) const;
    void SetLayerVisible( int aLayer, bool aVisible );
    EDA_COLOR_T GetVisibleElementColor( GERBER_VISIBLE_ID aItemIdVisible ) const;
    void    SetVisibleElementColor( GERBER_VISIBLE_ID aItemIdVisible, EDA_COLOR_T aColor );
    EDA_COLOR_T GetLayerColor( int aLayer ) const;
    void SetLayerColor( int aLayer, EDA_COLOR_T aColor );
    EDA_COLOR_T GetNegativeItemsColor() const;
    bool DisplayLinesSolidMode() const { return m_DisplayOptions.m_DisplayLinesFill; }
    bool DisplayPolygonsSolidMode() const { return m_DisplayOptions.m_DisplayPolygonsFill; }
    bool DisplayFlashedItemsSolidMode() const { return m_DisplayOptions.m_DisplayFlashedItemsFill; }
    EDA_COLOR_T GetDrawBgColor() const;

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

    bool                Read_EXCELLON_File( const wxString& aFullFileName );

    void                CopyDCodesSizeToItems();

    void    PrintPage( wxDC* aDC, LSET aPrintMasklayer, bool aPrintMirrorMode,
		       void* aData = NULL );

    int GetDisplayMode() { return m_displayMode; }
    bool    IsElementVisible( GERBER_VISIBLE_ID aItemIdVisible ) const;

    void    RedrawActiveWindow( wxDC* DC, bool EraseBg );
    void    DrawItemsDCodeID( wxDC* aDC, GR_DRAWMODE aDrawMode );

    void DrawWorkSheet( wxDC* aDC, BASE_SCREEN* aScreen, int aLineWidth,
                         double aScale, const wxString &aFilename );

    void onDraw( wxDC& DC, bool EraseBg );

    bool Clear_DrawLayers();
    void Erase_Current_DrawLayer();
};
