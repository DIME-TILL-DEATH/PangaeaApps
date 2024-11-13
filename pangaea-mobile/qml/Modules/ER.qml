import QtQuick
import QtQuick.Controls

import StyleSettings 1.0
import Elements 1.0
import Tutorials

import CppObjects

BaseModule
{
    id: main

    property EarlyReflections module

    property int currentType

    moduleDescription: qsTr("Early reflections")

    width: parent.width
    height: parent.height

    contentItem: Column
    {
        height: parent.height
        width: parent.width
        CustomSlider
        {
            id: _slider

            height: parent.height/2
            width: parent.width

            ctrlValInstance: module.reflectionsVolume

            bottomLineEnabled: false
            moduleOn: main.on

        }

        MComboBox
        {
            id: _comboBox

            property bool deviceUpdatingValues

            height: parent.height/2
            width: parent.width

            on: main.on

            currentIndex: module.reflectionsType.displayValue

            model: ["SHORT","MEDIUM","LONG"]

            onActivated:
            {
                if(!deviceUpdatingValues)
                    module.reflectionsType.displayValue = currentIndex;
            }

            Connections{
                target: UiCore.currentDevice

                function onDeviceUpdatingValues()
                {
                    _comboBox.deviceUpdatingValues = true;
                    _comboBox.currentIndex = module.reflectionsType.displayValue;
                    _comboBox.deviceUpdatingValues = false;
                }
            }
        }
    }

    tutorialItem: SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Early reflections")

        text: qsTr("This effect block emulates room acoustics. Unlike Reverb, Early Reflections are sounds that you hear right after the direct sound before full reverberation takes over. It’s more like a series of discrete delays rather than a smooth tail associated with Reverb.

VOLUME: Amount of reflections mixed to the direct signal.

TYPE: There are 3 types of the effect to choose from:
1. Short: Small room
2. Medium: Meduim size room
3. Long: Large room");
    }
}
