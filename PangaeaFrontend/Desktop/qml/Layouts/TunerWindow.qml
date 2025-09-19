import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

import StyleSettings

import Elements
import Layouts

import CP100FX
import CppObjects
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
                text: UiCore.currentDevice.tuner.note

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
                width: parent.width/75
                height: parent.height - parent.border.width * 2

                x: parent.border.width + parent.width/2 - width/2  + UiCore.currentDevice.tuner.cents * parent.width/100
                y: parent.border.width

                color: Style.currentTheme.highlightColor

                visible: UiCore.currentDevice.tuner.note !== "-"

                anchors.verticalCenter: parent.verticalCenter

                radius: height/2

                Behavior on x {
                    NumberAnimation{duration: 100}
                }
            }
        }

        Row{
            Layout.preferredWidth: parent.width
            Layout.fillHeight: true

            MButton{
                id: _leftBtn

                width: 40
                height: parent.height

                text: "-"
                scaleText: 4

                onClicked: {
                    UiCore.currentDevice.tuner.decreaseRefFreq()
                }
            }
            MLabel{
                text: qsTr("Ref. freq: ") + UiCore.currentDevice.tuner.refFreq + "Hz"

                height: 30

                verticalAlignment: Label.AlignVCenter
            }

            MButton{
                id: _rightBtn

                width: 40
                height: 30

                text: "+"
                scaleText: 4

                onClicked: {
                    UiCore.currentDevice.tuner.increaseRefFreq()
                }
            }
        }

    }

    Timer{
        interval: 1000

        running: _root.visible
        repeat: _root.visible

        onTriggered: {
            // UiCore.currentDevice.tuner.getSamples(64 + UiCore.currentDevice.systemSettings.tunerSpeed);
        }
    }

    onVisibilityChanged: {
        UiCore.currentDevice.tuner.moduleEnabled = visible;

        if(visible) UiCore.currentDevice.tuner.getSamples(64 + UiCore.currentDevice.systemSettings.tunerSpeed);
    }
}
