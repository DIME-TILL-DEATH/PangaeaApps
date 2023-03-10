import QtQuick

import Modules

import CppObjects 1.0

Item
{
    id: main

    property string nameValue: "amp_type"
    property int value: 0
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
                check: (value==0)
                onClicked: {value=0; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==1)
                onClicked: {main.value=1; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==2)
                onClicked: {main.value=2; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==3)
                onClicked: {main.value=3; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==4)
                onClicked: {main.value=4; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==5)
                onClicked: {main.value=5; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==6)
                onClicked: {main.value=6; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==7)
                onClicked: {main.value=7; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==8)
                onClicked: {main.value=8; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==9)
                onClicked: {main.value=9; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==10)
                onClicked: {main.value=10; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==11)
                onClicked: {main.value=11; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==12)
                onClicked: {main.value=12; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==13)
                onClicked: {main.value=13; UiCore.setParameter(main.nameValue, main.value)}
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
                check: (value==14)
                onClicked: {main.value=14; UiCore.setParameter(main.nameValue, main.value)}
                onWheel: wheelChange(angleDelta);
            }
        }
    }


    function wheelChange(angleDelta)
    {
        if((angleDelta>0)&&(main.value<8))
            main.value++;
        if((angleDelta<0)&&(main.value>0))
            main.value--;
        UiCore.setParameter(main.nameValue, main.value)
    }

    Connections
    {
        target: UiCore
        function onSgSetUIParameter(nameParam, value)
        {
            if((main.nameValue.length>0)&&(nameParam===main.nameValue))
                main.value=value;
        }
    }
}
