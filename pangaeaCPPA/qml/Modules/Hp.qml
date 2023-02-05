import QtQuick 2.15
//import QtQuick.Controls 1.5

import Elements 1.0

Item
{
    id: main

    property string name: "HP"
    property string nameValue: "hpf_on"

    property bool on: true

    BaseModule{
        id: _baseModule

        moduleName: main.name
        nameValue: main.nameValue

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
                nameValue: "hpf_volume"
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
}
