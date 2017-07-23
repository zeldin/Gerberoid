cmake_minimum_required(VERSION 3.4.1)

set(GERBVIEW_SRCS
	src/main/cpp/gerbview/class_GERBER.cpp
	src/main/cpp/gerbview/class_am_param.cpp
	src/main/cpp/gerbview/class_aperture_macro.cpp
	src/main/cpp/gerbview/class_gbr_layout.cpp
	src/main/cpp/gerbview/class_gerber_draw_item.cpp
	src/main/cpp/gerbview/class_X2_gerber_attributes.cpp
	src/main/cpp/gerbview/dcode.cpp
	src/main/cpp/gerbview/draw_gerber_screen.cpp
	src/main/cpp/gerbview/readgerb.cpp
	src/main/cpp/gerbview/rs274d.cpp
	src/main/cpp/gerbview/rs274x.cpp
	src/main/cpp/gerbview/rs274_read_XY_and_IJ_coordinates.cpp
	src/main/cpp/gerbview/common/colors.cpp
	src/main/cpp/gerbview/common/dlist.cpp
	src/main/cpp/gerbview/common/drawtxt.cpp
	src/main/cpp/gerbview/common/eda_text.cpp
	src/main/cpp/gerbview/common/newstroke_font.cpp
	src/main/cpp/gerbview/common/string.cpp
	src/main/cpp/gerbview/common/trigo.cpp
	src/main/cpp/gerbview/common/utf8.cpp
	src/main/cpp/gerbview/common/gal/stroke_font.cpp

	src/main/cpp/shim/base_struct.cpp
	src/main/cpp/shim/common.cpp
	src/main/cpp/shim/confirm.cpp
	src/main/cpp/shim/draw_panel.cpp
	src/main/cpp/shim/gerbview_frame.cpp
	src/main/cpp/shim/gr_basic.cpp
	src/main/cpp/shim/html_messagebox.cpp
	src/main/cpp/shim/msgpanel.cpp
	src/main/cpp/shim/threadscopedcontext.cpp
	src/main/cpp/shim/worksheet.cpp
	src/main/cpp/shim/wx/bitmap.cpp
	src/main/cpp/shim/wx/dc.cpp
	src/main/cpp/shim/wx/debug.cpp
	src/main/cpp/shim/wx/filefn.cpp
	src/main/cpp/shim/wx/msgdlg.cpp
	src/main/cpp/shim/wx/string.cpp
	src/main/cpp/shim/wx/utils.cpp

	src/main/cpp/jni/AlertDialogBuilder.cpp
	src/main/cpp/jni/Bitmap.cpp
	src/main/cpp/jni/Canvas.cpp
	src/main/cpp/jni/GerbviewFrame.cpp
	src/main/cpp/jni/Html.cpp
	src/main/cpp/jni/Paint.cpp
	src/main/cpp/jni/Toast.cpp
	src/main/cpp/jni/View.cpp
	src/main/cpp/jni/inithook.cpp
	src/main/cpp/jni/jniref.cpp
	src/main/cpp/jni/localframe.cpp
)

add_library( gerbview SHARED ${GERBVIEW_SRCS} )

target_compile_definitions( gerbview PRIVATE -DGERBVIEW -DWX_ANDROID_TAG="gerbview")

target_include_directories( gerbview PRIVATE
	src/main/cpp/gerbview
	src/main/cpp/gerbview/include
	src/main/cpp/shim
	src/main/cpp/jni
)

find_library( log-lib log )

target_link_libraries( gerbview ${log-lib} )

