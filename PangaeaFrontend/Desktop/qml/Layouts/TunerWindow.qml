import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

import Qt5Compat.GraphicalEffects

import StyleSettings

import Elements
import Layouts

import CP100FX
import PangaeaFrontend
import PangaeaBackend

Window{
    id: _root

    title: qsTr("Tuner")

    width: Screen.width * 0.4
    height: width * 0.25

    modality: Qt.ApplicationModal
    flags: Qt.Dialog | Qt.WindowStaysOnTopHintS

    color: Style.currentTheme.mainEnabledColor

    minimumWidth: Screen.width * 0.4 * 1
    minimumHeight: Screen.width * 0.3 * 0.5 * 0.75
    maximumWidth: Screen.width * 0.4 * 1
    maximumHeight: Screen.width * 0.3 * 0.5 * 0.75

    property Cp100fx cp100fx: UiCore.currentDevice as Cp100fx

    ColumnLayout{
        anchors.fill: parent

        anchors.leftMargin: 50
        anchors.rightMargin: 50
        anchors.topMargin: 15
        anchors.bottomMargin: 10

        Row{
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: parent.width/5
            MLabel{
                text: qsTr("Note: ")

            }

            MLabel{
                text: _root.cp100fx.tuner.note

            }
        }

        Rectangle{
            id: _boundRect

            // anchors.centerIn: parent

            Layout.fillWidth: true
            Layout.preferredHeight: _root.height * 0.5

            color: "transparent"

            border.width: 2
            border.color: Style.currentTheme.borderOn

            Repeater{
                model: 9
                Rectangle{
                    required property int index

                    x: _boundRect.width/10 * (index + 1)
                    y: _boundRect.border.width

                    width: 2
                    height: _boundRect.height - _boundRect.border.width * 2

                    color: Style.currentTheme.borderOff
                }

            }

            Rectangle{
                id: bar

                width: parent.width/75
                height: parent.height - parent.border.width * 2

                property bool isBullseye: Math.abs(_root.cp100fx.tuner.cents) < 5

                x: parent.border.width + parent.width/2 - width/2  + _root.cp100fx.tuner.cents * parent.width/100 * !isBullseye
                y: parent.border.width

                color: Style.currentTheme.highlightColor

                visible: _root.cp100fx.tuner.note !== "-"

                anchors.verticalCenter: parent.verticalCenter

                radius: height/2

                Behavior on x {
                    NumberAnimation{duration: 100}
                }
            }

            RectangularGlow{
                id: effect

                anchors.fill: bar

                visible: _root.cp100fx.tuner.note !== "-" & bar.isBullseye

                glowRadius: 5
                spread: 0.1
                color: "green"
                cornerRadius: bar.radius + glowRadius
            }
        }

        Row{
            Layout.preferredWidth: _boundRect.width
            Layout.fillHeight: true

            MButton{
                id: _leftBtn

                width: 40
                height: parent.height

                text: "-"
                scaleText: 4

                onClicked: {
                    _root.cp100fx.tuner.decreaseRefFreq()
                }
            }
            MLabel{
                id: _refFreqLabel

                text: qsTr("Ref. freq: ") + _root.cp100fx.tuner.refFreq + "Hz"

                height: parent.height

                verticalAlignment: Label.AlignVCenter
            }

            MButton{
                id: _rightBtn

                width: 40
                height: parent.height

                text: "+"
                scaleText: 4

                onClicked: {
                    UiCore.currentDevice.tuner.increaseRefFreq()
                }
            }

            Item{
                width: parent.width - _tunerSpeedBar.width - _leftBtn.width * 2 - _refFreqLabel.width
                height: parent.height/2
            }

            ParameterBar{
                id: _tunerSpeedBar

                width: parent.width/2.5
                height: parent.height



                controlValue: _root.cp100fx.systemSettings.tunerSpeed
            }
        }

    }

    Timer{
        interval: 1000

        running: _root.visible
        repeat: _root.visible

        onTriggered: {
            _root.cp100fx.tuner.getSamples(64 + _root.cp100fx.systemSettings.tunerSpeed.displayValue);
        }
    }

    onVisibilityChanged: {
        UiCore.currentDevice.tuner.moduleEnabled = visible;

        InterfaceManager.logData = !visible;

        if(visible)
        {
            _root.cp100fx.tuner.getRefFreq()
            _root.cp100fx.tuner.getSamples(64 + _root.cp100fx.systemSettings.tunerSpeed.displayValue);
        }
    }
}
