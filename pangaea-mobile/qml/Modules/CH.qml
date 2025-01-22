import QtQuick 2.15
import QtQuick.Controls 2.12

import Elements 1.0
import StyleSettings 1.0

import CppObjects
import Tutorials

BaseModule
{
    id: main

    property Chorus module

    contentItem: Column
    {
        height: parent.height
        width: parent.width
        CustomSlider
        {
            height: parent.height/4
            width: parent.width

            ctrlValInstance: module.rate

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/4
            width: parent.width

            ctrlValInstance: module.width

            moduleOn: main.on
        }

        CustomSlider
        {
            fontSize: 8 * Style.dip

            height: parent.height/4
            width: parent.width

            ctrlValInstance: module.hpf

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.height/4
            width: parent.width
            bottomLineEnabled: false

            ctrlValInstance: module.mix

            moduleOn: main.on
        }
    }

    tutorialItem: TutorialCH{}
}
