qt_add_library(qml_layouts STATIC)

qt_add_qml_module(qml_layouts
    URI "Layouts"
    VERSION 1.0
    QML_FILES
        ConnectPage.qml
        MapPage.qml
        ServicePage.qml
        SettingsPage.qml
)
