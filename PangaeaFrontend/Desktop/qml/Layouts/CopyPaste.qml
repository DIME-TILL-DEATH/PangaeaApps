import QtQuick

import Elements
import StyleSettings

import CppObjects 1.0
import PangaeaBackend

Item
{
    id: main

    property bool copyFirst: false

    Rectangle
    {
        anchors.fill: parent
        color: Style.mainEnabledColor
        Column
        {
            anchors.fill: parent
            MButton
            {
                width:  parent.width
                height: parent.height/2
                text: "COPY"
                enabled: UiCore.currentDevice.presetManager.currentState !== PresetState.Compare
                onClicked: {
                    UiCore.currentDevice.copyPreset();
                    copyFirst=true;
                }
            }

            MButton
            {
                width:  parent.width
                height: parent.height/2
                text: "PASTE"
                scaleText: 0.88
                enabled: copyFirst & (UiCore.currentDevice.presetManager.currentState !== PresetState.Compare)
                onClicked: UiCore.currentDevice.pastePreset();
            }
        }
    }
}
