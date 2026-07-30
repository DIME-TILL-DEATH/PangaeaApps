qt_add_library(qml_cp16 STATIC)

qt_add_qml_module(qml_cp16
    URI "CP16"
    VERSION 1.0
    # NO_PLUGIN
    QML_FILES
)
