import QtQuick 2.15

import Elements 1.0
import CppObjects

Item
{
    id: main

    property string name: "VL"

    property bool on: true

    BaseModule{
        id: _baseModule

        moduleName: main.name

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

            Dial
            {
                width:  parent.width
                height: parent.height/1000*165

                enabled: main.on
                name: "VOLUME"
                paramType: DeviceParameter.MASTER_VOLUME
                checkable: false
            }

            Item
            {
                width:  parent.width
                height: parent.height/1000*25
            }
        }
    }
}
