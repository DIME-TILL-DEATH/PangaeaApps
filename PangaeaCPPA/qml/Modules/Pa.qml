import QtQuick

import Elements 1.0
import CppObjects 1.0

Item
{
    id: main


    BaseModule{
        id: _baseModule

        // moduleName: main.name


        // on: main.on

        module: PowerAmp

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
            }

            Item
            {
                width:  parent.width
                height: parent.height/1000//*55
            }

            ParameterDial{
                controlValue: PowerAmp.volume
            }

            ParameterDial{
                controlValue: PowerAmp.presence
            }

            ParameterDial{
                controlValue: PowerAmp.slave
            }

            Item
            {
                width:  parent.width
                height: parent.height/1000*25
            }
        }
    }
}
