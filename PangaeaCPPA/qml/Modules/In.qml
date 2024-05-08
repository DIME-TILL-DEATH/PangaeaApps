import QtQuick

import Elements
import StyleSettings

import CppObjects

Rectangle {
    id: root

    color: Style.mainEnabledColor

    Column {
        id: _layout

        anchors.centerIn: parent
        width: parent.width*0.9
        height: parent.height*0.75

        spacing: height/50

        Item{
            // item for "visible = false" mode
            width: parent.width * 0.5
            height: parent.height * 0.45
            anchors.horizontalCenter: parent.horizontalCenter
            SignalBar
            {
                id: _bar
                anchors.fill: parent
            }
        }

        Connections{
            target: UiCore

            function onSgSetUiDeviceParameter(paramType, value)
            {
                switch(paramType)
                {
                case DeviceParameter.SIGNAL_IN: _bar.value = value; break;
                }
            }
        }

        Image
        {
            width: parent.width*0.9
            height: width

            anchors.horizontalCenter: parent.horizontalCenter

            source:  "qrc:/qml/Images/Jack_6_3.svg"
            sourceSize.width:  width
            sourceSize.height: height
        }

        MText {
            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: Math.min(root.height/27, root.width/2.75)

            anchors.horizontalCenter: parent.horizontalCenter

            text: "IN"
        }
    }
}
