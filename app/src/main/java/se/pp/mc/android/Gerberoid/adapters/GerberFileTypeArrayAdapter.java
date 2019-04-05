package se.pp.mc.android.Gerberoid.adapters;

import android.content.Context;
import android.widget.ArrayAdapter;

import se.pp.mc.android.Gerberoid.model.FileType;

public class GerberFileTypeArrayAdapter extends ArrayAdapter<FileType> {

    public GerberFileTypeArrayAdapter(Context context) {
        super(context, android.R.layout.simple_spinner_dropdown_item, FileType.values());
    }

}
