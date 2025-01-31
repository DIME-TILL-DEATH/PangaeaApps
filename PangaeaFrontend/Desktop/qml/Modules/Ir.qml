import QtQuick

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property CabSim module

    contentItem: Column
    {
        anchors.fill: parent
        Item
        {
            width:  parent.width
            height: parent.height/1000*150
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*160*4
        }

        SwitchIr
        {
            id: switchIr

            width:  parent.width
            height: parent.height/1000*165
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }
    }
}
