package se.pp.mc.android.Gerberoid.model

import java.io.File
import java.io.Serializable

enum class FileType {
    GERBER, DRILL
}

class GerberZipEntry() : Serializable {

    var name: String? = null
    var type: FileType? = null
    var isSelected: Boolean = false
    var file: File? = null

    constructor(name: String, type: FileType, selected: Boolean, file: File) : this() {
        this.name = name
        this.type = type
        this.isSelected = selected
        this.file = file
    }


}
