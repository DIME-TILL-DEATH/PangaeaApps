import QtQuick 2.15
import QtQuick.Controls 2.12

import Elements 1.0
import StyleSettings 1.0

import CppObjects

Item
{
    id: main

    property bool on: true
    property int paramType: DeviceParameter.PREAMP_ON

    BaseModule{
        id: _baseModule

        moduleName: qsTr("PR")
        moduleDescription: qsTr("Preamp")

        contentItem: Column
        {
            height: parent.height
            width: parent.width
            CustomizerSlider
            {
                paramType: DeviceParameter.PREAMP_HIGH
                nameValue: "High"

                valueMin: (-64)
                valueMax: (63)
                dispMin:  (0)
                dispMax:  (127)
                height: parent.height/4
                width: parent.width
                moduleOn: on
            }

            CustomizerSlider
            {
                paramType: DeviceParameter.PREAMP_MID
                nameValue: "Mid"

                valueMin: (-64)
                valueMax: (63)
                dispMin:  (0)
                dispMax:  (127)
                height: parent.height/4
                width: parent.width
                moduleOn: on
            }
            CustomizerSlider
            {
                paramType: DeviceParameter.PREAMP_LOW
                nameValue: "Low"

                valueMin: (-64)
                valueMax: (63)
                dispMin:  (0)
                dispMax:  (127)
                height: parent.height/4
                width: parent.width
                moduleOn: on
            }

            CustomizerSlider
            {
                paramType: DeviceParameter.PREAMP_VOLUME
                nameValue: "Volume"

                height: parent.height/4
                width: parent.width
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
