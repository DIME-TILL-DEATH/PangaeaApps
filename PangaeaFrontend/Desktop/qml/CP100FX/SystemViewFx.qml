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

    color: Style.mainEnabledColor

    RowLayout{
        anchors.fill: parent


        Rectangle{
            color: "transparent"
            border.width: 1
            border.color: Style.borderOn
        }

        Rectangle{
            id: _fswMenuRect

            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width/4

            color: "transparent"
            border.width: 1
            border.color: Style.borderOn

            property int stringHeight: parent.height/8

            Column{
                width: parent.width * 0.9
                height: parent.height * 0.9
                anchors.centerIn: parent

                MLabel{
                    height: _fswMenuRect.stringHeight
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Footswitch settings:"

                }

                Row{
                    width: parent.width
                   height: _fswMenuRect.stringHeight

                    MLabel{
                        width: parent.width/2
                        text: "Footswitch: "
                    }

                    ComboBox{
                        id: _comboFswSelect

                        width: parent.width/2
                        currentIndex: 0
                        model: ["DOWN", "CONFIRM", "UP"]

                        onActivated: (index) => {
                            currentIndex = index
                        }
                    }

                }

                Row{
                    width: parent.width
                    height: _fswMenuRect.stringHeight

                    MLabel{
                        width: parent.width/2
                        text: "Mode: "
                    }

                    ComboBox{
                        width: parent.width/2

                        currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].mode
                        model: ["SINGLE", "DOUBLE"]

                        onActivated: (index) => {
                            currentIndex = index;
                            UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].mode = currentIndex;
                        }
                    }

                }

                Row{
                    id: _pressType

                    width: parent.width
                    height: _fswMenuRect.stringHeight

                    MLabel{
                        width: parent.width/2
                        text: "Press type: "
                    }

                    ComboBox{
                        width: parent.width/2

                        currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType
                        model: ["DEFAULT", "CONTROLLER", "TUNER",
                                "PRESET MAP 1" , "PRESET MAP 2" , "PRESET MAP 3" , "PRESET MAP 4"]

                        onActivated: (index) => {
                            currentIndex = index;
                            UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType = currentIndex;
                        }
                    }

                }

                Row{
                    id: _pressController

                    visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType === FswFx.Controller

                    width: parent.width
                    height: _fswMenuRect.stringHeight

                    MLabel{
                        width: parent.width/2
                        text: "Press CC#: "
                    }

                    ComboBox{
                        width: parent.width/2

                        currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].controllerPressNum
                        model: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].ccNames
                    }

                }

                Row{
                    id: _pressMap

                    width: parent.width
                    height: _fswMenuRect.stringHeight

                    visible: (UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType >= FswFx.PresetMap1)

                    MLabel{
                        width: parent.width/6
                        text: "Seq.:"
                    }

                    ComboBox{
                        width: parent.width/8
                        model: UiCore.currentDevice.strPresetNumbers

                        indicator: Item{}
                    }

                    MLabel{
                        visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType >= FswFx.PresetMap2
                        width: parent.width/10
                        horizontalAlignment: Text.AlignHCenter
                        text: "->"
                    }

                    ComboBox{
                        width: parent.width/8
                        visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType >= FswFx.PresetMap2
                        model: UiCore.currentDevice.strPresetNumbers

                        indicator: Item{}
                    }

                    MLabel{
                        visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType >= FswFx.PresetMap3
                        width: parent.width/10
                        horizontalAlignment: Text.AlignHCenter
                        text: "->"
                    }

                    ComboBox{
                        width: parent.width/8
                        visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType >= FswFx.PresetMap3
                        model: UiCore.currentDevice.strPresetNumbers

                        indicator: Item{}
                    }

                    MLabel{
                        visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType === FswFx.PresetMap4
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width/10

                        text: "->"
                    }

                    ComboBox{
                        width: parent.width/8
                        visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType === FswFx.PresetMap4
                        model: UiCore.currentDevice.strPresetNumbers

                        indicator: Item{}
                    }
                }

                Row{
                    id: _holdType

                    visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].mode === FswFx.Double

                    width: parent.width
                    height: _fswMenuRect.stringHeight

                    MLabel{
                        width: parent.width/2
                        text: "Hold type: "
                    }

                    ComboBox{
                        width: parent.width/2

                        currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType
                        model: ["DEFAULT", "CONTROLLER", "TUNER",
                                "PRESET MAP 1" , "PRESET MAP 2" , "PRESET MAP 3" , "PRESET MAP 4"]

                        onActivated: (index) => {
                            currentIndex = index;
                            UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType = currentIndex;
                        }
                    }

                }

                Row{
                    id: _holdController

                    visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].mode === FswFx.Double
                        && UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType === FswFx.Controller

                    width: parent.width
                    height: _fswMenuRect.stringHeight

                    MLabel{
                        width: parent.width/2
                        text: "Hold CC#: "
                    }

                    ComboBox{
                        width: parent.width/2

                        currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].controllerHoldNum

                        model: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].ccNames
                    }

                }
            }
        }

        Rectangle{
            color: "transparent"
            border.width: 1
            border.color: Style.borderOn
        }
    }
}
