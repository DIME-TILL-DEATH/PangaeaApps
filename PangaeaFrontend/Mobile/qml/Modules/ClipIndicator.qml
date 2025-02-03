import QtQuick 2.15

import StyleSettings 1.0
import Elements 1.0

import CppObjects
import PangaeaBackend

Item{
    property int maxClips: 16000/32

    property bool on: true

    function setInIndicator(inClips){
        var inIntensity = Math.log10(10 * inClips / maxClips + 1);
        if(inIntensity > 1) inIntensity = 1;

        if(_indicatorIn.indicationColor.a < inIntensity)
            _indicatorIn.indicationColor.a = inIntensity;
    }

    function setOutIndicator(outClips){
        var outIntensity = Math.log10(10 * outClips / maxClips + 1);
        if(outIntensity > 1) outIntensity = 1;

        if(_indicatorOut.indicationColor.a < outIntensity)
            _indicatorOut.indicationColor.a = outIntensity;
    }
    Row{
        id: _indicatorsRow

        width: parent.width * 0.8 //- spacing * 4
        height: parent.height * 0.95


        anchors.centerIn: parent

        spacing: 4


        Rectangle{
            id: _indicatorIn

            width: parent.height * 0.8
            height: width

            anchors.verticalCenter: parent.verticalCenter

            radius: width/2

            border.width: 1
            border.color: on ? Style.colorText : Style.currentTheme.colorTextDisabled

            property color indicationColor: "red"

            color: indicationColor
        }

        Item{
            id: _txtIn

            width: (parent.width - _indicatorIn.width*2)/6
            height: parent.height
            MText{
                anchors.fill: parent
                text: qsTr("IN")
                color: on ? Style.colorText : Style.currentTheme.colorTextDisabled

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }

        Item{
            width: (parent.width - _indicatorIn.width*2 - _txtIn.width * 2 - _indicatorsRow.spacing * 4)
            height: parent.height
            MText{
                anchors.fill: parent
                text: qsTr("CLIPPING")
                color: on ? Style.colorText : Style.currentTheme.colorTextDisabled

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Item{
            width: (parent.width - _indicatorIn.width*2)/4
            height: parent.height
            MText{
                anchors.fill: parent
                text: qsTr("OUT")
                color: on ? Style.colorText : Style.currentTheme.colorTextDisabled

                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }
        }


        Rectangle{
            id: _indicatorOut

            width: _indicatorIn.width
            height: width

            anchors.verticalCenter: parent.verticalCenter

            radius: width/2

            border.width: 1
            border.color: on ? Style.colorText : Style.currentTheme.colorTextDisabled

            property color indicationColor: "red"

            color: indicationColor
        }

        Timer{
            interval: 50

            repeat: true
            running: true

            onTriggered:{
                if(_indicatorIn.indicationColor.a- 0.05 > 0)
                    _indicatorIn.indicationColor.a =  _indicatorIn.indicationColor.a - 0.025;

                if(_indicatorOut.indicationColor.a- 0.05 > 0)
                    _indicatorOut.indicationColor.a =  _indicatorOut.indicationColor.a - 0.025;
            }
        }

        Component.onCompleted:
        {
            _indicatorIn.indicationColor.a = 0;
            _indicatorOut.indicationColor.a = 0;
        }
    }
}
