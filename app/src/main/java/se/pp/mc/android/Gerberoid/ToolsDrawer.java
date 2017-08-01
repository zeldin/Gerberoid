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
import android.graphics.Rect;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
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

class ToolsDrawer {

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
	    case R.id.opt_padsketch:
	    case R.id.opt_linesketch:
	    case R.id.opt_polysketch:
	    case R.id.opt_negghost:
	    case R.id.opt_showdcode:
	    case R.id.opt_layers_mode_0:
	    case R.id.opt_layers_mode_1:
	    case R.id.opt_layers_mode_2:
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
		    new ColorSelectorDialogFragment() {
			@Override
			public void onColorSelected(int color) {
			    int newColor = 0;
			    switch(v.getId()) {
			    case R.id.grid_color:
			    case R.id.ghost_color:
			    case R.id.dcode_color:
				break;
			    }
			    ((ImageButton)v).setImageDrawable(new ColorDrawable(newColor));
			}
		    }.show(((Activity)v.getContext()).getFragmentManager(),
			   "visibleColor");
		}
	    });
	}
    }

    public ToolsDrawer(View drawer)
    {
	((RadioGroup)drawer.findViewById(R.id.opt_layers_mode)).
	    setOnCheckedChangeListener(new ToggleListener());
	new ToggleAndColorSetting((ToggleButton)drawer.findViewById(R.id.opt_grid),
				  (ImageButton)drawer.findViewById(R.id.grid_color),
				  false, 0xffcccccc);
	new ToggleSetting((ToggleButton)drawer.findViewById(R.id.opt_padsketch),
			  false);
	new ToggleSetting((ToggleButton)drawer.findViewById(R.id.opt_linesketch),
			  false);
	new ToggleSetting((ToggleButton)drawer.findViewById(R.id.opt_polysketch),
			  false);
	new ToggleAndColorSetting((ToggleButton)drawer.findViewById(R.id.opt_negghost),
				  (ImageButton)drawer.findViewById(R.id.ghost_color),
				  false, 0xffffff00);
	new ToggleAndColorSetting((ToggleButton)drawer.findViewById(R.id.opt_showdcode),
				  (ImageButton)drawer.findViewById(R.id.dcode_color),
				  false, 0xffff00ff);
	new RadioSetting((ToggleButton)drawer.findViewById(R.id.opt_layers_mode_0),
			 true);
	new RadioSetting((ToggleButton)drawer.findViewById(R.id.opt_layers_mode_1),
			 false);
	new RadioSetting((ToggleButton)drawer.findViewById(R.id.opt_layers_mode_2),
			 false);
    }

}

