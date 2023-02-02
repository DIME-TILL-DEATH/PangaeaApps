import QtQuick 2.7
import QtQuick.Controls 1.5

Item
{
    id: main
    property string fonColor: "#EBECEC"
    property string devColor: "#5E5971"
    property int   value: 1

    //TODO вроде как и не нужен
    signal chValue(int value)

    Column
    {
        anchors.fill:  parent

        Switch2
        {
            id:    mSwitch

            width: parent.width
            height: parent.height/100*80

            value: main.value
            opacity: main.enabled?1:0.5
            onChValue: main.chValue(value)
        }

        Row
        {
            width: parent.width
            height: parent.height/100*20

            CheckText
            {
                width: parent.width/2
                height: parent.height
                text: "On"
                check: mSwitch.value==0
                onClicked: {mSwitch.value = 0; main.chValie(value);}
            }

            CheckText
            {
                width: parent.width/2
                height: parent.height
                text: "Off"
                check: mSwitch.value==1
                onClicked: {mSwitch.value =1; main.chValie(value)}
            }
        }

    }
    function setValue(value)
    {
        mSwitch.value = value;
    }

    Connections
    {
        target: _uiCore
        function onSgSetUIParameter(nameParam, nameValue)
        {
            if((nameParam === "cabinet_enable"))
            {
                main.value = !nameValue
            }
        }

        function onSgSetParameter(nameParam, nameValue)
        {
            if((nameParam === "cabinet_enable"))
            {
                main.value = !nameValue
            }
        }
    }
}
