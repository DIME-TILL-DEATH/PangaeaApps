import QtQuick 2.15
import QtQuick.Controls 2.12

import Elements 1.0
import StyleSettings 1.0

import CppObjects
import PangaeaBackend

Column
{
    id: _root

    property EqBand eqBand: eqModule.EqBands[0];
    property var shelfType
    property var enabledController: eqModule

    spacing: parent.height/12

    CustomLogSlider
    {
        ctrlValInstance: eqBand.Fc
        fontSize: 8 * Style.dip
        moduleOn: enabledController.moduleEnabled

        height: parent.height/5
    }

    CustomSlider
    {
        ctrlValInstance: eqBand.gain
        moduleOn: enabledController.moduleEnabled

        height: parent.height/5
    }

    CustomSlider
    {
        ctrlValInstance: eqBand.Q
        moduleOn: enabledController.moduleEnabled
        precision: 1

        height: parent.height/5
    }

    Item{
        width: parent.width
        height: parent.height/5

        MButton{
            textButton: "PEAK"
            textColor: enabledController.moduleEnabled ? Style.colorText : Style.currentTheme.colorTextDisabled

            width: parent.width/3
            height: parent.height

            anchors.left: parent.left

            highlighted: eqBand.filterType === FilterType.PEAKING
            onMbPressed: eqBand.filterType = FilterType.PEAKING
        }

        MButton{
            textButton: "SHELF"
            textColor: enabledController.moduleEnable ? Style.colorText : Style.currentTheme.colorTextDisabled

            width: parent.width/3
            height: parent.height

            anchors.right: parent.right

            highlighted: eqBand.filterType === shelfType
            onMbPressed: eqBand.filterType = shelfType
        }
    }
}
