qt_add_library(qml_elements STATIC)

qt_add_qml_module(qml_elements
        URI "Elements"
        VERSION 1.0
        QML_FILES
            ApplicationHeader.qml
            BankSpin.qml
            BleListDelegate.qml
            BleListHighlight.qml
            CustomComboBox.qml
            CustomProgressBar.qml
            CustomSlider.qml
            CustomLogSlider.qml
            EqPoint.qml
            ModeSpin_LA.qml
            MButton.qml
            MComboBox.qml
            MRadioButton.qml
            MText.qml
            PresetSpin.qml
            PresetSpin_LA.qml
            PresetsListDelegate.qml
    )
