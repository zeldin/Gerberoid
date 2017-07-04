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

#ifndef _WX_WX_H_
#define _WX_WX_H_

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "wx/string.h"
#include "wx/arrstr.h"
#include "wx/gdicmn.h"
#include "wx/window.h"
#include "wx/log.h"
#include "wx/debug.h"

class wxTextCtrl;
class wxStaticText;
class wxProcess;
class wxFileName;
class wxConfigBase;

class wxDateTime
{
 private:
  time_t m_time;
 public:
  wxDateTime(time_t t) : m_time(t) {}
  static wxDateTime Now() { return wxDateTime(std::time(NULL)); }
  static constexpr int Country_Default = 0;
  static constexpr int Local = 0;
  void SetCountry(int n) {}
  wxString Format(const char *fmt, int timezone) {
    char mbstr[100];
    if (std::strftime(mbstr, sizeof(mbstr), fmt, std::localtime(&m_time))) {
      return mbstr;
    } else {
      return "";
    }
  }
};

class wxDC
{
};

class wxColour
{
 private:
  uint32_t m_ARGB;

 public:
  wxColour(unsigned char aRed, unsigned char aGreen, unsigned char aBlue, unsigned char aAlpha = wxALPHA_OPAQUE) :
    m_ARGB(aAlpha << 24 | aRed << 16 | aGreen << 8 | aBlue) {}
    unsigned char Red() const { return (m_ARGB >> 16) & 0xff; }
    unsigned char Green() const { return (m_ARGB >> 8) & 0xff; }
    unsigned char Blue() const { return m_ARGB & 0xff; }
    unsigned char Alpha() const { return (m_ARGB >> 24) & 0xff; }
};

class wxBrush
{
 public:
  void SetColour(unsigned char aRed, unsigned char aGreen, unsigned char aBlue);
};

typedef enum {
  wxPENSTYLE_SOLID
} wxPenStyle;

wxString wxPathOnly(const wxString& path);
void wxSetWorkingDirectory(const wxString& path);

int wxMessageBox(const wxString& message);
void wxBell();

#endif // _WX_WX_H_
