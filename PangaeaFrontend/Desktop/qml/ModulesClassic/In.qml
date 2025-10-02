import QtQuick
import Qt5Compat.GraphicalEffects

import Elements
import StyleSettings

import CppObjects
import PangaeaBackend

Rectangle {
    id: root

    color: Style.currentTheme.mainEnabledColor

    property alias fontPixelSize: _text.font.pixelSize
    property alias tunerBtnVisible: _tunerBtn.visible

    signal showTunerWindow()

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

                ignoreUnknownSignals: true

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
            id: _text

            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: Math.min(root.height/27, root.width/2.75)

            anchors.horizontalCenter: parent.horizontalCenter

            text: "IN"
        }

        Rectangle{
            id: _tunerBtn

            width: parent.width*0.9
            height: width

            anchors.horizontalCenter: parent.horizontalCenter

            border.width: 1
            border.color: Style.currentTheme.borderOn

            visible: false

            radius: height/10
            color: "transparent"
            clip: true

            opacity: _btnMa.pressed ? 0.5 : 1.0

            Image
            {
                id: image
                source: "qrc:/Images/tuning-fork.svg"
                anchors.fill: parent
                anchors.margins: parent.width /10

                fillMode: Image.PreserveAspectFit
                transformOrigin: Item.Center
            }

            ColorOverlay {
                anchors.fill: image
                source: image
                color: Style.currentTheme.textEnabled
            }

            MouseArea{
                id: _btnMa

                anchors.fill: parent

                onClicked: {
                    root.showTunerWindow()
                }
            }
        }
    }
}
