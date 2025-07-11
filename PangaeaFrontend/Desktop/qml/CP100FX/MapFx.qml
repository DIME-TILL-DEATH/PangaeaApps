import QtQuick
import QtQuick.Controls

import Elements
import StyleSettings

import CppObjects
import PangaeaBackend

Window
{
    id: main

    width: Screen.width/5
    height: Screen.width/3

    minimumWidth: Screen.width/8
    minimumHeight: Screen.width/10

    title: qsTr("Preset map")

    Rectangle{
        color: Style.mainEnabledColor
        anchors.fill: parent

        ListView{
            id: _presetListView

            width: parent.width * 0.95
            height: parent.height

            currentIndex: UiCore.currentDevice.bank * UiCore.currentDevice.maxBankCount + UiCore.currentDevice.preset

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            // boundsBehavior: Flickable.StopAtBounds
            clip: true

            ScrollBar.vertical: ScrollBar{
                active: true
                visible: true

                policy: ScrollBar.AlwaysOn

                contentItem: Rectangle {
                         width: _presetListView.width*0.025
                         implicitHeight: _presetListView.height/4
                         radius: width / 2
                         // color: "white"
                     }
            }

            spacing: 4


            delegate: Rectangle{
                width: _presetListView.width*0.95
                height: _presetListView.height/10
                radius: height/10

                border.width: index === _presetListView.currentIndex ? 4 : 2
                border.color: index === _presetListView.currentIndex ? Style.highlightColor : "white"


                color: "gray"

                MouseArea{
                    anchors.fill: parent
                    z: 10

                    onClicked: {
                        UiCore.sgQmlRequestChangePreset(UiCore.currentDevice.bank, index);
                    }
                }

                Row{
                    width: parent.width*0.95
                    height: parent.height*0.95
                    anchors.centerIn: parent

                    Column{
                        height: parent.height
                        width: parent.width/2
                        MText{
                            width: parent.width
                            height: parent.height/2
                            text: (presetNumber*1 + 1)  + ". " + presetName

                            elide: Text.ElideMiddle
                        }

                        MText{
                            width: parent.width
                            height: parent.height/2
                            text: presetComment

                            elide: Text.ElideMiddle
                        }
                    }

                    GridView{
                        id: _modulesGridView

                        height: parent.height * 0.85
                        width: parent.width/2

                        anchors.verticalCenter: parent.verticalCenter

                        cellWidth: width/7
                        cellHeight: height/2

                        model: activeModules

                        delegate: Rectangle{
                            width: _modulesGridView.cellWidth * 0.9
                            height: _modulesGridView.cellHeight * 0.9

                            color: "transparent"

                            border.width: 1
                            border.color: Style.borderOn

                            MText{
                                anchors.fill: parent
                                text: modelData

                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                font.pixelSize: parent.height/2
                            }
                        }
                    }
                }
            }



            Connections{
                target:  UiCore.currentDevice.presetListModel

                function onModelReset(){
                    // console.log("Model reset")
                    _presetListView.model = UiCore.currentDevice.presetListModel
                }
            }

            Connections
            {
                target: UiCore

                function onCurrentDeviceChanged()
                {
                    // console.log("Current device changed", UiCore.currentDevice.deviceType)

                    switch(UiCore.currentDevice.deviceType)
                    {
                        case DeviceType.UNKNOWN_DEVICE:
                        {
                            _presetListView.model = undefined
                            break;
                        }
                    }
                }

                // function onPresetSwitched(){
                //     _presetListView.model = UiCore.currentDevice.presetListModel
                //     // console.log("preset switched")
                //     // console.log(UiCore.currentDevice.presetListModel.rowCount())
                //     // console.log(_presetListView.model)
                // }
            }
        }

    }
}
