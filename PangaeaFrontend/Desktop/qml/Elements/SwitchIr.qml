import QtQuick

import CppObjects 1.0

Item
{
    id: main

    Column
    {
        anchors.fill:  parent

        Switch2
        {
            id: mSwitch

            width: parent.width
            height: parent.height/100*80

            value: module.moduleEnabled

            opacity: module.moduleEnabled ? 1 : 0.5

            onChValue: val => {
                           module.moduleEnabled = val
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
                text: "Off"
                check: mSwitch.value==0
                onClicked: module.moduleEnabled = 0
            }

            CheckText
            {
                width: parent.width/2
                height: parent.height
                text: "On"
                check: mSwitch.value==1
                onClicked: module.moduleEnabled = 1
            }

        }

    }
}
