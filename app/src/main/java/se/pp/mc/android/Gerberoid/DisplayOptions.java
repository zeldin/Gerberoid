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

package se.pp.mc.android.Gerberoid;

public interface DisplayOptions
{
    public boolean GetDisplayDCodesFlag();
    public void SetDisplayDCodesFlag(boolean flag);
    public boolean GetDisplayFlashedItemsFillFlag();
    public void SetDisplayFlashedItemsFillFlag(boolean flag);
    public boolean GetDisplayLinesFillFlag();
    public void SetDisplayLinesFillFlag(boolean flag);
    public boolean GetDisplayNegativeObjectsFlag();
    public void SetDisplayNegativeObjectsFlag(boolean flag);
    public boolean GetDisplayPolygonsFillFlag();
    public void SetDisplayPolygonsFillFlag(boolean flag);
    public boolean GetDisplayGridFlag();
    public void SetDisplayGridFlag(boolean flag);
    public int GetDisplayMode();
    public void SetDisplayMode(int mode);
    public int GetDCodesVisibleColorARGB();
    public void SetDCodesVisibleColor(int color);
    public int GetGerberGridVisibleColorARGB();
    public void SetGerberGridVisibleColor(int color);
    public int GetNegativeObjectsVisibleColorARGB();
    public void SetNegativeObjectsVisibleColor(int color);
}
