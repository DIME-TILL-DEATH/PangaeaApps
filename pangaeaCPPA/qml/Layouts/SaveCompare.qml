import QtQuick 2.15
////import QtQuick.Controls 1.5
import "../Elements/"
Item
{
    id: main

    property string fonColor: "#EBECEC"
    property string devColor: "#5E5971"

    property bool editable: true
    property bool edit: true
    property bool compareState: false

    Rectangle
    {
        anchors.fill: parent
        color: devColor
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
//                    opacity: enabled?1:0.5
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
