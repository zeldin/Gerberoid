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
import android.content.res.Resources;
import android.graphics.Canvas;
import android.os.Bundle;
import android.util.AttributeSet;
import android.util.Pair;
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.ScaleGestureDetector;
import android.view.View;

import java.util.Arrays;

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

    private Layer[] layers;

    public GerbviewFrame(Context context) {
	super(context);
    }

    public GerbviewFrame(Context context, AttributeSet attrs) {
	super(context, attrs);
    }

    public Layer[] getLayers() {
	return layers;
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
	layers = new Layer[getContext().getResources().getInteger(R.integer.number_of_layers)];
	for (int i=0; i<layers.length; i++)
	    layers[i] = new Layer();
	logicalOriginX = 0;
	logicalOriginY = 0;
	userScale = 5e-5f;
	SetOriginAndScale();
	isScaling = false;
	scaleGestureDetector = new ScaleGestureDetector(getContext(), this);
	gestureDetector = new GestureDetector(getContext(), this);
	setLayerType(LAYER_TYPE_SOFTWARE, null);
    }

    void onDestroy()
    {
	NativeDestroy(nativeHandle);
	nativeHandle = 0;
	layers = null;
    }

    void onRestoreInstanceState(Bundle savedInstanceState)
    {
	Resources resources = getContext().getResources();
	int[] layerColors = savedInstanceState.getIntArray("layerColors");
	if (layerColors == null)
	    layerColors = resources.getIntArray(R.array.default_layer_colors);
	if (layerColors != null)
	    SetLayerColors(layerColors);
	boolean[] layerVisibilities = savedInstanceState.getBooleanArray("layerVisibilities");
	if (layerVisibilities == null) {
	    layerVisibilities = new boolean[layers.length];
	    Arrays.fill(layerVisibilities, true);
	}
	SetLayerVisibilities(layerVisibilities);
	int[] visibleElementColors = savedInstanceState.getIntArray("visibleElementColors");
	if (visibleElementColors == null)
	    visibleElementColors = resources.getIntArray(R.array.default_visible_element_colors);
	if (visibleElementColors != null)
	    SetVisibleElementColors(visibleElementColors);
	android.util.Log.i("GerbviewFram", "RGF => "+Read_GERBER_File("/sdcard/Download/riser-B.Cu.gbr"));
    }

    void onSaveInstanceState(Bundle savedInstanceState) {
	if (layers != null) {
	    int[] layerColors = new int[layers.length];
	    GetLayerColors(layerColors);
	    savedInstanceState.putIntArray("layerColors", layerColors);
	    boolean[] layerVisibilities = new boolean[layers.length];
	    GetLayerVisibilities(layerVisibilities);
	    savedInstanceState.putBooleanArray("layerVisibilities", layerVisibilities);
	}
	if (nativeHandle != 0) {
	    Resources resources = getContext().getResources();
	    int[] visibleElementColors = new int[resources.getInteger(R.integer.number_of_visible_elements)];
	    GetVisibleElementColors(visibleElementColors);
	    savedInstanceState.putIntArray("visibleElementColors", visibleElementColors);
	}
    }

    private void SetLayerColors(int[] colors)
    {
	for(int i=0; i<colors.length; i++) {
	    NativeSetLayerColor(nativeHandle, i, colors[i]);
	    layers[i].SetColor(NativeMakeColour(colors[i]));
	}
    }

    private void GetLayerColors(int[] colors)
    {
	for(int i=0; i<colors.length; i++)
	    colors[i] = NativeGetLayerColor(nativeHandle, i);
    }

    private void SetLayerVisibilities(boolean[] visibilities)
    {
	for(int i=0; i<visibilities.length; i++) {
	    NativeSetLayerVisible(nativeHandle, i, visibilities[i]);
	    layers[i].SetVisible(visibilities[i]);
	}
    }

    private void GetLayerVisibilities(boolean[] visibilities)
    {
	for(int i=0; i<visibilities.length; i++)
	    visibilities[i] = NativeIsLayerVisible(nativeHandle, i);
    }

    private void SetVisibleElementColors(int[] colors)
    {
	for(int i=0; i<colors.length; i++)
	    NativeSetVisibleElementColor(nativeHandle, i+1, colors[i]);
    }

    private void GetVisibleElementColors(int[] colors)
    {
	for(int i=0; i<colors.length; i++)
	    colors[i] = NativeGetVisibleElementColor(nativeHandle, i+1);
    }

    void SetLayerColor(int layer, int color)
    {
	NativeSetLayerColor(nativeHandle, layer, color);
	layers[layer].SetColor(NativeMakeColour(color));
	invalidate();
    }

    int GetLayerColor(int layer)
    {
	return NativeGetLayerColor(nativeHandle, layer);
    }

    boolean Read_GERBER_File(String GERBER_FullFileName, String D_Code_FullFileName)
    {
	boolean result = NativeRead_GERBER_File(nativeHandle, GERBER_FullFileName, D_Code_FullFileName);
	invalidate();
	return result;
    }

    boolean Read_GERBER_File(String FullFileName)
    {
	return Read_GERBER_File(FullFileName, FullFileName);
    }

    boolean Read_EXCELLON_File(String EXCELLON_FullFileName)
    {
	boolean result = NativeRead_EXCELLON_File(nativeHandle, EXCELLON_FullFileName);
	invalidate();
	return result;
    }

    boolean Clear_DrawLayers()
    {
	boolean result = NativeClear_DrawLayers(nativeHandle);
	invalidate();
	return result;
    }

    void Erase_Current_DrawLayer()
    {
	NativeErase_Current_DrawLayer(nativeHandle);
	invalidate();
    }

    private void SetOriginAndScale()
    {
	NativeSetOriginAndScale(nativeHandle, logicalOriginX, logicalOriginY, userScale);
    }

    void SetLayerVisible(int layer, boolean visible)
    {
	if (layers != null) {
	    layers[layer].SetVisible(visible);
	    NativeSetLayerVisible(nativeHandle, layer, visible);
	    invalidate();
	}
    }

    static Pair<int[], String[]> getColors(Context context)
    {
	Resources resources = context.getResources();
	int numColors = resources.getInteger(R.integer.number_of_colors);
	int[] colors = new int[numColors];
	String[] names = new String[numColors];
	for (int i=0; i<colors.length; i++)
	    colors[i] = NativeMakeColour(i);
	for (int i=0; i<names.length; i++)
	    names[i] = NativeColorGetName(i);
	return new Pair<int[], String[]>(colors, names);
    }

    private native long NativeCreate();
    private native void NativeDestroy(long handle);
    private native void NativeSetLayerColor(long handle, int layer, int color);
    private native int NativeGetLayerColor(long handle, int layer);
    private native void NativeSetVisibleElementColor(long handle, int layer, int color);
    private native int NativeGetVisibleElementColor(long handle, int layer);
    private native boolean NativeRead_GERBER_File(long handle, String GERBER_FullFileName, String D_Code_FullFileName);
    private native boolean NativeRead_EXCELLON_File(long handle, String EXCELLON_FullFileName);
    private native boolean NativeClear_DrawLayers(long handle);
    private native void NativeErase_Current_DrawLayer(long handle);
    private native void NativeOnDraw(long handle, Canvas canvas, boolean eraseBg);
    private native void NativeSetOriginAndScale(long handle, int logicalOriginX, int logicalOriginY, float userScale);
    private native boolean NativeIsLayerVisible(long handle, int layer);
    private native void NativeSetLayerVisible(long handle, int layer, boolean visible);
    private native static int NativeMakeColour(int color);
    private native static String NativeColorGetName(int color);
};
