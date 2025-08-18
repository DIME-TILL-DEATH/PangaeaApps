import QtQuick

import StyleSettings

Item
{
    id: main

    property int value: 0
    signal chValue(int value)

    property int invertedValue: 0

    Rectangle
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter:  parent.verticalCenter
        width: parent.width/1.5
        height: width/2
        radius: height/2
        color: Style.currentTheme.borderOff
        border.color: Style.currentTheme.borderOn
        Rectangle
        {
            color: Style.currentTheme.borderOn
            border.color: Style.currentTheme.borderOff

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            width:  parent.height*0.8
            height: parent.height*0.8
            radius: height
            anchors.horizontalCenterOffset: (value===invertedValue) ? (parent.width/-4) : (parent.width/4)
            Behavior on anchors.horizontalCenterOffset  {NumberAnimation { duration: 200 }}
        }

        Row
        {
            anchors.fill:  parent
            MouseArea
            {
                height: parent.height
                width: parent.width/2
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    invertedValue ? main.chValue(1) : main.chValue(0);
                }
                // onWheel:   wheelChange(wheel.angleDelta.y);
            }
            MouseArea
            {
                height: parent.height
                width: parent.width/2
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    invertedValue ? main.chValue(0) : main.chValue(1);
                }
                // onWheel:   wheelChange(wheel.angleDelta.y);
            }
        }
    }
}
