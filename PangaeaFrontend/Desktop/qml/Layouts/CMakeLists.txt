qt_add_library(qml_layouts STATIC)

qt_add_qml_module(qml_layouts
    URI "Layouts"
    VERSION 1.0
    # NO_PLUGIN
    QML_FILES
    FileBrowserWindow.qml
        BCutControl.qml
        BPeakingControl.qml
        BPeakingShelfControl.qml
        ConnectionLayout.qml
        CopyPaste.qml
        DialogTextInput.qml
        EqWindow.qml
        Main.qml
        MainMenu.qml
        MBusy.qml
        MapList.qml
        MapTable.qml
        NativeMessageDialog.qml
        SaveCompare.qml
        StartLayout.qml
        TunerWindow.qml
)
