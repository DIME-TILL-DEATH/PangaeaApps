import QtQuick
import QtQuick.Controls

import StyleSettings
import Elements

Row{
    id: _root

    property alias text: _label.text
    property alias currentIndex: _combo.currentIndex
    property alias model: _combo.model

    signal activated(index: int)

    MLabel{
        id: _label
        width: _label.text !== "" ? parent.width/2 : 0
        // height: parent.height
        anchors.verticalCenter: parent.verticalCenter


    }

    ComboBox{
        id: _combo

        width: _label.text !== "" ? parent.width/2 : parent.width
        // height: parent.height * 0.75

        anchors.verticalCenter: parent.verticalCenter

        onActivated: (index) => {
            _root.activated(index);
        }

        contentItem: Text {
            width: _combo.width - _combo.indicator.width - _combo.spacing
            leftPadding: width/20

            text: _combo.displayText

            color: Style.currentTheme.textInverted
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
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
                 color: Style.currentTheme.textInverted

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
                context.fillStyle = Style.currentTheme.textInverted
                context.fill();
            }
        }
    }
}
