import QtQuick 2.15
import QtQuick.Controls 2.12

import Elements 1.0
import StyleSettings 1.0

import CppObjects
import Tutorials

BaseModule
{
    id: main

    property Phaser module

    moduleDescription: qsTr("Phaser")

    contentItem: Column
    {
        height: parent.height
        width: parent.width
        CustomSlider
        {
            height: parent.height/6
            width: parent.width

            ctrlValInstance: module.rate

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/6
            width: parent.width

            ctrlValInstance: module.width

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/6
            width: parent.width

            ctrlValInstance: module.center

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/6
            width: parent.width

            ctrlValInstance: module.feedback

            moduleOn: main.on
        }

        MComboBox
        {
            id: _comboBox

            property bool deviceUpdatingValues: false

            on: main.on

            height: parent.height/6
            width: parent.width

            model: ["Stages 3",
                    "Stages 5",
                    "Stages 7"]

            currentIndex: module.stages.displayValue

            onActivated:
            {
                if(!deviceUpdatingValues)
                    module.stages.displayValue = currentIndex;
            }

            Connections{
                target: UiCore.currentDevice

                function onDeviceUpdatingValues()
                {
                    _comboBox.deviceUpdatingValues = true;
                    _comboBox.currentIndex = module.stages.displayValue;
                    _comboBox.deviceUpdatingValues = false;
                }
            }
        }


        CustomSlider
        {
            height: parent.height/6
            width: parent.width
            bottomLineEnabled: false

            ctrlValInstance: module.mix

            moduleOn: main.on
        }
    }

    tutorialItem: TutorialPH{}
}
