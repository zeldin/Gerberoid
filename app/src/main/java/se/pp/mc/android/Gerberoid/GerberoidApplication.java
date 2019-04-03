package se.pp.mc.android.Gerberoid;

import android.app.Application;

public class GerberoidApplication extends Application  {

    private static GerberoidApplication INSTANCE = null;

    @Override
    public void onCreate() {
        super.onCreate();
        INSTANCE = this;
    }

    public static GerberoidApplication get(){
        return INSTANCE;
    }

}
