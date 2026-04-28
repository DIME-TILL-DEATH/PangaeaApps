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

    color: Style.currentTheme.mainEnabledColor

    property int stringHeight: height/16
    property SystemSettingsFx systemSettings: UiCore.currentDevice.systemSettings

    RowLayout{
        anchors.fill: parent
        anchors.margins: 2


        Rectangle{
            id: _sysMenu1

            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width/4

            color: "transparent"
            border.width: 1
            border.color: Style.currentTheme.borderOn

            Column{
                width: parent.width * 0.9
                height: parent.height * 0.9
                anchors.centerIn: parent

                spacing: _main.stringHeight

                ParameterComboBox{
                    width: parent.width
                    height: _main.stringHeight

                    isHorizontal: true

                    ctrlValInstance: UiCore.currentDevice.systemSettings.mode

                    // moduleOn: true

                    model: ["CabSim On", "CabSim Off"]
                }

                MComboHorizontal{
                    id: _comboCabConfig

                    width: parent.width
                    height: _main.stringHeight

                    text: UiCore.currentDevice.systemSettings.cabNumber.name

                    currentIndex: _main.systemSettings.cabNumber.displayValue
                    model: ["1 L+R", "1R Amp, Pres.", "2 L+R", "1R Amp", "1R Pres.", "1R Dry"]

                    onActivated: (index) => {

                        if(UiCore.currentDevice.systemSettings.cabNumber.displayValue === 2)
                        {
                            if(_comboCabConfig.currentIndex !== UiCore.currentDevice.systemSettings.cabNumber.displayValue)
                            {
                                restartDialog.show();
                            }
                            else
                            {
                                UiCore.currentDevice.systemSettings.cabNumber.displayValue = _comboCabConfig.currentIndex;
                            }
                        }
                        else
                        {
                            if(_comboCabConfig.currentIndex === 2)
                            {
                                restartDialog.show();
                            }
                            else
                            {
                                UiCore.currentDevice.systemSettings.cabNumber.displayValue = _comboCabConfig.currentIndex;
                            }
                        }
                    }

                    NativeMessageDialog
                    {
                        id: restartDialog

                        title: qsTr("Restart device?")
                        text: qsTr("Changing stereo/mono cab config needs device restart. Do you to do it now?")

                        buttons: DialogButtonBox.Yes | DialogButtonBox.No

                        modality: Qt.ApplicationModal
                        onAccepted: {
                            UiCore.currentDevice.systemSettings.cabNumber.displayValue = _comboCabConfig.currentIndex;
                            UiCore.currentDevice.restartDevice()
                        }
                        onRejected: {
                            _comboCabConfig.currentIndex = _main.systemSettings.cabNumber.displayValue

                        }
                    }
                }

                ParameterComboBox{
                    width: parent.width
                    height: _main.stringHeight
                    isHorizontal: true

                    ctrlValInstance: UiCore.currentDevice.systemSettings.midiChannel
                    model: _midiChannelModel


                    ListModel{
                        id: _midiChannelModel

                        Component.onCompleted: {
                            for(let i =1; i<16; i++){
                                append({value: i})
                            }
                        }
                    }
                }

                ParameterComboBox{
                    width: parent.width
                    height: _main.stringHeight
                    isHorizontal: true

                    ctrlValInstance: UiCore.currentDevice.systemSettings.spdif
                    model: ["Main Output", "Dry Input"]
                }

                ParameterComboBox{
                    width: parent.width
                    height: _main.stringHeight
                    isHorizontal: true

                    ctrlValInstance: UiCore.currentDevice.systemSettings.tempo
                    model: ["Preset", "Global", "Glob.+MIDI"]
                }

                ParameterComboBox{
                    width: parent.width
                    height: _main.stringHeight
                    isHorizontal: true

                    ctrlValInstance: UiCore.currentDevice.systemSettings.timeFormat
                    model: ["Sec", "BPM"]
                }

                ParameterComboBox{
                    width: parent.width
                    height: _main.stringHeight
                    isHorizontal: true

                    ctrlValInstance: UiCore.currentDevice.systemSettings.swapConf
                    model: ["Off", "On"]
                }


                ParameterBar{
                    width: parent.width
                    height: _main.stringHeight

                    controlValue: systemSettings.tunerSpeed
                }
            }
        }

        Rectangle{
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width/4

            color: "transparent"
            border.width: 1
            border.color: Style.currentTheme.borderOn
            property int stringHeight: parent.height/16

            Column{
                width: parent.width * 0.9
                height: parent.height * 0.9
                anchors.centerIn: parent

                spacing: _main.stringHeight

                ParameterSwitch{
                    id: _exprSwitch

                    width: parent.width

                    ctrlValInstance: _main.systemSettings.exprOn

                    moduleOn: true
                    isHorizontal: true
                }


                ParameterComboBox{
                    width: parent.width
                    height: _main.stringHeight
                    isHorizontal: true

                    enabled: _exprSwitch.position
                    opacity: enabled ? 1 : 0.5

                    ctrlValInstance: UiCore.currentDevice.systemSettings.exprType
                    model: ["Std. Volume", "Alt. Volume", "Std. CC", "Alt.CC"]
                }

                ParameterComboBox{
                    width: parent.width
                    height: _main.stringHeight
                    isHorizontal: true

                    enabled: _exprSwitch.position
                    opacity: enabled ? 1 : 0.5

                    ctrlValInstance: UiCore.currentDevice.systemSettings.exprCC
                    model: _exprCCModel

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

                ParameterSwitch{
                    width: parent.width
                    // height: _main.stringHeight

                    ctrlValInstance: UiCore.currentDevice.systemSettings.exprStoreLevel

                    moduleOn: true
                    isHorizontal: true
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
            border.color: Style.currentTheme.borderOn
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

                        currentIndex: systemSettings.midiPcMap[_comboPcChoice.currentIndex]

                        onActivated: {
                            systemSettings.setMidiPcMap(_comboPcChoice.currentIndex, _comboPresetChoice.currentIndex)
                        }
                    }
                }

                MSwitchHorizontal{
                    width: parent.width
                    height: _main.stringHeight

                    text: "Tuner control: "

                    checked: UiCore.currentDevice.systemSettings.tunerControl

                    onClicked: {
                        systemSettings.tunerControl = checked
                    }
                }

                ParameterComboBox{
                    width: parent.width
                    height: _main.stringHeight
                    isHorizontal: true

                    enabled: UiCore.currentDevice.systemSettings.tunerControl
                    opacity: enabled ? 1 : 0.5

                    model: _midiPcMapModel

                    ctrlValInstance: UiCore.currentDevice.systemSettings.tunerCC
                }

            }
        }
    }
}
