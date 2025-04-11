import QtQuick

import Elements 1.0
import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: _baseModule

    property LowPassFilter module

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
            height: parent.height/1000*165*4
        }

        ParameterDial{
            controlValue: module.lpf
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }
    }
}
