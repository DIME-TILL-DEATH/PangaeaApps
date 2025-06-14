import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import QtQml.Models

import ModulesClassic
import StyleSettings

import CppObjects
import PangaeaBackend

Flickable{
    id: _mainFlickable

    flickableDirection: Flickable.HorizontalFlick

    contentWidth: _mainRow.width

    clip: true

    ScrollBar.horizontal: ScrollBar {
        orientation: Qt.Horizontal
        policy: ScrollBar.AlwaysOn
    }

    boundsBehavior: Flickable.StopAtBounds

    Row{
        id: _mainRow

        property bool isPaFirmware: true
        property int modulesCount: 15
        property bool moduleVisible: false
        property int widthWithoutSpaсe: _mainFlickable.width - spacing * 11

        spacing: 2

        width: inp.width + _listViewModules.width + dl.width + er.width + outp.width + spacing * 4
        height: parent.height

        In{
            id: inp

            height: _mainRow.height
            width:  _mainRow.widthWithoutSpaсe/_mainRow.modulesCount/2
        }

        ListView{
            id: _listViewModules

            width: contentWidth
            height: _mainRow.height

            spacing: _mainRow.spacing

            interactive: false
            orientation: ListView.Horizontal

            layoutDirection:  UiSettings.isModulesRightAligned ? Qt.RightToLeft : Qt.LeftToRight

            model: DelegateModel{
                id: _visualModel

                model: UiCore.currentDevice.modulesListModel;
                delegate: DropArea{
                    id: _delegateRoot

                    property int widthMult: 1
                    width: _mainRow.widthWithoutSpaсe/_mainRow.modulesCount * widthMult
                    height: _mainRow.height

                    property int modelIndex
                    property int visualIndex: DelegateModel.itemsIndex

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

                    Column{
                        id: _thing

                        property Item dragParent: _delegateRoot
                        property int visualIndex: _delegateRoot.visualIndex

                        width: _delegateRoot.width
                        height: _delegateRoot.height

                        Drag.active: _mouseAreaDrag.drag.active
                        Drag.source: _thing
                        Drag.hotSpot.x: width / 2
                        Drag.hotSpot.y: _dragRect.y + _dragRect.height / 2

                        opacity: Drag.active ? 0.5 : 1

                        Loader{
                            id: _delegateLoader

                            height: parent.height * 0.925
                            width: parent.width

                            Component.onCompleted: function(){
                                switch(moduleType)
                                {
                                case ModuleType.NG: _delegateLoader.source = "../ModulesClassic/Ng.qml"; break;
                                case ModuleType.CM: _delegateLoader.source = "../ModulesClassic/Cm.qml"; break;
                                case ModuleType.PR: _delegateLoader.source = "../ModulesClassic/Pr.qml"; break;
                                case ModuleType.PA: _delegateLoader.source = "../ModulesClassic/Pa.qml"; break;
                                case ModuleType.TR: _delegateLoader.source = "../ModulesClassic/Tr.qml"; break;
                                case ModuleType.CH: _delegateLoader.source = "../ModulesClassic/Ch.qml"; break;
                                case ModuleType.IR: _delegateLoader.source = "../ModulesClassic/IrCpModern.qml"; break;
                                case ModuleType.EQ1:
                                case ModuleType.EQ2:{
                                    _delegateLoader.source = "../ModulesClassic/EqModern.qml";
                                    _delegateRoot.widthMult = 5
                                    break;
                                }

                                case ModuleType.PH:{
                                    _delegateLoader.source = "../ModulesClassic/Ph.qml";
                                    _delegateRoot.widthMult = 2
                                    break;
                                }
                                }
                                _delegateLoader.item.module = moduleInstance;
                            }

                        }

                        Rectangle{
                            id: _separator
                            width: parent.width
                            height: 1
                        }

                        Rectangle{
                            id: _dragRect

                            width: parent.width
                            height: parent.height * 0.075

                            color: _delegateLoader.item.module.moduleEnabled ? Style.mainEnabledColor : Style.mainDisabledColor

                            MouseArea {
                                id: _mouseAreaDrag
                                anchors.fill: parent

                                drag.target: _thing
                                drag.axis: Drag.XAxis
                                // drag.minimumX: 0//_listViewModules.x
                                // drag.maximumX: _listViewModules.x + _listViewModules.width

                                onPressed: _delegateRoot.modelIndex = visualIndex
                                onReleased: _thing.Drag.drop()
                            }

                            Image{
                                id: _scanImg
                                source: "qrc:/Images/arrows-left-right.svg"

                                width: parent.width * 0.4
                                height: width

                                anchors.fill: parent

                                fillMode: Image.PreserveAspectFit
                            }
                            ColorOverlay {
                                anchors.fill: _scanImg
                                source: _scanImg
                                color: "white"
                            }

                        }
                        states: State {
                                when: _mouseAreaDrag.drag.active
                                ParentChange {
                                    target: _thing
                                    parent: _listViewModules
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

            // Transitions

            add: Transition{
                NumberAnimation { properties: "x"; duration: 200 }
            }

            move: Transition {
                 NumberAnimation { properties: "x"; duration: 250 }
            }

            displaced: Transition {
                 NumberAnimation { properties: "x"; duration: 250 }
             }
        }

        Dl{
            id: dl

            module: UiCore.currentDevice.DL
            height: _mainRow.height
            width: UiCore.currentDevice.DL.used ? _mainRow.widthWithoutSpaсe/_mainRow.modulesCount * 2 : 0
        }

        Er{
            id: er

            module: UiCore.currentDevice.ER
            height: _mainRow.height
            width:  UiCore.currentDevice.ER.used ? _mainRow.widthWithoutSpaсe/_mainRow.modulesCount : 0
        }

        Out{
            id: outp

            height: _mainRow.height
            width:  _mainRow.widthWithoutSpaсe/_mainRow.modulesCount/2
        }
    }
}
