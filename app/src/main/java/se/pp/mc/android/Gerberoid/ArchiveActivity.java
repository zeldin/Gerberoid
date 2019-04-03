package se.pp.mc.android.Gerberoid;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Parcel;
import android.os.Parcelable;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import se.pp.mc.android.Gerberoid.adapter.ZipEntryAdapter;

public class ArchiveActivity extends AppCompatActivity {

    private ZipEntryAdapter adapter = new ZipEntryAdapter();
    private RecyclerView list;
    private View progress;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_archive);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setHomeButtonEnabled(true);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        setTitle("Archive");

        this.list = findViewById(R.id.list);
        this.progress = findViewById(R.id.progress);

        this.list.setAdapter(adapter);
        this.list.setLayoutManager(new LinearLayoutManager(this));

        setResult(RESULT_CANCELED);
        Uri uri = getIntent().getParcelableExtra("extra.uri");
        if(uri == null){
            this.finish();
        }

        new ZipTask().execute(uri);

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.archive_toolbar, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        switch (item.getItemId()) {

            case android.R.id.home:
                this.finish();

            case R.id.action_add:

                final Intent data = new Intent();
                data.putExtra("extra.files", adapter.getSelectedData().toArray(new GerberZipEntry[0]));
                setResult(RESULT_OK, data);
                finish();

                return true;

            default:
                return super.onOptionsItemSelected(item);

        }

    }

    public static Intent newInstance(Context context, Uri uri) {

        Intent intent = new Intent(context, ArchiveActivity.class);
        intent.putExtra("extra.uri", uri);
        return intent;

    }

    private class ZipTask extends AsyncTask<Uri, Object, List<GerberZipEntry>>{

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            ArchiveActivity.this.progress.setVisibility(View.VISIBLE);
        }

        @Override
        protected List<GerberZipEntry> doInBackground(Uri... uris) {

            final List<GerberZipEntry> output = new ArrayList<>();

            File zipFile = FileUtils.writeUriToTempFile(ArchiveActivity.this, uris[0], "archive_");
            if(zipFile == null){
                return null;
            }

            File outputDir = new File(ArchiveActivity.this.getCacheDir(), zipFile.getName().replace(".tmp", ""));

            if(!outputDir.exists()) {
                if (!outputDir.mkdirs()){
                    return null;
                }
            }

            File[] allFiles = outputDir.listFiles();
            if(allFiles != null && allFiles.length > 0) {

                for (File f : allFiles) {
                    f.delete();
                }
            }

            FileUtils.unpackZip(outputDir.getAbsolutePath(), zipFile.getAbsolutePath());

            allFiles = outputDir.listFiles();
            if(allFiles != null && allFiles.length > 0){

                for(File f : allFiles) {
                    output.add(new GerberZipEntry(f.getName(), f.getName().endsWith(".drl") ? GerberZipEntryType.DRILL : GerberZipEntryType.GERBER, !(f.getName().endsWith(".ps") || f.getName().endsWith(".rpt")), f));
                }

            }

            return output;

        }

        @Override
        protected void onPostExecute(List<GerberZipEntry> zipEntries) {
            super.onPostExecute(zipEntries);
            ArchiveActivity.this.adapter.setData(zipEntries);
            ArchiveActivity.this.progress.setVisibility(View.GONE);
        }

    }

    public enum GerberZipEntryType {
        GERBER, DRILL
    }

    public static class GerberZipEntry implements Parcelable {

        private final String name;
        private GerberZipEntryType type;
        private boolean selected;
        private final File file;

        public GerberZipEntry(String name, GerberZipEntryType type, boolean selected, File file) {
            this.name = name;
            this.type = type;
            this.selected = selected;
            this.file = file;
        }

        public boolean isSelected() {
            return selected;
        }

        public String getName() {
            return name;
        }

        public GerberZipEntryType getType() {
            return type;
        }

        public File getFile() {
            return file;
        }

        public void setSelected(boolean selected) {
            this.selected = selected;
        }

        public void setType(GerberZipEntryType type) {
            this.type = type;
        }

        @Override
        public int describeContents() {
            return 0;
        }

        @Override
        public void writeToParcel(Parcel dest, int flags) {
            dest.writeString(this.name);
            dest.writeInt(this.type == null ? -1 : this.type.ordinal());
            dest.writeByte(this.selected ? (byte) 1 : (byte) 0);
            dest.writeSerializable(this.file);
        }

        protected GerberZipEntry(Parcel in) {
            this.name = in.readString();
            int tmpType = in.readInt();
            this.type = tmpType == -1 ? null : GerberZipEntryType.values()[tmpType];
            this.selected = in.readByte() != 0;
            this.file = (File) in.readSerializable();
        }

        public static final Creator<GerberZipEntry> CREATOR = new Creator<GerberZipEntry>() {
            @Override
            public GerberZipEntry createFromParcel(Parcel source) {
                return new GerberZipEntry(source);
            }

            @Override
            public GerberZipEntry[] newArray(int size) {
                return new GerberZipEntry[size];
            }
        };

    }

}
