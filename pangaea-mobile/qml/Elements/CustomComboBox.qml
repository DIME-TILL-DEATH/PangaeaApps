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

    Row{
        anchors.fill: parent

        MComboBox
        {
            id: _comboBox

            property bool deviceUpdatingValues: false

            on: moduleOn

            height: parent.height
            width: parent.width - _editValueItem.width

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

        Rectangle{
            id: _separator

            width: Style.mainSpacing
            height: parent.height

            color: moduleOn ? Style.currentTheme.colorBorderOn
                             : Style.currentTheme.colorTextDisabled
        }

        Item{
            id: _editValueItem


            height: parent.height
            width: parent.width * 1/5 - _separator.width

            Text{
                id: textValue
                anchors.fill: parent

                text: _comboBox.currentIndex + 1 + " " + ctrlValInstance.units + " "
                font.pixelSize: 10 * Style.dip
                font.bold: true

                horizontalAlignment: TextInput.AlignHCenter
                verticalAlignment: TextInput.AlignVCenter
                color: moduleOn ? Style.colorText : Style.currentTheme.colorTextDisabled
                z:1
            }
        }
    }
}
