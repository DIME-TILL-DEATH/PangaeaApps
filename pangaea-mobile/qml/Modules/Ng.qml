import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    property bool on: true
    property int paramType: DeviceParameter.GATE_ON

    BaseModule{
        id: _baseModule

        width: parent.width
        height: parent.height

        moduleName: qsTr("NG")
        moduleDescription: qsTr("Noise gate")

        contentItem: Column
         {
             height: parent.height
             width: parent.width
             CustomizerSlider
             {
                 height: parent.height/2
                 width: parent.width

                 nameValue: "Threshold"
                 paramType: DeviceParameter.GATE_THRESHOLD

                 moduleOn: on
             }

             CustomizerSlider
             {
                 height: parent.height/2
                 width: parent.width
                 bottomLineEnabled: false

                 nameValue: "Decay"
                 paramType: DeviceParameter.GATE_DECAY

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
