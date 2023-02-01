import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0

Item {
    id: main

    property string name: "ER"
    property string nameValue: "early_on"

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
                nameValue: "early_volume"
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
                main.on=nameValue;
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
