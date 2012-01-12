#GUI
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTSVG TRUE)
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSCRIPT TRUE)
SET(QT_USE_QTWEBKIT TRUE)
SET(QT_USE_QTDECLARATIVE TRUE)

include(${QT_USE_FILE})

#QWT LIBRARY FROM EXTERNAL SVN

SET(QWT_SRC_DIR ${PROJECT_SOURCE_DIR}/GUI/qwt_svn/src)
SET(QWT_INCLUDES ${PROJECT_SOURCE_DIR}/GUI/qwt_svn/src})

#Create a library with the sources 
SET(QWT_SRCS
    #Base
    ${QWT_SRC_DIR}/qwt_abstract_scale_draw.cpp
    ${QWT_SRC_DIR}/qwt_interval_symbol.cpp
    ${QWT_SRC_DIR}/qwt_clipper.cpp
    ${QWT_SRC_DIR}/qwt_color_map.cpp
    ${QWT_SRC_DIR}/qwt_column_symbol.cpp
    ${QWT_SRC_DIR}/qwt_interval.cpp
    ${QWT_SRC_DIR}/qwt_dyngrid_layout.cpp
    ${QWT_SRC_DIR}/qwt_math.cpp
    ${QWT_SRC_DIR}/qwt_magnifier.cpp
    ${QWT_SRC_DIR}/qwt_panner.cpp
    ${QWT_SRC_DIR}/qwt_null_paintdevice.cpp
    ${QWT_SRC_DIR}/qwt_painter.cpp
    ${QWT_SRC_DIR}/qwt_picker.cpp
    ${QWT_SRC_DIR}/qwt_round_scale_draw.cpp
    ${QWT_SRC_DIR}/qwt_scale_div.cpp
    ${QWT_SRC_DIR}/qwt_scale_draw.cpp
    ${QWT_SRC_DIR}/qwt_scale_map.cpp
    ${QWT_SRC_DIR}/qwt_spline.cpp
    ${QWT_SRC_DIR}/qwt_text_engine.cpp
    ${QWT_SRC_DIR}/qwt_text_label.cpp
    ${QWT_SRC_DIR}/qwt_text.cpp
    ${QWT_SRC_DIR}/qwt_event_pattern.cpp
    ${QWT_SRC_DIR}/qwt_picker_machine.cpp
    ${QWT_SRC_DIR}/qwt_point_3d.cpp
    ${QWT_SRC_DIR}/qwt_point_polar.cpp
    ${QWT_SRC_DIR}/qwt_scale_engine.cpp
    ${QWT_SRC_DIR}/qwt_symbol.cpp
    ${QWT_SRC_DIR}/qwt_system_clock.cpp
    #QWT_PLOT
    ${QWT_SRC_DIR}/qwt_curve_fitter.cpp
    ${QWT_SRC_DIR}/qwt_legend.cpp
    ${QWT_SRC_DIR}/qwt_legend_item.cpp
    ${QWT_SRC_DIR}/qwt_plot.cpp
    ${QWT_SRC_DIR}/qwt_plot_renderer.cpp
    ${QWT_SRC_DIR}/qwt_plot_xml.cpp
    ${QWT_SRC_DIR}/qwt_plot_axis.cpp
    ${QWT_SRC_DIR}/qwt_plot_curve.cpp
    ${QWT_SRC_DIR}/qwt_plot_dict.cpp
    ${QWT_SRC_DIR}/qwt_plot_directpainter.cpp
    ${QWT_SRC_DIR}/qwt_plot_grid.cpp
    ${QWT_SRC_DIR}/qwt_plot_histogram.cpp
    ${QWT_SRC_DIR}/qwt_plot_item.cpp
    ${QWT_SRC_DIR}/qwt_plot_intervalcurve.cpp
    ${QWT_SRC_DIR}/qwt_plot_spectrogram.cpp
    ${QWT_SRC_DIR}/qwt_plot_spectrocurve.cpp
    ${QWT_SRC_DIR}/qwt_plot_scaleitem.cpp
    ${QWT_SRC_DIR}/qwt_plot_seriesitem.cpp
    ${QWT_SRC_DIR}/qwt_plot_marker.cpp
    ${QWT_SRC_DIR}/qwt_plot_layout.cpp
    ${QWT_SRC_DIR}/qwt_plot_canvas.cpp
    ${QWT_SRC_DIR}/qwt_plot_panner.cpp
    ${QWT_SRC_DIR}/qwt_plot_rasteritem.cpp
    ${QWT_SRC_DIR}/qwt_plot_picker.cpp
    ${QWT_SRC_DIR}/qwt_plot_zoomer.cpp
    ${QWT_SRC_DIR}/qwt_plot_magnifier.cpp
    ${QWT_SRC_DIR}/qwt_plot_rescaler.cpp
    ${QWT_SRC_DIR}/qwt_raster_data.cpp
    ${QWT_SRC_DIR}/qwt_matrix_raster_data.cpp
    ${QWT_SRC_DIR}/qwt_sampling_thread.cpp
    ${QWT_SRC_DIR}/qwt_series_data.cpp
    ${QWT_SRC_DIR}/qwt_scale_widget.cpp
    #SVG
    ${QWT_SRC_DIR}/qwt_plot_svgitem.cpp
    #WIDGETS
    ${QWT_SRC_DIR}/qwt_abstract_slider.cpp
    ${QWT_SRC_DIR}/qwt_abstract_scale.cpp
    ${QWT_SRC_DIR}/qwt_arrow_button.cpp
    ${QWT_SRC_DIR}/qwt_analog_clock.cpp
    ${QWT_SRC_DIR}/qwt_compass.cpp
    ${QWT_SRC_DIR}/qwt_compass_rose.cpp
    ${QWT_SRC_DIR}/qwt_counter.cpp
    ${QWT_SRC_DIR}/qwt_dial.cpp
    ${QWT_SRC_DIR}/qwt_dial_needle.cpp
    ${QWT_SRC_DIR}/qwt_double_range.cpp
    ${QWT_SRC_DIR}/qwt_knob.cpp
    ${QWT_SRC_DIR}/qwt_slider.cpp
    ${QWT_SRC_DIR}/qwt_thermo.cpp
    ${QWT_SRC_DIR}/qwt_wheel.cpp
)

SET(QWT_HEADERS
    #Base
    ${QWT_SRC_DIR}/qwt.h
    ${QWT_SRC_DIR}/qwt_abstract_scale_draw.h
    ${QWT_SRC_DIR}/qwt_interval_symbol.h
    ${QWT_SRC_DIR}/qwt_clipper.h
    ${QWT_SRC_DIR}/qwt_color_map.h
    ${QWT_SRC_DIR}/qwt_compat.h
    ${QWT_SRC_DIR}/qwt_column_symbol.h
    ${QWT_SRC_DIR}/qwt_interval.h
    ${QWT_SRC_DIR}/qwt_dyngrid_layout.h
    ${QWT_SRC_DIR}/qwt_global.h
    ${QWT_SRC_DIR}/qwt_math.h
    ${QWT_SRC_DIR}/qwt_magnifier.h
    ${QWT_SRC_DIR}/qwt_null_paintdevice.h
    ${QWT_SRC_DIR}/qwt_painter.h
    ${QWT_SRC_DIR}/qwt_panner.h
    ${QWT_SRC_DIR}/qwt_picker.h
    ${QWT_SRC_DIR}/qwt_picker_machine.h
    ${QWT_SRC_DIR}/qwt_point_3d.h
    ${QWT_SRC_DIR}/qwt_point_polar.h
    ${QWT_SRC_DIR}/qwt_round_scale_draw.h
    ${QWT_SRC_DIR}/qwt_scale_div.h
    ${QWT_SRC_DIR}/qwt_scale_draw.h
    ${QWT_SRC_DIR}/qwt_scale_engine.h
    ${QWT_SRC_DIR}/qwt_scale_map.h
    ${QWT_SRC_DIR}/qwt_spline.h
    ${QWT_SRC_DIR}/qwt_symbol.h
    ${QWT_SRC_DIR}/qwt_system_clock.h
    ${QWT_SRC_DIR}/qwt_text_engine.h
    ${QWT_SRC_DIR}/qwt_text_label.h
    ${QWT_SRC_DIR}/qwt_text.h
    #QWT_PLOT
    ${QWT_SRC_DIR}/qwt_curve_fitter.h
    ${QWT_SRC_DIR}/qwt_event_pattern.h
    ${QWT_SRC_DIR}/qwt_legend.h
    ${QWT_SRC_DIR}/qwt_legend_item.h
    ${QWT_SRC_DIR}/qwt_legend_itemmanager.h
    ${QWT_SRC_DIR}/qwt_plot.h
    ${QWT_SRC_DIR}/qwt_plot_renderer.h
    ${QWT_SRC_DIR}/qwt_plot_curve.h
    ${QWT_SRC_DIR}/qwt_plot_dict.h
    ${QWT_SRC_DIR}/qwt_plot_directpainter.h
    ${QWT_SRC_DIR}/qwt_plot_grid.h
    ${QWT_SRC_DIR}/qwt_plot_histogram.h
    ${QWT_SRC_DIR}/qwt_plot_item.h
    ${QWT_SRC_DIR}/qwt_plot_intervalcurve.h
    ${QWT_SRC_DIR}/qwt_plot_layout.h
    ${QWT_SRC_DIR}/qwt_plot_marker.h
    ${QWT_SRC_DIR}/qwt_plot_rasteritem.h
    ${QWT_SRC_DIR}/qwt_plot_spectrogram.h
    ${QWT_SRC_DIR}/qwt_plot_spectrocurve.h
    ${QWT_SRC_DIR}/qwt_plot_scaleitem.h
    ${QWT_SRC_DIR}/qwt_plot_seriesitem.h
    ${QWT_SRC_DIR}/qwt_plot_canvas.h
    ${QWT_SRC_DIR}/qwt_plot_panner.h
    ${QWT_SRC_DIR}/qwt_plot_picker.h
    ${QWT_SRC_DIR}/qwt_plot_zoomer.h
    ${QWT_SRC_DIR}/qwt_plot_magnifier.h
    ${QWT_SRC_DIR}/qwt_plot_rescaler.h
    ${QWT_SRC_DIR}/qwt_raster_data.h
    ${QWT_SRC_DIR}/qwt_matrix_raster_data.h
    ${QWT_SRC_DIR}/qwt_sampling_thread.h
    ${QWT_SRC_DIR}/qwt_series_data.h
    ${QWT_SRC_DIR}/qwt_scale_widget.h
    #SVG
    ${QWT_SRC_DIR}/qwt_plot_svgitem.h
    #WIDGETS
    ${QWT_SRC_DIR}/qwt_abstract_slider.h
    ${QWT_SRC_DIR}/qwt_abstract_scale.h
    ${QWT_SRC_DIR}/qwt_arrow_button.h
    ${QWT_SRC_DIR}/qwt_analog_clock.h
    ${QWT_SRC_DIR}/qwt_compass.h
    ${QWT_SRC_DIR}/qwt_compass_rose.h
    ${QWT_SRC_DIR}/qwt_counter.h
    ${QWT_SRC_DIR}/qwt_dial.h
    ${QWT_SRC_DIR}/qwt_dial_needle.h
    ${QWT_SRC_DIR}/qwt_double_range.h
    ${QWT_SRC_DIR}/qwt_knob.h
    ${QWT_SRC_DIR}/qwt_slider.h
    ${QWT_SRC_DIR}/qwt_thermo.h
    ${QWT_SRC_DIR}/qwt_wheel.h
)

#This will generate moc_* for Qt
QT4_WRAP_CPP(QWT_MOC_SRCS ${QWT_HEADERS})

#Include directories for QWT
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR} ${QWT_SRC_DIR} plugins/ScopeView)

#Build library
add_library(qwt-wrap SHARED ${QWT_SRCS} ${QWT_MOC_SRCS} ${QWT_HEADERS})
target_link_libraries(qwt-wrap ${QT_LIBRARIES})
INSTALL(TARGETS qwt-wrap DESTINATION bin)
INSTALL(TARGETS qwt-wrap DESTINATION lib EXPORT qwt-wrap-export-targets)
INSTALL(EXPORT qwt-wrap-export-targets DESTINATION share/cmake)

INSTALL(FILES ${QWT_HEADERS} DESTINATION include/qwt)

SET(NETWORKVIEWER_GENERATED_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/src)


#ADD src subdirectory
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(plugins)
