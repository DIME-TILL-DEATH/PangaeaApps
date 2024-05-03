import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0
import StyleSettings 1.0

import CppObjects

Item
{
    id: main

    property string name: qsTr("VL")
    property bool on: true

    property string nameValue: qsTr("Preset volume")

    property int valueMin:  0
    property int valueMax:  31


    property bool softUpdate: false
    property bool editabled: false

    Rectangle {
        id: _contentRect

        width: parent.width
        height: parent.height
        color: Style.colorModul

        radius: Style.baseRadius
        border.width: 1
        border.color: Style.currentTheme.colorBorderOn

        Item{
            id: _contentItem

            anchors.fill: parent
            anchors.rightMargin: parent.width/50
            anchors.leftMargin: parent.width/50
            anchors.bottomMargin: 3
            anchors.topMargin: 3
            CustomizerSlider
            {
                width: _contentItem.width
                height: _contentItem.height

                bottomLineEnabled: false

                name: main.nameValue
                paramType: DeviceParameter.MASTER_VOLUME
            }
        }
    }

}
