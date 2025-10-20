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

            focusPolicy: Qt.StrongFocus

            anchors.horizontalCenter: parent.horizontalCenter

            from: _root.controlValue.minDisplayValue
            to: _root.controlValue.maxDisplayValue
            value: _root.controlValue.displayValue

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
                text: control.value.toFixed(_root.floatDigits)

                validator: DoubleValidator{
                    bottom: _root.controlValue.minDisplayValue
                    top: _root.controlValue.maxDisplayValue
                    decimals: _root.floatDigits

                    locale: "en"
                }

                onEditingFinished: {
                    if(parseFloat(text) > _root.controlValue.maxDisplayValue) text = _root.controlValue.maxDisplayValue
                    if(parseFloat(text) < _root.controlValue.minDisplayValue) text = _root.controlValue.minDisplayValue

                    _root.controlValue.displayValue = text

                    focus = false
                }

                onFocusChanged: {
                    if(!focus) editingFinished();
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

                color: Style.currentTheme.textInverted
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
                _root.controlValue.displayValue = control.value
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton//Qt.NoButton
                focusPolicy: Qt.StrongFocus

                onWheel: wheel => {
                    var step = (_root.controlValue.maxDisplayValue - _root.controlValue.minDisplayValue)/120/100;
                    // if(control.stepSize === 1) step = (controlValue.maxDisplayValue - controlValue.minDisplayValue)/120/100
                    // else step = control.stepSize
                    var finalValue = control.value + wheel.angleDelta.y * step;

                    if(finalValue > _root.controlValue.maxDisplayValue) finalValue = _root.controlValue.maxDisplayValue;
                    if(finalValue < _root.controlValue.minDisplayValue) finalValue = _root.controlValue.minDisplayValue;

                    _root.controlValue.displayValue = finalValue;
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

