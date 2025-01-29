import QtQuick

import CppObjects 1.0

Item
{
    id: main

    property ControlValue controlValue

    enabled: controlValue.enabled

    Column
    {
        anchors.fill:  parent

        Switch3
        {
            id:    mSwitch
            width: parent.width
            height: parent.height/100*50
            opacity: main.enabled?1:0.5

            value: controlValue.displayValue

            onChValue: function changeValue(value){
                controlValue.displayValue = value
            }
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
                font.pixelSize: height/1.4
                horizontalAlignment: Text.AlignLeft

                onClicked: controlValue.displayValue = 0
            }

            CheckText
            {
                width: parent.width
                height: parent.height/3
                text: "MEDIUM"
                check: mSwitch.value==1
                font.pixelSize: height/1.4

                onClicked: controlValue.displayValue = 1
            }

            CheckText
            {
                width: parent.width
                height: parent.height/3
                text: "LONG"
                check: mSwitch.value==2
                font.pixelSize: height/1.4
                horizontalAlignment: Text.AlignRight

                onClicked: controlValue.displayValue = 2
            }
        }
    }
}
