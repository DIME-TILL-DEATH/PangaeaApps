import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements
import StyleSettings

Column{
    id: _root

    height: parent.height
    width: parent.width / 8

    property alias text: _textLabel.text
    property alias currentIndex: _combo.currentIndex
    property alias model: _combo.model
    property alias comboText: _combo.currentText

    signal activated(index: int)

    MLabel{
        id: _textLabel

        height: parent.height/3
        width: parent.width

        visible: text !== ""

        text: "Controller"
        font.pixelSize: 5 * Style.dip
        font.bold: true

        elide: Text.ElideMiddle

        color: Style.textEnabled

        horizontalAlignment: TextInput.AlignHCenter
        verticalAlignment: TextInput.AlignVCenter
    }

    ComboBox
    {
        id: _combo


        height: parent.height / 3
        width: parent.width * 0.9

        anchors.horizontalCenter: parent.horizontalCenter

        currentIndex: 0

        onActivated: (index) => {
            _root.activated(index);
        }

        background: Rectangle {
             implicitWidth: 120
             implicitHeight: 30

             // y: _combo.height * (1 - 0.8)

             border.color: Style.borderOn
             border.width: _combo.visualFocus ? 2 : 1
             radius: 2

             color: Style.backgroundColor
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
                        color: Style.barHigh

                        opacity: sbControl.policy === ScrollBar.AlwaysOn || (sbControl.size < 1.0) ? 1 : 0
                    }
                }
            }

            background: Rectangle {
                border.width: 1
                radius: 2

                color: Style.backgroundColor
            }
        }
    }
}
