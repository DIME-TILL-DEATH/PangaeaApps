qt_add_library(qml_layouts STATIC)

qt_add_qml_module(qml_layouts
    URI "Layouts"
    VERSION 1.0
    # NO_PLUGIN
    QML_FILES
        Main.qml
        ConnectionLayout.qml
        CopyPaste.qml
        MainMenu.qml
        MBusy.qml
        SaveCompare.qml
        StartLayout.qml
        BPeakingShelfControl.qml
        BCutControl.qml
        BPeakingControl.qml
        EqWindow.qml
)
