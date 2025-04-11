import QtQuick

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property CabSim module

    contentItem: Column
    {
        anchors.fill: parent
        Item
        {
            width:  parent.width
            height: parent.height/1000*150
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*75
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*60

            MText {
                horizontalAlignment: Text.AlignHCenter

                // font.pixelSize: Math.min(root.height/27, root.width/2.75)

                anchors.horizontalCenter: parent.horizontalCenter
                // anchors.bottom: parent.bottom

                text: qsTr("CLIP")

                opacity: module.moduleEnabled ? 1 : 0.5
            }
        }

        Item
        {
            width:  parent.width * 0.5
            height: parent.height/1000*160
            anchors.horizontalCenter: parent.horizontalCenter

            ClipIndicator{
                id: inIrClipInd

                anchors.centerIn: parent

                width: parent.width
                height: parent.height

                label: qsTr("IN")

                on: module.moduleEnabled
            }

            Connections{
                target: UiCore.currentDevice

                function onIrClipped(inClips, outClips){
                    inIrClipInd.setIndicator(inClips);
                }
            }
        }

        Item
        {
            width:  parent.width * 0.5
            height: parent.height/1000*160
            anchors.horizontalCenter: parent.horizontalCenter

            ClipIndicator{
                id: outIrClipInd

                anchors.centerIn: parent

                width: parent.width
                height: parent.height

                label: qsTr("OUT")

                on: module.moduleEnabled
            }

            Connections{
                target: UiCore.currentDevice

                function onIrClipped(inClips, outClips){
                    outIrClipInd.setIndicator(outClips);
                }
            }
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }

        ParameterDial
        {

            controlValue: module.send
        }

        SwitchIr
        {
            id: switchIr

            width:  parent.width
            height: parent.height/1000*165
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }
    }
}
