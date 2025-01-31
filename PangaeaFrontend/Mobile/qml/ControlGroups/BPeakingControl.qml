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
    property bool bandOn: true
    property var enabledController: eqModule

    spacing: parent.height/12

    CustomLogSlider
    {
        ctrlValInstance: eqBand.Fc
        moduleOn: enabledController.moduleEnabled
        fontSize: 8 * Style.dip

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
}
