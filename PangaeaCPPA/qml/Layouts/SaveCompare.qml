import QtQuick

import Elements
import StyleSettings
import CppObjects 1.0

Item
{
    id: main

    property bool editable: true
    property bool edit: true
    property bool compareState: false

    Rectangle
    {
        anchors.fill: parent
        color: Style.mainEnabledColor
        Column
        {
            anchors.fill: parent
            MButton
            {
                width:  parent.width
                height: parent.height/2
                text: "SAVE"
                enabled: main.editable & main.edit & !main.compareState
                onClicked: DeviceProperties.saveChanges();
            }

            MButton
            {
                id: bComp
                width:  parent.width
                height: parent.height/2
                text: "COMP"

                enabled:  main.edit
                highlighted: main.compareState

                onClicked: {
                    UiCore.setParameter("compare", 0);
                }
            }
        }
    }

    Connections
    {
        target: UiCore

        function onSgSetUIParameter(nameParam, value)
        {
            if(nameParam === "compare_state")
            {
                main.compareState = value;
            }
        }
    }
}
