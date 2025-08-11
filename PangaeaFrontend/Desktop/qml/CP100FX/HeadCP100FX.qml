import QtQuick
import QtQuick.Controls

import Qt5Compat.GraphicalEffects

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

        Rectangle
        {
            height: parent.height
            width:  row.widthWithoutSpase/15*2
            color: Style.mainEnabledColor
            Column
            {
                anchors.fill: parent
                MButton
                {
                    width:  parent.width
                    height: parent.height/2
                    text: "SAVE"

                    enabled: UiCore.currentDevice.deviceParamsModified
                    // enabled: UiCore.currentDevice.deviceParamsModified & (UiCore.currentDevice.presetManager.currentState !== PresetState.Compare)
                    onClicked: UiCore.currentDevice.saveChanges();
                }

                MButton
                {
                    id: bCopy
                    width:  parent.width
                    height: parent.height/2
                    text: "COPY"

                    onClicked: UiCore.currentDevice.copyPreset();
                }
            }
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

        MImageButton{
            height: parent.height
            width:  row.widthWithoutSpase/15*1

            imageSource: "qrc:/Images/list1.svg";
            imageColor: Style.mainEnabledColor

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

    MapList{
        id: map

        delegate: Rectangle{
            width: map.width*0.9
            height: map.height/10
            radius: height/10

            border.width: index === map.currentIndex ? 4 : 2
            border.color: index === map.currentIndex ? Style.highlightColor : Style.textEnabled

            color: Style.barLow

            MouseArea{
                anchors.fill: parent
                z: 10

                onClicked: {
                    UiCore.sgQmlRequestChangePreset(UiCore.currentDevice.bank, index);
                }
            }

            Row{
                width: parent.width*0.95
                height: parent.height*0.95
                anchors.centerIn: parent

                Column{
                    height: parent.height
                    width: parent.width/2
                    MText{
                        width: parent.width
                        height: parent.height/2
                        text: (presetNumber*1 + 1)  + ". " + presetName

                        elide: Text.ElideMiddle
                    }

                    MText{
                        width: parent.width
                        height: parent.height/2
                        text: presetComment

                        elide: Text.ElideMiddle
                    }
                }

                GridView{
                    id: _modulesGridView

                    height: parent.height * 0.85
                    width: parent.width/2

                    anchors.verticalCenter: parent.verticalCenter

                    cellWidth: width/7
                    cellHeight: height/2

                    model: activeModules

                    delegate: Rectangle{
                        width: _modulesGridView.cellWidth * 0.9
                        height: _modulesGridView.cellHeight * 0.9

                        color: "transparent"

                        border.width: 1
                        border.color: Style.borderOn

                        MText{
                            anchors.fill: parent
                            text: modelData

                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter

                            font.pixelSize: parent.height/2
                        }
                    }
                }
            }
        }
    }
}
