import QtQuick 2.15
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtCore

import QtQuick.Window 2.15

import Elements 1.0
import ModulesFX 1.0
import StyleSettings 1.0
import Layouts 1.0

import CP100FX 1.0

import CppObjects
import PangaeaBackend


Column
{
    id: _mainUi
    focus: true
    // spacing: 2

    property alias modulesListView: _modulesListView


    Rectangle{
        width: parent.width
        height: parent.height/4
        anchors.horizontalCenter: parent.horizontalCenter

        color: Style.mainEnabledColor

        Row{
            anchors.fill: parent
            ParameterComboBox{
                id: _attenuator

                width: height * 1.5
                height: parent.height

                anchors.verticalCenter: parent.verticalCenter

                ctrlValInstance: UiCore.currentDevice.attenuatorVolume.volume

                moduleOn: true

                model: [" +4 dB", " +3 dB", " +2 dB", " +1 dB", "  0 dB", " -1 dB", " -2 dB", " -3 dB",
                    " -4 dB", " -5 dB", " -6 dB", " -7 dB", " -8 dB", " -9 dB", "-10 dB", "-11 dB", "-12 dB",
                    "-13 dB", "-14 dB", "-15 dB", "-16 dB", "-17 dB", "-18 dB", "-19 dB", "-20 dB", "-21 dB",
                    "-23 dB", "-24 dB", "-27 dB", "-30 dB", "-34 dB", "-35 dB", "-36 dB", "-37 dB", "-38 dB",
                    "-40 dB", "-41 dB", "-42 dB", "-43 dB", "-44 dB", "-45 dB", "-46 dB", "-47 dB", "-48 dB",
                    "-49 dB", "-50 dB", "-51 dB", "-52 dB", "-53 dB", "-54 dB", "-55 dB", "-56 dB", "-57 dB",
                    "-58 dB", "-59 dB", "-60 dB"]
            }

            Rectangle{
                width: 1
                height: parent.height
            }

            MEQ{
                width: parent.height * 5.1
                height: parent.height
            }

            Rectangle{
                width: 1
                height: parent.height
            }

            ParameterDial{
                id: vlControl
                property Volume module: UiCore.currentDevice.masterVolume

                width: height
                height: parent.height
                anchors.bottomMargin: parent.height/10
                anchors.topMargin: parent.height/10

                controlValue: UiCore.currentDevice.masterVolume.volume
             }

            ParameterDial{
                id: phonesControl
                property Volume module: UiCore.currentDevice.phonesVolume

                width: height
                height: parent.height

                controlValue: UiCore.currentDevice.phonesVolume.volume
            }

            Rectangle{
                width: 1
                height: parent.height
            }
        }
    }

    Rectangle{
        width: parent.width
        height: parent.height/2
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
        height: parent.height/4

        property var selectedModuleInstance

    }
}
