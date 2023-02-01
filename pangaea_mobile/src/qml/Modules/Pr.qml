import QtQuick 2.15
import QtQuick.Controls 2.12

import Elements 1.0
import StyleSettings 1.0

Item
{
    id: main

    property bool on: true
    property string nameValue: "preamp_on"

    BaseModule{
        id: _baseModule

        moduleName: qsTr("PR")
        moduleDescription: qsTr("Preamp")

        contentItem: Column
        {
            height: parent.height
            width: parent.width
            CustomizerSlider
            {
                nameParam: "preamp_high"
                valueMin: (-64)
                valueMax: (63)
                dispMin:  (0)
                dispMax:  (127)
                height: parent.height/4
                width: parent.width
                nameValue: "High"
                moduleOn: on
            }

            CustomizerSlider
            {
                nameParam: "preamp_mid"
                valueMin: (-64)
                valueMax: (63)
                dispMin:  (0)
                dispMax:  (127)
                height: parent.height/4
                width: parent.width
                nameValue: "Mid"
                moduleOn: on
            }
            CustomizerSlider
            {
                nameParam: "preamp_low"
                valueMin: (-64)
                valueMax: (63)
                dispMin:  (0)
                dispMax:  (127)
                height: parent.height/4
                width: parent.width
                nameValue: "Low"
                moduleOn: on
            }

            CustomizerSlider
            {
                nameParam: "preamp_volume"
                height: parent.height/4
                width: parent.width
                nameValue: "Volume"
                bottomLineEnabled: false
                moduleOn: on
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
