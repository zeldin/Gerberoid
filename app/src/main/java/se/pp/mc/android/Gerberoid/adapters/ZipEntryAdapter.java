package se.pp.mc.android.Gerberoid.adapters;

import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.Spinner;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

import se.pp.mc.android.Gerberoid.R;
import se.pp.mc.android.Gerberoid.model.GerberZipEntry;
import se.pp.mc.android.Gerberoid.model.FileType;

public class ZipEntryAdapter extends RecyclerView.Adapter<ZipEntryAdapter.ZipEntryViewHolder> {

    private final List<GerberZipEntry> entries = new ArrayList<GerberZipEntry>();

    @NonNull
    @Override
    public ZipEntryViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
        return new ZipEntryViewHolder(LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.item_archive_entry, viewGroup, false));
    }

    @Override
    public void onBindViewHolder(@NonNull ZipEntryViewHolder zipEntryViewHolder, int i) {
        zipEntryViewHolder.bind(entries.get(i));
    }

    @Override
    public int getItemCount() {
        return entries.size();
    }

    public List<GerberZipEntry> getSelectedData() {

        List<GerberZipEntry> output = new ArrayList<>();

        for(GerberZipEntry e : entries){
            if(e.isSelected()) {
                output.add(e);
            }
        }

        return output;

    }

    public void setData(List<GerberZipEntry> entries){

        this.entries.clear();

        if(entries != null) {
            this.entries.addAll(entries);
        }

        this.notifyDataSetChanged();

    }

    static class ZipEntryViewHolder extends RecyclerView.ViewHolder {

        private final CheckBox cbSelected;
        private final TextView tvName;
        private final Spinner spType;

        private GerberZipEntry entry;

        public ZipEntryViewHolder(@NonNull View itemView) {
            super(itemView);

            this.cbSelected = itemView.findViewById(R.id.cbSelected);
            this.cbSelected.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

                @Override
                public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                    entry.setSelected(isChecked);
                }

            });

            this.tvName = itemView.findViewById(R.id.tvName);

            this.spType = itemView.findViewById(R.id.spType);
            this.spType.setAdapter(new GerberFileTypeArrayAdapter(itemView.getContext()));
            this.spType.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {

                @Override
                public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                    entry.setType((FileType) spType.getItemAtPosition(position));
                }

                @Override
                public void onNothingSelected(AdapterView<?> parent) {

                }

            });

        }

        private void bind(GerberZipEntry entry){

            this.entry = entry;
            this.cbSelected.setChecked(entry.isSelected());
            this.tvName.setText(entry.getName());
            this.spType.setSelection(((GerberFileTypeArrayAdapter)spType.getAdapter()).getPosition(entry.getType()));

        }

    }

}
