import QtQuick

import Elements
import StyleSettings
import CppObjects
import CppEnums

Item
{
    id: main



    property int  maxMapRow: 10
    property int controlMultiplier: 10
    property int  presetNom: bank.value*controlMultiplier+preset.value

    property int  bank:   bank.value
    property int  preset: preset.value
    property bool editable: true
    property bool edit: true

    property bool compareState: false

    property bool irOn: true

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

        Presets
        {
            id: bank

            height: parent.height
            width:  row.widthWithoutSpase/15*1

            text: "BANK"
            paramType: DeviceParameter.BANK
            onChPreset: UiCore.setParameter("set_preset_change", presetNom)
            enabled: main.editable
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

            editable: main.editable
            edit: main.edit
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

            enabled: main.editable & !main.compareState
        }

        Presets
        {
            id: preset

            height: parent.height
            width:  row.widthWithoutSpase/15*1

            text: "PRESET"
            paramType: DeviceParameter.PRESET

            onChPreset: UiCore.setParameter("set_preset_change", presetNom)
            enabled: main.editable
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

        function onSgSetUIParameter(nameParam, value)
        {
            if(nameParam === "compare_state")
            {
                main.compareState = value;
            }
        }

        function onSgSetUiDeviceParameter(paramType, value)
        {
            switch(paramType)
            {
            case DeviceParameter.DEVICE_TYPE:
            {
                switch (value)
                {
                    case DeviceType.CP100:
                    {
                        maxMapRow = 10;
                        switchPostPre.isAvaliable = false;
                        break;
                    }
                    case DeviceType.CP16:
                    {
                        controlMultiplier = 16;
                        maxMapRow = 4;
                        switchPostPre.isAvaliable = false;
                        break;
                    }
                    case DeviceType.CP16PA:
                    {
                        controlMultiplier = 16;
                        maxMapRow = 4;
                        switchPostPre.isAvaliable = true;
                        break;
                    }
                    case DeviceType.CP100PA:
                    {
                        maxMapRow = 10;
                        switchPostPre.isAvaliable = true;
                        break;
                    }
                }
                break;
            }

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
