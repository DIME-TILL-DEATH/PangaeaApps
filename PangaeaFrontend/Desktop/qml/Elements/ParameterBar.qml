import QtQuick
import QtQuick.Controls

import StyleSettings

import PangaeaFrontend
import PangaeaBackend

Row {
    id: _root

    width: parent.width
    height: parent.height

    enabled: true

    required property ControlValue controlValue
    opacity: _root.enabled ? 1 : 0.5

    property string name: controlValue.name
    property string units: controlValue.units

    signal moved()

    onControlValueChanged: {
        _bar.minValue = controlValue.minDisplayValue
        _bar.maxValue = controlValue.maxDisplayValue
    }

    MBar {
        id: _bar

        width: parent.width
        height: parent.height

        text: _root.units !== "" ? _root.name + ", " + _root.units : _root.name
        // textWidth: 0
        barWidth: parent.width * 0.5

        minValue: controlValue.minDisplayValue
        maxValue: controlValue.maxDisplayValue
        value: controlValue.displayValue

        onUserChangedValue: (calcVal) => {
            controlValue.displayValue = calcVal
            _root.moved()
        }
    }
}
