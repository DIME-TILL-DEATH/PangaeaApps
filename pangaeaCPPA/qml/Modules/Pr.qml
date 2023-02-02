import QtQuick 2.15
import QtQuick.Controls 1.5

import Elements 1.0

Item
{
    id: main

    property string name:     "PR"
    property string nameValue: "preamp_on"

    property bool on: true

    BaseModule{
        id: _baseModule

        moduleName: main.name
        nameValue: main.nameValue

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
                nameValue: "preamp_high"
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
                nameValue: "preamp_mid"
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
                nameValue: "preamp_low"
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
                nameValue: "preamp_volume"
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
        target: _uiCore
        function onSgSetUIParameter(nameParam, nameValue)
        {
            if((nameParam === main.nameValue))
            {
                main.on=nameValue
            }
        }
    }


    Connections{
        target: _baseModule
        function onSgModuleOnOf()
        {
            main.on = (!main.on);
        }
    }
}
