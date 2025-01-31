import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    width: _list.width
    height: 50

    MText
    {
        color: "white"
        width: parent.width-40
        leftPadding: parent.width/20
        anchors.verticalCenter: parent.verticalCenter

        text: deviceDescription.name + "\n" + deviceDescription.address
        // text: name + "\n" + address
    }


    MouseArea
    {
        anchors.fill: parent
        z: 1
        propagateComposedEvents: true

        onClicked:
        {
            _list.autoSelectedItem = index
        }
        onDoubleClicked:
        {
            _list.autoSelectedItem = index
            InterfaceManager.connectToDevice(modelData)
        }
    }
}

