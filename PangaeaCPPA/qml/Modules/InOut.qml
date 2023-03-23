import QtQuick
import QtQuick.Layouts

import Elements
import StyleSettings

Rectangle {
    id: root

    property string text: ""

    color: Style.mainEnabledColor

    ColumnLayout {
        id: _layout

        anchors.centerIn: parent
        width: parent.width*0.9
        height: parent.height*0.175

        Image
        {
            width: parent.width*0.9
            height: width

            Layout.preferredWidth:  height
            Layout.preferredHeight: width

            Layout.fillWidth: true

            source:  "qrc:/qml/Images/Jack_6_3.svg"
            sourceSize.width:  width
            sourceSize.height: height
        }

        MText {
            horizontalAlignment: Text.AlignHCenter

            Layout.fillWidth: true
            font.pixelSize: Math.min(root.height/27, root.width/2.75)

            text: root.text
        }
    }
}
