import QtQuick

import Elements 1.0
import CppObjects 1.0

Item
{
    id: main

    property string name: "LP"
    property string nameValue: "lpf_on"

    property bool on

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
                nameValue: "lpf_volume"

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
