import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings 1.0
import Elements 1.0
import Tutorials

import CppObjects

BaseModule
{
    id: main

    property PowerAmp module

    property int ampType

    moduleDescription: qsTr("Power amp")

    contentItem: Column{

        property alias curInd: _comboBox.currentIndex

        height: parent.height
        width: parent.width

        CustomSlider
        {
            height: parent.height/4
            width: parent.width
            ctrlValInstance: module.volume

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/4
            width: parent.width

            ctrlValInstance: module.presence

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/4
            width: parent.width

            ctrlValInstance: module.slave

            moduleOn: main.on
        }

        MComboBox
        {
            id: _comboBox

            property bool deviceUpdatingValues: false

            on: main.on

            height: parent.height/4
            width: parent.width

            model: ["01.Push-pull 6L6",
                    "02.Push-pull EL34",
                    "03.Single-ended 6L6",
                    "04.Single-ended EL34",
                    "05.AMT Tube cake",
                    "06.California",
                    "07.British M",
                    "08.British L",
                    "09.Flat",
                    "10.Califonia modern",
                    "11.California vintage",
                    "12.PVH Presence 01",
                    "13.PVH Presence 02",
                    "14.PVH Presence 03",
                    "15.PVH Presence 04"]

            currentIndex: module.ampType.displayValue

            onActivated:
            {
                if(!deviceUpdatingValues)
                    module.ampType.displayValue = currentIndex;
            }

            Connections{
                target: UiCore.currentDevice

                function onDeviceUpdatingValues()
                {
                    _comboBox.deviceUpdatingValues = true;
                    _comboBox.currentIndex = module.ampType.displayValue;
                    _comboBox.deviceUpdatingValues = false;
                }
            }
        }
    }

    tutorialItem: TutorialPA{}
}
