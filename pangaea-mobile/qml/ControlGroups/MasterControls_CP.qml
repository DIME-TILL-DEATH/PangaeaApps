import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15

import StyleSettings 1.0

import Elements 1.0
import Modules 1.0

import CppObjects

Item
{
    id: _root

    width:  100
    height: 100

    property int borderSpacings : Style.mainSpacing

    signal openPresetsList()

    Row
    {
        id: _row

        width:  parent.width
        height: parent.height

        spacing: borderSpacings

        BankSpin
        {
            width:  parent.width*1/6
            height: parent.height

            onOpenPresetsList: _root.openPresetsList();
        }

        ControlButtonsGroup{
            id: _controlButtonsGroup

            width: parent.width * 4/6 - _row.spacing*2
            height: parent.height

            columnSpacing: borderSpacings
            rowSpacing: borderSpacings
        }

        PresetSpin
        {
            width:  parent.width*1/6
            height: parent.height

            onOpenPresetsList: _root.openPresetsList();
        }
    }


    Connections{
        target: _controlButtonsGroup

        function onOpenPresetsList(){
            _root.openPresetsList();
        }
    }
}
