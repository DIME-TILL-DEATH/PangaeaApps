import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects
import PangaeaBackend

Column
{
    id: _root
    property EqBand eqBand: eqModule.EqBands[0];

    spacing: parent.height/3

    CustomLogSlider
    {
        ctrlValInstance: eqBand.Fc
        moduleOn: eqBand.enabled
        fontSize: 8 * Style.dip

        height: parent.height/4
    }

    MButton{
        width: parent.width/3
        height: parent.height/4

        anchors.horizontalCenter: parent.horizontalCenter

        textButton: qsTr("CUT ON")
        textColor: eqBand.enabled ? Style.colorText : Style.currentTheme.colorTextDisabled

        highlighted: eqBand.enabled

        onMbPressed:
        {
            console.log("band num:", eqBand, eqBand.filterType, eqBand.enabled)
            eqBand.enabled = !eqBand.enabled
        }
    }
}
