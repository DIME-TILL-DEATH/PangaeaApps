pragma ComponentBehavior: Bound

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import StyleSettings 1.0
import Elements 1.0

import PangaeaFrontend
import PangaeaBackend

Grid{
    id: _root

    height: 50
    width: 200

    property bool moduleOn: true
    required property ControlValue ctrlValInstance
    property alias model: _combo.model
    property alias currentIndex: _combo.currentIndex

    property bool isHorizontal: false

    columns: _root.isHorizontal ? 2 : 1
    rows: _root.isHorizontal ? 1 : 2

    Item{
        id: _editValueItem

        height: _root.isHorizontal ? parent.height : parent.height/3
        width: _root.isHorizontal ? parent.width * 0.5 : parent.width

        MLabel{
            id: textValue
            anchors.fill: parent

            text: _root.ctrlValInstance.name

            horizontalAlignment: _root.isHorizontal ? undefined : Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            opacity: moduleOn ? 1.0 : 0.5
        }
    }

    Item{
        height:  _root.isHorizontal ? parent.height : parent.height * 2/3
        width: _root.isHorizontal ? parent.width * 0.5 : parent.width

        ComboBox
        {
            id: _combo

            property bool deviceUpdatingValues: false

            opacity: moduleOn ? 1.0 : 0.5

            height: _root.isHorizontal ? parent.height : parent.height * 0.5
            width: _root.isHorizontal ? parent.width : parent.width * 0.9

            anchors.horizontalCenter: _root.isHorizontal ? undefined : parent.horizontalCenter
            // anchors.verticalCenter: _root.isHorizontal ? parent.verticalCenterv : undefined

            currentIndex: _root.ctrlValInstance.displayValue

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
                if(!deviceUpdatingValues)
                    _root.ctrlValInstance.displayValue = currentIndex;
            }

            background: Rectangle {
                implicitWidth: 120
                implicitHeight: 30

                // y: _combo.height * (1 - 0.8)

                border.color: Style.currentTheme.borderOn
                border.width: _combo.visualFocus ? 2 : 1
                radius: 2

                color: Style.currentTheme.backgroundColor
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

                    font.bold: _combo.currentIndex === delegate.index

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
                    _combo.currentIndex = _root.ctrlValInstance.displayValue;
                    _combo.deviceUpdatingValues = false;
                }
            }

            Connections{
                target: _root.ctrlValInstance

                function onDisplayValueChanged()
                {
                    _combo.currentIndex = _root.ctrlValInstance.displayValue;
                    // console.log("combo updating")
                    // console.log(_root.ctrlValInstance.name, _root.ctrlValInstance.displayValue)
                }
            }
        }
    }
}
