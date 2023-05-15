import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    property bool on: true
    property string nameValue: "gate_on"

    BaseModule{
        id: _baseModule

        width: parent.width
        height: parent.height

        moduleName: qsTr("NG")
        moduleDescription: qsTr("Noise gate")

        contentItem: Column
         {
             height: parent.height
             width: parent.width
             CustomizerSlider
             {
                 height: parent.height/2
                 width: parent.width
                 nameValue: "Threshold"
                 nameParam: "gate_threshold"
                 moduleOn: on
             }

             CustomizerSlider
             {
                 height: parent.height/2
                 width: parent.width
                 bottomLineEnabled: false
                 nameValue: "Decay"
                 nameParam: "gate_decay"
                 moduleOn: on
             }
         }
    }

    Connections
    {
        target: UiCore
        function onSgSetUIParameter(nameParam, value)
        {
            if((nameParam === main.nameValue))
            {
                main.on=value
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
