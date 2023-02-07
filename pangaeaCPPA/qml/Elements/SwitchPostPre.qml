import QtQuick

import StyleSettings
import CppObjects 1.0

Item
{
    id: main

    property string nameValue: "eq_pre"

    property bool isAvaliable: true
    
    Rectangle
    {
        anchors.fill:  parent
        color: main.enabled  ? Style.mainEnabledColor : Style.mainDisabledColor
    }

    Column
    {
        anchors.fill:  parent

        Item
        {
            width: parent.width
            height: parent.height/100*5
        }

        Switch2
        {
            id:    mSwitch

            width: parent.width
            height: parent.height/100*40

            onChValue: UiCore.setParameter(main.nameValue, mSwitch.value)

            visible: isAvaliable
        }

        Row
        {
            width: parent.width
            height: parent.height/100*20
            CheckText
            {
                width: parent.width/2
                height: parent.height
                text: "POST"
                check: mSwitch.value==0

                visible: isAvaliable
                onClicked:
                {
                    mSwitch.value = 0;
                    UiCore.setParameter(main.nameValue, mSwitch.value)
                }
            }

            CheckText
            {
                width: parent.width/2
                height: parent.height
                text: "PRE"
                check: mSwitch.value==1

                visible: isAvaliable
                onClicked:
                {
                    mSwitch.value = 1;
                    UiCore.setParameter(main.nameValue, mSwitch.value)
                }
            }
        }

        Item
        {
            width: parent.width
            height: parent.height/100*20
            MText
            {
                anchors.fill: parent
                text: "EQ"
                color: Style.backgroundColor
                font.family: "Arial Black"
                font.bold: true
                font.pixelSize: parent.height/1.5
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop

                visible: isAvaliable
            }
        }

        Rectangle
        {
            width: parent.width
            height: parent.height/100*15
            color: main.enabled ? Style.mainEnabledColor : Style.mainDisabledColor
        }
    }

    //TODO switch аналогичен IR, но работает по-разному!
    Connections
    {
        target: mSwitch
        function onChValue(value)
        {
            mSwitch.value=value;
            UiCore.setParameter(main.nameValue, mSwitch.value)

        }
    }

    Connections
    {
        target: UiCore
        function onSgSetUIParameter(nameParam, value)
        {
            if((nameParam === "eq_pre"))
            {
                mSwitch.value = value
            }
        }

        function onSgSetParameter(nameParam, value)
        {
            if((nameParam === "eq_pre"))
            {
                mSwitch.value = value
            }
        }
    }
}
