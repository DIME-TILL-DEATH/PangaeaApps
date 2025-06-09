import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Preamp module

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
            height: parent.height/1000*165
        }


        ParameterDial{
            controlValue: module.high
        }


        ParameterDial{
            controlValue: module.mid
        }

        ParameterDial{
            controlValue: module.low
        }

        ParameterDial{
            controlValue: module.volume
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }
    }
}


