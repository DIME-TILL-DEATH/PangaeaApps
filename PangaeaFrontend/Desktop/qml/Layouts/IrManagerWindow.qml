import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

import StyleSettings 1.0

import Modules 1.0
import Elements 1.0

import CppObjects

Window{
    id: _root

    width: 300
    height: 500

    property string dstIrPath: (_bar.currentIndex === 0) ? "ir_library/"
                                                         : "bank_" + UiCore.currentDevice.bank + "/preset_" + UiCore.currentDevice.preset+ "/"

    signal uploadFileReq();

    title: qsTr("IR management")

    Rectangle{

        width: parent.width
        height: parent.height

        z: parent.z+5

        color: Style.mainEnabledColor

        Column{
            width: parent.width*0.95
            height: parent.height// - _dialogButtonBox.height

            anchors.horizontalCenter: parent.horizontalCenter

            spacing: parent.height * 0.01

            Button{
                width: parent.width * 0.9
                height: parent.height * 0.07

                anchors.horizontalCenter: parent.horizontalCenter

                text: qsTr("UPLOAD IR")

                onClicked: {
                    uploadFileReq();
                }
            }

            ListView{
                id: _irListView

                width: parent.width * 0.9
                height: parent.height * 0.7

                anchors.horizontalCenter: parent.horizontalCenter

                model: (_bar.currentIndex === 0) ? UiCore.currentDevice.irsInLibrary
                                                 : UiCore.currentDevice.irsInFolder

                boundsBehavior: Flickable.StopAtBounds
                // clip: true

                ScrollBar.vertical: ScrollBar{
                    active: true
                    visible: true
                }

                spacing: 4

                snapMode: ListView.SnapToItem

                delegate: Item{
                    id: _item

                    width: _irListView.width
                    height: _irListView.height/25

                    property var irFile: modelData

                    MText{
                        text: modelData.irName

                        anchors.fill: parent

                        font.pixelSize: _item.height

                        color: isCurrentIr() ? Style.highlightColor : Style.textEnabled

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

                border.width: 1
                border.color: "white"

                color: _dropDelete.containsDrag ? "#60FFFFFF" : "transparent"

                ColorImage
                {
                    id: _scanImg
                    source: "qrc:/Images/bin.svg"

                    anchors.centerIn: parent

                    height: parent.height * 0.5

                    fillMode: Image.PreserveAspectFit

                    color: "white"
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
                     text: qsTr("Preset folder")
                 }
            }
        }
    }
}
