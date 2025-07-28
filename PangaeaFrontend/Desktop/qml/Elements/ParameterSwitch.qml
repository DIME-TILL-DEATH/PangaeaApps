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

    property alias textLeft: _textLeft.text
    property alias textRight: _textRight.text

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

        Switch2
        {
            id: _switch

            property bool deviceUpdatingValues: false

            opacity: moduleOn ? 1.0 : 0.5

            height: parent.height / 3
            width: parent.width * 0.9

            anchors.horizontalCenter: parent.horizontalCenter

            value: ctrlValInstance.displayValue

            onChValue:
            {
                if(!deviceUpdatingValues)
                    ctrlValInstance.displayValue = value;
            }


            Connections{
                target: UiCore.currentDevice

                function onDeviceUpdatingValues()
                {
                    _switch.deviceUpdatingValues = true;
                    _switch.value = ctrlValInstance.displayValue;
                    _switch.deviceUpdatingValues = false;
                }
            }
        }

        Item {
            height: parent.height / 3
            width: parent.width * 0.75

            anchors.horizontalCenter: parent.horizontalCenter

            MText
            {
                id: _textLeft

                anchors.left: parent.left

                horizontalAlignment: TextInput.AlignHCenter
                verticalAlignment: TextInput.AlignVCenter

                text: "POST"

                opacity: moduleOn ? 1.0 : 0.5
                color: (module.moduleEnabled) ? Style.textEnabled: "darkgrey"
                font.pixelSize: parent.width/8
            }

            MText
            {
                id: _textRight

                anchors.right: parent.right

                horizontalAlignment: TextInput.AlignHCenter
                verticalAlignment: TextInput.AlignVCenter

                text: "PRE"

                opacity: moduleOn ? 1.0 : 0.5
                color: (module.moduleEnabled) ? Style.textEnabled: "darkgrey"
                font.pixelSize: parent.width/8
            }
        }
    }
}
