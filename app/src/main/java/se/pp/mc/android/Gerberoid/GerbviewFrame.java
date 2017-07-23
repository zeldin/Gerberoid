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

import android.content.Context;
import android.graphics.Canvas;
import android.os.Bundle;
import android.util.AttributeSet;
import android.view.View;

public class GerbviewFrame extends View {

    static {
        System.loadLibrary("gerbview");
    }

    private long nativeHandle;

    public GerbviewFrame(Context context) {
	super(context);
    }

    public GerbviewFrame(Context context, AttributeSet attrs) {
	super(context, attrs);
    }

    @Override
    protected void onDraw (Canvas canvas)
    {
        super.onDraw(canvas);
	NativeOnDraw(nativeHandle, canvas, false);
    }

    void onCreate()
    {
	nativeHandle = NativeCreate();
    }

    void onDestroy()
    {
	NativeDestroy(nativeHandle);
	nativeHandle = 0;
    }

    void onRestoreInstanceState(Bundle savedInstanceState)
    {
	android.util.Log.i("GerbviewFram", "RGF => "+Read_GERBER_File("/sdcard/Download/riser-B.Cu.gbr"));
    }

    void onSaveInstanceState(Bundle savedInstanceState) {
    }

    boolean Read_GERBER_File(String GERBER_FullFileName, String D_Code_FullFileName)
    {
	return NativeRead_GERBER_File(nativeHandle, GERBER_FullFileName, D_Code_FullFileName);
    }

    boolean Read_GERBER_File(String FullFileName)
    {
	return Read_GERBER_File(FullFileName, FullFileName);
    }

    private native long NativeCreate();
    private native void NativeDestroy(long handle);
    private native boolean NativeRead_GERBER_File(long handle, String GERBER_FullFileName, String D_Code_FullFileName);
    private native void NativeOnDraw(long handle, Canvas canvas, boolean eraseBg);
};
