import QtQuick 2.15

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

        Rectangle{
            height: parent.height
            width:  row.widthWithoutSpase/15*1
            color: Style.mainEnabledColor

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
                            font.pixelSize: Math.min(parent.height/1.5, parent.width/7.5)

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

                        color: Style.textEnabled
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

                color: enabled ? Style.headColor : "gray"

                border.width: 1

                opacity: main.irModule.moduleEnabled ? 1:0.3

                Behavior on opacity{
                    NumberAnimation{duration: 500}
                }

                Row{
                    anchors.fill: parent

                    Rectangle{
                        border.width: 1
                        color: Style.mainEnabledColor
                        height: parent.height
                        width: impuls.enabled ? parent.width/5 : 0
                        MText{

                            anchors.fill: parent
                            verticalAlignment:   Text.AlignVCenter
                            font.pixelSize: Math.min(parent.height/1.5, parent.width/7.5)

                            visible: impuls.enabled

                            text: qsTr(" IR name")
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

                            color: "black"

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

            Item{
                height: parent.height/3
                width:  parent.width

                Button{
                    width: parent.width
                    height: parent.height

                    text: qsTr("Add/Remove module")

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

            color: Style.mainEnabledColor


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

        Rectangle
        {
            color: Style.mainEnabledColor

            height: parent.height
            width:  row.widthWithoutSpase/15*1

            Grid{
                id: _laModeSelector

                anchors.fill: parent

                property bool isDeviceUpdatingValues: false
                columns: 2
                rows: 2

                MText{
                    width: parent.width/2
                    height: parent.height/2

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    text: qsTr("CLN")
                    color: "lightgreen"
                }

                ComboBox{
                    id: clnCombo

                    width: parent.width/2
                    height: parent.height/2

                    model: UiCore.currentDevice.maxPresetCount

                    onActivated: {
                        if(!_laModeSelector.isDeviceUpdatingValues){
                            UiCore.currentDevice.setLa3Mappings(clnCombo.currentIndex, _dstCombo.currentIndex);
                        }
                    }
                }

                MText{
                    width: parent.width/2
                    height: parent.height/2

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    text: qsTr("DST")
                    color: "red"
                }

                ComboBox{
                    id: _dstCombo
                    width: parent.width/2
                    height: parent.height/2

                    model: UiCore.currentDevice.maxPresetCount

                    onActivated: {
                        if(!_laModeSelector.isDeviceUpdatingValues){
                            UiCore.currentDevice.setLa3Mappings(clnCombo.currentIndex, _dstCombo.currentIndex);
                        }
                    }
                }

                Connections{
                    target: UiCore.currentDevice

                    function onPresetMapChanged()
                    {
                        _laModeSelector.isDeviceUpdatingValues = true;
                        clnCombo.currentIndex = UiCore.currentDevice.clnPresetMap
                        _dstCombo.currentIndex = UiCore.currentDevice.drvPresetMap
                        _laModeSelector.isDeviceUpdatingValues = false;
                    }
                }
            }
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


    }

    MapList{
        id: map

        delegate: Rectangle{
            width: map.width * 0.9
            height: map.height / 10
            radius: height / 10

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
                        text: presetNumber  + ". " + presetName

                        elide: Text.ElideMiddle
                    }

                    MText{
                        width: parent.width
                        height: parent.height/2
                        text: "IR name: " + impulseName

                        elide: Text.ElideMiddle
                    }
                }
            }
        }
    }
}
