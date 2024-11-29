import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

import StyleSettings 1.0

import CustomOverlays 1.0
import ControlGroups 1.0
import Modules 1.0
import Elements 1.0

import CppObjects

CustomMessageDialog {
    id: _root

    closeOnDisconnect: true

    width: Screen.width * 0.9
    height: Screen.height * 0.85

    parentHeight: height

    x: Screen.width/2-width/2
    y: Screen.height-height

    background: Rectangle {
       id: _backRect

        width: _root.width
        height: _root.height

        gradient: Gradient{
            GradientStop{position: 0.0; color: Style.colorModul}
            GradientStop{position: 1.0; color: "black"}
        }

        radius: Style.baseRadius
        border.color: Style.currentTheme.colorBorderOn
        border.width: 1
    }

    headerText: qsTr("IR management")

    footer: Rectangle {
        id: _dialogButtonBox

        width: _root.width
        height: _root.height*0.1
        color: "transparent"
        radius: Style.baseRadius
        border.color: Style.currentTheme.colorBorderOn
        border.width: 1

        Button{
            text: qsTr("HIDE")

            width: parent.width*0.8
            height: parent.height*0.6

            anchors.centerIn: parent

            onClicked: {
                _root.close()
            }
        }
    }

    Column{
        width: _root.width*0.95
        height: _root.height - _dialogButtonBox.height

        anchors.horizontalCenter: parent.horizontalCenter

        spacing: parent.height * 0.01

        MButton{
            width: parent.width * 0.7
            height: parent.height * 0.08

            anchors.horizontalCenter: parent.horizontalCenter

            textButton: qsTr("Upload IR")
        }

        ListView{
            id: _irListView

            width: parent.width * 0.9
            height: parent.height * 0.75

            anchors.horizontalCenter: parent.horizontalCenter

            currentIndex: UiCore.currentDevice.bank * UiCore.currentDevice.maxPresetCount + UiCore.currentDevice.preset

            model: (_bar.currentIndex === 0) ? UiCore.currentDevice.irsInLibrary
                                             : UiCore.currentDevice.irsInFolder

            boundsBehavior: Flickable.StopAtBounds
            clip: true

            ScrollBar.vertical: ScrollBar{
                active: true
                visible: true
            }

            spacing: 4

            snapMode: ListView.SnapToItem

            delegate: Item{
                width: _irListView.width
                height: _irListView.height/30

                MText{
                    text: modelData.irName

                    anchors.fill: parent

                    color: isCurrentIr() ? Style.currentTheme.colorModulOn: Style.colorText


                    elide: Text.ElideMiddle

                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }

                MouseArea{
                    anchors.fill: parent

                    onClicked: {
                        console.log(UiCore.currentDevice.currentIrFile.irName, UiCore.currentDevice.currentIrFile.irLinkPath,
                                    modelData.irName, modelData.irLinkPath)

                        UiCore.currentDevice.currentIrFile = modelData
                    }
                }

                function isCurrentIr()
                {
                    if((_bar.currentIndex === 1) & UiCore.currentDevice.currentIrFile.irLinkPath === "")
                    {
                        return UiCore.currentDevice.currentIrFile.irName === modelData.irName
                    }
                    else
                    {
                        return (UiCore.currentDevice.currentIrFile.irName === modelData.irName) & (UiCore.currentDevice.currentIrFile.irLinkPath === modelData.irLinkPath)
                    }
                }
            }
        }

        TabBar{
                id: _bar

                width: parent.width
                height: parent.height * 0.08

                currentIndex: 0

                TabButton {
                    height: parent.height
                    anchors.top: parent.top
                    text: qsTr("Library")
                 }
                 TabButton {
                     height: parent.height
                     anchors.top: parent.top
                     text: qsTr("Folder")
                 }
            }
    }

    onOpened: {
        // _presetListView.positionViewAtIndex(_presetListView.currentIndex, ListView.Center)
    }

    onAccepted: {
        _root.close()
    }
}
