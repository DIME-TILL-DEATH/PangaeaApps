import QtQuick 2.15
import QtQuick.Controls 2.0
import Qt5Compat.GraphicalEffects

import StyleSettings 1.0

import CppObjects

Item{
    id: _root

    implicitWidth: parent.width
    implicitHeight: parent.height/12

    required property ControlValue ctrlValInstance

    property string name: ctrlValInstance.name
    property string units: ctrlValInstance.units
    property int precision: 0

    property bool moduleOn: true

    property bool inverse: false
    property bool bottomLineEnabled: true

    onCtrlValInstanceChanged: {
        _slider.from = ctrlValInstance.minDisplayValue
        _slider.to = ctrlValInstance.maxDisplayValue
    }

    Slider
    {
        id: _slider

        width: parent.width - _editValueItem.width
        height: parent.height


        from: ctrlValInstance.minDisplayValue
        to: ctrlValInstance.maxDisplayValue
        value: ctrlValInstance.displayValue


        onMoved:
        {
            ctrlValInstance.displayValue = _slider.value;
            ctrlValInstance.isModified = true;

            _longPressTimer.restart()
        }

        leftPadding: 0
        rightPadding: 0

        ToolTip
        {
            id: _toolTip

            parent: _slider.handle
            visible: _slider.pressed
            text: _slider.value.toFixed(_root.precision)
            y : -40

            // timeout: 5000
            // MouseArea{
            //     anchors.fill: parent
            //     onClicked: {
            //         _valueDialog.open()
            //     }
            // }
        }

        background: Rectangle
        {
            x: _slider.leftPadding
            width: _slider.availableWidth
            height: _slider.height
            color: Style.colorFon

            Rectangle
            {
                id: blueRect
                width: _slider.inverse ? (_slider.width-_slider.handle.x + _slider.handle.width/2)
                                    :(_slider.visualPosition==0? 0 : _slider.handle.x+_slider.handle.width/2)
                x: _slider.inverse?(_slider.handle.x):(_slider.x)

                height: parent.height
                gradient: Gradient{
                    orientation: Gradient.Horizontal
                    GradientStop{
                        position: _slider.inverse ? 1.0 : 1.0-_slider.visualPosition;
                        color: Style.colorModul}
                    GradientStop{position: _slider.inverse? _slider.visualPosition - 1.0 : 1.0; color: moduleOn ? Style.currentTheme.colorModulOn : Style.currentTheme.colorModulOff}
                }
                layer.enabled: _slider.hovered | _slider.pressed
            }

            Rectangle
            {
                y: _slider.height-1
                width: parent.width
                height: 1
                color: "#999999"
                z: 1
                visible: bottomLineEnabled
            }
        }

        handle: Rectangle
        {
            x: _slider.leftPadding + _slider.visualPosition * (_slider.availableWidth - width)

            color:  Style.currentTheme.colorSlider

            width: 2
            height: _slider.height


            layer.enabled: _slider.hovered | _slider.pressed
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

        //----------------------------Manual enter dialog----------------------
        onPressedChanged:
        {
            if(pressed)
            {
                // _toolTip.show(ctrlValInstance.displayValue.toFixed(precision), 1000)

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
            interval: 1000
            repeat: false
            onTriggered: {
                _valueDialog.open()
            }
        }

        EditValueDialog{
            id: _valueDialog

            height: Screen.height/4

            controlValue: ctrlValInstance
            precision:  _root.precision
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

    Rectangle{
        id: _editValueItem

        MouseArea{
            id: _editMa

            anchors.fill: parent
            z: parent.z+5

            onClicked: {
                _valueDialog.open()
            }
        }

        opacity: _editMa.containsPress ? 0.5 : 1

        height: parent.height
        width: parent.width * 1/5 - Style.mainSpacing * 2

        anchors.right: parent.right
        color: "transparent"
        border.width: 1
        border.color: moduleOn ? Style.colorText : Style.currentTheme.colorTextDisabled
        TextInput{
            id: textValue
            anchors.fill: parent

            text: _slider.value.toFixed(precision) //+ " " + units + " "
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
                decimals: _root.precision

                locale: "en"
            }

            // на яндекс клавиатуре не выдаёт сигнал
            // onAccepted: {
            // }
            // onEditingFinished:{
            //     if(parseFloat(text) > _root.ctrlValInstance.maxDisplayValue) contentText = _root.ctrlValInstance.maxDisplayValue
            //     if(parseFloat(text) < _root.ctrlValInstance.minDisplayValue) contentText = _root.ctrlValInstance.minDisplayValue

            //     _root.ctrlValInstance.displayValue = text;
            //     _root.ctrlValInstance.isModified = true;
            // }

            onActiveFocusChanged: function(focusState){
                if(parseFloat(text) > _root.ctrlValInstance.maxDisplayValue) text = _root.ctrlValInstance.maxDisplayValue
                if(parseFloat(text) < _root.ctrlValInstance.minDisplayValue) text = _root.ctrlValInstance.minDisplayValue

                _root.ctrlValInstance.displayValue = text;
                _root.ctrlValInstance.isModified = true;
            }
        }
    }
}
