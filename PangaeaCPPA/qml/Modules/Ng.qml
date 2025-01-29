import QtQuick

import Elements 1.0
import CppObjects 1.0


BaseModule{
    id: _baseModule

    property NoiseGate module

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
            controlValue: module.threshold
        }


        ParameterDial{
            controlValue: module.decay
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }
    }
}
