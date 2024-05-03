import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0

Item
{
    id: main

    property string name:     "CM"
    property int paramType: DeviceParameter.COMPRESSOR_ON

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
                height: parent.height/1000*165*3
            }

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "SUSTAIN"
                checkable: false
                paramType: DeviceParameter.COMPRESSOR_SUSTAIN
            }

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "VOLUME"
                checkable: false
                paramType: DeviceParameter.COMPRESSOR_VOLUME
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
