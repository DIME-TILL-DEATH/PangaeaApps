import QtQuick

import Elements
import StyleSettings
import CppObjects 1.0

Item
{
    id: main

    property bool copyFirst: false

    property bool compareState: false

    Rectangle
    {
        anchors.fill: parent
        color: Style.mainEnabledColor
        Column
        {
            anchors.fill: parent
            Item
            {
                width:  parent.width
                height: parent.height/2
                MButton
                {
                    text: "COPY"
                    enabled: editable & !compareState
                    onClicked: {UiCore.setParameter("copy", 0); copyFirst=true;}
                }
            }

            Item
            {
                width:  parent.width
                height: parent.height/2
                MButton
                {
                    text: "PASTE"
                    scaleText: 0.88
                    enabled: editable & copyFirst & !compareState
                    onClicked: UiCore.setParameter("paste", 0)
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
