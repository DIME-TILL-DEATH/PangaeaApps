import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings 1.0
import Elements 1.0

import CppObjects
import PangaeaBackend

Item{
    height: 50
    width: 200

    property bool moduleOn
    required property ControlValue ctrlValInstance
    property alias model: _combo.model
    property alias currentIndex: _combo.currentIndex

    Column{
        anchors.fill: parent

        Item{
            id: _editValueItem


            height: parent.height/3
            width: parent.width

            Text{
                id: textValue
                anchors.fill: parent

                text: ctrlValInstance.name
                font.pixelSize: 5 * Style.dip
                font.bold: true

                color: Style.textEnabled

                horizontalAlignment: TextInput.AlignHCenter
                verticalAlignment: TextInput.AlignVCenter

                opacity: moduleOn ? 1.0 : 0.5
            }
        }

        ComboBox
        {
            id: _combo

            property bool deviceUpdatingValues: false

            opacity: moduleOn ? 1.0 : 0.5

            height: parent.height / 3
            width: parent.width * 0.9

            anchors.horizontalCenter: parent.horizontalCenter

            currentIndex: ctrlValInstance.displayValue

            onActivated:
            {
                if(!deviceUpdatingValues)
                    ctrlValInstance.displayValue = currentIndex;
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


            Connections{
                target: UiCore.currentDevice

                function onDeviceUpdatingValues()
                {
                    _combo.deviceUpdatingValues = true;
                    _combo.currentIndex = ctrlValInstance.displayValue;
                    _combo.deviceUpdatingValues = false;
                }
            }
        }
    }
}
