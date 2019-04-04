package se.pp.mc.android.Gerberoid.utils

import android.content.Context
import android.content.SharedPreferences
import android.graphics.Color
import android.preference.PreferenceManager
import se.pp.mc.android.Gerberoid.gerber.DisplayOptions
import se.pp.mc.android.Gerberoid.gerber.GerbviewFrame
import se.pp.mc.android.Gerberoid.gerber.Layers

private const val NO_COLOR = -1

private const val PREF_DCODES = "pref.display.dcodes"
private const val PREF_FLASHED_ITEMS = "pref.display.flasheditems"
private const val PREF_FILL_FLAGS = "pref.display.fillflags"
private const val PREF_NEGATIVE_OBJECTS = "pref.display.negativeobjects"
private const val PREF_POLYGONS_FILL = "pref.display.polygonsfill"
private const val PREF_GRID = "pref.display.grid"
private const val PREF_DISPLAY_MODE = "pref.display.displaymode"
private const val PREF_DCODES_COLOR = "pref.display.dcodecolor"
private const val PREF_GRID_COLOR = "pref.display.gridcolor"
private const val PREF_NEGATIVE_OBJECTS_COLOR = "pref.display.negativeobjectcolor"

class Preferences(applicationContext : Context) {

    private val preferences = PreferenceManager.getDefaultSharedPreferences(applicationContext)
    private val colors = GerbviewFrame.getColors(applicationContext).first

    fun restoreDefaults(){
        preferences.edit().clear().apply()
    }

    fun storeDisplayOptions(options: DisplayOptions){

        preferences.edit().apply {

            putBoolean(PREF_DCODES, options.GetDisplayDCodesFlag())
            putBoolean(PREF_FLASHED_ITEMS, options.GetDisplayFlashedItemsFillFlag())
            putBoolean(PREF_FILL_FLAGS, options.GetDisplayLinesFillFlag())
            putBoolean(PREF_NEGATIVE_OBJECTS, options.GetDisplayNegativeObjectsFlag())
            putBoolean(PREF_POLYGONS_FILL, options.GetDisplayPolygonsFillFlag())
            putBoolean(PREF_GRID, options.GetDisplayGridFlag())

            putInt(PREF_DISPLAY_MODE, options.GetDisplayMode())

            writeColor(preferences, PREF_DCODES_COLOR) { options.GetDCodesVisibleColorARGB() }
            writeColor(preferences, PREF_GRID_COLOR) { options.GetGerberGridVisibleColorARGB() }
            writeColor(preferences, PREF_NEGATIVE_OBJECTS_COLOR) { options.GetNegativeObjectsVisibleColorARGB() }

        }.apply()

    }

    fun restoreDisplayOptions(options: DisplayOptions){

        if(preferences.contains(PREF_DCODES)){

            options.SetDisplayDCodesFlag(preferences.getBoolean(PREF_DCODES, true))
            options.SetDisplayFlashedItemsFillFlag(preferences.getBoolean(PREF_FLASHED_ITEMS, true))
            options.SetDisplayLinesFillFlag(preferences.getBoolean(PREF_FILL_FLAGS, true))
            options.SetDisplayNegativeObjectsFlag(preferences.getBoolean(PREF_NEGATIVE_OBJECTS, true))
            options.SetDisplayPolygonsFillFlag(preferences.getBoolean(PREF_POLYGONS_FILL, true))
            options.SetDisplayGridFlag(preferences.getBoolean(PREF_GRID, true))

            options.SetDisplayMode(preferences.getInt(PREF_DISPLAY_MODE, Color.WHITE))

            readColor(preferences, PREF_DCODES_COLOR) { c -> options.SetDCodesVisibleColor(c) }
            readColor(preferences, PREF_GRID_COLOR) { c -> options.SetGerberGridVisibleColor(c) }
            readColor(preferences, PREF_NEGATIVE_OBJECTS_COLOR) { c -> options.SetNegativeObjectsVisibleColor(c) }

        }

    }

    fun storeLayerColors(layers: Layers){

        val edit = preferences.edit()
        for(i in 0 until layers.layerCount){

            val color = findColor(layers.getLayer(i).GetColor())
            val key = "pref.layer.color.$i"
            if(color != NO_COLOR){
                edit.putInt(key, color)
            }

        }
        edit.apply()

    }

    fun restoreLayerColors(layers: Layers){

        for(i in 0 until layers.layerCount){

            val key = "pref.layer.color.$i"
            if(preferences.contains(key)){
                layers.SetLayerColor(i, preferences.getInt(key, NO_COLOR))
            }

        }

    }

    private fun readColor(prefs: SharedPreferences, key: String, put: ((Int) -> (Unit))) {

        val color = prefs.getInt(key, NO_COLOR)
        if(color != NO_COLOR){
            put(color)
        }

    }

    private fun writeColor(prefs: SharedPreferences, key: String, get: (() -> Int)){

        val color = findColor(get())
        if(color != NO_COLOR){
            prefs.edit().putInt(key, color).apply()
        }

    }

    private fun findColor(argb : Int?) : Int {

        colors.forEachIndexed { index, color -> if(argb == color) return index }
        return NO_COLOR

    }

}