import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    property string name:     "PS"
    property bool on: true
    property string nameValue: "presence_on"

    BaseModule{
        id: _baseModule

        moduleName: qsTr("PS")
        moduleDescription: qsTr("Presence")

        showDescription: false

        contentItem:     Row
        {
            anchors.fill: parent
            spacing: 0

            Column
            {
                height: parent.height
                width: parent.width
                CustomizerSlider
                {
                    height: parent.height
                    width: parent.width
                    nameValue: "Presence"
                    nameParam: "presence_volume"
                    bottomLineEnabled: false
                    moduleOn: on
                }
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
