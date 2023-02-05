import QtQuick 2.15
//import QtQuick.Controls 1.5

import Elements 1.0

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
        on: main.on

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
                MouseArea
                {
                    anchors.fill: parent
                    enabled: !switchEnMap.eqOn
                    hoverEnabled: true
                    cursorShape:  Qt.PointingHandCursor
                    onClicked:
                    {
                        if(!switchEnMap.eqOn)
                        {
                            switchEnMap.eqOn = true;
                            _uiCore.setParameter("eq_on", switchEnMap.eqOn);
                            switchEnMap.redraw();
                        }
                    }
                }

                Eqs
                {
                    on: switchEnMap.eqOn
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
