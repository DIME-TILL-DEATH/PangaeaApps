import QtQuick 2.15
import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property EarlyReflections module

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
            height: parent.height/1000*165*2
        }

        SwitchRoom
        {
            width:  parent.width
            height: parent.height/1000*165

            enabled: module.moduleEnabled

            controlValue: module.reflectionsType
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*165
        }

        ParameterDial{
            controlValue: module.reflectionsVolume
        }

        Item
        {
            width:  parent.width
            height: parent.height/1000*25
        }
    }


}
