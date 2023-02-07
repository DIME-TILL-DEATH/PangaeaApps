import QtQuick 2.15

import Elements 1.0

Item
{
    id: main

    property string name:     "CM"
    property string nameValue: "compressor_on"

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
                name: "SUSTAIN"
                checkable: false
                nameValue: "compressor_sustein"
            }

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "VOLUME"
                checkable: false
                nameValue: "compressor_volume"
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
