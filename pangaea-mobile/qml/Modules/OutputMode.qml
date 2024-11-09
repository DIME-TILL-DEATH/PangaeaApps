import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15

import StyleSettings 1.0

import Elements 1.0
import Modules 1.0

import CppObjects
import CppEnums

Rectangle {
    id: _outputModeModule

    color: Style.colorModul

    radius: Style.baseRadius
    border.width: 1
    border.color: Style.currentTheme.colorBorderOn
    MComboBox
    {
        id: _comboBox
        property bool deviceUpdatingValues: false

        width: parent.width*0.95
        height: parent.height*0.95

        model: UiCore.currentDevice.avaliableOutputModes

        anchors.centerIn: parent

        currentIndex: UiCore.currentDevice.outputMode

        onActivated:
        {
            if(!deviceUpdatingValues)
                UiCore.currentDevice.outputMode = currentIndex;
        }

        Connections{
            target: UiCore.currentDevice

            function onDeviceUpdatingValues()
            {
                _comboBox.deviceUpdatingValues = true;
                _comboBox.currentIndex = UiCore.currentDevice.outputMode;
                _comboBox.deviceUpdatingValues = false;
            }
        }
    }
}
