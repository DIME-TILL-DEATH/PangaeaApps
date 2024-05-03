import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0

Item
{
    id: main

    property string name:     "PR"
    property int paramType: DeviceParameter.PREAMP_ON

    property bool on: true

    BaseModule{
        id: _baseModule

        moduleName: main.name
        paramType: main.paramType

        on: main.on

        contentItem: Column
        {
            id: _column
            anchors.fill: parent
            Item
            {
                width:  parent.width
                height: parent.height/1000*150
            }

            Item
            {
                width:  parent.width
                height: parent.height/1000*165
            }

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "HIGH"
                checkable: false
                paramType: DeviceParameter.PREAMP_HIGH
                valueMin: (-64)
                valueMax: (63)
                dispMin:  (0)
                dispMax:  (127)
            }

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "MID"
                checkable: false
                paramType: DeviceParameter.PREAMP_MID
                valueMin: (-64)
                valueMax: (63)
                dispMin:  (0)
                dispMax:  (127)
            }

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "LOW"
                checkable: false
                paramType: DeviceParameter.PREAMP_LOW
                valueMin: (-64)
                valueMax: (63)
                dispMin:  (0)
                dispMax:  (127)
            }

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165
                enabled: main.on
                name: "VOLUME"
                checkable: false
                paramType: DeviceParameter.PREAMP_VOLUME
            }

            Item
            {
                width:  parent.width
                height: parent.height/1000*25
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
                main.on = value;
            }
        }
    }


    Connections{
        target: _baseModule
        function onSgModuleOnOf()
        {
            main.on = (!main.on);
            UiCore.setDeviceParameter(main.paramType, main.on);
        }
    }
}
