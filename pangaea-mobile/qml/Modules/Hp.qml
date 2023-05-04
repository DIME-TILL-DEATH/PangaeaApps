import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    property bool on: true
    property string nameValue: "hpf_on"


    BaseModule{
        id: _baseModule

        moduleName: qsTr("HP")
        moduleDescription: qsTr("High-pass filter")

        showDescription: false

        contentItem: Column{
            height: parent.height
            width: parent.width
            CustomizerSlider
            {
                dispMax: 1000
                dispMin: 20
                valueMax: 255
                valueMin: 0

                height: parent.height
                width: parent.width
                nameValue: "High pass filter"
                nameParam: "hpf_volume"
                inverse: true
                bottomLineEnabled: false

                moduleOn: on
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
