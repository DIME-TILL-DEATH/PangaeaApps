import QtQuick
import QtQuick.Controls

import Elements
import StyleSettings
import Modules

import CppObjects
import PangaeaBackend

Item
{
    id: main

    property CabSim irModule

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

        SwitchOutput
        {
            height: parent.height
            width:  row.widthWithoutSpase/15*1
        }

        SaveCompare
        {
            height: parent.height
            width:  row.widthWithoutSpase/15*1
        }

        Column{
            width:  row.widthWithoutSpase/15*7+4
            height: parent.height
            Rectangle
            {
                id: impuls

                width: parent.width
                height: parent.height * 0.7

                color: Style.headColor

                border.width: 1

                opacity: main.irModule.moduleEnabled ? 1:0.3

                Behavior on opacity{
                    NumberAnimation{duration: 500}
                }

                MText
                {
                    anchors.fill: parent
                    id: impulsTxt
                    text: irModule.impulseName === "" ? qsTr("Empty") : irModule.impulseName
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment:   Text.AlignVCenter

                    color: "black"

                    font.pixelSize: parent.height/2.5
                    wrapMode: Text.Wrap
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: openIrManagerWindow();

                    cursorShape: Qt.PointingHandCursor
                }
            }

            Item{
                height: parent.height * 0.3
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
                property PresetVolume module: UiCore.currentDevice.MV

                width:  parent.width * 0.9
                height: parent.height * 0.9

                controlValue: UiCore.currentDevice.MV.presetVolume

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

        Button
        {
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
