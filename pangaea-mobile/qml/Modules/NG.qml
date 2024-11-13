import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0
import Tutorials

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

    tutorialItem: SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Noise gate")

        text: qsTr("Noise Gate is used to attenuate the signal in pauses when you don’t play guitar, helping you to hide some hiss, hum and other noise present in any guitar setup.

THRESHOLD: The Noise Gate allows the signal to pass through only when it is above the threshold.
When the input signal is below the threshold, the gate is ‘closed’ and no signal is allowed to pass.
Set it a little higher than the noise level in your system to achieve noise dampening. If the threshold is set too high, it may cut out yoursoft notes.

DECAY: Determines how fast the gate closes. Low values won’t let any noise to pass but may cut your notes short.");
    }
}
