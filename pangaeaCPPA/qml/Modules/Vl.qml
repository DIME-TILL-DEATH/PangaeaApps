import QtQuick 2.15
import QtQuick.Controls 1.5

import Elements 1.0

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
                nameValue: "master_volume"
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
