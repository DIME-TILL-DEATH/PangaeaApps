import QtQuick

import Elements 1.0

import CppObjects 1.0

Item
{
    id: main

    property string name: "IR"
    property int paramType: DeviceParameter.CABINET_ENABLE

    property bool on: true

    BaseModule{
        id: _baseModule

        moduleName: main.name
        paramType: main.paramType

        on: main.on

        contentItem: Column
        {
            anchors.fill: parent
            property alias value: switchIr.value
            Item
            {
                width:  parent.width
                height: parent.height/1000*150
            }

            Item
            {
                width:  parent.width
                height: parent.height/1000*165*4
            }

            SwitchIr
            {
                id: switchIr

                width:  parent.width
                height: parent.height/1000*165
                enabled: main.on

                onChValue: value => {
                    UiCore.setDeviceParameter(main.paramType, !value);
                }
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

        function onSgSetDeviceParameter(paramType, value)
        {
            if((paramType === main.paramType))
            {
                main.on=value
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
