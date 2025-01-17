import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11

import Elements 1.0
import Tutorials 1.0
import StyleSettings 1.0

import CppObjects
import CppEnums

Rectangle {
    id: _main

    property bool on: module.moduleEnabled

    property bool disabled: UiCore.currentDevice.presetManager.currentState === PresetState.Compare

    property string moduleName: module.moduleName
    property string moduleDescription: "Description"

    property bool showDescription : true

    property Component contentItem
    property SimpleTutorialMessage tutorialItem

    signal showTutorial()

    signal editingChanged(var editingState)

    width: parent.width
    height: parent.height

    color: Style.colorFon

    property var simmetryElementsWidth: width/6

    Rectangle{
        id: _maskCompare

        anchors.fill: _main

        opacity: 0.25
        visible: disabled
        z: _mainRow.z + 1

        SequentialAnimation on color{
            running: true
            loops: Animation.Infinite
            ColorAnimation {from: "white"; to: "lightcoral"; duration: 1000}
            ColorAnimation {from: "lightcoral"; to: "white"; duration: 1000}
        }
    }

    Row {
        id: _mainRow

        enabled: !disabled

        width: _main.width
        height: _main.height

        spacing: Style.mainSpacing

        Rectangle {
            id: _headerRect

            width: simmetryElementsWidth //parent.width * 0.25
            height: parent.height


            gradient: Gradient{
                GradientStop{
                    position:_main.on ? 0.0 : 1.0
                    color: Style.colorModul

                    Behavior on position {NumberAnimation{duration: 100}}
                }
                GradientStop{position: 1.0; color: "black"}

            }

            radius: Style.baseRadius
            border.width: 1
            border.color: _main.on ? Style.currentTheme.colorBorderOn : Style.currentTheme.colorBorderOff

            Behavior on color {ColorAnimation{duration: 200}}

            GridLayout {
                id: _headerColumn

                anchors.fill: parent

                rows: 3
                columns: 2

               Item{
                    Layout.row: 1
                    Layout.column: 1
                    Layout.preferredWidth: _headerRect.width //* 0.5
                    Layout.preferredHeight: _headerRect.height // /2
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    Layout.fillWidth: true

                    MText {
                        id: _text

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        text: _main.moduleName
                        color: _main.on ? Style.currentTheme.colorTextEnabled : Style.currentTheme.colorTextDisabled
                    }
                }
            }
            MouseArea{
                anchors.fill: parent
                z: 5
                pressAndHoldInterval: 500

                onPressAndHold: {
                    tutorialItem.open()
                }

                onClicked: {
                    module.moduleEnabled = !module.moduleEnabled
                }
            }
        }


        Rectangle {
            id: _contentRect

            width: parent.width - _headerRect.width - _mainRow.spacing
            height: parent.height
            color: Style.colorModul

            clip: true

            radius: Style.baseRadius //width/40
            border.width: 1
            border.color: _main.on ? Style.currentTheme.colorBorderOn : Style.currentTheme.colorBorderOff

            Item {
                id: _contentItem

                anchors.fill: parent
                anchors.rightMargin: parent.width/50
                anchors.leftMargin: parent.width/50
                anchors.bottomMargin: 3
                anchors.topMargin: 3

                Loader{

                    width: _contentItem.width
                    height: _contentItem.height

                    sourceComponent: contentItem
                }
            }
        }
    }
}
