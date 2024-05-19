import QtQuick

import Elements
import StyleSettings
import CppObjects
import CppEnums

Item
{
    id: main

    property bool irOn: true

    signal setImpuls()

    //TODO: disable on wait

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

            edit: DeviceProperties.presetModified
        }

        Rectangle
        {
            id: impuls

            height: parent.height
            width:  row.widthWithoutSpase/15*7+4

            color: Style.headColor

            opacity: main.irOn ? 1:0.3
            Behavior on opacity{
                NumberAnimation{duration: 500}
            }

            MText
            {
                anchors.fill: parent
                id: impulsTxt
                text: "empty"
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

            isAvaliable: (DeviceProperties.deviceType === DeviceType.CP16PA) |
                         (DeviceProperties.deviceType === DeviceType.CP100PA) |
                         (DeviceProperties.deviceType === DeviceType.LA3PA)

            enabled: !AppProperties.compareState
        }

        PresetSpin
        {
            id: preset

            height: parent.height
            width:  row.widthWithoutSpase/15*1
        }

        Item
        {
            height: parent.height
            width:  row.widthWithoutSpase/15*1
        }
    }

    Connections
    {
        target: UiCore
        function onSgSetUIText(nameParam, value)
        {
            if (nameParam==="impulse_name")
            {
                impulsTxt.text = (value==="") ? qsTr("empty") : value;
            }
        }

        function onSgSetUiDeviceParameter(paramType, value)
        {
            switch(paramType)
            {
            case DeviceParameter.CABINET_ENABLE:
            {
                main.irOn = value;
                break;
            }
            }
        }

        function onSgSetDeviceParameter(paramType, value)
        {
            if(paramType === DeviceParameter.CABINET_ENABLE)
            {
                main.irOn = value;
            }
        }
    }
}
