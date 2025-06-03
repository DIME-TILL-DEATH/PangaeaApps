import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property ResonanceFilter module: _moduleLoader.selectedModuleInstance

    contentItem: Row
    {
        anchors.fill: parent


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.mix
        }

        Rectangle{
            width: 1
            height: parent.height
        }

        ParameterComboBox{
            width: main.dialWidth //* 2
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.fType

            moduleOn: module.moduleEnabled

            model: ["LPF", "HPF", "BPF"]
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.loFreq
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.hiFreq
        }

        Rectangle{
            width: 1
            height: parent.height
        }

        ParameterComboBox{
            id: _modeCombo

            width: main.dialWidth * 2
            height: main.dialHeight

            property bool isLfoMode: currentIndex === 0
            property bool isDynMode: currentIndex === 1

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.fMod

            moduleOn: module.moduleEnabled

            model: ["LFO", "DYNAMIC", "EXTERNAL"]
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.lfo

            visible: _modeCombo.isLfoMode
        }

        ParameterComboBox{
            width: main.dialWidth * 2
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.lfoType

            visible: _modeCombo.isLfoMode

            moduleOn: module.moduleEnabled

            model: ["Tri", "Rand", "Rand/2", "Rand/3", "Rand/4", "Rand/6", "Rand/8"]
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.dynTh

            visible: _modeCombo.isDynMode
        }



        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.dynAtt

            visible: _modeCombo.isDynMode
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.dynRel

            visible: _modeCombo.isDynMode
        }

        Rectangle{
            width: 1
            height: parent.height
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.res
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.volume
        }
    }
}
