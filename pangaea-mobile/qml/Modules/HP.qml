import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0
import Tutorials

import CppObjects

BaseModule
{
    id: main

    property HiPassFilter module

    moduleDescription: qsTr("High-pass filter")

    showDescription: false

    contentItem: Column{
        height: parent.height
        width: parent.width
        CustomSlider
        {
            // dispMax: 1000
            // dispMin: 20
            // valueMax: 255
            // valueMin: 0

            height: parent.height
            width: parent.width

            ctrlValInstance: module.hpf

            inverse: true
            bottomLineEnabled: false

            moduleOn: main.on
        }
    }

    tutorialItem: SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("High pass filter")

        text: qsTr("High-pass filter passes through frequencies above the cutoff frequency and attenuates everything below that point.");
    }
}
