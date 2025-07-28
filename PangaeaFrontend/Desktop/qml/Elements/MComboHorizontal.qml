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
