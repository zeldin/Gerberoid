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
    private GerbviewFrame gerber;
    private Layer[] layers;

    public LayerSpinnerAdapter(Context context, GerbviewFrame gerber)
    {
	this.context = context;
	this.gerber = gerber;
	this.layers = gerber.getLayers();
    }

    @Override
    public int getCount() {
	return layers.length;
    }

    @Override
    public Object getItem(int position) {
	return layers[position];
    }

    @Override
    public long getItemId(int position) {
	return layers[position].hashCode();
    }

    @Override
    public View getView(final int position, View view, ViewGroup parent)
    {
	if (view == null)
	    view = ((Activity)context).getLayoutInflater().inflate(R.layout.layer_spinner_entry, parent, false);
	TextView text = (TextView) view.findViewById(R.id.layer_name);
	text.setText(text.getResources().getString(R.string.layer_name, position+1));
	ImageButton button = (ImageButton) view.findViewById(R.id.layer_color);
	button.setImageDrawable(new ColorDrawable(layers[position].GetColor()));
	button.setOnClickListener(new View.OnClickListener() {
		@Override
		public void onClick(View v) {
		    new ColorSelectorDialogFragment() {
			@Override
			public void onColorSelected(int color) {
			    gerber.SetLayerColor(position, color);
			    notifyDataSetChanged();
			}
		    }.show(((Activity)context).getFragmentManager(),
			   "layerColor");
		}
	    });
	ToggleButton toggle = (ToggleButton) view.findViewById(R.id.layer_visible);
	toggle.setOnCheckedChangeListener(null);
	toggle.setChecked(layers[position].IsVisible());
	toggle.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
		@Override
		public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
		    gerber.SetLayerVisible(position, isChecked);
		    notifyDataSetChanged();
		}
	    });
	return view;
    }
}
