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
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Parcelable;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
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

import com.kennyc.bottomsheet.BottomSheet;
import com.kennyc.bottomsheet.BottomSheetListener;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    private static final int REQUEST_GERBER = 10001;
    private static final int REQUEST_DRILL = 10002;
    private static final int REQUEST_ARCHIVE = 10003;
    private static final int REQUEST_SELECT_ARCHIVE = 10004;

    private GerberViewer gerber;
    private Layers layers;
    private DisplayOptions displayOptions;
    private ViewPort viewPort;
    private Spinner layerSpinner;
    private DrawerLayout drawerLayout;
    private View toolsDrawer;
    private View progress;

    private View btnAdd;
    private View btnClear;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        toolbar = (Toolbar) findViewById(R.id.toolbar_bottom);

        btnAdd = findViewById(R.id.btnAdd);
        btnAdd.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                addLayers();
            }

        });

        btnClear = findViewById(R.id.btnClear);
        btnClear.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                clearLayers();
            }

        });

        progress = findViewById(R.id.progress);

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

    private void addLayers() {

        new BottomSheet.Builder(this)
                .setSheet(R.menu.bottom_toolbar)
                .setTitle("Add Layer")
                .setListener(new BottomSheetListener() {

                    @Override
                    public void onSheetShown(@NonNull BottomSheet bottomSheet, @Nullable Object object) {

                    }

                    @Override
                    public void onSheetItemSelected(@NonNull BottomSheet bottomSheet, MenuItem item, @Nullable Object object) {

                        switch (item.getItemId()) {

                            case R.id.action_gerber:
                                SelectFile(REQUEST_GERBER);
                                break;
                            case R.id.action_drill:
                                SelectFile(REQUEST_DRILL);
                                break;
                            case R.id.action_archive:
                                SelectFile(REQUEST_ARCHIVE);
                                break;


                        }

                    }

                    @Override
                    public void onSheetDismissed(@NonNull BottomSheet bottomSheet, @Nullable Object object, int dismissEvent) {

                    }

                }).show();

    }

    private void clearLayers() {
        layers.Clear_DrawLayers();
        layerSpinner.setSelection(layers.getActiveLayer());
    }

    private void SelectFile(Parcelable[] files) {

        if (files != null && files.length > 0) {

            boolean success = true;
            for (Parcelable f : files) {
                success &= LoadThing(this, layers, (ArchiveActivity.GerberZipEntry) f);
            }

            if (!success) {

                new AlertDialog.Builder(MainActivity.this)
                        .setTitle("Error")
                        .setMessage("Selected files could not be loaded!")
                        .setPositiveButton("OK", null)
                        .show();

            }

        }

    }

    private void SelectFile(int requestCode) {

        final Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType(requestCode == REQUEST_ARCHIVE ? "application/zip" : "*/*");
        startActivityForResult(intent, requestCode);

    }

    private static class LoadTask extends AsyncTask<Uri, Void, File> {

        private Layers layers;
        private int requestCode;
        private Callback callback;

        private LoadTask(Layers layers, int requestCode, Callback callback) {
            this.layers = layers;
            this.requestCode = requestCode;
            this.callback = callback;
        }

        @Override
        protected File doInBackground(Uri... uris) {
            return toPrivateFile(GerberoidApplication.get().getApplicationContext(), uris[0], this.layers.getActiveLayer());
        }

        @Override
        protected void onPreExecute() {
            callback.onStarted();
            super.onPreExecute();
        }

        @Override
        protected void onPostExecute(File file) {

            boolean success = false;

            if (file != null) {

                try {

                    if (requestCode == REQUEST_GERBER) {
                        success = layers.LoadGerber(file);
                    } else {
                        success = layers.LoadDrill(file);
                    }

                    callback.onFinished(success);

                } catch (Exception e) {
                    e.printStackTrace();
                }

                file.delete();

            }

            callback.onFinished(success);

        }

        private interface Callback {
            void onStarted();

            void onFinished(boolean success);
        }

    }

    private static boolean LoadThing(Context context, Layers layers, ArchiveActivity.GerberZipEntry gerberZipEntry) {

        boolean success = false;

        final int index = layers.getActiveLayer();
        final File file = toPrivateFile(context, gerberZipEntry.getFile(), index);
        if (file == null) {
            return false;
        }

        try {

            if (gerberZipEntry.getType() == ArchiveActivity.GerberZipEntryType.GERBER) {
                success = layers.LoadGerber(file);
            } else {
                success = layers.LoadDrill(file);
            }

        } catch (Exception e) {
            e.printStackTrace();
        }

        return success;

    }

    private static boolean LoadThing(Context context, Layers layers, int requestCode, Uri uri) {

        boolean success = false;

        final int index = layers.getActiveLayer();
        final File file = toPrivateFile(context, uri, index);
        if (file == null) {
            return false;
        }

        try {

            if (requestCode == REQUEST_GERBER) {
                success = layers.LoadGerber(file);
            } else {
                success = layers.LoadDrill(file);
            }

        } catch (Exception e) {
            e.printStackTrace();
        }

        return success;

    }

    private final LoadTask.Callback mLoadCallback = new LoadTask.Callback() {

        @Override
        public void onStarted() {
            progress.setVisibility(View.VISIBLE);
        }

        @Override
        public void onFinished(boolean success) {
            progress.setVisibility(View.GONE);

            if (!success) {

                new AlertDialog.Builder(MainActivity.this)
                        .setTitle("Error")
                        .setMessage("Selected file could not be loaded!")
                        .setPositiveButton("OK", null)
                        .show();

            }

        }

    };

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case REQUEST_SELECT_ARCHIVE:

                if (resultCode == RESULT_OK) {

                    Parcelable[] files = data.getParcelableArrayExtra("extra.files");
                    if (files != null) {
                        SelectFile(files);
                    }

                }
                break;

            case REQUEST_ARCHIVE:

                if (resultCode == RESULT_OK) {
                    startActivityForResult(ArchiveActivity.newInstance(this, data.getData()), REQUEST_SELECT_ARCHIVE);
                }

                break;

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
                            if (!LoadThing(this, layers, requestCode,
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
                        new LoadTask(layers, requestCode, mLoadCallback).execute(data.getData());
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

    public static File toPrivateFile(Context context, Uri uri, int currentLayer) {

        File dataDir = context.getFilesDir();
        File output = new File(dataDir.getAbsolutePath() + "/layers/" + currentLayer + ".layer");
        FileUtils.copyFile(context, uri, output);
        return output;

    }

    public static File toPrivateFile(Context context, File file, int currentLayer) {

        File dataDir = context.getFilesDir();
        File output = new File(dataDir.getAbsolutePath() + "/layers/" + currentLayer + ".layer");
        FileUtils.copyFile(file, output);
        return output;

    }

}


