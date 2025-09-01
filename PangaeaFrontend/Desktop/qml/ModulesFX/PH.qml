import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings 1.0
import Elements 1.0

import CppObjects
import PangaeaBackend

BaseModule{
    id: main

    property Phaser module: _moduleLoader.selectedModuleInstance

    contentItem: Row
    {
        anchors.fill: parent


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.mix
        }


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.rate
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.center
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.width
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.feedback
        }

        // ParameterComboBox{
        //     width: main.dialWidth
        //     height: main.dialHeight

        //     anchors.verticalCenter: parent.verticalCenter

        //     ctrlValInstance: module.stages

        //     moduleOn: module.moduleEnabled

        //     model: ["4", "6", "8"]
        // }

        // Кастомный из того что значения Stages в устройстве 0, 2, 4
        Item{
            id: _stagesCombo

            width: main.dialWidth
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            property bool moduleOn: module.moduleEnabled
            property ControlValue ctrlValInstance: module.stages

            Column{
                anchors.fill: parent

                Item{
                    id: _editValueItem


                    height: parent.height/3
                    width: parent.width

                    Text{
                        id: textValue
                        anchors.fill: parent

                        text: _stagesCombo.ctrlValInstance.name
                        // font.pixelSize: 5
                        font.bold: true

                        color: Style.currentTheme.textEnabled

                        horizontalAlignment: TextInput.AlignHCenter
                        verticalAlignment: TextInput.AlignVCenter

                        opacity: module.moduleEnabled ? 1.0 : 0.5
                    }
                }

                ComboBox
                {
                    id: _combo

                    property bool deviceUpdatingValues: false

                    model: ["4", "6", "8"]

                    opacity: _stagesCombo.moduleOn ? 1.0 : 0.5

                    height: parent.height / 3
                    width: parent.width * 0.9

                    anchors.horizontalCenter: parent.horizontalCenter

                    currentIndex: _stagesCombo.ctrlValInstance.displayValue/2

                    contentItem: Text {
                        width: _combo.width - _combo.indicator.width - _combo.spacing - leftPadding
                        leftPadding: 8

                        text: _combo.displayText
                        font: _combo.font
                        color: Style.currentTheme.textCombo
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }

                    onActivated:
                    {
                        if(!_combo.deviceUpdatingValues)
                            _stagesCombo.ctrlValInstance.displayValue = currentIndex * 2;
                    }

                    delegate: ItemDelegate{
                        id: delegate

                        required property var model
                        required property int index

                        width: _combo.width
                        contentItem: Text {
                            text: delegate.model[_combo.textRole]
                            color: Style.currentTheme.textCombo

                            width: parent.width

                            font.bold: _combo.currentIndex == index

                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40

                            color: highlighted ? Style.currentTheme.highlightColor :  Style.currentTheme.backgroundColor
                        }
                        highlighted: _combo.highlightedIndex === index
                    }

                    popup: Popup {
                        y: _combo.height
                        width: _combo.width
                        height: Math.min(contentItem.implicitHeight + topMargin + bottomMargin, _combo.Window.height - topMargin - bottomMargin)

                        topMargin: 6
                        bottomMargin: 6

                        contentItem: ListView {
                            clip: true
                            implicitHeight: contentHeight
                            model: _combo.delegateModel
                            currentIndex: _combo.highlightedIndex

                            ScrollBar.vertical: ScrollBar {
                                id: sbControl
                                // size: 0.3
                                position: 0.2
                                active: true
                                orientation: Qt.Vertical

                                contentItem: Rectangle {
                                    implicitWidth: 6
                                    implicitHeight: 100
                                    radius: width / 2
                                    color: Style.currentTheme.barHigh

                                    opacity: sbControl.policy === ScrollBar.AlwaysOn || (sbControl.size < 1.0) ? 1 : 0
                                }
                            }
                        }

                        background: Rectangle {
                            border.width: 1
                            radius: 2

                            color: Style.currentTheme.backgroundColor
                        }
                    }

                    indicator: Canvas {
                        id: indicator
                        x: _combo.width - width - _combo.rightPadding
                        y: _combo.topPadding + (_combo.availableHeight - height) / 2
                        width: height
                        height: _combo.height / 3
                        contextType: "2d"

                        Connections {
                            target: _combo
                            function onPressedChanged() { indicator.requestPaint(); }
                        }

                        Connections {
                        target: Style
                            function onCurrentThemeChanged() { indicator.requestPaint(); }
                        }

                        onPaint: {
                            context.reset();
                            context.moveTo(0, 0);
                            context.lineTo(width, 0);
                            context.lineTo(width / 2, height);
                            context.closePath();
                            context.fillStyle = Style.currentTheme.textCombo
                            context.fill();
                        }
                    }


                    Connections{
                        target: UiCore.currentDevice

                        function onDeviceUpdatingValues()
                        {
                            _combo.deviceUpdatingValues = true;
                            _combo.currentIndex =  _stagesCombo.ctrlValInstance.displayValue / 2;
                            _combo.deviceUpdatingValues = false;
                        }
                    }
                }
            }
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.hpf
        }

        Rectangle{
            width: 1
            height: parent.height
        }

        ParameterSwitch{
            width: main.dialWidth
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            invertedValue: 1

            textLeft: "PRE"
            textRight: "POST"

            ctrlValInstance: module.position

            moduleOn: module.moduleEnabled
        }
    }
}
