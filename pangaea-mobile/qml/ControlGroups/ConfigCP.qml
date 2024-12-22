// pragma ComponentBehavior: Bound

import QtQuick 2.15
import QtQuick.Effects
import QtQuick.Controls

import Elements
import StyleSettings
import CppObjects

Item {
    id: _main

    Rectangle
    {
        anchors.fill: parent
        color: Style.colorFon
    }

    Column{
        anchors.fill: parent

        ListView{
            id: _listConfig

            width: parent.width
            height: parent.height * 6/8

            z: _rectConf.z + 5

            interactive: false

            // clip: true

            model: DelegateModel{
                id: _visualModel

                model: UiCore.currentDevice.modulesListModel

                delegate: DropArea{
                    id: _delegateRoot

                    property int modelIndex
                    property int visualIndex: DelegateModel.itemsIndex

                    width: _listConfig.width
                    height: _listConfig.height/8

                    onEntered: function (drag) {
                        var from = drag.source.visualIndex;
                        var to = _thing.visualIndex;
                        _visualModel.items.move(from, to);
                    }

                    onDropped: function (drag) {
                        var from = modelIndex;
                        var to = (drag.source as Item).visualIndex;
                        UiCore.currentDevice.modulesListModel.moveModule(from, to);
                    }

                    MouseArea {
                        id: _mouseAreaDrag
                        anchors.fill: parent

                        drag.target: _thing
                        drag.axis: Drag.YAxis //Drag.XAndYAxis
                        drag.minimumY: _listConfig.y
                        drag.maximumY: _listConfig.y + _listConfig.height + _rectDelete.height

                        onPressed: _delegateRoot.modelIndex = visualIndex

                        onReleased: _thing.Drag.drop()
                    }

                    Rectangle{
                        id: _thing

                        property Item dragParent: _delegateRoot
                        property int visualIndex: _delegateRoot.visualIndex

                        width: _delegateRoot.width
                        height: _delegateRoot.height

                        gradient: Gradient{
                            GradientStop{
                                position: 0.0; color: Style.colorModul
                            }
                            GradientStop{position: 1.0; color: "black"}

                        }

                        radius: Style.baseRadius
                        border.color: Style.currentTheme.colorBorderOn
                        border.width: 1

                        Drag.active: _mouseAreaDrag.drag.active
                        Drag.source: _thing
                        Drag.hotSpot.x: width / 2
                        Drag.hotSpot.y: height / 2


                        MText{
                            text: moduleInstance.moduleName + qsTr(", cost: ") + moduleInstance.processingTime

                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter

                            color: Style.currentTheme.colorTextEnabled
                        }

                        states: State {
                                when: _mouseAreaDrag.drag.active
                                ParentChange {
                                    target: _thing
                                    parent: _listConfig
                                }

                                AnchorChanges {
                                    target: _thing
                                    anchors.horizontalCenter: undefined
                                    anchors.verticalCenter: undefined
                                }
                            }
                    }
                }
            }

            //==================Animations=================================
            displaced: Transition {
                NumberAnimation {
                    properties: "x,y";
                    easing.type: Easing.OutQuad;}
            }

            moveDisplaced: Transition {
                     NumberAnimation { properties: "y"; duration: 150 }
            }

            remove: Transition{
                PropertyAction {
                    property: "ListView.delayRemove"
                    value: true
                }
                NumberAnimation {
                    property: "height"
                    to: 0
                    easing.type: Easing.InOutQuad
                }
                PropertyAction {
                    property: "ListView.delayRemove"
                    value: false
                }
            }

        }

        Rectangle{
            id: _rectDelete

            width: parent.width
            height: parent.height * 1/8

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

                onDropped:{
                    console.log("droped index", drag.source.dragParent.modelIndex)
                }
            }
        }

        Rectangle{
            id: _rectConf

            width: parent.width
            height: parent.height * 1/8

            property bool confAcceplatble: UiCore.currentDevice.processingUsed < UiCore.currentDevice.processingBudget
            radius: Style.baseRadius
            border.width: 1
            border.color: Style.currentTheme.colorBorderOn

            color: confAcceplatble ? "green" : "red"

            MText{
                text: "Used " + UiCore.currentDevice.processingUsed
                      + qsTr(" of ") + UiCore.currentDevice.processingBudget

                color: "white"

                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

        }

        Connections{
            target: UiCore.currentDevice.modulesListModel

            function onModelReset(){
                _visualModel.model = UiCore.currentDevice.modulesListModel
            }
        }
    }
}

// SwipeDelegate {
//     id: swipeDelegate

//     signal showLargePreview(var dataPoints)
//     signal hideLargePreview()

//     contentItem: DelegateItem{
//         width: swipeDelegate.width
//         height: swipeDelegate.height

//         Component.onCompleted: {
//             showLargePreview.connect(swipeDelegate.showLargePreview)
//             hideLargePreview.connect(swipeDelegate.hideLargePreview)
//         }
//     }


//     swipe.right: Label {
//         id: deleteLabel
//         text: qsTr("Delete")
//         color: "black"
//         verticalAlignment: Label.AlignVCenter
//         padding: 12
//         height: parent.height
//         anchors.right: parent.right

//         SwipeDelegate.onClicked: PlaylistModel.remove(index)

//         background: Rectangle {
//             color: deleteLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
//         }
//     }

//     background: Rectangle{
//         id: _mainRec

//         color:model.playlistElement.isCurrentPrintingElement ? "lightgray" : "transparent"
//     }

//     onClicked: {
//         _listView.currentIndex = model.index
//     }

//     onDoubleClicked: {
//         PlaylistModel.changePrint(model.index);
//     }
// }
