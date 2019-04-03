package se.pp.mc.android.Gerberoid;

import android.content.Context;
import android.widget.ArrayAdapter;

public class GerberZipEntryAdapter extends ArrayAdapter<ArchiveActivity.GerberZipEntryType> {

    public GerberZipEntryAdapter(Context context) {
        super(context, android.R.layout.simple_spinner_item, ArchiveActivity.GerberZipEntryType.values());
    }

}
