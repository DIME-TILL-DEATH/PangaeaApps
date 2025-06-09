import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Chorus module

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
            controlValue: module.rate
        }


        ParameterDial{
            controlValue: module.width
        }

        ParameterDial{
            controlValue: module.hpf
        }

        ParameterDial{
            controlValue: module.mix
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }
    }
}
