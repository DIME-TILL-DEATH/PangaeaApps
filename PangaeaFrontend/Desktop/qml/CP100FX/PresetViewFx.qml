import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import Elements 1.0
import ModulesFX 1.0
import StyleSettings 1.0
import Layouts 1.0

import CP100FX 1.0

import CppObjects
import PangaeaBackend


ColumnLayout
{
    id: _mainUi
    focus: true
    spacing: 0

    property alias modulesListView: _modulesListView


    Rectangle{
        Layout.fillWidth: true
        Layout.preferredHeight: parent.height/4

        color: Style.mainEnabledColor

        Row{
            id: _selection

            width: parent.width
            height: parent.height

            Column{
                id: _menuColumn

                height: parent.height
                width:  height

                property int selectedMenu: 0

                MButton
                {
                    width:  parent.width
                    height: parent.height/2
                    text: "MASTER"
                    highlighted: _menuColumn.selectedMenu === 0

                    onClicked: {
                        _menuColumn.selectedMenu = 0
                    }
                }

                MButton
                {
                    width:  parent.width
                    height: parent.height/2
                    text: "PRESET"
                    highlighted: _menuColumn.selectedMenu === 1

                    onClicked: {
                        _menuColumn.selectedMenu = 1
                    }
                }
            }

            Rectangle{
                width: 2
                height: parent.height
            }

            MasterMenu{
                id: _masterMenu

                width: parent.width - _menuColumn.width - 2
                height: parent.height

                visible: !_menuColumn.selectedMenu
            }

            ControllersMenu{
                id: _controllerMenu

                visible: _menuColumn.selectedMenu

                width: parent.width - _menuColumn.width - 2
                height: parent.height
            }
        }
    }

    Rectangle{
        Layout.fillWidth: true
        Layout.preferredHeight: 2
    }

    Rectangle{
        Layout.fillWidth: true
        Layout.fillHeight: true

        color: Style.mainEnabledColor

        ListView{
            id: _modulesListView

            model: UiCore.currentDevice.modulesListModel;

            anchors.fill: parent

            spacing: 0 //width/500

            interactive: false
            orientation: ListView.Horizontal

            // currentIndex: -1
            property var moduleTypeSelected

            // signal moduleTypeSelectedChanged();

            delegate: Item{
                width: _modulesListView.width/14 - _modulesListView.spacing
                height: _modulesListView.height

                Rectangle{
                    width: parent.width
                    height: parent.height * 0.95

                    anchors.centerIn: parent

                    color: "transparent"
                    border.width: 2
                    // border.color: index === _modulesListView.currentIndex ? "white" : "transparent"
                    border.color: _modulesListView.moduleTypeSelected === moduleInstance.moduleType ? "white" : "transparent"

                    Image
                    {
                        id: _image

                        anchors.centerIn: parent
                        width: parent.width
                        height: parent.height

                        fillMode: Image.PreserveAspectFit
                        smooth: true

                        source: "qrc:/Images/pedal.svg"

                        opacity: moduleInstance.moduleEnabled ? 1 : 0.35
                        Behavior on opacity {
                            PropertyAnimation{}
                        }
                    }

                    MText{
                        anchors.horizontalCenter: parent.horizontalCenter

                        y: parent.height * 5.5/10

                        color: "black"

                        text: moduleInstance.moduleName
                    }

                    MouseArea{
                        anchors.fill: parent

                        onClicked: {
                            // if(index !== _modulesListView.currentIndex)
                            // {
                            if(_modulesListView.moduleTypeSelected !== moduleInstance.moduleType)
                            {
                                _moduleLoader.source = ""
                                _moduleLoader.selectedModuleInstance = moduleInstance

                                _modulesListView.moduleTypeSelected = moduleInstance.moduleType
                                _modulesListView.moduleTypeSelectedChanged();
                                // _modulesListView.currentIndex = index
                            }
                        }

                        onDoubleClicked: {
                            moduleInstance.moduleEnabled = ! moduleInstance.moduleEnabled
                        }
                    }
                }
            }

            add: Transition{
                NumberAnimation { properties: "x"; duration: 200 }
            }

            move: Transition {
                 NumberAnimation { properties: "x"; duration: 250 }
            }

            displaced: Transition {
                 NumberAnimation { properties: "x"; duration: 250 }
             }

            // onCurrentIndexChanged: {
            onModuleTypeSelectedChanged:{
                switch(_moduleLoader.selectedModuleInstance.moduleType)
                {
                    case ModuleType.RF: _moduleLoader.source = "../ModulesFX/RF.qml"; break;
                    case ModuleType.NG: _moduleLoader.source = "../ModulesFX/NG.qml"; break;
                    case ModuleType.CM: _moduleLoader.source = "../ModulesFX/CM.qml"; break;
                    case ModuleType.PR: _moduleLoader.source = "../ModulesFX/PR.qml"; break;
                    case ModuleType.PA: _moduleLoader.source = "../ModulesFX/PA.qml"; break;
                    case ModuleType.IR_STEREO: _moduleLoader.source = "../ModulesFX/IR.qml"; break;
                    case ModuleType.EQ1: _moduleLoader.source = "../ModulesFX/EQ.qml"; break;
                    case ModuleType.DELAY: _moduleLoader.source = "../ModulesFX/DL.qml"; break;
                    case ModuleType.PH: _moduleLoader.source = "../ModulesFX/PH.qml"; break;
                    case ModuleType.FL: _moduleLoader.source = "../ModulesFX/FL.qml"; break;
                    case ModuleType.CH: _moduleLoader.source = "../ModulesFX/CH.qml"; break;
                    case ModuleType.ER_MONO:
                    case ModuleType.ER_STEREO: _moduleLoader.source = "../ModulesFX/ER.qml"; break;
                    case ModuleType.RV: _moduleLoader.source = "../ModulesFX/RV.qml"; break;
                    case ModuleType.TR: _moduleLoader.source = "../ModulesFX/TR.qml"; break;
                }
            }
        }
    }

    Loader{
        id: _moduleLoader

        Layout.fillWidth: true
        Layout.preferredHeight: parent.height/4

        property var selectedModuleInstance

        sourceComponent: Rectangle{
            width: _moduleLoader.width
            height: _moduleLoader.height

            color: Style.mainEnabledColor
        }
    }
}
