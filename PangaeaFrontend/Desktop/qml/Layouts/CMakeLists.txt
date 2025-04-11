qt_add_library(qml_layouts STATIC)

qt_add_qml_module(qml_layouts
    URI "Layouts"
    VERSION 1.0
    # NO_PLUGIN
    QML_FILES
        Main.qml
        ConnectionLayout.qml
        CopyPaste.qml
        HeadLegacy.qml
        MainMenu.qml
        MBusy.qml
        SaveCompare.qml
        StartLayout.qml
        ControlLayoutLegacy.qml
        ControlLayoutCPModern.qml
        ControlLayoutCP100FX.qml
        ModulesListModern.qml
        ModulesConfigWindow.qml
        HeadCPModern.qml
        Map.qml
        BPeakingShelfControl.qml
        BCutControl.qml
        BPeakingControl.qml
        IrManagerWindow.qml
        HeadLA3.qml
)
