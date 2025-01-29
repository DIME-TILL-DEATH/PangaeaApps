import QtQuick 2.15

import Elements 1.0
import CppObjects


BaseModule{
    id: _baseModule

    property PresetVolume module: UiCore.currentDevice.MV

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

            controlValue: UiCore.currentDevice.MV.presetVolume
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }
    }
}
