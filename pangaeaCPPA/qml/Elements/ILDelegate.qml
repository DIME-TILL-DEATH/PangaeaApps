import QtQuick
import QtQuick.Controls

import Elements
import StyleSettings
import CppObjects 1.0

Item{
    MText{
        width: parent.width-40
        leftPadding: parent.width/20
        anchors.verticalCenter: parent.verticalCenter

        text: modelData.name + "\n" + modelData.address
    }

    MouseArea
    {
        anchors.fill: parent
        z: 1
        propagateComposedEvents: true

        onClicked:
        {
            listView.autoSelectedItem = index
        }
        onDoubleClicked:
        {
            listView.autoSelectedItem = index
            InterfaceManager.connectToDevice(modelData)
        }
    }
}
