import QtQuick
import QtQuick.Controls

import StyleSettings

import CppObjects
import PangaeaBackend

Item{
    id: _root

    width:  parent.width
    height: parent.height/1000*165

    enabled: module.moduleEnabled


    required property ControlValue controlValue
    opacity: _root.enabled ? 1:0.5

    property alias step: control.stepSize
    property alias font: _txtParamName.font

    property int floatDigits: 0

    property string name: controlValue.name
    property string units: controlValue.units

    signal moved()

    onControlValueChanged: {
        control.from = controlValue.minDisplayValue
        control.to = controlValue.maxDisplayValue
    }

    Column{
        anchors.fill: parent

        Dial {
            id: control

            height: parent.height * 0.7
            width: height

            anchors.horizontalCenter: parent.horizontalCenter

            from: controlValue.minDisplayValue
            to: controlValue.maxDisplayValue
            value: controlValue.displayValue

            wheelEnabled: true

            stepSize: 1

            snapMode: Dial.SnapAlways


            TextInput{
                id: _txtInput

                inputMethodHints: Qt.ImhFormattedNumbersOnly

                anchors.centerIn: parent
                color: acceptableInput ? Style.currentTheme.textCombo : Style.currentTheme.error

                font.bold: true
                font.family: "Arial Black"
                font.pixelSize: parent.width/5
                text: control.value.toFixed(floatDigits)

                validator: DoubleValidator{
                    bottom: controlValue.minDisplayValue
                    top: controlValue.maxDisplayValue
                    decimals: floatDigits

                    locale: "en"
                }

                onEditingFinished: {
                    if(parseFloat(text) > controlValue.maxDisplayValue) text = controlValue.maxDisplayValue
                    if(parseFloat(text) < controlValue.minDisplayValue) text = controlValue.minDisplayValue

                    controlValue.displayValue = text

                    focus = false
                }
            }

            background: Rectangle {
                x: control.width / 2 - width / 2
                y: control.height / 2 - height / 2

                width: control.width
                height: width
                radius: width / 2

                color: Style.currentTheme.backgroundColor

                border.width: 1
                border.color: Style.currentTheme.textInverted
            }

            handle: Rectangle {
                id: handleItem

                x: control.background.x + control.background.width / 2 - width / 2
                y: control.background.y + control.background.height / 2 - height / 2

                width: control.width/10
                height: width
                radius: width/2

                color: Style.currentTheme.mainDisabledColor
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
                controlValue.displayValue = control.value
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton//Qt.NoButton

                onWheel: wheel => {
                    var step = (controlValue.maxDisplayValue - controlValue.minDisplayValue)/120/100;
                    // if(control.stepSize === 1) step = (controlValue.maxDisplayValue - controlValue.minDisplayValue)/120/100
                    // else step = control.stepSize
                    controlValue.displayValue = control.value + wheel.angleDelta.y * step;
                }

                onClicked: mouse => {
                    _txtInput.focus = true;
                }
            }
        }

        Item{
            width: parent.width
            height: parent.height - control.height
            MText
            {
                id: _txtParamName

                anchors.horizontalCenter: parent.horizontalCenter

                maximumLineCount: 2
                wrapMode: Text.WordWrap

                color: _root.enabled ? Style.currentTheme.textEnabled : Style.currentTheme.textDisabled
                font.pixelSize: Math.min(parent.height*0.9, parent.width/8)
                text: _root.units !=="" ? _root.name + ", " + _root.units  : _root.name
            }
        }
    }
}

