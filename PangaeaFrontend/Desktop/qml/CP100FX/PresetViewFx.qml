import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import Elements 1.0
import ModulesClassic
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

    function getModuleComponent(moduleType) {
        switch(moduleType) {
            case ModuleType.RF: return rfComponent;
            case ModuleType.NG: return ngComponent;
            case ModuleType.CM: return cmComponent;
            case ModuleType.PR: return prComponent;
            case ModuleType.PA: return paComponent;
            case ModuleType.IR_STEREO: return irComponent;
            case ModuleType.EQ1: return eqComponent;
            case ModuleType.DELAY: return dlComponent;
            case ModuleType.PH: return phComponent;
            case ModuleType.FL: return flComponent;
            case ModuleType.CH: return chComponent;
            case ModuleType.ER_MONO:
            case ModuleType.ER_STEREO: return erComponent;
            case ModuleType.RV: return rvComponent;
            case ModuleType.TR: return trComponent;
            default: return null;
        }
    }

    Component { id: rfComponent; RF {} }
    Component { id: ngComponent; NG {} }
    Component { id: cmComponent; CM {} }
    Component { id: prComponent; PR {} }
    Component { id: paComponent; PA {} }
    Component { id: irComponent; IR {} }
    Component { id: eqComponent; EQ {} }
    Component { id: dlComponent; DL {} }
    Component { id: phComponent; PH {} }
    Component { id: flComponent; FL {} }
    Component { id: chComponent; CH {} }
    Component { id: erComponent; ER {} }
    Component { id: rvComponent; RV {} }
    Component { id: trComponent; TR {} }
    Component { id: meqComponent; MEQ {} }


    Rectangle{
        Layout.fillWidth: true
        Layout.preferredHeight: parent.height/4

        color: Style.currentTheme.mainEnabledColor

        Row{
            id: _selection

            width: parent.width
            height: parent.height

            Column{
                id: _menuColumn

                height: parent.height
                width:  height * 1.5

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

                anchors.margins: 4

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

        color: Style.currentTheme.mainEnabledColor

        RowLayout{
            anchors.fill: parent

            In{
                id: inp

                Layout.preferredWidth: parent.width/40
                Layout.fillHeight: true

                tunerBtnVisible: true

                fontPixelSize: Math.min(height/15, width/2.75)

                onShowTunerWindow:{
                    tunerWindow.show()
                }

                TunerWindow{
                    id: tunerWindow
                }
            }

            ListView{
                id: _modulesListView

                model: UiCore.currentDevice.modulesListModel;

                Layout.fillWidth: true
                Layout.fillHeight: true

                spacing: 0

                interactive: false
                orientation: ListView.Horizontal

                property var moduleTypeSelected

                delegate: Item{
                    width: _modulesListView.width/14 - _modulesListView.spacing
                    height: _modulesListView.height

                    Rectangle{
                        width: parent.width
                        height: parent.height * 0.95

                        anchors.centerIn: parent

                        color: "transparent"
                        border.width: 2
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

                            y: parent.height/2 + parent.height * 1/20

                            color: "black"

                            text: moduleInstance.moduleName
                        }

                        Rectangle{
                            width: parent.width/10
                            height: width
                            radius: width/2

                            x: parent.width/2 + _image.paintedWidth * 2/10
                            y: parent.height/2 + _image.paintedHeight * 2/10

                            border.width: 1

                            color: moduleInstance.moduleEnabled ? "red" : "grey"
                        }


                        MouseArea{
                            anchors.fill: parent

                            onClicked: {
                                if(_modulesListView.moduleTypeSelected !== moduleInstance.moduleType)
                                {
                                    _moduleLoader.sourceComponent = null
                                    _moduleLoader.selectedModuleInstance = moduleInstance
                                    _modulesListView.moduleTypeSelected = moduleInstance.moduleType

                                    _modulesListView.moduleTypeSelectedChanged();
                                }
                                else
                                {
                                    moduleInstance.moduleEnabled = ! moduleInstance.moduleEnabled
                                }
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

                populate: Transition {
                     NumberAnimation { properties: "x"; duration: 200 }
                 }

                onModuleTypeSelectedChanged:{
                    _moduleLoader.sourceComponent = getModuleComponent(_moduleLoader.selectedModuleInstance.moduleType);
                }
            }

            Out{
                Layout.preferredWidth: parent.width/40
                Layout.fillHeight: true

                fontPixelSize: Math.min(height/15, width/2.75)
            }
        }
    }

    Rectangle{
        Layout.fillWidth: true
        Layout.preferredHeight: parent.height/4
        color: Style.currentTheme.mainEnabledColor

        Loader{
            id: _moduleLoader

            anchors.fill: parent
            anchors.margins: 4

            property var selectedModuleInstance
        }
    }
}
