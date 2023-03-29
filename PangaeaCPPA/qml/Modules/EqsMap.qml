import QtQuick

import Elements 1.0
import CppObjects 1.0

Item
{
    id: main

    property string name
    property string nameValue: "eq_on"

    property bool on

    BaseModule{
        id: _baseModule

        moduleName: main.name
        nameValue: main.nameValue

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
                    visible: !switchEnMap.map
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
        function onSgSetUIParameter(nameParam, value)
        {
            if((nameParam === main.nameValue))
            {
                main.on=value
            }
        }

        function onSgSetParameter(nameParam, value)
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
