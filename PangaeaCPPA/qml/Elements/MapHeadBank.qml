import QtQuick
import QtQuick.Controls

import StyleSettings

import CppObjects
import CppEnums

Item
{
    id: main
    property string fonColor: "#EBECEC"
    property string devColor: "#5E5971"

    property int curVal: DeviceProperties.bank

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
                model: DeviceProperties.banksList
                Item
                {
                    width:  parent.width
                    height: parent.height/DeviceProperties.banksList.length
                    Rectangle
                    {
                        anchors.fill: parent
                        color: Style.mainEnabledColor
                    }

                    property bool isLA3: (DeviceProperties.deviceType === DeviceType.LA3PA) | (DeviceProperties.deviceType === DeviceType.LA3RV)
                    MText
                    {
                        anchors.fill: parent
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment:   Qt.AlignVCenter
                        text: modelData
                        color: index===curVal ? (isLA3 ? ((index<2) ? "lightgreen" : Style.highlightColor ) : Style.highlightColor)
                                                : Style.backgroundColor
                        font.pixelSize: isLA3 ? parent.width/2.2 : parent.width/1.5
                    }
                }
            }
        }
    }
}
