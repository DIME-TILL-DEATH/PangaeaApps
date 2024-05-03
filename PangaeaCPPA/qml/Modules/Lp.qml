import QtQuick

import Elements 1.0
import CppObjects 1.0

Item
{
    id: main

    property string name: "LP"
    property int paramType: DeviceParameter.LPF_ON

    property bool on

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
                height: parent.height/1000*165*4
            }

            Dial
            {
                id: _dial
                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "1k Hz 20k"
                checkable: false
                paramType: DeviceParameter.LPF_VOLUME

                angleMin: 140
                angleMax: -140
                valueMax:  195
                valueMin:  0
                dispMin:   1000
                dispMax:   20000
                dispValue: (Math.pow(195.0-value, 2) * (19000.0/Math.pow(195.0, 2.0)) + 1000.0).toFixed()
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
