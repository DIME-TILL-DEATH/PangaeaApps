import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    width: _list.width
    height: 50

    Column{
        anchors.fill: parent
        Item{
            width: parent.width
            height: parent.height/2

            MText
            {
                color: "white"
                width: parent.width - leftPadding*2
                leftPadding: parent.width/20
                anchors.verticalCenter: parent.verticalCenter

                text: deviceDescription.name
                elide: Text.ElideMiddle
            }
        }

        Item{
            width: parent.width
            height: parent.height/2
            MText
            {
                color: "white"
                width: parent.width - leftPadding*2
                height: parent.height/2
                leftPadding: parent.width/20

                text: deviceDescription.address
                elide: Text.ElideMiddle
            }
        }
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

