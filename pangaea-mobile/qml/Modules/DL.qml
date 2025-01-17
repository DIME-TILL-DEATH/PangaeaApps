import QtQuick

import StyleSettings
import Elements
import Tutorials

import CppObjects

BaseModule
{
    id: main

    property Delay module

    moduleDescription: qsTr("Delay")

    width: parent.width
    height: parent.height


    contentItem: Column
    {
        height: parent.height
        width: parent.width

        property int sliderWidth: width
        property int sliderHeight: height/5

        CustomSlider
        {
            id: _slider

            width: parent.sliderWidth
            height: parent.sliderHeight

            ctrlValInstance: main.module.mix
            moduleOn: main.on
        }

        CustomSlider
        {
            width: parent.sliderWidth
            height: parent.sliderHeight

            ctrlValInstance: module.time

            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.sliderHeight
            width: parent.sliderWidth

            ctrlValInstance: module.feedback
            moduleOn: main.on
        }

        CustomSlider
        {
            height: parent.sliderHeight
            width: parent.sliderWidth

            ctrlValInstance: module.hpf

            moduleOn: main.on

        }

        CustomSlider
        {
            fontSize: 8 * Style.dip

            height: parent.sliderHeight
            width: parent.sliderWidth

            ctrlValInstance: module.lpf

            inverse: true

            moduleOn: main.on
        }
    }

    tutorialItem: TutorialDL{}
}
