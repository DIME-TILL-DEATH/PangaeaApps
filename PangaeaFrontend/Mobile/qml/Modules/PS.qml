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

    property Presence module

    moduleDescription: qsTr("Presence")

    showDescription: false

    contentItem: Row
    {
        anchors.fill: parent
        spacing: 0

        Column
        {
            height: parent.height
            width: parent.width
            CustomSlider
            {
                fontSize: 8 * Style.dip

                height: parent.height
                width: parent.width

                ctrlValInstance: module.presenceVolume

                bottomLineEnabled: false
                moduleOn: on
            }
        }
    }

    tutorialItem: SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Presence")

        text: qsTr("Just like a real power amp this effect block has a Presence control. Use it to add some extra brightness to the tone.");
    }
}
