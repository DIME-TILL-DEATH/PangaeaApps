import QtQuick 2.15

import StyleSettings 1.0
import Elements 1.0

import PangaeaBackend

Item{
    id: _root

    height: 50
    width: 200

    property bool moduleOn: true
    required property ControlValue ctrlValInstance

    property alias textLeft: _textLeft.text
    property alias textRight: _textRight.text

    property alias invertedValue: _switch.invertedValue
    property alias position: _switch.value

    property bool isHorizontal: false



    Item{
        id: _editValueItem


        height: _root.isHorizontal ? parent.height : parent.height/3
        width: _root.isHorizontal ? parent.width * 0.5 : parent.width

        MLabel{
            id: textValue
            anchors.fill: parent

            text: ctrlValInstance.name

            horizontalAlignment: _root.isHorizontal ? Text.AlignLeft : Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            opacity: _root.moduleOn ? 1.0 : 0.5
        }
    }

    Switch2
    {
        id: _switch

        property bool deviceUpdatingValues: false

        height:  _root.isHorizontal ? parent.height * 0.5 : parent.height / 3
        width: _root.isHorizontal ? parent.width * 0.3 : parent.width //* 0.9

        anchors.top: _root.isHorizontal ? undefined : _editValueItem.bottom
        anchors.verticalCenter: _root.isHorizontal ? parent.verticalCenter : undefined
        anchors.left: _root.isHorizontal ? _editValueItem.right : undefined


        isHorizontal: _root.isHorizontal

        value: ctrlValInstance.displayValue

        opacity: _root.moduleOn ? 1.0 : 0.5

        onChValue: value => {
            if(!deviceUpdatingValues)
                ctrlValInstance.displayValue = value;
        }
    }

    Item {
        height: parent.height / 3
        width: parent.width * 0.75

        visible: !_root.isHorizontal

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _switch.bottom

        MText
        {
            id: _textLeft

            anchors.left: parent.left

            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter

            text: "OFF"

            opacity: moduleOn ? 1.0 : 0.5
            color: moduleOn ? Style.currentTheme.textEnabled: Style.currentTheme.textDisabled
            font.pixelSize: parent.width/8
        }

        MText
        {
            id: _textRight

            anchors.right: parent.right

            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter

            text: "ON"

            opacity: moduleOn ? 1.0 : 0.5
            color: moduleOn ? Style.currentTheme.textEnabled: Style.currentTheme.textDisabled
            font.pixelSize: parent.width/8
        }
    }
}
