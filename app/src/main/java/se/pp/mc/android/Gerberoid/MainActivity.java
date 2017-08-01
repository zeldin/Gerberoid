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

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.widget.DrawerLayout;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MenuInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Spinner;

import com.ipaulpro.afilechooser.utils.FileUtils;

import java.io.File;


public class MainActivity extends Activity {


    private static final int REQUEST_GERBER = 10001;
    private static final int REQUEST_DRILL  = 10002;

    private GerberViewer gerber;
    private Layers layers;
    private ViewPort viewPort;
    private Spinner layerSpinner;
    private DrawerLayout drawerLayout;
    private View toolsDrawer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        gerber = (GerberViewer) findViewById(R.id.gerber_viewer);
        if (gerber != null) {
	    gerber.onCreate();
	    gerber.onRestoreInstanceState((savedInstanceState == null? new Bundle() : savedInstanceState));
	    layers = gerber.getLayers();
	    viewPort = gerber.getViewPort();
	} else {
	    layers = null;
	    viewPort = null;
	}
	layerSpinner = (Spinner) findViewById(R.id.layer_spinner);
	if (layerSpinner != null) {
	    layerSpinner.setAdapter(new LayerSpinnerAdapter(this, layers));
	    layerSpinner.setSelection(layers.getActiveLayer());
	    layerSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
		    @Override
		    public void onItemSelected(AdapterView<?> parent, View view,
					       int pos, long id) {
			layers.setActiveLayer(pos);
		    }

		    @Override
		    public void onNothingSelected(AdapterView<?> parent) {
		    }
		});
	}
	drawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);
	toolsDrawer = findViewById(R.id.tools_drawer);
	if (toolsDrawer != null)
	    new ToolsDrawer(toolsDrawer);
    }

    @Override
    protected void onSaveInstanceState(Bundle savedInstanceState) {
	super.onSaveInstanceState(savedInstanceState);
	if (gerber != null)
	    gerber.onSaveInstanceState(savedInstanceState);
    }

    @Override
    protected void onDestroy ()
    {
	drawerLayout = null;
	toolsDrawer = null;
	layerSpinner = null;
	layers = null;
	viewPort = null;
	if (gerber != null) {
	    gerber.onDestroy();
	    gerber = null;
	}
	super.onDestroy();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.action_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
	switch (item.getItemId()) {
	case R.id.action_settings:
	    if (drawerLayout.isDrawerOpen(toolsDrawer))
		drawerLayout.closeDrawer(toolsDrawer);
	    else
		drawerLayout.openDrawer(toolsDrawer);
	    break;
	case R.id.action_gerber:
	    SelectFile(REQUEST_GERBER, R.string.file_request_gerber,
		       "application/vnd.gerber");
	    break;
	case R.id.action_drill:
	    SelectFile(REQUEST_DRILL, R.string.file_request_drill,
		       "application/octet-stream");
	    break;
	case R.id.action_clear:
	    layers.Clear_DrawLayers();
	    layerSpinner.setSelection(layers.getActiveLayer());
	    break;
	case R.id.action_zoom_in:
	    viewPort.SetPreviousZoom();
	    break;
	case R.id.action_zoom_out:
	    viewPort.SetNextZoom();
	    break;
	case R.id.action_zoom_fit:
	    viewPort.Zoom_Automatique();
	    break;
	case R.id.action_about:
	    String version = "?";
	    try {
		PackageInfo pInfo = getPackageManager().getPackageInfo(getPackageName(), 0);
		version = pInfo.versionName;
	    } catch(PackageManager.NameNotFoundException e) {}
	    AlertDialog.Builder builder = new AlertDialog.Builder(this);
	    builder.setTitle(R.string.action_about)
		.setMessage(getResources().getString(R.string.about_text, version))
		.create()
		.show();
	    break;
	}
	return true;
    }

    private void SelectFile(int requestCode, int titleResource,
			    String mimeType)
    {
	final Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
	intent.setType(mimeType);
	intent.addCategory(Intent.CATEGORY_OPENABLE);
	startActivityForResult(Intent.createChooser(intent,
						    getResources().getString(titleResource)),
			       requestCode);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
	switch (requestCode) {
	case REQUEST_GERBER:
	case REQUEST_DRILL:
	    if (resultCode == RESULT_OK) {
		final Uri uri = data.getData();
		final File file = FileUtils.getFile(this, uri);
		if (file != null) {
		    if (requestCode == REQUEST_GERBER)
			layers.LoadGerber(file);
		    else
			layers.LoadDrill(file);
		    layerSpinner.setSelection(layers.getActiveLayer());
		}
	    }
	    break;
	}
    }
}
