import QtQuick
import QtQuick.Controls

import Elements
import StyleSettings

import CppObjects
import PangaeaBackend

Item
{
    id: main


    Row
    {
        id: row
        anchors.fill: parent
        spacing: 2
        property int widthWithoutSpase: width-spacing*10

        Logo
        {
            height: parent.height
            width:  row.widthWithoutSpase/15*1
        }

        SaveCompare
        {
            height: parent.height
            width:  row.widthWithoutSpase/15*1
        }

        CopyPaste
        {
            height: parent.height
            width:  row.widthWithoutSpase/15*1
        }


        Column{
            width:  row.widthWithoutSpase/15*8+4
            height: parent.height

            Rectangle{
                id: _presetNameRect

                width: parent.width
                height: parent.height/2

                border.width: 1

                color: "transparent"
                Row{
                    anchors.fill: parent
                    Rectangle{
                        border.width: 1
                        color: Style.mainEnabledColor
                        height: parent.height
                        width: parent.width/5
                        MText{
                            color: palette.text

                            anchors.fill: parent
                            verticalAlignment:   Text.AlignVCenter
                            font.pixelSize: Math.min(parent.height/1.5, parent.width/9)

                            text: qsTr(" Preset name")
                        }
                    }

                    TextInput{
                        id: _presetNameEdit
                        text: UiCore.currentDevice.currentPresetName

                        height: parent.height
                        width: parent.width*4/5

                        horizontalAlignment: TextInput.AlignHCenter
                        verticalAlignment:   TextInput.AlignVCenter

                        color: palette.text
                        font.bold: true
                        font.family: "Arial Black"
                        font.pixelSize: Math.min(parent.height/1.5, parent.width/15)

                        onAccepted:{
                            focus = false
                            UiCore.currentDevice.currentPresetName = _presetNameEdit.text
                        }
                    }
                }
            }

            Rectangle{
                id: impuls

                width: parent.width
                height: parent.height/2

                border.width: 1

                color: "transparent"
                Row{
                    anchors.fill: parent
                    Rectangle{
                        border.width: 1
                        color: Style.mainEnabledColor
                        height: parent.height
                        width: parent.width/5
                        MText{
                            color: palette.text

                            anchors.fill: parent
                            verticalAlignment:   Text.AlignVCenter
                            font.pixelSize: Math.min(parent.height/1.5, parent.width/9)

                            text: qsTr(" Preset comment")
                        }
                    }

                    TextInput{
                        id: _presetCommentEdit
                        text: UiCore.currentDevice.currentPresetComment

                        height: parent.height
                        width: parent.width*4/5

                        horizontalAlignment: TextInput.AlignHCenter
                        verticalAlignment:   TextInput.AlignVCenter

                        color: palette.text
                        font.bold: true
                        font.family: "Arial Black"
                        font.pixelSize: Math.min(parent.height/1.5, parent.width/15)

                        onAccepted:{
                            focus = false
                            UiCore.currentDevice.currentPresetComment = _presetCommentEdit.text
                        }
                    }
                }
            }
        }

        Rectangle{
            height: parent.height
            width:  row.widthWithoutSpase/15*1

            color: Style.mainEnabledColor

            ParameterDial{
                id: vlControl
                property Volume module: UiCore.currentDevice.masterVolume

                width:  parent.width * 0.9
                height: parent.height * 0.9

                controlValue: UiCore.currentDevice.masterVolume.volume

                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Rectangle{
            height: parent.height
            width:  row.widthWithoutSpase/15*1

            color: Style.mainEnabledColor
            ParameterDial{
                id: phonesControl
                property Volume module: UiCore.currentDevice.phonesVolume

                width:  parent.width * 0.9
                height: parent.height * 0.9

                controlValue: UiCore.currentDevice.phonesVolume.volume

                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        PresetSpin{
            id: preset

            height: parent.height
            width:  row.widthWithoutSpase/15*1
        }

        Button{
            height: parent.height
            width:  row.widthWithoutSpase/15*1

            text: qsTr("MAP")

            onClicked: {
                map.show()
            }
        }
    }

    Map{
        id: map

    }
}
