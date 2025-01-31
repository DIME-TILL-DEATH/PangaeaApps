import QtQuick

import StyleSettings
import CppObjects 1.0

Item
{
    id: main

    Rectangle
    {
        anchors.fill:  parent
        color: Style.mainEnabledColor
    }

    Column
    {
        anchors.fill:  parent

        Item
        {
            width: parent.width
            height: parent.height/100*5
        }

        Switch3
        {
            id:    mSwitch
            width: parent.width
            height: parent.height/100*40

            value: UiCore.currentDevice.outputMode

            onChValue: val => {
                UiCore.currentDevice.outputMode = val;
            }
        }

        Row
        {
            width: parent.width
            height: parent.height/100*20

            CheckText
            {
                width: parent.width/2
                height: parent.height
                text: "PH"
                check: mSwitch.value==0
                onClicked: UiCore.currentDevice.outputMode = 0
            }

            // CheckText - Просто текст. Ему Item нужен. Возможно надо внутри модуля добавить
            Item
            {
                width: parent.width/2
                height: parent.height
                CheckText
                {
                    anchors.fill: parent
                    text: "BAL"
                    check: mSwitch.value==2
                    onClicked: UiCore.currentDevice.outputMode = 2
                }
            }
        }

        Item
        {
            width: parent.width
            height: parent.height/100*20
            CheckText
            {
                anchors.fill: parent
                text: "LINE"
                check: mSwitch.value==1
                onClicked:  UiCore.currentDevice.outputMode = 1
            }
        }


        Item
        {
            width: parent.width
            height: parent.height/100*15
        }
    }
}
