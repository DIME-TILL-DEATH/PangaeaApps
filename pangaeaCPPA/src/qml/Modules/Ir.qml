import QtQuick 2.15
import QtQuick.Controls 1.5

import Elements 1.0

Item
{
    id: main

    property string name: "IR"
    property string nameValue: "cabinet_enable"

    property bool on: true

    BaseModule{
        id: _baseModule

        moduleName: main.name
        nameValue: main.nameValue

        on: main.on

        contentItem: Column
        {
            anchors.fill: parent
            property alias value: switchIr.value
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

            SwitchIr
            {
                id: switchIr

                width:  parent.width
                height: parent.height/1000*165
                enabled: main.on

                onChValue:
                {
                        _uiCore.setParameter(nameValue, !value);
                    console.log("tumbler value, chValue", _baseModule.contentProperties.value)
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
        target: _uiCore
        function onSgSetUIParameter(nameParam, nameValue)
        {
            if((nameParam === main.nameValue))
            {
                main.on=nameValue
            }
        }

        function onSgSetParameter(nameParam, nameValue)
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
