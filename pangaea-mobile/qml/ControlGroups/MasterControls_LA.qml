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

    width:  parent.width
    height: parent.height

    property int borderSpacings : Style.mainSpacing

    signal openPresetsList()

    Row
    {
        id: _row

        width:  parent.width
        height: parent.height

        spacing: borderSpacings

        ControlButtonsGroup{
            id: _controlButtonsGroup

            width: parent.width * 4/6 - _row.spacing*2
            height: parent.height

            columnSpacing: borderSpacings
            rowSpacing: borderSpacings
        }

        ModeSpin_LA
        {
            width:  parent.width*1/6 - _row.spacing
            height: parent.height

            visible: DeviceProperties.isLa3Mode
        }

        PresetSpin_LA
        {
            width:  parent.width*1/6 -_row.spacing
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
