import QtQuick 2.15

import Elements 1.0
import CppObjects

import PangaeaBackend

BaseModule{
    id: _baseModule

    property Volume module: UiCore.currentDevice.MV

    moduleName: "VL"

    on: true

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
            id: vlControl

            controlValue: UiCore.currentDevice.MV.volume
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }
    }

    MouseArea{
        anchors.fill: parent
    }
}
