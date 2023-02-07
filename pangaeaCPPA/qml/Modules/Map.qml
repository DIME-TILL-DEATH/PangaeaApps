import QtQuick
import QtQuick.Controls

import Elements
import StyleSettings

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

                    model: _presetListModel

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
        target: _uiCore
        function onSgSetUIParameter(nameParam, value)
        {
            if(nameParam === "bank")
            {
                currentBank = value;
                mapHeadBank.curVal = value
            }

            if(nameParam === "preset")
            {
                currentPreset = value;
                mapHeadPreset.curVal = value
            }

            if(nameParam===("type_dev"))
            {
                switch (value)
                {
                case 1: maxMapRow = 10;  break;
                case 2: maxMapRow = 4;  break;
                case 3: maxMapRow = 4; break;
                case 4: maxMapRow = 10; break;
                }
            }
        }
    }
}
