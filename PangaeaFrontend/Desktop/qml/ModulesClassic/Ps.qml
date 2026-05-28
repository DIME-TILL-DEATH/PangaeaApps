import QtQuick

import Elements 1.0
import PangaeaFrontend 1.0
import PangaeaBackend


BaseModule{
    id: _baseModule

    property Presence module

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
            controlValue: module.presenceVolume
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }
    }
}

