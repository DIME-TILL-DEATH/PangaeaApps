import QtQuick 2.15
import QtQuick.Controls 2.0
import Qt5Compat.GraphicalEffects

import CustomOverlays 1.0
import StyleSettings 1.0

import CppObjects

Row{
    id: _root

    width: parent.width
    height: parent.height/12

    required property ControlValue ctrlValInstance

    property string name: ctrlValInstance.name
    property string units: ctrlValInstance.units
    property int precision: 0

    property real fontSize: 10 * Style.dip

    property bool moduleOn: true

    property bool inverse: false
    property bool bottomLineEnabled: true

    onCtrlValInstanceChanged: {
        _slider.fromLog = Math.log10(ctrlValInstance.minDisplayValue)
        _slider.toLog = Math.log10(ctrlValInstance.maxDisplayValue)
        _slider.value = (Math.log10(ctrlValInstance.displayValue) - _slider.fromLog)/_slider.scale + _slider.from
    }

    Slider
    {
        id: _slider

        width: parent.width - _editValueItem.width
        height: parent.height

        from: 0
        to: 10

        property real fromLog: Math.log10(ctrlValInstance.minDisplayValue);
        property real toLog: Math.log10(ctrlValInstance.maxDisplayValue);

        property real scale: (toLog-fromLog) / (to-from);

        onMoved:
        {
            ctrlValInstance.displayValue = Math.pow(10, fromLog + scale*(value-from));
            ctrlValInstance.isModified = true;
        }

        leftPadding: 0
        rightPadding: 0

        Connections{
            target: ctrlValInstance

            function onDisplayValueChanged()
            {
                _slider.value = (Math.log10(ctrlValInstance.displayValue) - _slider.fromLog)/_slider.scale + _slider.from
            }
        }

        ToolTip
        {
            id: _toolTip

            parent: _slider.handle
            visible: _slider.pressed
            text: ctrlValInstance.displayValue.toFixed(_root.precision)
            y : -40
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


        EditValueDialog{
            id: _valueDialog

            height: Screen.height/4

            controlValue: ctrlValInstance
            precision:  _root.precision
        }

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
        id: _separator

        width: Style.mainSpacing
        height: parent.height

        color: moduleOn ? Style.currentTheme.colorBorderOn : Style.currentTheme.colorTextDisabled
    }

    Item{
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
        width: parent.width * 1/5 - _separator.width //- Style.mainSpacing * 2

        TextInput{
            id: textValue
            anchors.fill: parent

            text: ctrlValInstance.displayValue.toFixed(precision) + " " + units + " "
            font.pixelSize: fontSize
            font.bold: true

            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            color: moduleOn ? Style.colorText : Style.currentTheme.colorTextDisabled
            z:1
        }
    }
}
