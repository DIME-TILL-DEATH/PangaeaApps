import QtQuick 2.15
//import QtQuick.Controls 1.5

import Elements 1.0

Item
{
    id: main

    property string name:     "PA"
    property string nameValue: "amp_on"

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
                nameValue: "amp_volume"
            }

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165

                id: presence
                enabled: main.on
                name: "PRESENCE"
                nameValue: "presence_volume"
                checkable: false
                onChValue: main.chPresence(value)
            }

            Dial
            {
                id: slave

                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "SLAVE"
                checkable: false
                nameValue: "amp_slave"
                valueMin:  1
                dispMin:   1
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
