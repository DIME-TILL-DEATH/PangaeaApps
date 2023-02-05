import QtQuick 2.7
////import QtQuick.Controls 1.5
import "../Elements/"

Item
{
    id: main

    visible: false

    property string fonColor: "#EBECEC"
    property string devColor: "#5E5971"

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
            nameValue: "bank"
            onChPreset: _uiCore.setParameter("set_preset_change", presetNom)
            enabled: main.editable
        }

        SwitchOutput
        {
            height: parent.height
            width:  row.widthWithoutSpase/15*1
            property string fonColor: "#EBECEC"
            property string devColor: "#5E5971"
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

            color: "MediumSeaGreen"

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
                font.bold: true
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
            nameValue: "preset"
            onChPreset: _uiCore.setParameter("set_preset_change", presetNom)
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
        target: _uiCore
        function onSgSetUIText(nameParam, value)
        {
            if (nameParam==="impulse_name")
            {
                impulsTxt.text=value;
            }

            if(nameParam===("port_opened"))
            {
                main.visible = true;
            }
            if(nameParam===("port_closed"))
            {
                main.visible = false;
            }
        }

        function onSgSetUIParameter(nameParam, value)
        {
            if(nameParam===("type_dev"))
            {              
                switch (value)
                {
                    case 1:
                    {
                        maxMapRow = 10;
                        switchPostPre.isAvaliable = false;
                        break;
                    }
                    case 2:
                    {
                        controlMultiplier = 16;
                        maxMapRow = 4;
                        switchPostPre.isAvaliable = false;
                        break;
                    }
                    case 3:
                    {
                        controlMultiplier = 16;
                        maxMapRow = 4;
                        switchPostPre.isAvaliable = true;
                        break;
                    }
                    case 4:
                    {
                        maxMapRow = 10;
                        switchPostPre.isAvaliable = true;
                        break;
                    }
                }
            }

            if(nameParam==="cabinet_enable")
            {
                main.irOn = value;
            }

            if(nameParam === "compare_state")
            {
                main.compareState = value;
            }
        }

        function onSgSetParameter(nameParam, value)
        {
            if(nameParam==="cabinet_enable")
            {
                main.irOn = value;
            }
        }
    }
}
