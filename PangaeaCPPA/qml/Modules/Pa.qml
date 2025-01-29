import QtQuick

import Elements 1.0
import CppObjects 1.0


BaseModule{
    id: main

    property PowerAmp module


    contentItem: Column
    {
        id: _column
        anchors.fill: parent
        Item
        {
            width:  parent.width
            height: parent.height/1000*70
        }
        Item
        {
            width:  parent.width
            height: parent.height/1000//*90
        }

        TypePA
        {
            width:  parent.width
            height: parent.height/1000*(265+90+55)
            // enabled: main.on

            controlValue: module.ampType
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000//*55
        }

        ParameterDial{
            controlValue: module.volume
        }

        ParameterDial{
            controlValue: module.presence
        }

        ParameterDial{
            controlValue: module.slave
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }
    }
}

