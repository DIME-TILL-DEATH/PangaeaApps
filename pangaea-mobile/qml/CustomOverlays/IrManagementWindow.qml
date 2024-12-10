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

Item{
    id: _root

    width: parent.width
    height: parent.height

    z: parent.z+10

    property string dstIrPath: (_bar.currentIndex === 0) ? "ir_library/"
                                                         : "bank_" + UiCore.currentDevice.bank + "/preset_" + UiCore.currentDevice.preset+ "/"


    MouseArea{
        z: _irListView.z-5
        anchors.fill: parent
    }

    Rectangle{
        id: _back

        anchors.fill: parent
        color: "white"
        opacity: 0.6
    }

    Rectangle{

        width: parent.width * 0.9
        height: parent.height * 0.9

        anchors.centerIn: parent

        z: parent.z+5

        gradient: Gradient{
            GradientStop{position: 0.0; color: Style.colorModul}
            GradientStop{position: 1.0; color: "black"}
        }

        radius: Style.baseRadius
        border.color: Style.currentTheme.colorBorderOn
        border.width: 1

        Column{
            width: parent.width*0.95
            height: parent.height// - _dialogButtonBox.height

            anchors.horizontalCenter: parent.horizontalCenter

            spacing: parent.height * 0.01

            Item{
                width: parent.width
                height: parent.height * 0.05
                MText{
                    text: qsTr("IR management")
                    color: "white"
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

            }

            Rectangle {
                id: _sepTop

                width: parent.width
                height: 2
                color: Style.currentTheme.colorBorderOn
            }

            Button{
                width: parent.width * 0.9
                height: parent.height * 0.07

                anchors.horizontalCenter: parent.horizontalCenter

                text: qsTr("UPLOAD IR")

                onClicked: {
                    UiCore.uploadIr("", dstIrPath);
                }
            }

            ListView{
                id: _irListView

                width: parent.width * 0.9
                height: parent.height * 0.65

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
                            UiCore.currentDevice.currentIrFile = modelData
                        }
                    }

                    function isCurrentIr()
                    {
                        if((_bar.currentIndex === 1) & UiCore.currentDevice.currentIrFile.irLinkPath === "")
                        {
                            return (UiCore.currentDevice.currentIrFile.irName.indexOf(modelData.irName) !== -1)
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
                height: parent.height * 0.07

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

            Button{
                text: qsTr("HIDE")

                width: parent.width * 0.9
                height: parent.height * 0.07

                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    _root.visible = false
                }
            }
        }
    }
}
