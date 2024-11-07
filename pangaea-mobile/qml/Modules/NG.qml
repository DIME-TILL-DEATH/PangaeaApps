import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

BaseModule
{
    id: main

    property NoiseGate module


    moduleDescription: qsTr("Noise gate")

    contentItem: Column
    {
        height: parent.height
        width: parent.width
        CustomSlider
        {
            height: parent.height/2
            width: parent.width

            ctrlValInstance: module.threshold

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/2
            width: parent.width
            bottomLineEnabled: false

            ctrlValInstance: module.decay

            moduleOn: main.on
        }
    }
}
