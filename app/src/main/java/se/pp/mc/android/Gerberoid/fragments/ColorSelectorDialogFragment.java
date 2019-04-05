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

package se.pp.mc.android.Gerberoid.fragments;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Pair;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.GridLayout;
import android.widget.HorizontalScrollView;
import android.widget.ScrollView;

import se.pp.mc.android.Gerberoid.gerber.GerbviewFrame;
import se.pp.mc.android.Gerberoid.R;

public abstract class ColorSelectorDialogFragment extends DialogFragment
{
    private static boolean isDark(int color)
    {
	final int r = Color.red(color);
	final int g = Color.green(color);
	final int b = Color.blue(color);
	return r*r*0.241 + g*g*0.691 + b*b*0.068 < 16384;
    }

    private View createView()
    {
	final Context context = getActivity();
	final GridLayout grid = new GridLayout(context);
	grid.setOrientation(GridLayout.VERTICAL);
	grid.setRowCount(6);
	final GridLayout.LayoutParams layoutParams =
	    new GridLayout.LayoutParams();
	layoutParams.setGravity(Gravity.FILL);
	final Pair<int[], String[]> pair = GerbviewFrame.getColors(context);
	final int[] colors = pair.first;
	final String[] names = pair.second;
	for (int i=0; i<colors.length; i++) {
	    final int colorIndex = i;
	    final int colorARGB = colors[i];
	    final Button button = new Button(context);
	    button.setBackgroundColor(colorARGB);
	    button.setTextColor(isDark(colorARGB)? Color.WHITE : Color.BLACK);
	    button.setText(names[i]);
	    button.setLayoutParams(new GridLayout.LayoutParams(layoutParams));
	    button.setOnClickListener(new View.OnClickListener() {
		    @Override
		    public void onClick(View v) {
			onColorSelected(colorIndex, colorARGB);
			dismiss();
		    }
		});
	    grid.addView(button);
	}
	final ScrollView scrollView1 = new ScrollView(context);
	scrollView1.addView(grid);
	final HorizontalScrollView scrollView2 = new HorizontalScrollView(context);
	scrollView2.addView(scrollView1);
	return scrollView2;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setMessage(R.string.colors_dialog_title)
	       .setView(createView())
               .setNegativeButton(R.string.cancel, new DialogInterface.OnClickListener() {
		       public void onClick(DialogInterface dialog, int id) {
			   onCanceled();
		       }
	       });
        return builder.create();
    }

    public void onCanceled()
    {
    }

    public abstract void onColorSelected(int color, int colorARGB);
}
