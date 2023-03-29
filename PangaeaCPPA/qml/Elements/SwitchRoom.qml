import QtQuick

import CppObjects 1.0

Item
{
    id: main

    property int   value: mSwitch.value

    Column
    {
        anchors.fill:  parent

        Switch3
        {
            id:    mSwitch
            width: parent.width
            height: parent.height/100*50
            opacity: main.enabled?1:0.5
            onChValue: UiCore.setParameter("early_type", mSwitch.value)
        }

        Column
        {
            width: parent.width*0.9
            height: parent.height/100*50
            anchors.horizontalCenter: parent.horizontalCenter
            CheckText
            {
                width: parent.width
                height: parent.height/3
                text: "SHORT"
                check: mSwitch.value==0
                onClicked: {mSwitch.value = 0; UiCore.setParameter("early_type", mSwitch.value)}
                font.pixelSize: height/1.4
                horizontalAlignment: Text.AlignLeft
            }

            CheckText
            {
                width: parent.width
                height: parent.height/3
                text: "MEDIUM"
                check: mSwitch.value==1
                onClicked: {mSwitch.value = 1; UiCore.setParameter("early_type", mSwitch.value)}
                font.pixelSize: height/1.4
            }

            CheckText
            {
                width: parent.width
                height: parent.height/3
                text: "LONG"
                check: mSwitch.value==2
                onClicked: {mSwitch.value = 2; UiCore.setParameter("early_type", mSwitch.value)}
                font.pixelSize: height/1.4
                horizontalAlignment: Text.AlignRight
            }
        }
    }

    Connections
    {
        target: UiCore
        function onSgSetUIParameter(nameParam, value)
        {
            if((nameParam.indexOf("early_type")>=0))
                mSwitch.value=value
        }
    }
}
