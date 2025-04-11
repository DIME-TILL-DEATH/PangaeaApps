import QtQuick

import CppObjects 1.0
import PangaeaBackend

Item
{
    id: main

    property ControlValue controlValue
    enabled: module.moduleEnabled

    Column
    {
        anchors.fill: parent
        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  01.PP 6L6"
                check: controlValue.displayValue == 0
                onClicked: controlValue.displayValue = 0
                onWheel: wheelChange(angleDelta);
            }
        }

        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  02.PP EL34"
                check: controlValue.displayValue == 1
                onClicked: controlValue.displayValue = 1
                onWheel: wheelChange(angleDelta);
            }
        }

        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  03.SE 6L6"
                check: controlValue.displayValue == 2
                onClicked: controlValue.displayValue = 2
                onWheel: wheelChange(angleDelta);
            }
        }

        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  04.SE EL34"
                check: controlValue.displayValue == 3
                onClicked: controlValue.displayValue = 3
                onWheel: wheelChange(angleDelta);
            }
        }

        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  05.AMT TC-3"
                check: controlValue.displayValue == 4
                onClicked: controlValue.displayValue = 4
                onWheel: wheelChange(angleDelta);
            }
        }

        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  06.CALIF"
                check: controlValue.displayValue == 5
                onClicked: controlValue.displayValue = 5
                onWheel: wheelChange(angleDelta);
            }
        }

        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  07.BRIT M"
                check: controlValue.displayValue == 6
                onClicked: controlValue.displayValue = 6
                onWheel: wheelChange(angleDelta);
            }
        }

        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  08.BRIT L"
                check: controlValue.displayValue == 7
                onClicked: controlValue.displayValue = 7
                onWheel: wheelChange(angleDelta);
            }
        }
        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  09.DEFAULT"
                check: controlValue.displayValue == 8
                onClicked: controlValue.displayValue = 8
                onWheel: wheelChange(angleDelta);
            }
        }
        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  10.CALIF MOD"
                check: controlValue.displayValue == 9
                onClicked: controlValue.displayValue = 9
                onWheel: wheelChange(angleDelta);
            }
        }
        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  11.CALIF VINT"
                check: controlValue.displayValue == 10
                onClicked: controlValue.displayValue = 10
                onWheel: wheelChange(angleDelta);
            }
        }
        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  12.PVH 01"
                check: controlValue.displayValue == 11
                onClicked: controlValue.displayValue = 11
                onWheel: wheelChange(angleDelta);
            }
        }
        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  13.PVH 02"
                check: controlValue.displayValue == 12
                onClicked: controlValue.displayValue = 12
                onWheel: wheelChange(angleDelta);
            }
        }
        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  14.PVH 03"
                check: controlValue.displayValue == 13
                onClicked: controlValue.displayValue = 13
                onWheel: wheelChange(angleDelta);
            }
        }
        Item
        {
            width:  parent.width
            height: parent.height/15
            CheckText
            {
                leftText: true
                text:  "  15.PVH 04"
                check: controlValue.displayValue == 14
                onClicked: controlValue.displayValue =14
                onWheel: wheelChange(angleDelta);
            }
        }
    }


    function wheelChange(angleDelta)
    {
        if((angleDelta>0)&&(main.value<8))
            controlValue.displayValue++;
        if((angleDelta<0)&&(main.value>0))
            controlValue.displayValue--;
    }
}
