import QtQuick
import QtQuick.Controls

import StyleSettings

Item{
    id: _root

    width: parent.width/3.5
    height: parent.height*0.9
    anchors.verticalCenter: parent.verticalCenter

    property alias value: control.value
    property alias step: control.stepSize
    property alias from: control.from
    property alias to: control.to

    property string annotation

    signal moved()

    Column{
        anchors.fill: parent

        Dial {
            id: control

            value: _root.value
            from: _root.from
            to: _root.to

            snapMode: Dial.SnapAlways

            height: parent.height * 0.7
            width: height

            anchors.horizontalCenter: parent.horizontalCenter

            MText
            {
                anchors.centerIn: parent
                color: Style.mainEnabledColor
                font.pixelSize: parent.width/5
                text: value.toFixed(2)
            }

            background: Rectangle {
                x: control.width / 2 - width / 2
                y: control.height / 2 - height / 2

                width: Math.min(control.width, control.height)
                height: width
                radius: width / 2

                color: "white"

                border.width: 1
                border.color: "black"
            }

            handle: Rectangle {
                id: handleItem

                x: control.background.x + control.background.width / 2 - width / 2
                y: control.background.y + control.background.height / 2 - height / 2

                width: control.width/10
                height: width
                radius: width/2

                color: "black"
                antialiasing: true
                transform: [
                    Translate {
                        y: -Math.min(control.background.width, control.background.height) * 0.4 + handleItem.height / 2
                    },
                    Rotation {
                        angle: control.angle
                        origin.x: handleItem.width / 2
                        origin.y: handleItem.height / 2
                    }
                ]
            }

            onMoved: {
                _root.moved();
            }
        }

        Item{
            width: parent.width
            height: parent.height - control.height
            MText
            {
                anchors.horizontalCenter: parent.horizontalCenter
                color: Style.backgroundColor
                font.pixelSize: parent.width/10
                text: annotation
            }
        }
    }
}

