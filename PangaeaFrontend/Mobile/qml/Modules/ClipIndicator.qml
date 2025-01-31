import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0

import Elements 1.0
import Modules 1.0

import CppObjects
import PangaeaBackend

Rectangle {
    id: _root

    property int maxClips: 16000/16

    color: Style.colorModul

    clip: true

    radius: Style.baseRadius
    border.width: 1
    border.color: Style.currentTheme.colorBorderOn


    Row{
        width: parent.width * 0.8 - spacing * 4
        height: parent.height * 0.95

        anchors.centerIn: parent

        spacing: 4

        Rectangle{
            id: _indicatorIr

            width: parent.height * 0.8
            height: width

            anchors.verticalCenter: parent.verticalCenter

            radius: width/2

            border.width: 1
            border.color: Style.colorText

            property color irIndicationColor: "red"

            color: irIndicationColor
        }

        Item{
            width: (parent.width - _indicatorIr.width*2)/3
            height: parent.height
            MText{
                anchors.fill: parent
                text: qsTr("IR")
                color: Style.colorText

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }

        Item{
            width: (parent.width - _indicatorIr.width*2)/3
            height: parent.height
            MText{
                anchors.fill: parent
                text: qsTr("CLIPPING")
                color: Style.colorText

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Item{
            width: (parent.width - _indicatorIr.width*2)/3
            height: parent.height
            MText{
                anchors.fill: parent
                text: qsTr("OUT")
                color: Style.colorText

                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter

                leftPadding: 50
            }
        }


        Rectangle{
            id: _indicatorOut

            width: parent.height * 0.8
            height: width

            anchors.verticalCenter: parent.verticalCenter

            radius: width/2

            border.width: 1
            border.color: Style.colorText

            property color outIndicationColor: "red"

            color: outIndicationColor
        }
    }


    Timer{
        interval: 125

        repeat: true
        running: true

        onTriggered:{
            if(_indicatorIr.irIndicationColor.a- 0.05 > 0)
                _indicatorIr.irIndicationColor.a =  _indicatorIr.irIndicationColor.a - 0.025;

            if(_indicatorOut.outIndicationColor.a- 0.05 > 0)
                _indicatorOut.outIndicationColor.a =  _indicatorOut.outIndicationColor.a - 0.025;
        }
    }

    Component.onCompleted:
    {
        _indicatorIr.irIndicationColor.a = 0;
        _indicatorOut.outIndicationColor.a = 0;
    }

    Connections{
        target: UiCore.currentDevice

        function onSigClipped(irClips, outClips){

            var irIntensity = Math.log10(10 * irClips / _root.maxClips + 1);
            var outIntensity = Math.log10(10 * outClips / _root.maxClips + 1);

            if(irIntensity > 1) irIntensity = 1;
            if(outIntensity > 1) outIntensity = 1;

            if(_indicatorIr.irIndicationColor.a < irIntensity)
                _indicatorIr.irIndicationColor.a = irIntensity;

            if(_indicatorOut.outIndicationColor.a < outIntensity)
                _indicatorOut.outIndicationColor.a = outIntensity;
        }
    }
}
