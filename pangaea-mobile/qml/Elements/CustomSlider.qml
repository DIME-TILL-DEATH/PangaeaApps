import QtQuick 2.15
import QtQuick.Controls 2.0
import Qt5Compat.GraphicalEffects

import StyleSettings 1.0

import CppObjects

Slider
{
    id: root

    implicitWidth: parent.width
    implicitHeight: parent.height/12

    required property ControlValue ctrlValInstance

    property string name: ctrlValInstance.name
    property string units: ctrlValInstance.units
    property int precision: 0

    from: ctrlValInstance.minDisplayValue
    to: ctrlValInstance.maxDisplayValue
    value: ctrlValInstance.displayValue

    property bool moduleOn: true

    property bool inverse: false
    property bool bottomLineEnabled: true

    onMoved:
    {
        ctrlValInstance.displayValue = root.value;
        ctrlValInstance.isModified = true;
    }

    leftPadding: 0
    rightPadding: 0

    ToolTip
    {
        id: _toolTip

        parent: root.handle
        visible: false//root.pressed
        text: value.toFixed(precision)
        y : -40

        timeout: 5000
        MouseArea{
            anchors.fill: parent
            onClicked: {
                _valueDialog.open()
            }
        }
    }

    background: Rectangle
    {
        x: root.leftPadding
        width: root.availableWidth
        height: root.height
        color: Style.colorFon

        Rectangle
        {
            id: blueRect
            width: root.inverse?(root.width-root.handle.x+root.handle.width/2):(root.visualPosition==0?0:root.handle.x+root.handle.width/2)
            x: root.inverse?(root.handle.x):(root.x)

            height: parent.height
            gradient: Gradient{
                orientation: Gradient.Horizontal
                GradientStop{
                    position: root.inverse ? 1.0 : 1.0-root.visualPosition;
                    color: Style.colorModul}
                GradientStop{position: root.inverse? root.visualPosition - 1.0 : 1.0; color: moduleOn ? Style.currentTheme.colorModulOn : Style.currentTheme.colorModulOff}
            }
            layer.enabled: root.hovered | root.pressed
        }

        Rectangle
        {
            y: root.height-1
            width: parent.width
            height: 1
            color: "#999999"
            z: 1
            visible: bottomLineEnabled
        }
    }

    handle: Rectangle
    {
        x: root.leftPadding + root.visualPosition * (root.availableWidth - width)

        color:  Style.currentTheme.colorSlider

        width: 2
        height: root.height


        layer.enabled: root.hovered | root.pressed
        layer.effect: DropShadow
        {
            transparentBorder: true
            color: "#00A0E3"
            samples: 10
        }
    }

    MText
    {
        id: modulName
        anchors.fill: parent
        text: ctrlValInstance.isModified ? ("  "+ name +"*") : ("  " + name)
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        leftPadding: 4
        color: moduleOn ? Style.colorText : Style.currentTheme.colorTextDisabled
        z:1
    }

    // MText
    // {
    //     id: textValue
    //     anchors.fill: parent
    //     text: value.toFixed(precision) + " " + units + " "
    //     horizontalAlignment: Text.AlignRight
    //     verticalAlignment: Text.AlignVCenter
    //     color: moduleOn ? Style.currentTheme.colorTextEnabled : Style.currentTheme.colorTextDisabled
    //     z:1
    // }


    Rectangle{
        height: parent.height
        width: parent.width * 0.2

        anchors.right: parent.right
        color: "transparent"
        border.width: 1
        border.color: moduleOn ? Style.colorText : Style.currentTheme.colorTextDisabled
        TextInput{
            id: textValue
            anchors.fill: parent

            text: value.toFixed(precision) //+ " " + units + " "
            font.pixelSize: 10 * Style.dip
            font.bold: true

            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            // color: moduleOn ? Style.colorText : Style.currentTheme.colorTextDisabled
            z:1

            inputMethodHints: Qt.ImhFormattedNumbersOnly

            color: acceptableInput ? (moduleOn ? Style.colorText : Style.currentTheme.colorTextDisabled)
                                   : "red"

            validator: DoubleValidator{
                bottom: ctrlValInstance.minDisplayValue
                top: ctrlValInstance.maxDisplayValue
                decimals: precision

                locale: "en"
            }

            // на яндекс клавиатуре не выдаёт сигнал
            // onAccepted: {
            // }
            onEditingFinished:{
                if(parseFloat(text) > root.ctrlValInstance.maxDisplayValue) contentText = root.ctrlValInstance.maxDisplayValue
                if(parseFloat(text) < root.ctrlValInstance.minDisplayValue) contentText = root.ctrlValInstance.minDisplayValue

                root.ctrlValInstance.displayValue = text;
                root.ctrlValInstance.isModified = true;
            }
        }
    }

    //----------------------------Manual enter dialog----------------------
    onPressedChanged:
    {
        if(pressed)
        {
            _toolTip.show(ctrlValInstance.displayValue.toFixed(precision), 1000)

            if(_resetDoubleClickTimer.pressedOnce) _valueDialog.open()
            else _resetDoubleClickTimer.pressedOnce = true
            _longPressTimer.restart()
        }
        else
        {
            _resetDoubleClickTimer.restart()
            _longPressTimer.running = false
        }
    }

    Timer{
        id: _resetDoubleClickTimer

        property bool pressedOnce: false

        interval: 100
        repeat: false
        onTriggered:
        {
            pressedOnce = false
        }
    }

    Timer{
        id: _longPressTimer
        interval: 500
        repeat: false
        onTriggered: {
            _valueDialog.open()
        }
    }

    EditValueDialog{
        id: _valueDialog

        height: Screen.height/4

        controlValue: ctrlValInstance
        precision:  root.precision
    }
    //---------------------------------------------------------------------

    Connections
    {
        target: UiCore.currentDevice

        function onDeviceUpdatingValues()
        {
            ctrlValInstance.isModified = false;
        }
    }
}
