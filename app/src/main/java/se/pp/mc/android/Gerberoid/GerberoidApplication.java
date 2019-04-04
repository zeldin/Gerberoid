package se.pp.mc.android.Gerberoid;

import android.app.Application;

import se.pp.mc.android.Gerberoid.utils.Preferences;

public class GerberoidApplication extends Application  {

    private static GerberoidApplication INSTANCE = null;

    private Preferences preferences;

    @Override
    public void onCreate() {
        super.onCreate();
        this.preferences = new Preferences(getApplicationContext());
        INSTANCE = this;
    }

    public Preferences getPreferences(){
        return preferences;
    }

    public static GerberoidApplication get(){
        return INSTANCE;
    }

}
