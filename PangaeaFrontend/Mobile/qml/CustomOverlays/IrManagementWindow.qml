import QtQml
import QtQuick 2.15
import QtQuick.Controls 2.15

import QtQuick.Window 2.15
import QtQuick.Dialogs

import StyleSettings 1.0


import Elements 1.0

import PangaeaBackend
import CppObjects

Item{
    id: _root

    width: parent.width
    height: parent.height * 0.9

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
        opacity: 0.3
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
                anchors.horizontalCenter: parent.horizontalCenter

                width: parent.width * 0.9
                height: parent.height * 0.065

                topInset: 0
                bottomInset: 0

                text: qsTr("UPLOAD IR")

                onClicked: {
                    if(Qt.platform.os === "android")
                    {
                        UiCore.uploadIr("", dstIrPath);

                    }
                    else
                        _iosFileDialog.open();
                }

            }

            ListView{
                id: _irListView

                width: parent.width * 0.95
                height: parent.height * 0.6

                anchors.horizontalCenter: parent.horizontalCenter

                currentIndex: UiCore.currentDevice.bank * UiCore.currentDevice.maxPresetCount + UiCore.currentDevice.preset

                model: (_bar.currentIndex === 0) ? UiCore.currentDevice.irsInLibrary
                                                 : UiCore.currentDevice.irsInFolder

                boundsBehavior: Flickable.StopAtBounds
                // clip: true

                ScrollBar.vertical: ScrollBar{
                    active: true
                    visible: true

                    policy: ScrollBar.AlwaysOn
                }

                clip: true

                spacing: 4

                snapMode: ListView.SnapToItem

                delegate: Item{
                    id: _item

                    width: _irListView.width*0.75
                    height: _irListView.height/20

                    anchors.horizontalCenter: parent.horizontalCenter

                    property var irFile: modelData

                    MText{
                        text: modelData.irName

                        anchors.fill: parent

                        font.pixelSize: _item.height

                        color: isCurrentIr() ? Style.currentTheme.colorModulOn: Style.colorText

                        elide: Text.ElideMiddle

                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                    }

                    MouseArea{
                        id: _mouseAreaDrag

                        anchors.fill: parent

                        drag.target: _item
                        drag.axis: Drag.XAndYAxis
                        // drag.minimumY: 2
                        // drag.maximumY: _listConfig.y + _listConfig.height + _rectDelete.height

                        onClicked: {
                            UiCore.currentDevice.currentIrFile = modelData
                        }

                        onReleased: _item.Drag.drop()
                    }

                    Drag.active: _mouseAreaDrag.drag.active
                    Drag.source: _item
                    Drag.hotSpot.x: width / 2
                    Drag.hotSpot.y: height / 2

                    states: State {
                            when: _mouseAreaDrag.drag.active
                            ParentChange {
                                target: _item
                                parent: _irListView
                            }

                            AnchorChanges {
                                target: _item
                                anchors.horizontalCenter: undefined
                                anchors.verticalCenter: undefined
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

            Rectangle{
                width: parent.width
                height: _root.height * 0.07

                radius: Style.baseRadius
                border.width: 1
                border.color: Style.currentTheme.colorBorderOn

                color: _dropDelete.containsDrag ? "#60FFFFFF" : "transparent"

                Image
                {
                    id: _scanImg
                    source: "qrc:/images/bin.svg"

                    anchors.centerIn: parent

                    height: parent.height * 0.5

                    fillMode: Image.PreserveAspectFit
                }

                DropArea{
                    id: _dropDelete

                    anchors.fill: parent

                    onDropped: function (drag){
                        UiCore.currentDevice.deleteIrFile((drag.source as Item).irFile);
                    }
                }
            }

            TabBar{
                id: _bar

                width: parent.width
                height: parent.height * 0.065

                currentIndex: 0

                TabButton {
                    height: parent.height
                    anchors.top: parent.top
                    text: qsTr("Library")
                 }
                 TabButton {
                     height: parent.height
                     anchors.top: parent.top
                     text: qsTr("Preset folder")
                 }
            }

            Button{
                text: qsTr("HIDE")

                width: parent.width * 0.9
                height: parent.height * 0.065

                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    _root.visible = false
                }

                topInset: 0
                bottomInset: 0
            }
        }
    }

    FileDialog{
        id: _iosFileDialog

        onAccepted:
        {            
            UiCore.uploadIr(_iosFileDialog.currentFile, _root.dstIrPath);

        }
    }
}
