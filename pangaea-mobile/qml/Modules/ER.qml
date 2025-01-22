import QtQuick
import QtQuick.Controls

import StyleSettings 1.0
import Elements 1.0
import Tutorials

import CppObjects

BaseModule
{
    id: main

    property EarlyReflections module

    property int currentType

    width: (parent.width + Style.mainSpacing) * 4/5
    height: parent.height

    contentItem: Column
    {
        height: parent.height
        width: parent.width
        CustomSlider
        {
            id: _slider

            height: parent.height/2
            width: parent.width

            ctrlValInstance: module.reflectionsVolume

            bottomLineEnabled: false
            moduleOn: main.on

        }

        CustomComboBox{
            width: parent.width
            height: parent.height/2

            ctrlValInstance: module.reflectionsType

            moduleOn: main.on

            model: ["SHORT","MEDIUM","LONG"]
        }
    }

    tutorialItem: TutorialER{}
}
