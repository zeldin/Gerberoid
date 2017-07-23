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

#include <wx/wx.h>
#include <html_messagebox.h>
#include "ContextProvider.h"
#include <Html.h>

HTML_MESSAGE_BOX::HTML_MESSAGE_BOX( ContextProvider* parent, const wxString& aTitle )
  : dialogBuilder(parent->getContext())
{
  dialogBuilder.setTitle(aTitle).setNegativeButton(_("Close"));
}

void HTML_MESSAGE_BOX::ListSet( const wxArrayString& aList )
{
  wxString msg = wxT( "<ul>" );

  for( unsigned ii = 0; ii < aList.GetCount(); ii++ )
  {
    msg += wxT( "<li>" );
    msg += aList.Item( ii ) + wxT( "</li>" );
  }

  msg += wxT( "</ul>" );

  dialogBuilder.setMessage(android::Html::fromHtml(msg));
}

int HTML_MESSAGE_BOX::ShowModal()
{
  dialogBuilder.show();
  return 0;
}
