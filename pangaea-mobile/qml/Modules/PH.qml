import QtQuick 2.15
import QtQuick.Controls 2.12

import Elements 1.0
import StyleSettings 1.0

import CppObjects
import Tutorials

import PangaeaBackend

BaseModule
{
    id: main

    property Phaser module

    contentItem: Column
    {
        height: parent.height
        width: parent.width
        CustomSlider
        {
            height: parent.height/6
            width: parent.width

            ctrlValInstance: module.rate

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/6
            width: parent.width

            ctrlValInstance: module.width

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/6
            width: parent.width

            ctrlValInstance: module.center

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/6
            width: parent.width

            ctrlValInstance: module.feedback

            moduleOn: main.on
        }

        CustomComboBox{
            width: parent.width
            height: parent.height/6

            ctrlValInstance: module.stages

            moduleOn: main.on

            model: ["Stages 3",
                    "Stages 5",
                    "Stages 7"]
        }

        CustomSlider
        {
            height: parent.height/6
            width: parent.width
            bottomLineEnabled: false

            ctrlValInstance: module.mix

            moduleOn: main.on
        }
    }

    tutorialItem: TutorialPH{}
}
