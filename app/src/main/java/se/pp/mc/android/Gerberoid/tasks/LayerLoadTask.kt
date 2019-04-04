package se.pp.mc.android.Gerberoid.tasks

import android.net.Uri
import android.os.AsyncTask
import se.pp.mc.android.Gerberoid.GerberoidApplication
import se.pp.mc.android.Gerberoid.gerber.Layers
import se.pp.mc.android.Gerberoid.model.FileType
import se.pp.mc.android.Gerberoid.utils.FileUtils.toInternalLayerFile
import java.io.File
import java.lang.ref.WeakReference

class LayerLoadTask(private val layers: Layers, callback: LayerLoadCallback) : AsyncTask<LoadRequest, Void, Boolean>() {

    private val weakCallback = WeakReference<LayerLoadCallback>(callback)

    override fun onPreExecute() {
        weakCallback.get()?.onStarted()
        super.onPreExecute()
    }

    override fun doInBackground(vararg request: LoadRequest): Boolean {

        var success = true
        val firstLayer = this.layers.activeLayer

        request.mapIndexed { index, r -> load(r, firstLayer + index) }.forEach { r ->

            if(r?.file != null) {

                try {

                    success = success && if (r.type == FileType.GERBER) {
                        layers.LoadGerber(r.file)
                    } else {
                        layers.LoadDrill(r.file)
                    }

                } catch (e: Exception) {
                    e.printStackTrace()
                }

            } else {
                success = false
            }

        }

        return success

    }

    private fun load(request: LoadRequest, layer: Int) : LoadResult? {

        val src = request.uri
        val result = when(src){

            is Uri -> toInternalLayerFile(GerberoidApplication.get().applicationContext, src, layer)
            is File -> toInternalLayerFile(GerberoidApplication.get().applicationContext, src, layer)
            else -> null

        }

        return LoadResult(request.uri, request.type, result)

    }

    override fun onPostExecute(success: Boolean) {
        weakCallback.get()?.onFinished(success)
    }

}

private data class LoadResult(val uri : Any, val type : FileType, val file : File?)
data class LoadRequest(val uri : Any, val type : FileType)

interface LayerLoadCallback {
    fun onStarted()
    fun onFinished(success: Boolean)
}