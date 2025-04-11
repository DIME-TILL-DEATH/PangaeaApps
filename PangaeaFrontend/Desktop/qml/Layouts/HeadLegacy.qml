import QtQuick
import QtQuick.Controls

import Elements
import StyleSettings

import CppObjects
import PangaeaBackend

Item
{
    id: main

    property CabSim irModule

    signal setImpuls()

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

        Rectangle
        {
            id: impuls

            height: parent.height
            width:  row.widthWithoutSpase/15*7+4

            color: Style.headColor

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

                font.pixelSize: parent.height/4
                wrapMode: Text.Wrap
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked: main.setImpuls()

                cursorShape: Qt.PointingHandCursor
            }
        }

        CopyPaste
        {
            height: parent.height
            width:  row.widthWithoutSpase/15*1
        }

        SwitchPostPre
        {
            id: switchPostPre

            height: parent.height
            width:  row.widthWithoutSpase/15*1

            isAvaliable: (UiCore.currentDevice.deviceType === DeviceType.LEGACY_CP16PA) |
                         (UiCore.currentDevice.deviceType === DeviceType.LEGACY_CP100PA)

            enabled: UiCore.currentDevice.presetManager.currentState !== PresetState.Compare
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
