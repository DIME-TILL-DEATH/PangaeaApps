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

        Row
        {
            width: parent.width * 0.8
            height: parent.height * 0.45

            spacing: width/25
            anchors.horizontalCenter: parent.horizontalCenter
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

            text: "OUT"
        }
    }
}
