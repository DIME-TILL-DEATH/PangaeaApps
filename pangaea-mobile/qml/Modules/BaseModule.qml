import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11

import Elements 1.0
import Tutorials
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

    width: parent.width
    height: parent.height

    color: Style.colorFon

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

        spacing: Style.mainSpacing //width / 75

        Rectangle {
            id: _headerRect

            width: parent.width * 0.25
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


                Text {
                    id: _description

                    Layout.preferredWidth: _headerRect.width * 0.9
                    Layout.preferredHeight: _headerRect.height/4
                    Layout.maximumHeight: _headerRect.height/4
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                    Layout.row: 0
                    Layout.columnSpan: 2

                    font.pixelSize: _headerRect.width * 0.12

                    text: _main.moduleDescription
                    color: _main.on ? Style.currentTheme.colorTextEnabled : Style.currentTheme.colorTextDisabled

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    elide: Text.ElideMiddle
                    wrapMode: Text.WordWrap
                    maximumLineCount: 3

                    visible: showDescription
                }

                Item{
                    Layout.row: 1
                    Layout.column: 0
                    Layout.preferredWidth: _headerRect.width * 0.5
                    Layout.preferredHeight: _headerRect.height/2
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.fillWidth: true

                    Rectangle{
                        width: _text.font.pixelSize*0.9
                        height: width
                        radius: width/2
                        border.width: 1

                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter

                        color: (_main.on) ? Style.currentTheme.colorLedOn : Style.currentTheme.colorLedOff

                        Behavior on color{ColorAnimation{duration:200}}
                    }
                }

               Item{
                    Layout.row: 1
                    Layout.column: 1
                    Layout.preferredWidth: _headerRect.width * 0.5
                    Layout.preferredHeight: _headerRect.height/2
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    Layout.fillWidth: true

                    MText {
                        id: _text

                        anchors.verticalCenter: parent.verticalCenter

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
