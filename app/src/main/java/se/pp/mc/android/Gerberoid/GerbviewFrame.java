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
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.ScaleGestureDetector;
import android.view.View;

public class GerbviewFrame extends View
    implements ScaleGestureDetector.OnScaleGestureListener,
	       GestureDetector.OnGestureListener
{

    static {
        System.loadLibrary("gerbview");
    }

    private long nativeHandle;

    private int logicalOriginX;
    private int logicalOriginY;
    private float userScale;
    private float startDevX;
    private float startDevY;
    private float startSpan;
    private int startLogOrgX;
    private int startLogOrgY;
    private float startUserScale;
    private boolean isScaling;

    private ScaleGestureDetector scaleGestureDetector;
    private GestureDetector gestureDetector;

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

    @Override
    public boolean onTouchEvent(MotionEvent ev) {
        boolean scaleResult = scaleGestureDetector.onTouchEvent(ev);
	boolean gestResult = gestureDetector.onTouchEvent(ev);
	return scaleResult || gestResult || super.onTouchEvent(ev);
    }

    @Override
    public boolean onScaleBegin(ScaleGestureDetector detector)
    {
	isScaling = true;
	startDevX = detector.getFocusX();
	startDevY = detector.getFocusY();
	startSpan = detector.getCurrentSpan();
	startLogOrgX = logicalOriginX;
	startLogOrgY = logicalOriginY;
	startUserScale = userScale;
	return true;
    }

    @Override
    public boolean onScale(ScaleGestureDetector detector)
    {
	userScale = startUserScale * detector.getCurrentSpan() / startSpan;
	logicalOriginX = startLogOrgX +
	    (int)(startDevX / startUserScale) -
	    (int)(detector.getFocusX() / userScale);
	logicalOriginY = startLogOrgY +
	    (int)(startDevY / startUserScale) -
	    (int)(detector.getFocusY() / userScale);
	SetOriginAndScale();
	invalidate();
	return true;
    }

    @Override
    public void onScaleEnd(ScaleGestureDetector detector)
    {
	isScaling = false;
    }

    @Override
    public boolean onDown(MotionEvent e)
    {
	return true;
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY)
    {
	return true;
    }

    @Override
    public void onLongPress(MotionEvent e)
    {
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)
    {
	if (!isScaling) {
	    logicalOriginX += (int)(distanceX / userScale);
	    logicalOriginY += (int)(distanceY / userScale);
	    SetOriginAndScale();
	    invalidate();
	}
	return true;
    }

    @Override
    public void onShowPress(MotionEvent e)
    {
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e)
    {
	return true;
    }

    void onCreate()
    {
	nativeHandle = NativeCreate();
	logicalOriginX = 0;
	logicalOriginY = 0;
	userScale = 5e-5f;
	SetOriginAndScale();
	isScaling = false;
	scaleGestureDetector = new ScaleGestureDetector(getContext(), this);
	gestureDetector = new GestureDetector(getContext(), this);
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

    private void SetOriginAndScale()
    {
	NativeSetOriginAndScale(nativeHandle, logicalOriginX, logicalOriginY, userScale);
    }

    private native long NativeCreate();
    private native void NativeDestroy(long handle);
    private native boolean NativeRead_GERBER_File(long handle, String GERBER_FullFileName, String D_Code_FullFileName);
    private native void NativeOnDraw(long handle, Canvas canvas, boolean eraseBg);
    private native void NativeSetOriginAndScale(long handle, int logicalOriginX, int logicalOriginY, float userScale);
};
