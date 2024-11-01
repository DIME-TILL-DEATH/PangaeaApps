import QtQuick

import StyleSettings
import CppObjects 1.0

Item
{
    id: main

    property int value: mSwitch.value
    property int paramType: DeviceParameter.OUTPUT_MODE
    
    Rectangle
    {
        anchors.fill:  parent
        color: Style.mainEnabledColor
    }

    Column
    {
        anchors.fill:  parent

        Item
        {
            width: parent.width
            height: parent.height/100*5
        }

        Switch3
        {
            id:    mSwitch
            width: parent.width
            height: parent.height/100*40
            onChValue: send()
        }

        Row
        {
            width: parent.width
            height: parent.height/100*20

            CheckText
            {
                width: parent.width/2
                height: parent.height
                text: "PH"
                check: mSwitch.value==0
                onClicked: {mSwitch.value = 0; send();}
            }

            // CheckText - Просто текст. Ему Item нужен. Возможно надо внутри модуля добавить
            Item
            {
                width: parent.width/2
                height: parent.height
                CheckText
                {
                    anchors.fill: parent
                    text: "BAL"
                    check: mSwitch.value==2
                    onClicked: {mSwitch.value = 2; send();}
                }
            }
        }

        Item
        {
            width: parent.width
            height: parent.height/100*20
            CheckText
            {
                anchors.fill: parent
                text: "LINE"
                check: mSwitch.value==1
                onClicked: {mSwitch.value = 1; send();}
            }
        }


        Item
        {
            width: parent.width
            height: parent.height/100*15
        }
    }

    function send()
    {
        UiCore.setDeviceParameter(main.paramType, mSwitch.value)
    }


    Connections
    {
        target: UiCore

        function onSgSetUiDeviceParameter(paramType, value)
        {
            if(paramType === main.paramType)
            {
                mSwitch.value=value;
            }
        }
    }
}
