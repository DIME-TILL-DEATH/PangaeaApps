import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    property string name: "LP"
    property bool on: true

    property int paramType: DeviceParameter.LPF_ON

    BaseModule{
        id: _baseModule

        moduleName: qsTr("LP")
        moduleDescription: qsTr("Low-pass filter")

        showDescription: false

        contentItem: Row
        {
            anchors.fill: parent
            spacing: 0

            Column
            {
                height: parent.height
                width: parent.width
                CustomizerSlider
                {
                    units: "Hz"
                    dispMax: 20000
                    dispMin: 1000
                    valueMax: 0
                    valueMin: 195
                    height: parent.height
                    width: parent.width
                    name: "Low-pass filter"
                    paramType: DeviceParameter.LPF_VOLUME

                    bottomLineEnabled: false
                    moduleOn: on
                }
            }
        }
    }

    Connections
    {
        target: UiCore
        function onSgSetUiDeviceParameter(paramType, value)
        {
            if(paramType === main.paramType)
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
            UiCore.setDeviceParameter(main.paramType, main.on)
        }
    }
}
