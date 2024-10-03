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

    property int maxMapRow: Math.max(DeviceProperties.banksList.length, DeviceProperties.presetsList.length)

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
                }

                GridView{
                    id: _mapGrid

                    width: parent.width
                    height: parent.height/11*10

                    cellWidth: width/maxMapRow
                    cellHeight: height/maxMapRow

                    interactive: false

                    currentIndex: DeviceProperties.bank*maxMapRow + DeviceProperties.preset

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

                    model: PresetListModel

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
}
