import QtQuick 2.15
import QtQuick.Controls 1.5

import Elements 1.0

Item
{
    id: main

    property string name:     "NG"
    property string nameValue: "gate_on"

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
                height: parent.height/1000*165*3
            }

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "THRESH"
                checkable: false
                nameValue: "gate_threshold"
            }

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165
                enabled: main.on
                name: "DECAY"
                checkable: false
                nameValue: "gate_decay"
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
