qt_add_library(qml_custom_overlays STATIC)

qt_add_qml_module(qml_custom_overlays
    URI "CustomOverlays"
    VERSION 1.0
    QML_FILES
        BusyScreen.qml
        CustomMessageDialog.qml
        EditTextDialog.qml
        EditValueDialog.qml
        FirmwareLoadingWindow.qml
        IrManagementWindow.qml
        ModulsManagementWindow.qml
        PresetsWindow.qml
)
