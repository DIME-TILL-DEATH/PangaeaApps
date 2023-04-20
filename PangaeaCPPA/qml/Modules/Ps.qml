import QtQuick

import Elements 1.0
import CppObjects 1.0

Item
{
    id: main

    property string name:     "PS"
    property string nameValue: "presence_on"

    property bool on: true

    signal chPresence(int value)

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
                id: presence

                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "PRESENCE"
                nameValue: "presence_volume"
                checkable: false
                onChValue: function (value) {
                    main.chPresence(value)
                }
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
