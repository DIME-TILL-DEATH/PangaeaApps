import QtQuick 2.15
import QtQuick.Controls.Fusion

import QtQuick.Window 2.15

import Elements 1.0
import StyleSettings 1.0
import Layouts 1.0

import CP100FX 1.0

import QtQuick.Layouts

import PangaeaFrontend
import PangaeaBackend

Rectangle{
    id: root

    color: Style.currentTheme.mainEnabledColor

    property int stringHeight: height/16

    property Cp100fx cp100fx: UiCore.currentDevice as Cp100fx

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

                spacing: root.stringHeight

                ParameterComboBox{
                    width: parent.width
                    height: root.stringHeight

                    isHorizontal: true

                    ctrlValInstance: root.cp100fx.systemSettings.mode

                    model: ["CabSim On", "CabSim Off"]
                }

                MComboHorizontal{
                    id: _comboCabConfig

                    width: parent.width
                    height: root.stringHeight

                    text: root.cp100fx.systemSettings.cabNumber.name

                    currentIndex: root.cp100fx.systemSettings.cabNumber.displayValue
                    model: ["1 L+R", "1R Amp, Pres.", "2 L+R", "1R Amp", "1R Pres.", "1R Dry"]

                    onActivated: (index) => {

                        if(root.cp100fx.systemSettings.cabNumber.displayValue === 2)
                        {
                            if(_comboCabConfig.currentIndex !== root.cp100fx.systemSettings.cabNumber.displayValue)
                            {
                                restartDialog.show();
                            }
                            else
                            {
                                root.cp100fx.systemSettings.cabNumber.displayValue = _comboCabConfig.currentIndex;
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
                                root.cp100fx.systemSettings.cabNumber.displayValue = _comboCabConfig.currentIndex;
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
                            root.cp100fx.systemSettings.cabNumber.displayValue = _comboCabConfig.currentIndex;
                            root.cp100fx.restartDevice()
                        }
                        onRejected: {
                            _comboCabConfig.currentIndex = root.cp100fx.systemSettings.cabNumber.displayValue

                        }
                    }
                }

                ParameterComboBox{
                    width: parent.width
                    height: root.stringHeight
                    isHorizontal: true

                    ctrlValInstance: root.cp100fx.systemSettings.midiChannel
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
                    height: root.stringHeight
                    isHorizontal: true

                    ctrlValInstance: root.cp100fx.systemSettings.spdif
                    model: ["Main Output", "Dry Input"]
                }

                ParameterComboBox{
                    width: parent.width
                    height: root.stringHeight
                    isHorizontal: true

                    ctrlValInstance: root.cp100fx.systemSettings.tempo
                    model: ["Preset", "Global", "Glob.+MIDI"]
                }

                ParameterComboBox{
                    width: parent.width
                    height: root.stringHeight
                    isHorizontal: true

                    ctrlValInstance: root.cp100fx.systemSettings.timeFormat
                    model: ["Sec", "BPM"]
                }

                ParameterComboBox{
                    width: parent.width
                    height: root.stringHeight
                    isHorizontal: true

                    ctrlValInstance: root.cp100fx.systemSettings.swapConf
                    model: ["Off", "On"]
                }


                ParameterBar{
                    width: parent.width
                    height: root.stringHeight

                    controlValue: root.cp100fx.systemSettings.tunerSpeed
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

                spacing: root.stringHeight

                ParameterSwitch{
                    id: _exprSwitch

                    width: parent.width
                    isHorizontal: true

                    ctrlValInstance: root.cp100fx.systemSettings.exprOn
                }

                ParameterComboBox{
                    width: parent.width
                    height: root.stringHeight
                    isHorizontal: true

                    enabled: _exprSwitch.position
                    opacity: enabled ? 1 : 0.5

                    ctrlValInstance: root.cp100fx.systemSettings.exprType
                    model: ["Std. Volume", "Alt. Volume", "Std. CC", "Alt.CC"]
                }

                ParameterComboBox{
                    width: parent.width
                    height: root.stringHeight
                    isHorizontal: true

                    enabled: _exprSwitch.position
                    opacity: enabled ? 1 : 0.5

                    ctrlValInstance: root.cp100fx.systemSettings.exprCC
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

                    ctrlValInstance: root.cp100fx.systemSettings.exprStoreLevel

                    moduleOn: true
                    isHorizontal: true
                }

            }
        }

        FswMenu{
            Layout.fillWidth: true
            Layout.fillHeight: true

            stringHeight: root.stringHeight
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

                spacing: root.stringHeight

                MLabel{
                    height: root.stringHeight
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "MIDI PC Map:"
                }

                RowLayout{
                    width: parent.width
                    height: root.stringHeight

                    MComboHorizontal{
                        id: _comboPcChoice

                        Layout.preferredWidth: parent.width/3

                        model: _midiPcMapModel

                        currentIndex: 0

                        ListModel{
                            id: _midiPcMapModel

                            Component.onCompleted: {
                                for(let i=0; i<=127; i++){
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

                        model: root.cp100fx.strPresetNumbers

                        currentIndex: root.cp100fx.systemSettings.midiPcMap[_comboPcChoice.currentIndex]

                        onActivated: {
                            root.cp100fx.systemSettings.setMidiPcMap(_comboPcChoice.currentIndex, _comboPresetChoice.currentIndex)
                        }
                    }
                }

                ParameterSwitch{
                    width: parent.width
                    isHorizontal: true

                    ctrlValInstance: root.cp100fx.systemSettings.tunerControl
                }

                ParameterComboBox{
                    width: parent.width
                    height: root.stringHeight
                    isHorizontal: true

                    enabled: root.cp100fx.systemSettings.tunerControl
                    opacity: enabled ? 1 : 0.5

                    model: _midiPcMapModel

                    ctrlValInstance: root.cp100fx.systemSettings.tunerCC
                }

            }
        }
    }
}
