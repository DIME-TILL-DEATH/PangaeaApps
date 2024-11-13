import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements
import Tutorials

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

    tutorialItem: SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Compressor")

        text: qsTr("Compressor reduces the volume of loud sounds and amplifies quiet sounds smoothening the dynamics. At the same time, it can be used as a sustainer making your single notes sound longer.
It’s a great tool to letting some parts stand out. It is as easy to find settings that work against you. Too much compression may bring up some hum and noise when used together with overdrive or distortion.

SUSTAIN: Combination of threshold and release parameters. the higher the value, the longer the notes will decay.

VOLUME: Sets the output volume. It can be used to compensate for signal attenuation caused by compression.");
    }
}
