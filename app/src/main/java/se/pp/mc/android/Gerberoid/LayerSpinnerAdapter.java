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
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

class LayerSpinnerAdapter extends BaseAdapter {

    private Context context;
    private Layer[] layers;

    public LayerSpinnerAdapter(Context context, GerbviewFrame gerber)
    {
	this.context = context;
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
    public View getView(int position, View view, ViewGroup parent)
    {
	if (view == null)
	    view = ((Activity)context).getLayoutInflater().inflate(R.layout.layer_spinner_entry, null, false);
	TextView text = (TextView) view.findViewById(R.id.layer_name);
	text.setText(text.getResources().getString(R.string.layer_name, position+1));
	return view;
    }
}
