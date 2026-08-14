import QtQuick 2.15

import StyleSettings 1.0
import Elements 1.0

import PangaeaBackend

Column
{
    id: _root
    property EqBand eqBand: eqModule.EqBands[0];

    spacing: parent.height/3

    CustomLogSlider
    {
        ctrlValInstance: _root.eqBand.Fc
        moduleOn: _root.eqBand.enabled
        fontSize: 8 * Style.dip

        height: parent.height/4
    }

    MButton{
        width: parent.width/3
        height: parent.height/4

        anchors.horizontalCenter: parent.horizontalCenter

        textButton: qsTr("CUT ON")
        textColor: _root.eqBand.enabled ? Style.colorText : Style.currentTheme.colorTextDisabled

        highlighted: _root.eqBand.enabled

        onMbPressed:
        {
            _root.eqBand.enabled = !_root.eqBand.enabled
        }
    }
}
