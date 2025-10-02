import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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
            color: Style.currentTheme.mainEnabledColor
            GridLayout
            {
                anchors.fill: parent
                columns: 2
                rows: 2

                MButton
                {
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    text: "SAVE"

                    enabled: UiCore.currentDevice.deviceParamsModified

                    onClicked: UiCore.currentDevice.saveChanges();
                }

                MButton
                {
                    id: bCopy

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    text: "COPY"

                    onClicked: _copyWindow.show();
                }

                MButton
                {
                    id: bErase

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    text: "ERASE"

                    onClicked: eraseDialog.show();

                    NativeMessageDialog
                    {
                        id: eraseDialog

                        title: qsTr("Erase preset")
                        text: qsTr("Do you want to erase preset?")

                        buttons: DialogButtonBox.Yes | DialogButtonBox.No

                        modality: Qt.ApplicationModal
                        onAccepted: {
                            UiCore.currentDevice.erasePreset()
                        }
                    }
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

                color: Style.currentTheme.headColor
                Row{
                    anchors.fill: parent
                    Rectangle{
                        border.width: 1
                        color: Style.currentTheme.mainEnabledColor
                        height: parent.height
                        width: parent.width/4
                        MText{
                            color: Style.currentTheme.textMain

                            width: parent.width * 0.9
                            height: parent.height
                            x: 5

                            verticalAlignment:   Text.AlignVCenter
                            font.pixelSize: Math.min(parent.height/2, parent.width/10)

                            elide: Text.ElideRight

                            text: qsTr("Preset name")
                        }
                    }

                    TextInput{
                        id: _presetNameEdit
                        text: UiCore.currentDevice.currentPresetName

                        height: parent.height
                        width: parent.width*3/4

                        horizontalAlignment: TextInput.AlignHCenter
                        verticalAlignment:   TextInput.AlignVCenter

                        validator: RegularExpressionValidator{
                            regularExpression: /[\x20-\x7E]{0,14}/
                        }

                        color: Style.currentTheme.textMain
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

                color: Style.currentTheme.headColor
                Row{
                    anchors.fill: parent
                    Rectangle{
                        border.width: 1
                        color: Style.currentTheme.mainEnabledColor
                        height: parent.height
                        width: parent.width/4

                        MText{
                            color: Style.currentTheme.textMain

                            width: parent.width * 0.9
                            height: parent.height
                            x: 5

                            elide: Text.ElideRight

                            verticalAlignment:   Text.AlignVCenter
                            font.pixelSize: Math.min(parent.height/2, parent.width/10)

                            text: qsTr("Preset comment")
                        }
                    }

                    TextInput{
                        id: _presetCommentEdit
                        text: UiCore.currentDevice.currentPresetComment

                        height: parent.height
                        width: parent.width*3/4

                        horizontalAlignment: TextInput.AlignHCenter
                        verticalAlignment:   TextInput.AlignVCenter

                        validator: RegularExpressionValidator{
                            regularExpression: /[\x20-\x7E]{0,14}/
                        }

                        color: Style.currentTheme.textMain
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
            imageColor: Style.currentTheme.mainEnabledColor

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

    CopyToWindow{
        id: _copyWindow
    }

    MapList{
        id: map

        delegate: Rectangle{
            width: map.width*0.9
            height: map.height/10
            radius: height/10

            border.width: index === map.currentIndex ? 4 : 2
            border.color: index === map.currentIndex ? Style.currentTheme.highlightColor : Style.currentTheme.textEnabled

            color: Style.currentTheme.mainEnabledColor

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
                        border.color: Style.currentTheme.borderOn

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
