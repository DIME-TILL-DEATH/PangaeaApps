import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0
import Tutorials

import CppObjects

import PangaeaBackend

BaseModule
{
    id: main

    property HiPassFilter module

    showDescription: false

    contentItem: Column{
        height: parent.height
        width: parent.width
        CustomSlider
        {
            fontSize: 8 * Style.dip

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
