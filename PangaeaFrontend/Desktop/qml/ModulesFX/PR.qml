import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Preamp module

    contentItem: Row
    {
        id: _column
        anchors.fill: parent


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.high
        }


        ParameterDial{
            width: parent.width/10
            height: parent.height/3

            controlValue: module.mid
        }

        ParameterDial{
            width: parent.width/10
            height: parent.height/3

            controlValue: module.low
        }

        ParameterDial{
            width: parent.width/10
            height: parent.height/3

            controlValue: module.volume
        }
    }
}


