import QtQuick 2.15
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtCore

import QtQuick.Window 2.15

import Elements 1.0
import ModulesFX 1.0
import StyleSettings 1.0
import Layouts 1.0

import CppObjects
import PangaeaBackend


Column
{
    id: _mainUi
    anchors.fill: parent
    focus: true
    spacing: 2

    property alias modulesListView: _modulesListView


    HeadCP100FX
    {
        id: _headCP100FX

        width:  parent.width
        height: parent.height/1000*200 - _mainUi.spacing * 3
    }

    Rectangle{
        height: parent.height/1000*300
        width: parent.width // * 0.975
        anchors.horizontalCenter: parent.horizontalCenter

        color: Style.mainEnabledColor

        ListView{
            id: _modulesListView

            model: UiCore.currentDevice.modulesListModel;

            anchors.fill: parent

            spacing: 0 //width/500

            interactive: false
            orientation: ListView.Horizontal

            currentIndex: -1

            delegate: Rectangle{
                width: _modulesListView.width/14 - _modulesListView.spacing
                height: _modulesListView.height //* 0.95

                color: "transparent"
                border.width: 2
                border.color: index === _modulesListView.currentIndex ? "white" : "transparent"


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
                    // sourceSize.width:  width
                    // sourceSize.height: height
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
                        if(index !== _modulesListView.currentIndex)
                        {
                            _moduleLoader.source = ""
                            _moduleLoader.selectedModuleInstance = moduleInstance
                            _modulesListView.currentIndex = index
                        }
                    }
                }
            }

            onCurrentIndexChanged: {
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

        width:  parent.width
        height: parent.height/1000*150

        property var selectedModuleInstance

    }
}
