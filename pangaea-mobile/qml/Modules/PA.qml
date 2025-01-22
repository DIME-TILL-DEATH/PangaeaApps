import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings 1.0
import Elements 1.0
import Tutorials

import CppObjects

BaseModule
{
    id: main

    property PowerAmp module

    property int ampType

    contentItem: Column{

        height: parent.height
        width: parent.width

        CustomSlider
        {
            height: parent.height/4
            width: parent.width
            ctrlValInstance: module.volume

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/4
            width: parent.width

            ctrlValInstance: module.presence

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/4
            width: parent.width

            ctrlValInstance: module.slave

            moduleOn: main.on
        }

        CustomComboBox{
            height: parent.height/4
            width: parent.width

            ctrlValInstance: module.ampType

            moduleOn: main.on

            model: ["Push-pull 6L6",
                    "Push-pull EL34",
                    "Single-ended 6L6",
                    "Single-ended EL34",
                    "AMT Tube cake",
                    "California",
                    "British M",
                    "British L",
                    "Flat",
                    "Califonia modern",
                    "California vintage",
                    "PVH Presence 01",
                    "PVH Presence 02",
                    "PVH Presence 03",
                    "PVH Presence 04"]
        }
    }

    tutorialItem: TutorialPA{}
}
