package se.pp.mc.android.Gerberoid.activities;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Toast;

import java.io.Serializable;
import java.util.List;

import se.pp.mc.android.Gerberoid.R;
import se.pp.mc.android.Gerberoid.adapters.ZipEntryAdapter;
import se.pp.mc.android.Gerberoid.model.GerberZipEntry;
import se.pp.mc.android.Gerberoid.tasks.ExtractZipTask;
import se.pp.mc.android.Gerberoid.tasks.ExtractZipTaskCallback;

public class ArchiveActivity extends AppCompatActivity {

    public static final String EXTRA_FILES = "extra.files";
    private static final String EXTRA_URI = "extra.uri";

    private ZipEntryAdapter adapter = new ZipEntryAdapter();
    private RecyclerView list;
    private View progress;
    private View tvNoData;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_archive);

        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setHomeButtonEnabled(true);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        setTitle(R.string.archive_title);

        this.list = findViewById(R.id.list);
        this.progress = findViewById(R.id.progress);
        this.tvNoData = findViewById(R.id.tvNoData);

        this.list.setAdapter(adapter);
        this.list.setLayoutManager(new LinearLayoutManager(this));

        setResult(RESULT_CANCELED);
        Uri uri = getIntent().getParcelableExtra(EXTRA_URI);
        if(uri == null){
            this.finish();
        }

        new ExtractZipTask(callback).execute(uri);

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
                data.putExtra(EXTRA_FILES, new ResultSet(adapter.getSelectedData().toArray(new GerberZipEntry[0])));
                setResult(RESULT_OK, data);
                finish();

                return true;

            default:
                return super.onOptionsItemSelected(item);

        }

    }

    public static Intent newInstance(Context context, Uri uri) {

        Intent intent = new Intent(context, ArchiveActivity.class);
        intent.putExtra(EXTRA_URI, uri);
        return intent;

    }

    private final ExtractZipTaskCallback callback = new ExtractZipTaskCallback() {

        @Override
        public void onStarted() {
            progress.setVisibility(View.VISIBLE);
        }

        @Override
        public void onFinished(List<GerberZipEntry> zipEntries) {

            if(zipEntries == null){
                Toast.makeText(ArchiveActivity.this, getString(R.string.no_archive_data_found), Toast.LENGTH_SHORT).show();
            }

            adapter.setData(zipEntries);
            progress.setVisibility(View.GONE);
            tvNoData.setVisibility(zipEntries == null || zipEntries.size() == 0 ? View.VISIBLE : View.GONE);

        }

    };

    public static class ResultSet implements Serializable {

        private GerberZipEntry[] data;

        public ResultSet(GerberZipEntry[] data) {
            this.data = data;
        }

        public GerberZipEntry[] getData() {
            return data;
        }

    }

}
