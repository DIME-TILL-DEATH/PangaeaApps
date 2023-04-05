import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    property bool on: true
    property string nameValue: "compressor_on"

    BaseModule{
        id: _baseModule

        moduleName: qsTr("CM")
        moduleDescription: qsTr("Compressor")

        contentItem: Column
        {
            height: parent.height
            width: parent.width
            CustomizerSlider
            {
                height: parent.height/2
                width: parent.width
                nameValue: "Sustain"
                nameParam: "compressor_sustain"
                moduleOn: on
            }

            CustomizerSlider
            {
                height: parent.height/2
                width: parent.width
                nameValue: "Volume"
                nameParam: "compressor_volume"
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
            if((nameParam===main.nameValue))
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
