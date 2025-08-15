import QtQuick 2.15

import StyleSettings 1.0
import Elements 1.0

import CppObjects
import PangaeaBackend

Item{
    id: _root

    property int maxClips: 16000/32

    property bool on: true

    function setIndicator(inClips){
        var intensity = Math.log10(10 * inClips / maxClips + 1);
        if(intensity > 1) intensity = 1;

        if(_indicator.indicationColor.a < intensity)
            _indicator.indicationColor.a = intensity;
    }

    property string label

    Column{
        id: _indicatorsRow

        width: parent.width
        height: parent.height


        anchors.centerIn: parent

        spacing: 4

        opacity: _root.on ? 1 : 0.5

        Rectangle{
            id: _indicator

            property real size: Math.min(parent.width, parent.height)

            width: size / 2
            height: size / 2

            anchors.horizontalCenter: parent.horizontalCenter

            radius: width/2

            border.width: 1
            border.color: Style.currentTheme.borderOn


            property color indicationColor: "red"

            color: indicationColor
        }

        Item{
            id: _txt

            width: parent.width
            height: parent.height / 2
            MText{
                anchors.fill: parent

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                text: _root.label
            }
        }


        Timer{
            interval: 50

            repeat: true
            running: true

            onTriggered:{
                if(_indicator.indicationColor.a- 0.05 > 0)
                    _indicator.indicationColor.a =  _indicator.indicationColor.a - 0.025;
            }
        }

        Component.onCompleted:
        {
            _indicator.indicationColor.a = 0;
        }
    }
}
