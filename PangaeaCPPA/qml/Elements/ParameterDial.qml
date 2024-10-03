import QtQuick
import QtQuick.Controls

import StyleSettings

import CppObjects

Item{
    id: _root

    width:  parent.width
    height: parent.height/1000*165

    required property ControlValue controlValue

    property alias step: control.stepSize

    property string name: controlValue.name
    property string units: controlValue.units

    signal moved()

    Column{
        anchors.fill: parent

        Dial {
            id: control

            from: controlValue.minValue
            to: controlValue.maxValue
            value: controlValue.value

            stepSize: 1

            snapMode: Dial.SnapAlways

            height: parent.height * 0.7
            width: height

            anchors.horizontalCenter: parent.horizontalCenter

            MText
            {
                anchors.centerIn: parent
                color: Style.mainEnabledColor
                font.pixelSize: parent.width/5
                text: control.value.toFixed(2)
            }

            background: Rectangle {
                x: control.width / 2 - width / 2
                y: control.height / 2 - height / 2

                width: Math.min(control.width, control.height)
                height: width
                radius: width / 2

                color: (controlValue.enabled) ? "white" : "darkgrey"

                border.width: 1
                border.color: "darkgrey"
            }

            handle: Rectangle {
                id: handleItem

                x: control.background.x + control.background.width / 2 - width / 2
                y: control.background.y + control.background.height / 2 - height / 2

                width: control.width/10
                height: width
                radius: width/2

                color: Style.mainDisabledColor
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
                controlValue.value = control.value
            }
        }

        Item{
            width: parent.width
            height: parent.height - control.height
            MText
            {
                anchors.horizontalCenter: parent.horizontalCenter
                color: (controlValue.enabled) ? "white" : "darkgrey"
                font.pixelSize: parent.width/10
                text: _root.name
            }
        }
    }
}

