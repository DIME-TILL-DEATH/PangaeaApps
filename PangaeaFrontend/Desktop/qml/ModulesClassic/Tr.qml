import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Tremolo module

    contentItem: Column
    {
        id: _column
        anchors.fill: parent
        Item
        {
            width:  parent.width
            height: parent.height/1000*150
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*165*3
        }


        ParameterDial{
            controlValue: module.rate
        }


        ParameterDial{
            controlValue: module.depth
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }
    }
}
