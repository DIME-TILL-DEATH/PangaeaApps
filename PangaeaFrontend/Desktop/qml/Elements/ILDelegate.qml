import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import Elements
import StyleSettings
import CppObjects 1.0

import PangaeaBackend

Item{
    id: root

    Row{
        width: parent.width * 0.9
        height: parent.height

        Item{
            id: imageItem

            anchors.verticalCenter: parent.verticalCenter

            width: root.height*0.6
            height: width
            Image
            {
                id: image
                source: (modelData.connectionType === DeviceDescription.BLE) ? "qrc:/Images/bluetooth-icon.svg"
                                                                             : "qrc:/Images/usb-icon.svg"

                anchors.fill: parent

                fillMode: Image.PreserveAspectFit
                transformOrigin: Item.Center
            }

            ColorOverlay {
                anchors.fill: image
                source: image
                color: Style.currentTheme.textEnabled
            }
        }
        Item{
            width: parent.width-imageItem.width
            height: parent.height
            MText{
                width: parent.width
                leftPadding: parent.width/20
                anchors.verticalCenter: parent.verticalCenter

                text: modelData.name + "\n" + modelData.address
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
            listView.autoSelectedItem = index
        }
        onDoubleClicked:
        {
            listView.autoSelectedItem = index
            InterfaceManager.connectToDevice(modelData)
        }
    }
}
