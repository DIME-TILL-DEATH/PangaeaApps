import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: _baseModule

    property Compressor module

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
            controlValue: module.sustain
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
