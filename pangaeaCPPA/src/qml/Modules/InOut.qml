import QtQuick 2.15
import QtQuick.Layouts 1.15

import Elements 1.0

Rectangle {
    id: root

    property string fonColor: "#EBECEC"
    property string devColor: "#5E5971"
    property string devColorDis: "#7E7991"

    property string text: ""

    color: devColor

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

            color: "#EBECEC"

            font.family: "Arial Black"
            font.bold: true
            font.pixelSize: root.height/27

            text: root.text
        }
    }
}
