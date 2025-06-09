import QtQuick

import Elements 1.0
import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: _baseModule

    property HiPassFilter module

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
            height: parent.height/1000*165*4
        }

        ParameterDial{
            controlValue: module.hpf
        }


        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }

    }
}
