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
    property alias model: _comboBox.model
    property alias currentIndex: _comboBox.currentIndex

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
                font.pixelSize: 10 * Style.dip
                font.bold: true

                color: Style.textEnabled

                horizontalAlignment: TextInput.AlignHCenter
                verticalAlignment: TextInput.AlignVCenter

                opacity: moduleOn ? 1.0 : 0.5
            }
        }

        ComboBox
        {
            id: _comboBox

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


            Connections{
                target: UiCore.currentDevice

                function onDeviceUpdatingValues()
                {
                    _comboBox.deviceUpdatingValues = true;
                    _comboBox.currentIndex = ctrlValInstance.displayValue;
                    _comboBox.deviceUpdatingValues = false;
                }
            }
        }
    }
}
