import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings 1.0
import Elements 1.0

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

            model: ["01.PP 6L6","02.PP EL34","03.SE 6L6","04.SE EL34","05.AMT TC-3","06.CALIF","07.BRIT M","08.BRIT L","09.DEFAULT","10.CALIF MOD","11.CALIF VINT","12.PVH 01","13.PVH 02","14.PVH 03","15.PVH 04"]

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
}