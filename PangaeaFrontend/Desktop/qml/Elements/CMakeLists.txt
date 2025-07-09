qt_add_library(qml_elements STATIC)

qt_add_qml_module(qml_elements
    URI "Elements"
    VERSION 1.0
    # NO_PLUGIN
    QML_FILES
        BankSpin.qml
        CheckText.qml
        Dial.qml
        ParameterDial.qml
        ParameterComboBox.qml
        ParameterSwitch.qml
        EqPoint.qml
        ILDelegate.qml
        ILHighlight.qml
        Indicator.qml
        Logo.qml
        MapDelegate.qml
        MapHeadBank.qml
        MapHeadPreset.qml
        Material.qml
        MButton.qml
        MText.qml
        PresetSpin.qml
        SignalBar.qml
        Switch2.qml
        Switch3.qml
        SwitchEqMap.qml
        SwitchIr.qml
        SwitchOutput.qml
        SwitchPostPre.qml
        SwitchRoom.qml
        TypePA.qml
        ClipIndicator.qml
        Tutorials.qml
        MLabel.qml
        MComboHorizontal.qml
        MBar.qml
        QML_FILES MSwitchHorizontal.qml
    )
