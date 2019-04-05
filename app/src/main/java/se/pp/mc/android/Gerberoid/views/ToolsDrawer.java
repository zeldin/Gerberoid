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

package se.pp.mc.android.Gerberoid.views;

import android.app.Activity;
import android.app.DialogFragment;
import android.graphics.Rect;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.style.ImageSpan;
import android.view.Gravity;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.ImageButton;
import android.widget.RadioGroup;
import android.widget.Toast;
import android.widget.ToggleButton;

import se.pp.mc.android.Gerberoid.R;
import se.pp.mc.android.Gerberoid.activities.MainActivity;
import se.pp.mc.android.Gerberoid.fragments.ColorSelectorDialogFragment;
import se.pp.mc.android.Gerberoid.gerber.DisplayOptions;

public class ToolsDrawer {

    private Activity activity;
    private DisplayOptions displayOptions;

    public static class VisibleColorSelectorDialogFragment
	extends ColorSelectorDialogFragment
    {
	@Override
	public void onColorSelected(int color, int argb) {
	    Bundle args = getArguments();
	    DisplayOptions displayOptions = ((MainActivity)getActivity()).getDisplayOptions();
	    if (args == null || displayOptions == null)
		return;
	    int id = args.getInt("viewId", -1);
	    switch(id) {
	    case R.id.grid_color:
		displayOptions.SetGerberGridVisibleColor(color);
		break;
	    case R.id.ghost_color:
		displayOptions.SetNegativeObjectsVisibleColor(color);
		break;
	    case R.id.dcode_color:
		displayOptions.SetDCodesVisibleColor(color);
		break;
	    }
	    ImageButton i = (ImageButton)(getActivity().findViewById(id));
	    i.setImageDrawable(new ColorDrawable(argb));
	}
    }

    private static void calculateToolTipPosition(Toast toast, View view)
    {
	final int[] screenPos = new int[2];
	final Rect displayFrame = new Rect();
	view.getLocationOnScreen(screenPos);
	view.getWindowVisibleDisplayFrame(displayFrame);
	final int width = view.getWidth();
	final int height = view.getHeight();
	int referenceX = screenPos[0] + width / 2;
	final int screenWidth = view.getContext().getResources().
	    getDisplayMetrics().widthPixels;
	referenceX = screenWidth - referenceX;
	toast.setGravity(Gravity.TOP | Gravity.END, referenceX,
			 screenPos[1] + height - displayFrame.top);
    }

    private static class ToggleListener implements
					    RadioGroup.OnCheckedChangeListener
    {
	@Override
        public void onCheckedChanged(final RadioGroup radioGroup, final int i) {
            for (int j = 0; j < radioGroup.getChildCount(); j++) {
                final ToggleButton view = (ToggleButton) radioGroup.getChildAt(j);
                view.setChecked(view.getId() == i);
            }
        }
    }

    private class ToggleSetting implements
				    CompoundButton.OnCheckedChangeListener
    {
	@Override
	public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
	    switch(buttonView.getId()) {
	    case R.id.opt_grid:
		displayOptions.SetDisplayGridFlag(isChecked);
		break;
	    case R.id.opt_padsketch:
		displayOptions.SetDisplayFlashedItemsFillFlag(!isChecked);
		break;
	    case R.id.opt_linesketch:
		displayOptions.SetDisplayLinesFillFlag(!isChecked);
		break;
	    case R.id.opt_polysketch:
		displayOptions.SetDisplayPolygonsFillFlag(!isChecked);
		break;
	    case R.id.opt_negghost:
		displayOptions.SetDisplayNegativeObjectsFlag(isChecked);
		break;
	    case R.id.opt_showdcode:
		displayOptions.SetDisplayDCodesFlag(isChecked);
		break;
	    case R.id.opt_layers_mode_0:
		displayOptions.SetDisplayMode(0);
		break;
	    case R.id.opt_layers_mode_1:
		displayOptions.SetDisplayMode(1);
		break;
	    case R.id.opt_layers_mode_2:
		displayOptions.SetDisplayMode(2);
		break;
	    }
	}

	private ToggleSetting(ToggleButton button, boolean initialState)
	{
	    Drawable[] drawables = button.getCompoundDrawables();
	    final CharSequence textOn = button.getTextOn();
	    final CharSequence textOff = button.getTextOff();
	    ImageSpan imageSpan = new ImageSpan(drawables[1]);
	    SpannableString content = new SpannableString("X");
	    content.setSpan(imageSpan, 0, 1, Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
	    button.setCompoundDrawables(null, null, null, null);
	    button.setTransformationMethod(null);
	    button.setText(content);
	    button.setTextOn(content);
	    button.setTextOff(content);
	    button.setChecked(initialState);
	    button.setOnLongClickListener(new View.OnLongClickListener() {
		    @Override
		    public boolean onLongClick(View view) {
			ToggleButton button = (ToggleButton) view;
			Toast toolTip = Toast.makeText(view.getContext(),
						       (button.isChecked()?
							textOff : textOn),
						       Toast.LENGTH_SHORT);
			calculateToolTipPosition(toolTip, view);
			toolTip.show();
			return true;
		    }
		});
	    button.setOnCheckedChangeListener(this);
	}
    }

    private class RadioSetting extends ToggleSetting implements
							 View.OnClickListener
    {
	@Override
	public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
	    if (isChecked)
		super.onCheckedChanged(buttonView, isChecked);
	}

	@Override
	public void onClick(View v) {
	    RadioGroup radioGroup = (RadioGroup)v.getParent();
	    radioGroup.check(0);
	    radioGroup.check(v.getId());
	}

	private RadioSetting(ToggleButton button, boolean initialState)
	{
	    super(button, false);
	    if (initialState)
		((RadioGroup)button.getParent()).check(button.getId());
	    button.setOnClickListener(this);
	}
    }

    private class ToggleAndColorSetting extends ToggleSetting
    {
	ImageButton imageButton;

	@Override
	public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
	    super.onCheckedChanged(buttonView, isChecked);
	    if (imageButton != null)
		imageButton.setEnabled(isChecked);
	}

	private ToggleAndColorSetting(ToggleButton toggleButton, ImageButton imageButton,
				      boolean initialState, int initialColor)
	{
	    super(toggleButton, initialState);
	    this.imageButton = imageButton;
	    imageButton.setImageDrawable(new ColorDrawable(initialColor));
	    imageButton.setEnabled(initialState);
	    imageButton.setOnClickListener(new View.OnClickListener() {
		@Override
		public void onClick(final View v) {
		    DialogFragment dialog = new VisibleColorSelectorDialogFragment();
		    Bundle bundle = new Bundle(1);
		    bundle.putInt("viewId", v.getId());
		    dialog.setArguments(bundle);
		    dialog.show(activity.getFragmentManager(), "visibleColor");
		}
	    });
	}
    }

    public ToolsDrawer(Activity activity, View drawer, DisplayOptions displayOptions)
    {
	this.activity = activity;
	this.displayOptions = displayOptions;
	((RadioGroup)drawer.findViewById(R.id.opt_layers_mode)).
	    setOnCheckedChangeListener(new ToggleListener());
	new ToggleAndColorSetting((ToggleButton)drawer.findViewById(R.id.opt_grid),
				  (ImageButton)drawer.findViewById(R.id.grid_color),
				  displayOptions.GetDisplayGridFlag(),
				  displayOptions.GetGerberGridVisibleColorARGB());
	new ToggleSetting((ToggleButton)drawer.findViewById(R.id.opt_padsketch),
			  !displayOptions.GetDisplayFlashedItemsFillFlag());
	new ToggleSetting((ToggleButton)drawer.findViewById(R.id.opt_linesketch),
			  !displayOptions.GetDisplayLinesFillFlag());
	new ToggleSetting((ToggleButton)drawer.findViewById(R.id.opt_polysketch),
			  !displayOptions.GetDisplayPolygonsFillFlag());
	new ToggleAndColorSetting((ToggleButton)drawer.findViewById(R.id.opt_negghost),
				  (ImageButton)drawer.findViewById(R.id.ghost_color),
				  displayOptions.GetDisplayNegativeObjectsFlag(),
				  displayOptions.GetNegativeObjectsVisibleColorARGB());
	new ToggleAndColorSetting((ToggleButton)drawer.findViewById(R.id.opt_showdcode),
				  (ImageButton)drawer.findViewById(R.id.dcode_color),
				  displayOptions.GetDisplayDCodesFlag(),
				  displayOptions.GetDCodesVisibleColorARGB());
	final int mode = displayOptions.GetDisplayMode();
	new RadioSetting((ToggleButton)drawer.findViewById(R.id.opt_layers_mode_0),
			 mode == 0);
	new RadioSetting((ToggleButton)drawer.findViewById(R.id.opt_layers_mode_1),
			 mode == 1);
	new RadioSetting((ToggleButton)drawer.findViewById(R.id.opt_layers_mode_2),
			 mode == 2);
    }

}

