import QtQuick
import QtQuick.Controls

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    property bool on: true
    property int paramType: DeviceParameter.EARLY_ON

    property int currentType

    BaseModule{
        id: _baseModule

        moduleName: qsTr("ER")
        moduleDescription: qsTr("Early reflections")

        width: parent.width
        height: parent.height

        contentItem: Column
        {
            height: parent.height
            width: parent.width
            CustomizerSlider
            {
                id: _slider

                height: parent.height/2
                width: parent.width
                name: "Early reflection"
                paramType: DeviceParameter.EARLY_VOLUME

                bottomLineEnabled: false
                moduleOn: on        

            }

            MComboBox
            {
                id: _combo

                height: parent.height/2
                width: parent.width

                on: main.on

                currentIndex: main.currentType

                model: ["SHORT","MEDIUM","LONG"]

                onActivated:
                {
                    UiCore.setDeviceParameter(DeviceParameter.EARLY_TYPE, currentIndex);
                }
            }
        }
    }


    Connections
    {
        target: UiCore
        function onSgSetUiDeviceParameter(paramType, value)
        {
            if(paramType === main.paramType)
            {
                main.on=value
            }

            if(paramType === DeviceParameter.EARLY_TYPE)
            {
                main.currentType = value               
            }
        }
    }

    Connections{
        target: _baseModule
        function onSgModuleOnOf()
        {
            main.on = (!main.on);
            UiCore.setDeviceParameter(main.paramType, main.on)
        }
    }


}
