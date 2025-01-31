qt_add_library(qml_pages STATIC)

qt_add_qml_module(qml_pages
    URI "Pages"
    VERSION 1.0
    QML_FILES
        ConnectPage.qml
        MapPage.qml
        ServicePage.qml
        SettingsPage.qml
)
