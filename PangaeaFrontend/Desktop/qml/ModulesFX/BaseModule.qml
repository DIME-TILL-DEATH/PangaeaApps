import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11

import Elements 1.0
import StyleSettings 1.0

import CppObjects
import PangaeaBackend

Rectangle {
    id: _main

    property bool on: module.moduleEnabled
    property string moduleName: module.moduleName

    property bool showDescription : true

    property bool disabled: UiCore.currentDevice.presetManager.currentState === PresetState.Compare

    property Component contentItem
    property int dialWidth: dialHeight
    property int dialHeight: height

    width: parent.width
    height: parent.height

    // color: on ? Style.mainEnabledColor : Style.mainDisabledColor
    color: Style.mainEnabledColor

    // property int simmetryElementsWidth: width/6

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

        spacing: width / 250

        Rectangle {
            id: _headerRect

            width: _main.dialWidth
            height: parent.height


            gradient: Gradient{
                GradientStop{
                    position: _main.on ? 0.0 : 1.0
                    color: Style.mainEnabledColor

                    Behavior on position {NumberAnimation{duration: 300}}
                }
                GradientStop{position: 1.0; color: Style.mainDisabledColor}

            }

            radius: width/10
            border.width: 1
            border.color: _main.on ? Style.borderOn : Style.textDisabled

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
                        color: _main.on ? Style.textEnabled : Style.textDisabled
                    }
                }
            }
            MouseArea{
                anchors.fill: parent
                z: 5

                onClicked: {
                    module.moduleEnabled = !module.moduleEnabled
                }
            }
        }

        MButton{
            id: _leftBtn

            visible: _contentItem.contentWidth > _contentItem.width

            width: dialWidth/3
            height: parent.height

            text: "<"
            scaleText: 3

            onClicked: {
                if(_contentItem.contentX > 0)
                {
                    _contentItem.contentX = _contentItem.contentX - dialWidth;
                    _contentItem.returnToBounds();
                }
            }
        }

        Rectangle {
            id: _contentRect

            width: parent.width - _headerRect.width - _mainRow.spacing * 3 - _leftBtn.width * 2
            height: parent.height
            // color: _main.on ? Style.mainEnabledColor : Style.mainDisabledColor
            color: Style.mainEnabledColor

            clip: true


            Flickable{
                id: _contentItem

                contentWidth: _loader.width
                contentHeight: _loader.height

                flickableDirection: Flickable.HorizontalFlick
                boundsBehavior: Flickable.StopAtBounds

                anchors.fill: parent
                anchors.rightMargin: parent.width/100
                anchors.leftMargin: parent.width/100
                anchors.bottomMargin: parent.height/10
                anchors.topMargin: parent.height/10

                // ScrollBar.horizontal:            ScrollBar{
    //                 active: true
    //                 parent: _contentRect
    //                 width: parent.width
    //                 height: parent.height/2

    //                 anchors.top: _contentItem.bottom
    //                 // anchors.left: _contentItem.right
    //                 // anchors.bottom: _contentItem.bottom

    //                 size: 1.0
    //             }

                Behavior on contentX{
                    NumberAnimation{
                        duration: 100
                    }
                }

                Loader{
                    id: _loader

                    // width: _contentItem.width
                    height: _contentItem.height

                    sourceComponent: contentItem
                }
            }
        }

        MButton{
            id: _rightBtn

            visible: _contentItem.contentWidth > _contentItem.width

            width: dialWidth/3
            height: parent.height

            text: ">"
            scaleText: 3

            onClicked: {
                if(_contentItem.contentX < _contentItem.contentWidth - _contentItem.width)
                {
                    _contentItem.contentX = _contentItem.contentX + dialWidth
                    _contentItem.returnToBounds();
                }
            }
        }
    }
}
