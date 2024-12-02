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

CustomMessageDialog {
    id: _root

    property int topHeaderSize: 0

    closeOnDisconnect: true

    width: parent.width*0.9
    height: parent.height-topHeaderSize

    anchors.centerIn: null

    x: parent.width/2-width/2
    y: parent.height-height

    BusyScreen{
        width: Screen.width
        height: Screen.height

        z: parent.z+5
    }

    background: Rectangle {
       id: _backRect

        width: _root.width
        height: _root.height

        gradient: Gradient{
            GradientStop{position: 0.0; color: Style.colorModul}
            GradientStop{position: 1.0; color: "black"}
        }

        radius: Style.baseRadius
        border.color: Style.currentTheme.colorBorderOn
        border.width: 1
    }

    headerText: qsTr("Presets list")

    footer: Rectangle {
        id: _dialogButtonBox

        width: _root.width
        height: parentHeight*0.3
        color: "transparent"
        radius: Style.baseRadius
        border.color: Style.currentTheme.colorBorderOn
        border.width: 1

        Button{ //MButton
            text: qsTr("HIDE")

            width: parent.width*0.8
            height: parent.height*0.6

            anchors.centerIn: parent

            onClicked: {
                _root.close()
            }
        }
    }

    ListView{
        id: _presetListView

        width: _root.width*0.95
        height: _root.height-headerHeight-footerHeight

        currentIndex: UiCore.currentDevice.bank * UiCore.currentDevice.maxPresetCount + UiCore.currentDevice.preset
            /*DeviceProperties.isLa3Mode? DeviceProperties.bank * 4 + DeviceProperties.preset
                                                : DeviceProperties.bank * DeviceProperties.banksList.length + DeviceProperties.preset*/

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        model: UiCore.currentDevice.presetListModel

        boundsBehavior: Flickable.StopAtBounds
        clip: true

        ScrollBar.vertical: ScrollBar{
            active: true
            visible: true
        }

        spacing: 4

        snapMode: ListView.SnapToItem

        delegate: PresetsListDelegate{
            width: _presetListView.width*0.95
        }


        section.property: "bankNumber"//DeviceProperties.isLa3Mode? "" : "bankNumber"
        section.criteria: ViewSection.FullString
        section.delegate: Item{
            id: _sectionHeaderContent

            width: childrenRect.width
            height: childrenRect.height*1.2
            required property string section

            Rectangle {
                width: _presetListView.width*0.95
                height: childrenRect.height
                color: "lightsteelblue"

                MText {
                    text: "Bank " + _sectionHeaderContent.section//DeviceProperties.banksList[_sectionHeaderContent.section]
                }
            }
        }
    }

    onOpened: {
        _presetListView.positionViewAtIndex(_presetListView.currentIndex, ListView.Center)
    }

    onAccepted: {
        _root.close()
    }
}
