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

    moduleName: qsTr("PA")
    moduleDescription: qsTr("Power amp")

    contentItem: Column{

        property alias curInd: _comboBox.currentIndex

        height: parent.height
        width: parent.width

        CustomizerSlider
        {
            height: parent.height/4
            width: parent.width
            ctrlValInstance: module.volume

            moduleOn: main.on
        }

        CustomizerSlider
        {
            height: parent.height/4
            width: parent.width

            ctrlValInstance: module.presence

            moduleOn: main.on
        }

        CustomizerSlider
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

            currentIndex: module.ampType.value

            onActivated:
            {
                if(!deviceUpdatingValues)
                    module.ampType.value = currentIndex;
            }

            Connections{
                target: CurrentDevice

                function onDeviceUpdatingValues()
                {
                    _comboBox.deviceUpdatingValues = true;
                    _comboBox.currentIndex = module.ampType.value;
                    _comboBox.deviceUpdatingValues = false;
                }
            }
        }
    }
}

    // когда включаем/выклаючаем PA, также включить/выключить presence
    // onOnChanged: {
    //     if(main.visible) // только если модуль есть в устройстве
    //         UiCore.setParameter("pa-ps_linked_on", main.on);
    // }

    // Connections
    // {
    //     target: UiCore

    //     function onSgSetUiDeviceParameter(paramType, value)
    //     {
    //         if(paramType === main.paramType)
    //         {
    //             main.on=value

    //             // if(main.visible) // только если модуль есть в устройстве
    //             //     UiCore.setParameter("pa-ps_linked_on", main.on);
    //         }


    //     }
    // }
