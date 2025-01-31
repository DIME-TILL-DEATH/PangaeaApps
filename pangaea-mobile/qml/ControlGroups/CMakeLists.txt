qt_add_library(qml_control_groups STATIC)

qt_add_qml_module(qml_control_groups
    URI "ControlGroups"
    VERSION 1.0
    QML_FILES
        BCutControl.qml
        BPeakingControl.qml
        BPeakingShelfControl.qml
        ControlButtonsGroup.qml
        ConfigCP.qml
        DocumentationWindow.qml
        MapCPLegacy.qml
        MapCPModern.qml
        MasterControls_CP.qml
        MasterControls_LA.qml
        ConfigUnavaliable.qml
)
