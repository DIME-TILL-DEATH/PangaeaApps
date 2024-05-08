import QtQuick

import Elements 1.0
import CppObjects 1.0

Item
{
    id: main

    property string name
    property int paramType: DeviceParameter.EQ_ON

    property bool on

    BaseModule{
        id: _baseModule

        moduleName: main.name
        paramType: main.paramType

        isHeaderVisible: false
        on: true

        contentItem: Column
        {
            anchors.fill: parent

            SwitchEqMap
            {
                id: switchEnMap

                eqOn: main.on

                height: parent.height/1000*72
                width:  parent.width

                z: _baseModule.z+1
            }

            Item
            {
                height: parent.height/1000*928
                width:  parent.width

                Eqs
                {
                    on: main.on
                    visible: (!switchEnMap.map) & (UiSettings.eqClassicView)
                    anchors.fill: parent
                }

                EqParametric
                {
                    on: main.on
                    visible: (!switchEnMap.map) & (!UiSettings.eqClassicView)
                    anchors.fill: parent
                }

                Map
                {
                    visible: switchEnMap.map
                    anchors.fill: parent
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
                main.on = value;
            }
        }

        function onSgSetDeviceParameter(paramType, value)
        {
            if((paramType === main.paramType))
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
            UiCore.setDeviceParameter(main.paramType, main.on);
        }
    }
}
