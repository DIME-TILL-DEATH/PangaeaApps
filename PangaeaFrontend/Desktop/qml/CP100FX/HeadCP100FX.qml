import QtQuick
import QtQuick.Controls

import Elements
import StyleSettings

import Layouts

import CppObjects
import PangaeaBackend

Item
{
    id: main

    signal selectMenu(menuNum: int)

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

                color: Style.headColor
                Row{
                    anchors.fill: parent
                    Rectangle{
                        border.width: 1
                        color: Style.mainEnabledColor
                        height: parent.height
                        width: parent.width/5
                        MText{
                            color: Style.textMain

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

                        validator: RegularExpressionValidator{
                            regularExpression: /[\x20-\x7E]{0,14}/
                        }

                        color: Style.textInverted
                        font.bold: true
                        font.family: "Arial Black"
                        font.pixelSize: Math.min(parent.height/2, parent.width/15)

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

                color: Style.headColor
                Row{
                    anchors.fill: parent
                    Rectangle{
                        border.width: 1
                        color: Style.mainEnabledColor
                        height: parent.height
                        width: parent.width/5
                        MText{
                            color: Style.textMain

                            anchors.fill: parent
                            verticalAlignment:   Text.AlignVCenter
                            font.pixelSize: Math.min(parent.height/2, parent.width/9)

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

                        validator: RegularExpressionValidator{
                            regularExpression: /[\x20-\x7E]{0,14}/
                        }

                        color: Style.textInverted
                        font.bold: true
                        font.family: "Arial Black"
                        font.pixelSize: Math.min(parent.height/2, parent.width/15)

                        onAccepted:{
                            focus = false
                            UiCore.currentDevice.currentPresetComment = _presetCommentEdit.text
                        }
                    }
                }
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


        Column{
            id: _menuColumn

            height: parent.height
            width:  row.widthWithoutSpase/15*2

            property int selectedMenu: 0

            MButton
            {
                width:  parent.width
                height: parent.height/2
                text: "PRESET"
                highlighted: _menuColumn.selectedMenu === 0

                onClicked: {
                    _menuColumn.selectedMenu = 0
                    main.selectMenu(_menuColumn.selectedMenu)
                }
            }

            MButton
            {
                width:  parent.width
                height: parent.height/2
                text: "GLOBAL"
                highlighted: _menuColumn.selectedMenu === 1

                onClicked: {
                    _menuColumn.selectedMenu = 1
                    main.selectMenu(_menuColumn.selectedMenu)
                }
            }
        }
    }

    MapFx{
        id: map

    }
}
