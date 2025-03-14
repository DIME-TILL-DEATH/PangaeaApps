import QtQuick
import QtQuick.Controls

import StyleSettings

import CppObjects
import PangaeaBackend

Item
{
    id: main
    property string fonColor: "#EBECEC"
    property string devColor: "#5E5971"

    property int curVal: UiCore.currentDevice.bank

    Row
    {
        anchors.fill: parent

        spacing: width/60
        Rectangle
        {
            width:  parent.width/2
            height: parent.height

            color:  Style.backgroundColor
            radius: parent.height
            MText
            {
                anchors.fill: parent
                text: "BANK"
                color: Style.mainEnabledColor
                rotation: 270
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
                font.pixelSize: parent.width/1.5
            }
        }

        Column
        {
            width:  parent.width /2
            height: parent.height

            Repeater
            {
                anchors.fill: parent
                model: UiCore.currentDevice.maxBankCount
                Item
                {
                    width:  parent.width
                    height: parent.height/UiCore.currentDevice.maxBankCount
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
                        color: index===curVal ?  Style.highlightColor
                                                : Style.backgroundColor
                        font.pixelSize: parent.width/1.5
                    }
                }
            }
        }
    }
}
