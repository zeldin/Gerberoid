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
import android.content.Context;
import android.database.DataSetObserver;
import android.graphics.drawable.ColorDrawable;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CompoundButton;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.ToggleButton;

class LayerSpinnerAdapter extends BaseAdapter {

    private Context context;
    private Layers layers;

    public LayerSpinnerAdapter(Context context, Layers layers)
    {
	this.context = context;
	this.layers = layers;
    }

    @Override
    public int getCount() {
	return layers.getLayerCount();
    }

    @Override
    public Object getItem(int position) {
	return layers.getLayer(position);
    }

    @Override
    public long getItemId(int position) {
	return layers.getLayer(position).hashCode();
    }

    @Override
    public View getView(final int position, View view, ViewGroup parent)
    {
	if (view == null)
	    view = ((Activity)context).getLayoutInflater().inflate(R.layout.layer_spinner_entry, parent, false);
	final Layer layer = layers.getLayer(position);
	TextView text = (TextView) view.findViewById(R.id.layer_name);
	text.setText(layer.GetDisplayName());
	ImageButton button = (ImageButton) view.findViewById(R.id.layer_color);
	button.setImageDrawable(new ColorDrawable(layer.GetColor()));
	button.setOnClickListener(new View.OnClickListener() {
		@Override
		public void onClick(View v) {
		    new ColorSelectorDialogFragment() {
			@Override
			public void onColorSelected(int color, int argb) {
			    layers.SetLayerColor(position, color);
			}
		    }.show(((Activity)context).getFragmentManager(),
			   "layerColor");
		}
	    });
	ToggleButton toggle = (ToggleButton) view.findViewById(R.id.layer_visible);
	toggle.setOnCheckedChangeListener(null);
	toggle.setChecked(layer.IsVisible());
	toggle.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
		@Override
		public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
		    layers.SetLayerVisible(position, isChecked);
		}
	    });
	return view;
    }

    @Override
    public void registerDataSetObserver(DataSetObserver observer)
    {
	layers.registerObserver(observer);
    }

    @Override
    public void unregisterDataSetObserver(DataSetObserver observer)
    {
	layers.unregisterObserver(observer);
    }
}
