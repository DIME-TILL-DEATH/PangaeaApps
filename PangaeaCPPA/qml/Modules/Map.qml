import QtQuick
import QtQuick.Controls

import Elements
import StyleSettings

import CppObjects
import CppEnums

Item
{
    id: main

    anchors.fill: parent

    property int maxMapRow: 10
    property int currentBank: 0
    property int currentPreset: 0

    Rectangle
    {
        anchors.fill: parent
        color: Style.mainEnabledColor

        Row
        {
            height: parent.height/1.1
            width:  parent.width/1.1
            anchors.centerIn: parent

            Column
            {
                height: parent.height
                width:  parent.width/11*1
                Item
                {
                    height: parent.height/11*1
                    width:  parent.width
                }

                MapHeadBank
                {
                    id: mapHeadBank

                    height: parent.height/11*10
                    width:  parent.width

                    maxPresetBank: main.maxMapRow
                }
            }

            Column
            {
                height: parent.height
                width:  parent.width/11*10

                MapHeadPreset
                {
                    id: mapHeadPreset

                    width:  parent.width
                    height: parent.height/11*1

                    maxPresetBank: main.maxMapRow
                }

                GridView{
                    id: _mapGrid

                    width: parent.width
                    height: parent.height/11*10

                    cellWidth: width/maxMapRow
                    cellHeight: height/maxMapRow

                    interactive: false

                    currentIndex: currentBank*maxMapRow + currentPreset

                    highlight: Rectangle{
                        color: "gray"
                        radius: _mapGrid.cellWidth/2
                        opacity: 0.4

                        SequentialAnimation on color{
                            running: true
                            loops: Animation.Infinite
                            alwaysRunToEnd: true
                            ColorAnimation {from: "salmon"; to: "gray"; duration: 1000}
                            ColorAnimation {from: "gray"; to: "salmon"; duration: 1000}
                        }

                    }

                    model: PresetListModel//_presetListModel

                    delegate: MapDelegate{
                        id: _root

                        width: _mapGrid.cellWidth
                        height: _mapGrid.cellHeight

                        currentIndex: _mapGrid.currentIndex
                    }
                }
            }
        }
    }


    Connections
    {
        target: UiCore

        function onSgSetUiDeviceParameter(paramType, value)
        {
            switch(paramType)
            {
            case DeviceParameter.MAP_SIZE:
            {
                switch (value)
                {
                    case DeviceType.CP100: maxMapRow = 10;  break;
                    case DeviceType.CP100PA: maxMapRow = 10;  break;
                    case DeviceType.CP16: maxMapRow = 4; break;
                    case DeviceType.CP16PA: maxMapRow = 4; break;
                }
                console.log("maxMaprow, hard:", maxMapRow);
                maxMapRow = value;
                console.log("maxMaprow, soft:", maxMapRow);
                break;
            }
            case DeviceParameter.BANK:
            {
                currentBank = value;
                mapHeadBank.curVal = value
                break;
            }

            case DeviceParameter.PRESET:
            {
                currentPreset = value;
                mapHeadPreset.curVal = value
            }
            }
        }
    }
}
