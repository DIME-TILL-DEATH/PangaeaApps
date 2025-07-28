import QtQuick 2.15
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtCore

import QtQuick.Window 2.15

import Elements 1.0
import ModulesFX 1.0
import StyleSettings 1.0
import Layouts 1.0

import CP100FX 1.0

import QtQuick.Layouts

import CppObjects
import PangaeaBackend

Rectangle{
    id: _main

    color: Style.mainEnabledColor

    property int stringHeight: height/16

    RowLayout{
        anchors.fill: parent


        Rectangle{
            id: _sysMenu1

            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width/4

            color: "transparent"
            border.width: 1
            border.color: Style.borderOn

            Column{
                width: parent.width * 0.9
                height: parent.height * 0.9
                anchors.centerIn: parent

                spacing: _main.stringHeight

                MComboHorizontal{
                    width: parent.width
                    height: _main.stringHeight

                    text: "Mode: "

                    currentIndex: UiCore.currentDevice.systemSettings.mode
                    model: ["CabSim On", "CabSim Off"]

                    onActivated: (index) => {

                        UiCore.currentDevice.systemSettings.mode = currentIndex;
                    }
                }

                MComboHorizontal{
                    width: parent.width
                    height: _main.stringHeight

                    text: "Cab. num: "

                    currentIndex: UiCore.currentDevice.systemSettings.cabNumber
                    model: ["1 L+R", "1R AP", "2 L+R", "1R A", "1R P", "1 R"]

                    onActivated: (index) => {

                        UiCore.currentDevice.systemSettings.cabNumber = currentIndex;
                    }
                }

                MComboHorizontal{
                    width: parent.width
                    height: _main.stringHeight

                    text: "MIDI channel: "

                    currentIndex: UiCore.currentDevice.systemSettings.midiChannel
                    model: _midiChannelModel

                    onActivated: (index) => {

                        UiCore.currentDevice.systemSettings.midiChannel = currentIndex;
                    }

                    ListModel{
                        id: _midiChannelModel

                        Component.onCompleted: {
                            for(let i =1; i<16; i++){
                                append({value: i})
                            }
                        }
                    }
                }

                MComboHorizontal{
                    width: parent.width
                    height: _main.stringHeight

                    text: "S/PDIF: "

                    currentIndex: UiCore.currentDevice.systemSettings.spdif
                    model: ["Main Output", "Dry Input"]

                    onActivated: (index) => {

                        UiCore.currentDevice.systemSettings.spdif = currentIndex;
                    }
                }

                MComboHorizontal{
                    width: parent.width
                    height: _main.stringHeight

                    text: "Tempo: "

                    currentIndex: UiCore.currentDevice.systemSettings.tempo
                    model: ["Preset", "Global", "Glob.+MIDI"]

                    onActivated: (index) => {

                        UiCore.currentDevice.systemSettings.tempo = currentIndex;
                    }
                }

                MComboHorizontal{
                    width: parent.width
                    height: _main.stringHeight

                    text: "Time format: "

                    currentIndex: UiCore.currentDevice.systemSettings.timeFormat
                    model: ["Sec", "BPM"]

                    onActivated: (index) => {

                        UiCore.currentDevice.systemSettings.timeFormat = currentIndex;
                    }
                }

                MComboHorizontal{
                    width: parent.width
                    height: _main.stringHeight

                    text: "Swap FSW: "

                    currentIndex: UiCore.currentDevice.systemSettings.swapConf
                    model: ["Off", "On"]

                    onActivated: (index) => {
                        UiCore.currentDevice.systemSettings.swapConf = currentIndex;
                    }
                }


                MBar{
                    width: parent.width
                    height: _main.stringHeight

                    text: "Tuner speed: "

                    value: UiCore.currentDevice.systemSettings.tunerSpeed

                    onUserChangedValue: {
                        UiCore.currentDevice.systemSettings.tunerSpeed = value
                    }
                }
            }
        }

        Rectangle{
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width/4

            color: "transparent"
            border.width: 1
            border.color: Style.borderOn
            property int stringHeight: parent.height/16

            Column{
                width: parent.width * 0.9
                height: parent.height * 0.9
                anchors.centerIn: parent

                spacing: _main.stringHeight

                MSwitchHorizontal{
                    id: _exprSwitch

                    width: parent.width
                    height: _main.stringHeight

                    text: "Expr. pedal: "

                    checked: UiCore.currentDevice.systemSettings.exprOn

                    onClicked: {
                        UiCore.currentDevice.systemSettings.exprOn = checked
                    }
                }


                MComboHorizontal{
                    width: parent.width
                    height: _main.stringHeight

                    text: "Expr. type: "

                    enabled: _exprSwitch.position
                    opacity: enabled ? 1 : 0.5

                    currentIndex: UiCore.currentDevice.systemSettings.exprType
                    model: ["Standart V", "Alternative V", "Standart CC", "Alternative CC"]

                    onActivated: (index) => {

                        UiCore.currentDevice.systemSettings.exprType = currentIndex;
                    }
                }

                MComboHorizontal{
                    width: parent.width
                    height: _main.stringHeight

                    text: "Expr. CC#: "

                    enabled: _exprSwitch.position
                    opacity: enabled ? 1 : 0.5

                    currentIndex: UiCore.currentDevice.systemSettings.exprCC
                    model: _exprCCModel

                    onActivated: (index) => {

                        UiCore.currentDevice.systemSettings.exprCC = currentIndex;
                    }

                    ListModel{
                        id: _exprCCModel

                        Component.onCompleted: {
                            append({value: "Off"})

                            for(let i=0; i<127; i++){
                                append({value: String(i)})
                            }
                        }
                    }
                }

                MComboHorizontal{
                    width: parent.width
                    height: _main.stringHeight

                    text: "Expr. store level: "

                    enabled: _exprSwitch.position
                    opacity: enabled ? 1 : 0.5

                    currentIndex: UiCore.currentDevice.systemSettings.exprStoreLevel
                    model: ["Off", "On"]

                    onActivated: (index) => {

                        UiCore.currentDevice.systemSettings.exprStoreLevel = currentIndex;
                    }
                }

            }
        }

        FswMenu{
            Layout.fillWidth: true
            Layout.fillHeight: true

            stringHeight: _main.stringHeight
        }

        Rectangle{
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: "transparent"
            border.width: 1
            border.color: Style.borderOn
            property int stringHeight: parent.height/16

            Column{
                width: parent.width * 0.9
                height: parent.height * 0.9
                anchors.centerIn: parent

                spacing: _main.stringHeight

                MLabel{
                    height: _main.stringHeight
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "MIDI PC Map:"
                }

                RowLayout{
                    width: parent.width
                    height: _main.stringHeight

                    MComboHorizontal{
                        id: _comboPcChoice

                        Layout.preferredWidth: parent.width/3

                        model: _midiPcMapModel

                        currentIndex: 0

                        ListModel{
                            id: _midiPcMapModel

                            Component.onCompleted: {
                                for(let i=1; i<=128; i++){
                                    append({value: i})
                                }
                            }
                        }
                    }

                    MLabel{
                        Layout.preferredWidth: parent.width/3

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "->"
                    }

                    MComboHorizontal{
                        id: _comboPresetChoice

                        Layout.fillWidth: true

                        model: UiCore.currentDevice.strPresetNumbers

                        currentIndex: UiCore.currentDevice.systemSettings.midiPcMap[_comboPcChoice.currentIndex]

                        onActivated: {
                            UiCore.currentDevice.systemSettings.setMidiPcMap(_comboPcChoice.currentIndex, _comboPresetChoice.currentIndex)
                        }
                    }
                }

                MSwitchHorizontal{
                    width: parent.width
                    height: _main.stringHeight

                    text: "Tuner control: "

                    checked: UiCore.currentDevice.systemSettings.tunerControl

                    onClicked: {
                        UiCore.currentDevice.systemSettings.tunerControl = checked
                    }
                }

                MComboHorizontal{

                    width: parent.width
                    height: _main.stringHeight

                    text: "Tuner on CC#: "

                    enabled: UiCore.currentDevice.systemSettings.tunerControl

                    model: _midiPcMapModel

                    currentIndex: UiCore.currentDevice.systemSettings.tunerCC


                    onActivated: {
                        UiCore.currentDevice.systemSettings.tunerCC = currentIndex
                    }
                }

            }
        }
    }
}
