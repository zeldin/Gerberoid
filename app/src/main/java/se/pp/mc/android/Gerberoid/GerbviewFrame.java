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
import android.database.DataSetObservable;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.AttributeSet;
import android.util.Pair;
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.ScaleGestureDetector;
import android.view.View;

import java.io.File;
import java.util.Arrays;

public class GerbviewFrame extends View implements GerberViewer
{

    static {
        System.loadLibrary("gerbview");
    }

    private class ViewPortImpl
	implements ViewPort,
		   ScaleGestureDetector.OnScaleGestureListener,
		   GestureDetector.OnGestureListener
    {
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
	private float[] zoomList;
	private float defaultZoom;

	private ScaleGestureDetector scaleGestureDetector;
	private GestureDetector gestureDetector;

	private ViewPortImpl()
	{
	    Resources resources = getContext().getResources();
	    float iuPerDecimils = resources.getInteger(R.integer.IU_PER_DECIMILS);
	    int[] zooms = resources.getIntArray(R.array.zoom_list);
	    zoomList = new float[zooms.length];
	    for (int i=0; i<zooms.length; i++)
		zoomList[i] = 100f / (iuPerDecimils * zooms[i]);
	    defaultZoom = 100f / (iuPerDecimils * resources.getInteger(R.integer.default_zoom));
	    logicalOriginX = 0;
	    logicalOriginY = 0;
	    userScale = 1f;
	    SetOriginAndScale();
	    isScaling = false;
	    scaleGestureDetector = new ScaleGestureDetector(getContext(), this);
	    gestureDetector = new GestureDetector(getContext(), this);
	}

	public void Zoom_Automatique()
	{
	    Rect bbox = NativeComputeBoundingBox(nativeHandle);
	    float w = getWidth();
	    float h = getHeight();
	    float xscale = (w > 0 && bbox.width() > 0? w / bbox.width() : defaultZoom);
	    float yscale = (h > 0 && bbox.height() > 0? h / bbox.height() : defaultZoom);
	    userScale = (xscale < yscale? xscale : yscale);
	    logicalOriginX = (int)(bbox.centerX() - w * 0.5f / userScale);
	    logicalOriginY = (int)(bbox.centerY() - h * 0.5f / userScale);
	    SetOriginAndScale();
	    invalidate();
	}

	private boolean SetZoom(float zoom)
	{
	    float focalx = getWidth() * 0.5f;
	    float focaly = getHeight() * 0.5f;
	    logicalOriginX += focalx / userScale - focalx / zoom;
	    logicalOriginY += focaly / userScale - focaly / zoom;
	    userScale = zoom;
	    SetOriginAndScale();
	    invalidate();
	    return true;
	}

	public boolean SetPreviousZoom()
	{
	    for (int i=zoomList.length; --i >= 0; ) {
		if (zoomList[i] > userScale) {
		    return SetZoom(zoomList[i]);
		}
	    }
	    return false;
	}

	public boolean SetNextZoom()
	{
	    for (int i=0; i<zoomList.length; i++) {
		if (zoomList[i] < userScale) {
		    return SetZoom(zoomList[i]);
		}
	    }
	    return false;
	}

	private boolean onTouchEvent(MotionEvent ev) {
	    boolean scaleResult = scaleGestureDetector.onTouchEvent(ev);
	    boolean gestResult = gestureDetector.onTouchEvent(ev);
	    return scaleResult || gestResult;
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

	private void onRestoreInstanceState(Bundle savedInstanceState)
	{
	    logicalOriginX = savedInstanceState.getInt("logicalOriginX", 0);
	    logicalOriginY = savedInstanceState.getInt("logicalOriginY", 0);
	    userScale = savedInstanceState.getFloat("userScale", defaultZoom);
	    SetOriginAndScale();
	}

	private void onSaveInstanceState(Bundle savedInstanceState) {
	    savedInstanceState.putInt("logicalOriginX", logicalOriginX);
	    savedInstanceState.putInt("logicalOriginY", logicalOriginY);
	    savedInstanceState.putFloat("userScale", userScale);
	}

	private void SetOriginAndScale()
	{
	    NativeSetOriginAndScale(nativeHandle, logicalOriginX, logicalOriginY, userScale);
	}
    }

    private class LayerImpl implements Layer
    {
	private int color;
	private int argb;
	private boolean visible;
	private String displayName;
	private String fileName;
	private boolean isDrill;

	public int GetColor() { return argb; }
	public boolean IsVisible() { return visible; }
	public String GetDisplayName() { return displayName; }
    }

    private class LayerManager extends DataSetObservable implements Layers
    {
	private LayerImpl[] layers;
	private int activeLayer;

	private LayerManager(int numberOfLayers)
	{
	    layers = new LayerImpl[numberOfLayers];
	    for (int i=0; i<layers.length; i++) {
		layers[i] = new LayerImpl();
	    }
	}

	public int getLayerCount()
	{
	    return layers.length;
	}

	public Layer getLayer(int layer) {
	    return (layer >= 0 && layer < layers.length? layers[layer] : null);
	}

	public void SetLayerColor(int layer, int color)
	{
	    NativeSetLayerColor(nativeHandle, layer, color);
	    layers[layer].color = color;
	    layers[layer].argb = NativeMakeColour(color);
	    notifyChanged();
	    invalidate();
	}

	public void SetLayerVisible(int layer, boolean visible)
	{
	    NativeSetLayerVisible(nativeHandle, layer, visible);
	    layers[layer].visible = visible;
	    notifyChanged();
	    invalidate();
	}

	private boolean LoadFile(String filename, boolean drill, boolean full)
	{
	    if (full)
		NativeErase_Current_DrawLayer(nativeHandle);
	    boolean result = (filename == null? false :
			      (drill? NativeRead_EXCELLON_File(nativeHandle, filename) :
			       NativeRead_GERBER_File(nativeHandle, filename, filename)));
	    if (result) {
		layers[activeLayer].fileName = filename;
		layers[activeLayer].isDrill = drill;
	    } else {
		layers[activeLayer].fileName = null;
		layers[activeLayer].isDrill = false;
	    }
	    layers[activeLayer].displayName = NativeGetDisplayName(activeLayer);
	    if (full) {
		int layer = NativegetNextAvailableLayer(nativeHandle, activeLayer);
		if (layer >= 0) {
		    activeLayer = layer;
		    NativesetActiveLayer(nativeHandle, layer);
		}
		notifyChanged();
		viewPort.Zoom_Automatique();
	    }
	    return result;
	}

	private boolean LoadFile(File file, boolean drill)
	{
	    return LoadFile(file.getAbsolutePath(), drill, true);
	}

	private boolean LoadFile(String filename, boolean drill)
	{
	    return LoadFile(filename, drill, false);
	}

	public boolean LoadGerber(File file)
	{
	    return LoadFile(file, false);
	}

	public boolean LoadDrill(File file)
	{
	    return LoadFile(file, true);
	}

	public boolean Clear_DrawLayers()
	{
	    boolean result = NativeClear_DrawLayers(nativeHandle);
	    for (int i=0; i<layers.length; i++) {
		layers[i].fileName = null;
		layers[i].isDrill = false;
		layers[i].displayName = NativeGetDisplayName(i);
	    }
	    activeLayer = 0;
	    notifyChanged();
	    invalidate();
	    return result;
	}

	public void setActiveLayer(int layer)
	{
	    activeLayer = layer;
	    NativesetActiveLayer(nativeHandle, layer);
	    notifyChanged();
	    invalidate();
	}

	public int getActiveLayer()
	{
	    return activeLayer;
	}

	private void onRestoreInstanceState(Bundle savedInstanceState, Resources resources)
	{
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
	    String[] layerFileNames = savedInstanceState.getStringArray("layerFileNames");
	    if (layerFileNames == null) {
		layerFileNames = new String[layers.length];
	    }
	    boolean[] layerDrills = savedInstanceState.getBooleanArray("layerDrills");
	    if (layerDrills == null) {
		layerDrills = new boolean[layers.length];
	    }
	    SetLayerFiles(layerFileNames, layerDrills);
	    activeLayer = savedInstanceState.getInt("activeLayer", 0);
	    NativesetActiveLayer(nativeHandle, activeLayer);
	    notifyChanged();
	}

	private void onSaveInstanceState(Bundle savedInstanceState) {
	    int[] layerColors = new int[layers.length];
	    GetLayerColors(layerColors);
	    savedInstanceState.putIntArray("layerColors", layerColors);
	    boolean[] layerVisibilities = new boolean[layers.length];
	    GetLayerVisibilities(layerVisibilities);
	    savedInstanceState.putBooleanArray("layerVisibilities", layerVisibilities);
	    String[] layerFileNames = new String[layers.length];
	    boolean[] layerDrills = new boolean[layers.length];
	    GetLayerFiles(layerFileNames, layerDrills);
	    savedInstanceState.putStringArray("layerFileNames", layerFileNames);
	    savedInstanceState.putBooleanArray("layerDrills", layerDrills);
	    savedInstanceState.putInt("activeLayer", activeLayer);
	}

	private void SetLayerColors(int[] colors)
	{
	    for(int i=0; i<colors.length; i++) {
		NativeSetLayerColor(nativeHandle, i, colors[i]);
		layers[i].color = colors[i];
		layers[i].argb = NativeMakeColour(colors[i]);
	    }
	}

	private void GetLayerColors(int[] colors)
	{
	    for(int i=0; i<colors.length; i++)
		colors[i] = layers[i].color;
	}

	private void SetLayerVisibilities(boolean[] visibilities)
	{
	    for(int i=0; i<visibilities.length; i++) {
		NativeSetLayerVisible(nativeHandle, i, visibilities[i]);
		layers[i].visible = visibilities[i];
	    }
	}

	private void GetLayerVisibilities(boolean[] visibilities)
	{
	    for(int i=0; i<visibilities.length; i++)
		visibilities[i] = layers[i].visible;
	}

	private void SetLayerFiles(String[] layerFileNames, boolean[] layerDrills)
	{
	    NativeClear_DrawLayers(nativeHandle);
	    for (activeLayer=0; activeLayer<layers.length; activeLayer++) {
		NativesetActiveLayer(nativeHandle, activeLayer);
		LoadFile(layerFileNames[activeLayer], layerDrills[activeLayer]);
	    }
	}

	private void GetLayerFiles(String[] layerFileNames, boolean[] layerDrills)
	{
	    for (int i=0; i<layers.length; i++) {
		layerFileNames[i] = layers[i].fileName;
		layerDrills[i] = layers[i].isDrill;
	    }
	}
    }

    private class DisplayOptionsImpl implements DisplayOptions
    {
	private int[] visibleElementColor;
	private int[] visibleElementArgb;
	private int displayFlags;
	private int displayMode;

	private static final int FLAG_DISPLAY_DCODES = 1;
	private static final int FLAG_DISPLAY_FLASHED_ITEMS_FILL = 2;
	private static final int FLAG_DISPLAY_LINES_FILL = 4;
	private static final int FLAG_DISPLAY_NEGATIVE_OBJECTS = 8;
	private static final int FLAG_DISPLAY_POLYGONS_FILL = 16;
	private static final int FLAG_DISPLAY_GRID = 32;

	private DisplayOptionsImpl()
	{
	    Resources resources = getContext().getResources();
	    final int numberOfVisibleElements = resources.getInteger(R.integer.number_of_visible_elements);
	    visibleElementColor = new int[numberOfVisibleElements];
	    visibleElementArgb = new int[numberOfVisibleElements];
	}

	public boolean GetDisplayDCodesFlag()
	{
	    return GetFlag(FLAG_DISPLAY_DCODES);
	}

	public void SetDisplayDCodesFlag(boolean flag)
	{
	    SetFlag(FLAG_DISPLAY_DCODES, flag);
	}

	public boolean GetDisplayFlashedItemsFillFlag()
	{
	    return GetFlag(FLAG_DISPLAY_FLASHED_ITEMS_FILL);
	}

	public void SetDisplayFlashedItemsFillFlag(boolean flag)
	{
	    SetFlag(FLAG_DISPLAY_FLASHED_ITEMS_FILL, flag);
	}

	public boolean GetDisplayLinesFillFlag()
	{
	    return GetFlag(FLAG_DISPLAY_LINES_FILL);
	}

	public void SetDisplayLinesFillFlag(boolean flag)
	{
	    SetFlag(FLAG_DISPLAY_LINES_FILL, flag);
	}

	public boolean GetDisplayNegativeObjectsFlag()
	{
	    return GetFlag(FLAG_DISPLAY_NEGATIVE_OBJECTS);
	}

	public void SetDisplayNegativeObjectsFlag(boolean flag)
	{
	    SetFlag(FLAG_DISPLAY_NEGATIVE_OBJECTS, flag);
	}

	public boolean GetDisplayPolygonsFillFlag()
	{
	    return GetFlag(FLAG_DISPLAY_POLYGONS_FILL);
	}

	public void SetDisplayPolygonsFillFlag(boolean flag)
	{
	    SetFlag(FLAG_DISPLAY_POLYGONS_FILL, flag);
	}

	public boolean GetDisplayGridFlag()
	{
	    return GetFlag(FLAG_DISPLAY_GRID);
	}

	public void SetDisplayGridFlag(boolean flag)
	{
	    SetFlag(FLAG_DISPLAY_GRID, flag);
	}

	public int GetDisplayMode()
	{
	    return displayMode;
	}

	public void SetDisplayMode(int mode)
	{
	    if (mode != displayMode) {
		displayMode = mode;
		NativeSetDisplayMode(nativeHandle, displayMode);
		invalidate();
	    }
	}

	public int GetDCodesVisibleColorARGB()
	{
	    return GetVisibleElementColorARGB(R.integer.DCODES_VISIBLE);
	}

	public void SetDCodesVisibleColor(int color)
	{
	    SetVisibleElementColor(R.integer.DCODES_VISIBLE, color);
	}

	public int GetGerberGridVisibleColorARGB()
	{
	    return GetVisibleElementColorARGB(R.integer.GERBER_GRID_VISIBLE);
	}

	public void SetGerberGridVisibleColor(int color)
	{
	    SetVisibleElementColor(R.integer.GERBER_GRID_VISIBLE, color);
	}

	public int GetNegativeObjectsVisibleColorARGB()
	{
	    return GetVisibleElementColorARGB(R.integer.NEGATIVE_OBJECTS_VISIBLE);
	}

	public void SetNegativeObjectsVisibleColor(int color)
	{
	    SetVisibleElementColor(R.integer.NEGATIVE_OBJECTS_VISIBLE, color);
	}

	private void onRestoreInstanceState(Bundle savedInstanceState, Resources resources)
	{
	    int[] visibleElementColors = savedInstanceState.getIntArray("visibleElementColors");
	    if (visibleElementColors == null)
		visibleElementColors = resources.getIntArray(R.array.default_visible_element_colors);
	    if (visibleElementColors != null)
		SetVisibleElementColors(visibleElementColors);
	    displayFlags = savedInstanceState.getInt("displayFlags", resources.getInteger(R.integer.default_display_flags));
	    NativeSetDisplayFlags(nativeHandle, displayFlags);
	    displayMode = savedInstanceState.getInt("displayMode", resources.getInteger(R.integer.default_display_mode));
	    NativeSetDisplayMode(nativeHandle, displayMode);
	}

	private void onSaveInstanceState(Bundle savedInstanceState) {
	    Resources resources = getContext().getResources();
	    int[] visibleElementColors = new int[resources.getInteger(R.integer.number_of_visible_elements)];
	    GetVisibleElementColors(visibleElementColors);
	    savedInstanceState.putIntArray("visibleElementColors", visibleElementColors);
	    savedInstanceState.putInt("displayFlags", displayFlags);
	    savedInstanceState.putInt("displayMode", displayMode);
	}

	private void SetVisibleElementColors(int[] colors)
	{
	    for(int i=0; i<colors.length; i++) {
		NativeSetVisibleElementColor(nativeHandle, i+1, colors[i]);
		visibleElementColor[i] = colors[i];
		visibleElementArgb[i] = NativeMakeColour(colors[i]);
	    }
	}

	private void GetVisibleElementColors(int[] colors)
	{
	    for(int i=0; i<colors.length; i++)
		colors[i] = visibleElementColor[i];
	}

	private int GetVisibleElementColorARGB(int id)
	{
	    final int visible = getContext().getResources().getInteger(id);
	    return visibleElementArgb[visible-1];
	}

	private void SetVisibleElementColor(int id, int color)
	{
	    final int visible = getContext().getResources().getInteger(id);
	    NativeSetVisibleElementColor(nativeHandle, visible, color);
	    visibleElementColor[visible-1] = color;
	    visibleElementArgb[visible-1] = NativeMakeColour(color);
	    invalidate();
	}

	private boolean GetFlag(int flag)
	{
	    return (displayFlags & flag) != 0;
	}

	private void SetFlag(int flag, boolean value)
	{
	    final boolean oldValue = ((displayFlags & flag) != 0);
	    if (value != oldValue) {
		displayFlags ^= flag;
		NativeSetDisplayFlags(nativeHandle, displayFlags);
		invalidate();
	    }
	}
    }

    private long nativeHandle;

    private LayerManager layerManager;
    private ViewPortImpl viewPort;
    private DisplayOptionsImpl displayOptions;

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
	return viewPort.onTouchEvent(ev) || super.onTouchEvent(ev);
    }

    public void onCreate()
    {
	Resources resources = getContext().getResources();
	nativeHandle = NativeCreate();
	layerManager = new LayerManager(resources.getInteger(R.integer.number_of_layers));
	viewPort = new ViewPortImpl();
	displayOptions = new DisplayOptionsImpl();
	setLayerType(LAYER_TYPE_SOFTWARE, null);
    }

    public void onDestroy()
    {
	final long handle = nativeHandle;
	nativeHandle = 0;
	NativeDestroy(handle);
	layerManager = null;
	viewPort = null;
	displayOptions = null;
    }

    public void onRestoreInstanceState(Bundle savedInstanceState)
    {
	Resources resources = getContext().getResources();
	layerManager.onRestoreInstanceState(savedInstanceState, resources);
	viewPort.onRestoreInstanceState(savedInstanceState);
	displayOptions.onRestoreInstanceState(savedInstanceState, resources);
    }

    public void onSaveInstanceState(Bundle savedInstanceState) {
	layerManager.onSaveInstanceState(savedInstanceState);
	viewPort.onSaveInstanceState(savedInstanceState);
	displayOptions.onSaveInstanceState(savedInstanceState);
    }

    public Layers getLayers() {
	return layerManager;
    }

    public ViewPort getViewPort() {
	return viewPort;
    }

    public DisplayOptions getDisplayOptions() {
	return displayOptions;
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
    private native void NativeSetVisibleElementColor(long handle, int layer, int color);
    private native boolean NativeRead_GERBER_File(long handle, String GERBER_FullFileName, String D_Code_FullFileName);
    private native boolean NativeRead_EXCELLON_File(long handle, String EXCELLON_FullFileName);
    private native boolean NativeClear_DrawLayers(long handle);
    private native void NativeErase_Current_DrawLayer(long handle);
    private native void NativeOnDraw(long handle, Canvas canvas, boolean eraseBg);
    private native void NativeSetOriginAndScale(long handle, int logicalOriginX, int logicalOriginY, float userScale);
    private native void NativeSetLayerVisible(long handle, int layer, boolean visible);
    private native void NativesetActiveLayer(long handle, int layer);
    private native int NativegetNextAvailableLayer(long handle, int layer);
    private native Rect NativeComputeBoundingBox(long handle);
    private native void NativeSetDisplayFlags(long handle, int flags);
    private native void NativeSetDisplayMode(long handle, int mode);
    private native static String NativeGetDisplayName(int layer);
    private native static int NativeMakeColour(int color);
    private native static String NativeColorGetName(int color);
};
