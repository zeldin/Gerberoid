package se.pp.mc.android.Gerberoid.tasks

import android.net.Uri
import android.os.AsyncTask
import se.pp.mc.android.Gerberoid.utils.FileUtils
import se.pp.mc.android.Gerberoid.GerberoidApplication
import se.pp.mc.android.Gerberoid.model.GerberZipEntry
import se.pp.mc.android.Gerberoid.model.FileType
import java.io.File
import java.lang.ref.WeakReference

class ExtractZipTask(callback : ExtractZipTaskCallback) : AsyncTask<Uri, Any, List<GerberZipEntry>>() {

    private val weakCallback = WeakReference<ExtractZipTaskCallback>(callback)

    override fun onPreExecute() {
        super.onPreExecute()
        weakCallback.get()?.onStarted()
    }

    override fun doInBackground(vararg uris: Uri): List<GerberZipEntry>? {

        val context = GerberoidApplication.get()

        val zipFile = FileUtils.writeUriToTempFile(context, uris[0], "archive_") ?: return null
        val outputDir = File(context.cacheDir, zipFile.name.replace(".tmp", ""))

        if (!outputDir.exists()) {
            if (!outputDir.mkdirs()) {
                return null
            }
        }

        outputDir.listFiles()?.forEach { it.delete() }

        if(!FileUtils.unpackZip(outputDir.absolutePath, zipFile.absolutePath)){
            return null
        }

        return outputDir.listFiles()?.map {  f ->  GerberZipEntry(f.name, getGerberTypeFor(f.name), isSelectedByDefault(f.name), f) }

    }

    override fun onPostExecute(zipEntries: List<GerberZipEntry>?) {
        super.onPostExecute(zipEntries)
        weakCallback.get()?.onFinished(zipEntries)
    }

}

interface ExtractZipTaskCallback {
    fun onStarted()
    fun onFinished(zipEntries: List<GerberZipEntry>?)
}

private val UNKNOWN_FILE_TYPES = listOf(".ps", ".rpt")
private val KNOWN_DRILL_TYPES = listOf(".drl")

private fun isSelectedByDefault(name: String) : Boolean {
    return UNKNOWN_FILE_TYPES.none { name.toLowerCase().endsWith(it) }
}

private fun getGerberTypeFor(name: String) : FileType {
    return if(KNOWN_DRILL_TYPES.any { name.toLowerCase().endsWith(it) }) FileType.DRILL else FileType.GERBER
}