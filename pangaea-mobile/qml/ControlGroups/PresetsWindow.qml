import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

import StyleSettings 1.0

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
        height: parentHeight*0.2
        color: "transparent"
        radius: Style.baseRadius
        border.color: Style.currentTheme.colorBorderOn
        border.width: 1

        MButton{
            textButton: qsTr("BACK")

            width: parent.width*0.3
            height: parent.height*0.7

            anchors.centerIn: parent

            onMbPressed: {
                _root.close()
            }
        }
    }

    ListView{
        id: _presetListView

        width: _root.width*0.95
        height: _root.height-headerHeight-footerHeight

        currentIndex: DeviceProperties.bank * DeviceProperties.banksList.length + DeviceProperties.preset

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        model: PresetListModel

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


        section.property: DeviceProperties.isLa3Mode? "" : "bankNumber"
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
                    text: "Bank " + DeviceProperties.banksList[_sectionHeaderContent.section]
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
