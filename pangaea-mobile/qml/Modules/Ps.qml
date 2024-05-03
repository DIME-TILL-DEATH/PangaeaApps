import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    property string name: "PS"
    property bool on: true
    property int paramType: DeviceParameter.PRESENCE_ON

    BaseModule{
        id: _baseModule

        moduleName: qsTr("PS")
        moduleDescription: qsTr("Presence")

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
                    height: parent.height
                    width: parent.width

                    name: "Presence"
                    paramType: DeviceParameter.PRESENCE_VOLUME

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
                console.log("Settling by sgSetDeviceParameter", paramType);
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
