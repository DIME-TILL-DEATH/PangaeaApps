import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

import StyleSettings 1.0

import CustomOverlays 1.0
import ControlGroups 1.0
import Modules 1.0
import Elements 1.0
import Tutorials 1.0

import CppObjects
import CppEnums

Item{
    id: _root

    width: parent.width
    height: parent.height

    z: parent.z+10

    MouseArea{
        z: _moduleListView.z-5
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
        height: parent.height * 0.95

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
                    text: qsTr("Add/Remove module")
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

            ListView{
                id: _moduleListView

                width: parent.width * 0.9
                height: parent.height * 0.8

                anchors.horizontalCenter: parent.horizontalCenter

              //  model: UiCore.currentDevice.avaliableModulesList


                boundsBehavior: Flickable.StopAtBounds
                clip: true

                ScrollBar.vertical: ScrollBar{
                    active: true
                    visible: true
                }

                spacing: 3

                snapMode: ListView.SnapToItem

                delegate: Rectangle{
                    id: _rectDelegate

                    width: _moduleListView.width
                    height: _moduleListView.height/11

                    gradient: Gradient{
                        GradientStop{
                            position: 0.0; color: Style.colorModul
                        }
                        GradientStop{position: 1.0; color: "black"}

                    }

                    radius: Style.baseRadius
                    border.color: Style.currentTheme.colorBorderOn
                    border.width: Drag.active? 5 : 1

                    // property Item tutorialItem

                    Row{
                        id: _delegateRow

                        width: parent.width * 0.9
                        height: parent.height * 0.8

                        spacing: parent.width / 32

                        anchors.centerIn: parent

                        property bool isResourcesEnough: (UiCore.currentDevice.processingUsed + modelData.processingTime < UiCore.currentDevice.processingBudget)

                        Item{
                            width: parent.width * 1/4
                            height: parent.height
                            MText{
                                text: modelData.moduleName


                                color:  modelData.used ? Style.colorBtnDisabled : Style.colorText

                                anchors.fill: parent

                                elide: Text.ElideMiddle

                                horizontalAlignment: Qt.AlignHCenter
                                verticalAlignment: Qt.AlignVCenter
                            }
                        }

                        Item{
                            width: parent.width * 1/4
                            height: parent.height
                            MText{
                                text: qsTr("cost: ") + modelData.processingTime

                                anchors.centerIn: parent
                                color: modelData.used ? (Style.colorBtnDisabled) :
                                                        (_delegateRow.isResourcesEnough ? Style.colorText : "red")


                                elide: Text.ElideMiddle

                                horizontalAlignment: Qt.AlignHCenter
                                verticalAlignment: Qt.AlignVCenter
                            }
                        }

                        MButton{
                            textButton: "?"

                            width: parent.width * 3/16
                            height: parent.height

                            onMbPressed:{
                                onClicked: _tutorialItem.item.open()
                            }
                        }

                        MButton{
                            // textButton: modelData.used ? qsTr("Used") : "+"
                            textButton: modelData.used ? "-" : "+"

                            width: parent.width * 3/16
                            height: parent.height

                            enabled: modelData.used ? "true" : _delegateRow.isResourcesEnough

                            onMbPressed:{
                                if(!modelData.used )
                                    UiCore.currentDevice.modulesListModel.appendModule(modelData)
                                else
                                    UiCore.currentDevice.modulesListModel.removeModuleByType(modelData.moduleType)
                            }
                        }
                    }

                    Loader{
                        id: _tutorialItem

                        width: parent.width * 1.2

                        // source: "/Tutorials/qml/Tutorials/TutorialNG.qml"
                    }

                    Component.onCompleted: function(){
                        switch(modelData.moduleType)
                        {
                        case ModuleType.NG: _tutorialItem.source = "/Tutorials/qml/Tutorials/TutorialNG.qml"; break;
                        case ModuleType.CM: _tutorialItem.source = "/Tutorials/qml/Tutorials/TutorialCM.qml"; break;
                        case ModuleType.PR: _tutorialItem.source = "/Tutorials/qml/Tutorials/TutorialPR.qml"; break;
                        case ModuleType.PA: _tutorialItem.source = "/Tutorials/qml/Tutorials/TutorialPA.qml"; break;
                        case ModuleType.IR: _tutorialItem.source = "/Tutorials/qml/Tutorials/TutorialIR.qml"; break;
                        case ModuleType.TR: _tutorialItem.source = "/Tutorials/qml/Tutorials/TutorialTR.qml"; break;
                        case ModuleType.CH: _tutorialItem.source = "/Tutorials/qml/Tutorials/TutorialCH.qml"; break;
                        case ModuleType.PH: _tutorialItem.source = "/Tutorials/qml/Tutorials/TutorialPH.qml"; break;
                        case ModuleType.EQ1:
                        case ModuleType.EQ2: _tutorialItem.source = "/Tutorials/qml/Tutorials/TutorialEQ.qml"; break;
                        }
                    }
                }
            }


            Button{
                text: qsTr("HIDE")

                width: parent.width * 0.9
                height: parent.height * 0.1

                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    _root.visible = false
                }
            }
        }
    }


    Component.onCompleted:{
       _moduleListView.model = UiCore.currentDevice.avaliableModulesList
    }
}
