qt_add_library(qml_layouts STATIC)

qt_add_qml_module(qml_layouts
    URI "Layouts"
    VERSION 1.0
    QML_FILES
        ConnectionLayout.qml
        CopyPaste.qml
        HeadLegacy.qml
        MainMenu.qml
        MBusy.qml
        SaveCompare.qml
        StartLayout.qml
        ControlLayoutLegacy.qml
        ControlLayoutCPModern.qml
        ModulesListModern.qml
        ModulesConfigWindow.qml
        HeadCPModern.qml
        Map.qml
)
