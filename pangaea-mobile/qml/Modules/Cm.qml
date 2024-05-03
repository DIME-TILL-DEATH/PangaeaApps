import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    property bool on: true
    property int paramType: DeviceParameter.COMPRESSOR_ON

    BaseModule{
        id: _baseModule

        moduleName: qsTr("CM")
        moduleDescription: qsTr("Compressor")

        contentItem: Column
        {
            height: parent.height
            width: parent.width
            CustomizerSlider
            {
                height: parent.height/2
                width: parent.width

                nameValue: "Sustain"
                paramType: DeviceParameter.COMPRESSOR_SUSTAIN

                moduleOn: on
            }

            CustomizerSlider
            {
                height: parent.height/2
                width: parent.width

                nameValue: "Volume"
                paramType: DeviceParameter.COMPRESSOR_VOLUME

                bottomLineEnabled: false
                moduleOn: on
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
                console.log("Settling by sgSetDeviceParameter", paramType);
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
