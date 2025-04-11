import QtQuick

import Elements
import StyleSettings

import CppObjects
import PangaeaBackend

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
            width: parent.width //* 0.5
            height: parent.height * 0.25
        }

        Item{
            // item for "visible = false" mode
            width: parent.width //* 0.5
            height: parent.height * 0.2
            anchors.horizontalCenter: parent.horizontalCenter


            ClipIndicator{
                id: clipInd

                visible: UiCore.currentDevice.deviceType > DeviceType.LEGACY_DEVICES

                anchors.centerIn: parent

                width: parent.width
                height: parent.height

                label: qsTr("CLIP")
            }

            Connections{
                target: UiCore.currentDevice

                function onIoClipped(inClips, outClips){
                    clipInd.setIndicator(inClips);
                }
            }
        }

        Image
        {
            width: parent.width*0.9
            height: width

            anchors.horizontalCenter: parent.horizontalCenter

            source:  "qrc:/Images/Jack_6_3.svg"
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
