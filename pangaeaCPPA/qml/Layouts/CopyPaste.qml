import QtQuick 2.7
//import QtQuick.Controls 1.5
import "../Elements/"
Item
{
    id: main
    property string fonColor: "#EBECEC"
    property string devColor: "#5E5971"
    property bool copyFirst: false

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
                    text: "COPY"
                    enabled: editable & !compareState
                    onClicked: {_uiCore.setParameter("copy", 0); copyFirst=true;}
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
                    onClicked: _uiCore.setParameter("paste", 0)
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
