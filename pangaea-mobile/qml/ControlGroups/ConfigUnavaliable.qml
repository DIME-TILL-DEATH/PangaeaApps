import QtQuick 2.15

import Elements
import StyleSettings

Item {
    // anchors.centerIn: parent


    property var modulesModel

    MText{
        color: Style.colorText
        width: parent.width * 0.7
        height: parent.height * 0.5

        anchors.centerIn: parent
        wrapMode: Text.WordWrap

        text: qsTr("The configuration of processing modules is not available in this firmware. Please update your device's firmware to the latest version.")
    }
}
