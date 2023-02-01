import QtQuick 2.0
import QtQuick.Controls 2.0
//import QtGraphicalEffects 1.0
import Qt5Compat.GraphicalEffects

import StyleSettings 1.0

Slider
{
    id: root
    orientation: Qt.Vertical

    property string nameValue: "DIAL"
    property string nameParam: "master_volume"
    property string units: " "

    property color colorHandle
    property color colorSlider

    property int valueMin:  0
    property int valueMax:  31

    property int dispMin:   0
    property int dispMax:   31

    property int x1val: 0
    property int x2val: 100

    property bool softUpdate: false

    //    property bool inverse: false
    property bool editabled: false

    property bool botLineEn: true

    property int     maximumValue: 1
    property int     minimumValue: 2


    property double kDisp: -((dispMax-dispMin)/(x1val-x2val))
    property double bDisp: -(((x2val*dispMin)-(x1val*dispMax))/(x1val-x2val))
    property double yDisp: Math.round(kDisp * root.value * 100 + bDisp)

    property double kVal: -((valueMax-valueMin)/(x1val-x2val))
    property double bVal: -(((x2val*valueMin)-(x1val*valueMax))/(x1val-x2val))
    property double yVal: Math.round(kVal * root.value * 100 + bVal)

    value: 0.5

    leftPadding: 0
    rightPadding: 0

    ToolTip
    {
        parent: root.handle
        visible: root.pressed
        text: yDisp
        scale: 1.2
        y : -40

    }

    background: Item
    {
        id: regulator
        anchors.fill: parent

        Rectangle
        {
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height
            width: 1
            color: "grey"
        }

        Rectangle
        {
            id: recTop
            visible: yDisp>0
            anchors.bottom: parent.verticalCenter
            color: Style.currentTheme.colorSlider
            width: parent.width
            height: parent.height*yDisp/35
            border.color: Style.colorFon
            border.width: 1
            opacity: 0.7
        }
        Rectangle
        {
            id: recBoot
            visible: yDisp<0
            anchors.top: parent.verticalCenter
            color: Style.currentTheme.colorSlider
            width: parent.width
            height: parent.height*(-yDisp)/35
            border.color: Style.colorFon
            border.width: 1
            opacity: 0.7
        }
    }

    handle: Rectangle
    {

        y: root.topPadding + root.availableHeight / 2 - height / 2 - regulator.height*yDisp/35


        implicitWidth:  root.width  //40
        implicitHeight: root.width  //15 //root.height
        radius: root.width/2
        color: colorHandle



        layer.enabled: root.hovered | root.pressed
        layer.effect: DropShadow
        {
            transparentBorder: true
            color: "#00A0E3"
            samples: 10
        }
    }

    onValueChanged:
    {
        if(!softUpdate)
        {
            _uiCore.setParameter(nameParam, Math.round(kVal * root.value * 100 + bVal));
            editabled = true;
        }
        else
        {
            editabled = false;
        }
    }

    Connections
    {
        target: _uiCore
        function onSgSetUIParameter(nameParam, nameValue)
        {
            if((root.nameParam.length>0) && nameParam.localeCompare(root.nameParam) === 0)
            {
                softUpdate = true;
                root.value = nameValue/(root.valueMax-root.valueMin);
                softUpdate = false;
            }

            if( nameParam === "presetEdit" )
            {
                main.edit = nameValue;
            }
        }
    }
}
