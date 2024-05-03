import QtQuick 2.15
//import QtQuick.Controls 2.15

import Elements 1.0

import CppObjects 1.0

Item {
    id: main

    property string name: "ER"
    property int paramType: DeviceParameter.EARLY_ON

    property bool on: true

    BaseModule{
        id: _baseModule

        moduleName: main.name
        paramType: main.paramType

        on: main.on

        contentItem: Column
        {
            anchors.fill: parent
            Item
            {
                width:  parent.width
                height: parent.height/1000*150
            }

            Item
            {
                width:  parent.width
                height: parent.height/1000*165*2
            }

            SwitchRoom
            {
                width:  parent.width
                height: parent.height/1000*165
                enabled: main.on
            }

            Item
            {
                width:  parent.width
                height: parent.height/1000*165
            }

            Dial
            {
                id: _tumbler
                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on

                name: "VOLUME"
                checkable: false
                paramType: DeviceParameter.EARLY_VOLUME
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
