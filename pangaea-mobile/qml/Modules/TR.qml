import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings 1.0
import Elements 1.0
import Tutorials

import CppObjects

import PangaeaBackend

BaseModule
{
    id: main

    property Tremolo module

    property int type

    contentItem: Column{

        height: parent.height
        width: parent.width

        CustomSlider
        {
            height: parent.height/2
            width: parent.width
            ctrlValInstance: module.rate

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/2
            width: parent.width

            ctrlValInstance: module.depth

            moduleOn: main.on
        }
    }

    tutorialItem: TutorialTR{}
}
