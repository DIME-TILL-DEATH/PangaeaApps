qt_add_library(qml_style STATIC)

set_source_files_properties(Style.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)
qt_add_qml_module(qml_style
    URI "StyleSettings"
    VERSION 1.0
    QML_FILES
        Style.qml
)
