import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings

import CppObjects

Item
{
    id: main

    property int curVal: UiCore.currentDevice.preset

    Column
    {
        anchors.fill: parent
        spacing: height/80
        Rectangle
        {
            width:  parent.width
            height: parent.height / 2

            color: Style.backgroundColor
            radius: parent.height
            MText
            {
                anchors.fill: parent
                text: "PRESET"
                color: Style.mainEnabledColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
                font.pixelSize: parent.height/1.5
            }
        }

        Row
        {
            width:  parent.width
            height: parent.height /2

            Repeater
            {
                anchors.fill: parent
                model: UiCore.currentDevice.maxPresetCount
                Item
                {
                    width:  parent.width/UiCore.currentDevice.maxPresetCount
                    height: parent.height
                    Rectangle
                    {
                        anchors.fill: parent
                        color: Style.mainEnabledColor
                    }
                    MText
                    {
                        anchors.fill: parent
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment:   Qt.AlignVCenter
                        text: modelData
                        color: index === curVal ? Style.highlightColor : Style.backgroundColor
                        font.pixelSize: parent.height/1.5
                    }
                }
            }
        }
    }
}
