import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import Elements 1.0
import ModulesFX 1.0
import StyleSettings 1.0
import Layouts 1.0

import CP100FX 1.0

import CppObjects
import PangaeaBackend

RowLayout{
    id: _row1

    Rectangle{
        Layout.preferredWidth: 2
        Layout.fillHeight: true
    }

    ParameterComboBox{
        id: _attenuator

        Layout.preferredWidth: height * 1.5
        Layout.fillHeight: true

        // anchors.verticalCenter: parent.verticalCenter

        ctrlValInstance: UiCore.currentDevice.attenuatorVolume.volume

        moduleOn: true

        model: [" +4 dB", " +3 dB", " +2 dB", " +1 dB", "  0 dB", " -1 dB", " -2 dB", " -3 dB",
            " -4 dB", " -5 dB", " -6 dB", " -7 dB", " -8 dB", " -9 dB", "-10 dB", "-11 dB", "-12 dB",
            "-13 dB", "-14 dB", "-15 dB", "-16 dB", "-17 dB", "-18 dB", "-19 dB", "-20 dB", "-21 dB",
            "-23 dB", "-24 dB", "-27 dB", "-30 dB", "-34 dB", "-35 dB", "-36 dB", "-37 dB", "-38 dB",
            "-40 dB", "-41 dB", "-42 dB", "-43 dB", "-44 dB", "-45 dB", "-46 dB", "-47 dB", "-48 dB",
            "-49 dB", "-50 dB", "-51 dB", "-52 dB", "-53 dB", "-54 dB", "-55 dB", "-56 dB", "-57 dB",
            "-58 dB", "-59 dB", "-60 dB"]
    }

    MEQ{
        Layout.preferredWidth: parent.height * 5
        Layout.fillHeight: true
    }

    Rectangle{
        Layout.preferredWidth: 2
        Layout.fillHeight: true
    }

    Row{
        Layout.preferredWidth: height*2
        Layout.fillHeight: true

        ParameterDial{
            id: vlControl
            property Volume module: UiCore.currentDevice.masterVolume

            y: parent.height/10

            enabled: true

            width: height
            height: parent.height

            controlValue: UiCore.currentDevice.masterVolume.volume
         }


        ParameterDial{
            id: phonesControl
            property Volume module: UiCore.currentDevice.phonesVolume

            y: parent.height/10

            enabled: true

            width: height
            height: parent.height

            controlValue: UiCore.currentDevice.phonesVolume.volume
        }
    }

    Rectangle{
        Layout.preferredWidth: 2
        Layout.fillHeight: true
    }
}
