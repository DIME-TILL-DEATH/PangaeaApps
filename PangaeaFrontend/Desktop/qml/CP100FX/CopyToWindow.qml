import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

import StyleSettings

import Elements
import Layouts

import CP100FX
import CppObjects
import PangaeaBackend

Window{
    id: _root

    width: Screen.width * 0.2
    height: width * 0.75

    minimumWidth: Screen.width * 0.3 * 1
    minimumHeight: Screen.width * 0.3 * 0.5 * 1.25
    maximumWidth: Screen.width * 0.3 * 1
    maximumHeight: Screen.width * 0.3 * 0.5 * 1.75

    color: Style.currentTheme.mainEnabledColor

    title: qsTr("Copy preset")

    Row{
        height: parent.height * 0.85
        width: parent.width * 0.9

        anchors.horizontalCenter: parent.horizontalCenter

        anchors.topMargin: parent.height/30

        spacing: 4
        Rectangle{
            id: _rectSelect

            color: "transparent"

            width: parent.width * 2/3
            height: parent.height

            border.width: 1
            border.color: Style.currentTheme.borderOn

            GridLayout{
                id: _selectLayout
                anchors.fill: parent

                columns:2

                Item{
                    Layout.columnSpan: 2

                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height / 10
                    MText{
                        text: qsTr("Select Items to be copied:")
                        anchors.fill: parent

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:   Text.AlignVCenter
                    }
                }

                MCheckBox{
                    text: qsTr("Preset name")

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: qsTr("Preset comment")

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: qsTr("Controllers")

                    Layout.columnSpan: 2

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "RF"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "GT"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "CM"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "PR"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "PA"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "IR"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "EQ"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "FL"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "PH"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "CH"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "DL"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "ER"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "RV"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: "TR"

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                MCheckBox{
                    text: qsTr("Preset volume")

                    Layout.columnSpan: 2

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }

        Rectangle{
            color: "transparent"

            width: parent.width * 1/3
            height: parent.height

            border.width: 1
            border.color: Style.currentTheme.borderOn

            Column{
                anchors.fill: parent

                MText{
                    height: parent.height/9
                    width: parent.width
                    text: qsTr("Target preset:")

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment:   Text.AlignVCenter
                }

                Tumbler{
                    id: _presetTumbler

                    height: parent.height * 6/9
                    width: parent.width

                    model: UiCore.currentDevice.strPresetNumbers
                    currentIndex: UiCore.currentDevice.preset

                    visibleItemCount: 3
                    delegate: Text
                    {
                        id: _delegateTumblerText

                        text: modelData

                        color: Style.currentTheme.textEnabled
                        font.pixelSize: Math.min(parent.height*0.6, parent.width*0.6)

                        opacity: 1.0 - Math.abs(Tumbler.displacement) / (_presetTumbler.visibleItemCount / 2)
                        scale: 1.0 - Math.abs(Tumbler.displacement) * 0.1

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:   Text.AlignVCenter
                        font.bold: true
                        font.family: "Arial Black"

                        transform: [
                            Rotation{
                                origin.x: _delegateTumblerText.width / 2
                                origin.y: _delegateTumblerText.height / 2
                                axis {x:1; y:0; z:0}

                                angle: Math.abs(_delegateTumblerText.Tumbler.displacement) * 45
                            }

                        ]
                    }
                }

                MButton{
                    text: qsTr("Select All")

                    height: parent.height/9
                    width: parent.width

                    onClicked: {
                        const checkBoxes = Array.from(_selectLayout.children).filter(child => child instanceof MCheckBox)
                        for(var i=0; i<checkBoxes.length; i++)
                        {
                            checkBoxes[i].checked = true
                        }
                    }
                }

                MButton{
                    text: qsTr("Deselect All")

                    height: parent.height/9
                    width: parent.width

                    onClicked: {
                        const checkBoxes = Array.from(_selectLayout.children).filter(child => child instanceof MCheckBox)
                        for(var i=0; i<checkBoxes.length; i++)
                        {
                            checkBoxes[i].checked = false
                        }
                    }
                }
            }
        }
    }

    MButton{
        width: parent.width * 0.5
        height: parent.height * 0.1

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom

        text: qsTr("COPY ELEMENTS")

        onClicked: {
            const checkBoxes = Array.from(_selectLayout.children).filter(child => child instanceof MCheckBox)

            var selectionMask = []
            for(var i=0; i<checkBoxes.length; i++)
            {
                selectionMask.push(checkBoxes[i].checked === true ? 1 : 0)
            }

            UiCore.currentDevice.copyPresetTo(_presetTumbler.currentIndex, selectionMask)
        }
    }
}
