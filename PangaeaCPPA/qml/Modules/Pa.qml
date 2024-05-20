import QtQuick

import Elements 1.0
import CppObjects 1.0

Item
{
    id: main

    property string name:     "PA"
    property int paramType: DeviceParameter.AMP_ON

    property bool on: false

    BaseModule{
        id: _baseModule

        moduleName: main.name


        on: main.on

        contentItem: Column
        {
            id: _column
            anchors.fill: parent
            Item
            {
                width:  parent.width
                height: parent.height/1000*70
            }
            Item
            {
                width:  parent.width
                height: parent.height/1000//*90
            }

            TypePA
            {
                width:  parent.width
                height: parent.height/1000*(265+90+55)
                enabled: main.on
            }

            Item
            {
                width:  parent.width
                height: parent.height/1000//*55
            }

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "VOLUME"
                checkable: false
                paramType: DeviceParameter.AMP_VOLUME
            }

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165

                id: presence
                enabled: main.on
                name: "PRESENCE"
                paramType: DeviceParameter.PRESENCE_VOLUME
                checkable: false
            }

            Dial
            {
                id: slave

                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "SLAVE"
                checkable: false
                paramType: DeviceParameter.AMP_SLAVE
                valueMin:  1
                dispMin:   1
            }

            Item
            {
                width:  parent.width
                height: parent.height/1000*25
            }
        }

        // когда включаем/выклаючаем PA, также включить/выключить presence
        onOnChanged: {
            if(main.visible) // только если модуль есть в устройстве
                UiCore.setParameter("pa-ps_linked_on", main.on);
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

                if(main.visible) // только если модуль есть в устройстве
                    UiCore.setParameter("pa-ps_linked_on", main.on);
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
