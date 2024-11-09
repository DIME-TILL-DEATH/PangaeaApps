import QtQuick 2.15
import QtQuick.Controls 2.12

import Elements 1.0
import StyleSettings 1.0

import CppObjects


BaseModule
{
    id: main

    property Preamp module

    moduleDescription: qsTr("Preamp")

    contentItem: Column
    {
        height: parent.height
        width: parent.width
        CustomSlider
        {
            // valueMin: (-64)
            // valueMax: (63)
            // dispMin:  (0)
            // dispMax:  (127)
            height: parent.height/4
            width: parent.width

            ctrlValInstance: module.low

            moduleOn: main.on
        }

        CustomSlider
        {
            // valueMin: (-64)
            // valueMax: (63)
            // dispMin:  (0)
            // dispMax:  (127)
            height: parent.height/4
            width: parent.width

            ctrlValInstance: module.mid

            moduleOn: main.on
        }
        CustomSlider
        {
            // valueMin: (-64)
            // valueMax: (63)
            // dispMin:  (0)
            // dispMax:  (127)
            height: parent.height/4
            width: parent.width

            ctrlValInstance: module.high

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/4
            width: parent.width
            bottomLineEnabled: false

            ctrlValInstance: module.volume

            moduleOn: main.on
        }
    }
}
