import QtQuick
import QtQuick.Controls

import StyleSettings 1.0
import Elements 1.0

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
}
