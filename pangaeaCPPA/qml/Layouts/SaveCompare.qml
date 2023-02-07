import QtQuick

import Elements
import StyleSettings
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
            Item
            {
                width:  parent.width
                height: parent.height/2
                MButton
                {
                    text: "SAVE"
                    enabled: main.editable & main.edit & !main.compareState
                    onClicked: _uiCore.setParameter("save_change", (-1))
                }
            }

            Item
            {
                width:  parent.width
                height: parent.height/2
                MButton
                {
                    id: bComp
                    text: "COMP"

                    enabled:  main.edit
                    highlighted: main.compareState

                    onClicked: {
                        _uiCore.setParameter("compare", 0);
                    }
                }
            }
        }
    }

    Connections
    {
        target: _uiCore

        function onSgSetUIParameter(nameParam, value)
        {
            if(nameParam === "compare_state")
            {
                main.compareState = value;
            }
        }
    }
}
