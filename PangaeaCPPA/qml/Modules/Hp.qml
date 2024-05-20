import QtQuick

import Elements 1.0
import CppObjects 1.0

Item
{
    id: main

    property string name: "HP"
    property int paramType: DeviceParameter.HPF_ON
    property bool on: true

    BaseModule{
        id: _baseModule

        moduleName: main.name
        // paramType: main.paramType

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
                height: parent.height/1000*165*4
            }

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "20 Hz 1k"
                checkable: false
                paramType: DeviceParameter.HPF_VOLUME
                valueMax: 255
                dispMin:   20
                dispMax:   1000
                dispValue: ((value*980.0/255.0)+20.0).toFixed()
            }

            Item
            {
                width:  parent.width
                height: parent.height/1000*25
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
}
