package se.pp.mc.android.Gerberoid;

import android.app.Application;

import se.pp.mc.android.Gerberoid.utils.Preferences;

public class GerberoidApplication extends Application  {

    private Preferences preferences;

    @Override
    public void onCreate() {
        super.onCreate();
        this.preferences = new Preferences(getApplicationContext());
    }

    public Preferences getPreferences(){
        return preferences;
    }

}
