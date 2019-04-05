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

package se.pp.mc.android.Gerberoid.gerber;

import android.database.DataSetObserver;

import java.io.File;

import se.pp.mc.android.Gerberoid.gerber.Layer;

public interface Layers
{
    public int getLayerCount();
    public Layer getLayer(int layer);
    public void SetLayerColor(int layer, int color);
    public void SetLayerVisible(int layer, boolean visible);
    public boolean LoadGerber(File file);
    public boolean LoadDrill(File file);
    public boolean Clear_DrawLayers();
    public void setActiveLayer(int layer);
    public int getActiveLayer();

    public void registerObserver(DataSetObserver observer);
    public void unregisterObserver(DataSetObserver observer);
}
