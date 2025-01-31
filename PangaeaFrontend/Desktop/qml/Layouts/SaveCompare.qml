import QtQuick

import Elements
import StyleSettings

import CppObjects
import PangaeaBackend

Item
{
    id: main

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
                text: "SAVE"
                enabled: UiCore.currentDevice.deviceParamsModified & (UiCore.currentDevice.presetManager.currentState !== PresetState.Compare)
                onClicked: UiCore.currentDevice.saveChanges();
            }

            MButton
            {
                id: bComp
                width:  parent.width
                height: parent.height/2
                text: "COMP"

                enabled:  UiCore.currentDevice.deviceParamsModified
                highlighted: UiCore.currentDevice.presetManager.currentState === PresetState.Compare

                onClicked: UiCore.currentDevice.comparePreset();
            }
        }
    }
}
