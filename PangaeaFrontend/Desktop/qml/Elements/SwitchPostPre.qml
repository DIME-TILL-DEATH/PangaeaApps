import QtQuick

import StyleSettings
import CppObjects 1.0

Item
{
    id: main

    property bool isAvaliable

    Rectangle
    {
        anchors.fill:  parent
        color: main.enabled  ? Style.currentTheme.mainEnabledColor : Style.currentTheme.mainDisabledColor
    }

    Column
    {
        anchors.fill:  parent

        Item
        {
            width: parent.width
            height: parent.height/100*5
        }

        Switch2
        {
            id:    mSwitch

            width: parent.width
            height: parent.height/100*40

            value: !UiCore.currentDevice.isPreEq

            onChValue: val => {
                           UiCore.currentDevice.isPreEq = !val;
                       }

            visible: isAvaliable
        }

        Row
        {
            width: parent.width
            height: parent.height/100*20
            CheckText
            {
                width: parent.width/2
                height: parent.height
                text: "PRE"
                check: mSwitch.value==0

                visible: isAvaliable
                onClicked: UiCore.currentDevice.isPreEq = 1
            }

            CheckText
            {
                width: parent.width/2
                height: parent.height
                text: "POST"
                check: mSwitch.value==1

                visible: isAvaliable
                onClicked: UiCore.currentDevice.isPreEq = 0

            }

        }

        Item
        {
            width: parent.width
            height: parent.height/100*20
            MText
            {
                anchors.fill: parent
                text: "EQ"

                font.pixelSize: parent.height/1.5
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop

                visible: isAvaliable
            }
        }

        Rectangle
        {
            width: parent.width
            height: parent.height/100*15
            color: main.enabled ? Style.currentTheme.mainEnabledColor : Style.currentTheme.mainDisabledColor
        }
    }
}
