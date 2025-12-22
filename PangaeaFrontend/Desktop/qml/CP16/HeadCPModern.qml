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

    property CabSim irModule: UiCore.currentDevice.IR

    signal openModulesConfigWindow()
    signal openIrManagerWindow()

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

        BankSpin
        {
            id: bank

            height: parent.height
            width:  row.widthWithoutSpase/15*1
        }

        Rectangle{
            height: parent.height
            width:  row.widthWithoutSpase/15*1
            color: Style.currentTheme.mainEnabledColor

            Column{
                anchors.fill: parent
                Item
                {
                    width:  parent.width
                    height: parent.height/4
                    CheckText
                    {
                        text:  qsTr("Phones")
                        check: UiCore.currentDevice.outputMode === 0
                        onClicked: UiCore.currentDevice.outputMode = 0

                        anchors.fill: parent
                    }
                }
                Item
                {
                    width:  parent.width
                    height: parent.height/4
                    CheckText
                    {
                        text:  qsTr("Line")
                        check: UiCore.currentDevice.outputMode === 1
                        onClicked: UiCore.currentDevice.outputMode = 1

                        anchors.fill: parent
                    }
                }
                Item
                {
                    width:  parent.width
                    height: parent.height/4
                    CheckText
                    {
                        text:  qsTr("Balanced")
                        check: UiCore.currentDevice.outputMode === 2
                        onClicked: UiCore.currentDevice.outputMode = 2

                        anchors.fill: parent
                    }
                }
                Item
                {
                    width:  parent.width
                    height: parent.height/4
                    CheckText
                    {
                        text:  qsTr("Wet/Dry")
                        check: UiCore.currentDevice.outputMode === 3
                        onClicked: UiCore.currentDevice.outputMode = 3

                        anchors.fill: parent
                    }
                }
            }
        }

        SaveCompare
        {
            height: parent.height
            width:  row.widthWithoutSpase/15*1
        }

        Column{
            width:  row.widthWithoutSpase/15*7+4
            height: parent.height
            Rectangle{
                id: _presetNameRect

                width: parent.width
                height: parent.height/3

                border.width: 1

                color: Style.currentTheme.barLow//"transparent"
                Row{
                    anchors.fill: parent
                    Rectangle{
                        border.width: 1
                        color: Style.currentTheme.mainEnabledColor
                        height: parent.height
                        width: parent.width/5
                        MText{
                            color: Style.currentTheme.textInverted //palette.text

                            anchors.fill: parent
                            verticalAlignment:   Text.AlignVCenter
                            font.pixelSize: Math.min(parent.height/1.5, parent.width/7.5)
                            x: 5

                            elide: Text.ElideRight

                            text: qsTr("Preset name")
                        }
                    }

                    TextInput{
                        id: _presetNameEdit
                        text: UiCore.currentDevice.currentPresetName

                        height: parent.height
                        width: parent.width*4/5

                        horizontalAlignment: TextInput.AlignHCenter
                        verticalAlignment:   TextInput.AlignVCenter

                        color: Style.currentTheme.textMain
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

            Rectangle
            {
                id: impuls

                width: parent.width
                height: parent.height/3

                enabled: UiCore.currentDevice.IR.used

                color: enabled ? Style.currentTheme.headColor : Style.currentTheme.borderOff

                border.width: 1

                opacity: main.irModule.moduleEnabled ? 1:0.5

                Behavior on opacity{
                    NumberAnimation{duration: 500}
                }

                Row{
                    anchors.fill: parent

                    Rectangle{
                        border.width: 1
                        color: Style.currentTheme.mainEnabledColor

                        height: parent.height
                        width: impuls.enabled ? parent.width/5 : 0

                        MText{

                            anchors.fill: parent
                            verticalAlignment:   Text.AlignVCenter
                            font.pixelSize: Math.min(parent.height/1.5, parent.width/7.5)
                            x: 5

                            elide: Text.ElideRight

                            color: Style.currentTheme.textMain

                            visible: impuls.enabled

                            text: qsTr("IR name")
                        }
                    }


                    Item{
                        width: impuls.enabled ? parent.width*4/5 : parent.width
                        height: parent.height
                        MText
                        {
                            id: impulsTxt

                            width: parent.width * 0.95
                            height: parent.height

                            anchors.horizontalCenter: parent.horizontalCenter

                            text: impuls.enabled ? (irModule.impulseName === "" ? qsTr("Empty") : irModule.impulseName)
                                                 : qsTr("IR module not configured")
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment:   Text.AlignVCenter

                            color: Style.currentTheme.textMain

                            font.pixelSize: Math.min(parent.height/1.5, parent.width/15)
                            elide: Text.ElideMiddle

                            MouseArea
                            {
                                id: _maIrSelect

                                anchors.fill: parent
                                onClicked: openIrManagerWindow();

                                cursorShape: Qt.PointingHandCursor

                                hoverEnabled: true
                            }

                        }
                        ToolTip{
                            id: __toolTipIrName

                            text: impulsTxt.text
                            visible: _maIrSelect.containsMouse & impulsTxt.truncated
                            timeout: 0
                        }
                    }
                }
            }

            Rectangle{
                height: parent.height/3
                width:  parent.width

                opacity: mA.pressed ? 0.5 : 1

                color: Style.currentTheme.mainEnabledColor

                MText{
                    text: qsTr("Add/Remove module")
                    anchors.fill: parent

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                MouseArea{
                    id: mA

                    anchors.fill: parent
                    onClicked: openModulesConfigWindow();
                }
            }
        }

        CopyPaste
        {
            height: parent.height
            width:  row.widthWithoutSpase/15*1
        }

        Rectangle
        {
            height: parent.height
            width:  row.widthWithoutSpase/15*1

            color: Style.currentTheme.mainEnabledColor


            ParameterDial{
                id: vlControl
                property Volume module: UiCore.currentDevice.MV

                width:  parent.width * 0.9
                height: parent.height * 0.9

                controlValue: UiCore.currentDevice.MV.volume

                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        PresetSpin
        {
            id: preset

            height: parent.height
            width:  row.widthWithoutSpase/15*1
        }

        MImageButton{
            height: parent.height
            width:  row.widthWithoutSpase/15*1

            imageSource: "qrc:/Images/table1.svg";
            imageColor: Style.currentTheme.mainEnabledColor

            onClicked: {
                map.show()
            }
        }
    }

    MapTable{
        id: map


        delegate: Item{
            id: _root

            width: map.cellWidth
            height: map.cellHeight

            property int currentIndex: map.currentIndex

            property bool currentImpulseEnabled
            property string currentImpulseName

            Rectangle{
                anchors.centerIn: parent

                width: Math.min(parent.width*0.5, parent.height*0.5)
                height: width

                radius: width/2

                color: isImpulseEmpty ? "transparent" : Style.currentTheme.highlightColor

                opacity: isImpulseEnabled ? 1 : 0.5
                border.width: Math.max(2, width/20)
                border.color: currentIndex === presetMapIndex ? Style.currentTheme.highlightColor : Style.currentTheme.backgroundColor

                MouseArea
                {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        UiCore.currentDevice.changePreset(bankNumber, presetNumber);
                    }
                    onEntered: tp.visible = (tp.text.length>0)
                    onExited:  tp.visible = false
                }

                ToolTip
                {
                    id: tp

                    text: qsTr("Preset name: ") + presetName + "\n" + qsTr("IR name: ") + impulseName

                    visible: false
                    timeout: 0
                }
            }
        }
    }
}
