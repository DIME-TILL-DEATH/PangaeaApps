import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

BaseModule{
    id: main

    property Compressor module

    moduleDescription: qsTr("Compressor")

    contentItem: Column
    {
        height: parent.height
        width: parent.width
        CustomSlider
        {
            height: parent.height/2
            width: parent.width

            ctrlValInstance: module.sustain

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/2
            width: parent.width

            ctrlValInstance: module.volume

            moduleOn: main.on
        }
    }
}
