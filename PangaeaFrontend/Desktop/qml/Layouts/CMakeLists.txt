qt_add_library(qml_layouts STATIC)

qt_add_qml_module(qml_layouts
    URI "Layouts"
    VERSION 1.0
    QML_FILES
        ConnectionLayout.qml
        CopyPaste.qml
        Head.qml
        MainMenu.qml
        MBusy.qml
        ModulesList.qml
        SaveCompare.qml
        StartLayout.qml
        ControlLayoutLegacy.qml
)
