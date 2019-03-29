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

import android.app.AlertDialog;
import android.content.ClipData;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Spinner;
import android.widget.Toast;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;


public class MainActivity extends AppCompatActivity implements Toolbar.OnMenuItemClickListener {

    private static final int REQUEST_GERBER = 10001;
    private static final int REQUEST_DRILL = 10002;

    private GerberViewer gerber;
    private Layers layers;
    private DisplayOptions displayOptions;
    private ViewPort viewPort;
    private Spinner layerSpinner;
    private DrawerLayout drawerLayout;
    private View toolsDrawer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        toolbar = (Toolbar) findViewById(R.id.toolbar_bottom);
        toolbar.setOnMenuItemClickListener(this);
        toolbar.inflateMenu(R.menu.bottom_toolbar);

        gerber = (GerberViewer) findViewById(R.id.gerber_viewer);
        if (gerber != null) {
            gerber.onCreate();
            gerber.onRestoreInstanceState((savedInstanceState == null ? new Bundle() : savedInstanceState));
            layers = gerber.getLayers();
            viewPort = gerber.getViewPort();
            displayOptions = gerber.getDisplayOptions();
        } else {
            layers = null;
            viewPort = null;
            displayOptions = null;
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
            new ToolsDrawer(this, toolsDrawer, displayOptions);
    }

    @Override
    protected void onSaveInstanceState(Bundle savedInstanceState) {
        super.onSaveInstanceState(savedInstanceState);
        if (gerber != null)
            gerber.onSaveInstanceState(savedInstanceState);
    }

    @Override
    protected void onDestroy() {
        drawerLayout = null;
        toolsDrawer = null;
        layerSpinner = null;
        layers = null;
        viewPort = null;
        displayOptions = null;
        if (gerber != null) {
            gerber.onDestroy();
            gerber = null;
        }
        super.onDestroy();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.action_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.action_settings:
                if (drawerLayout.isDrawerOpen(toolsDrawer))
                    drawerLayout.closeDrawer(toolsDrawer);
                else
                    drawerLayout.openDrawer(toolsDrawer);
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
                } catch (PackageManager.NameNotFoundException e) {
                }
                AlertDialog.Builder builder = new AlertDialog.Builder(this);
                builder.setTitle(R.string.action_about)
                        .setMessage(getResources().getString(R.string.about_text, version))
                        .create()
                        .show();
                break;
        }
        return true;
    }

    @Override
    public boolean onMenuItemClick(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.action_gerber:
                SelectFile(REQUEST_GERBER);
                break;
            case R.id.action_drill:
                SelectFile(REQUEST_DRILL);
                break;
            case R.id.action_clear:
                layers.Clear_DrawLayers();
                layerSpinner.setSelection(layers.getActiveLayer());
                break;
        }
        return true;
    }

    private void SelectFile(int requestCode) {

        final Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*");
        startActivityForResult(intent, requestCode);

    }

    private boolean LoadThing(int requestCode, Uri uri) {

        // TODO: make this async, show loading indicator
        final File file = toPrivateFile(uri);
        if (file == null) {
            return false;
        }

        boolean success;
        if (requestCode == REQUEST_GERBER) {
            success = layers.LoadGerber(file);
        } else {
            success = layers.LoadDrill(file);
        }

        file.delete();

        return success;

    }



    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case REQUEST_GERBER:
            case REQUEST_DRILL:
                if (resultCode == RESULT_OK) {
                    ClipData clipData = null;
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
                        clipData = data.getClipData();
                    if (clipData != null) {
                        final int n = clipData.getItemCount();
                        for (int i = 0; i < n; i++) {
                            final int oldActiveLayer = layers.getActiveLayer();
                            if (!LoadThing(requestCode,
                                    clipData.getItemAt(i).getUri()))
                                break;
                            if (layers.getActiveLayer() == oldActiveLayer &&
                                    i + 1 < n) {

                                Toast.makeText(this, R.string.no_more_empty_layers,
                                        Toast.LENGTH_LONG).show();
                                break;
                            }
                        }
                    } else {
                        LoadThing(requestCode, data.getData());
                    }
                    layerSpinner.setSelection(layers.getActiveLayer());
                }
                break;
        }
    }

    Layers getLayers() {
        return layers;
    }

    DisplayOptions getDisplayOptions() {
        return displayOptions;
    }

    private File toPrivateFile(Uri uri) {

        try {

            InputStream inputStream = new BufferedInputStream(getContentResolver().openInputStream(uri));
            File tmp = File.createTempFile("gerber", null);
            writeToFile(inputStream, tmp);
            return tmp;

        } catch (IOException e) {
            return null;
        }

    }

    private static void writeToFile(InputStream is, File file) throws IOException {

        OutputStream outputStream = null;

        try {
            outputStream = new FileOutputStream(file);

            int read = 0;
            byte[] bytes = new byte[1024];
            while ((read = is.read(bytes)) != -1) {
                outputStream.write(bytes, 0, read);
            }
        } finally {
            if (outputStream != null) {
                outputStream.close();
            }
        }
    }

}


