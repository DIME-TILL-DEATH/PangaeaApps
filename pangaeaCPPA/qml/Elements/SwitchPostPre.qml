import QtQuick 2.15
//import QtQuick.Controls 1.5

Item
{
    id: main

    property string fonColor: "#EBECEC"
    property string devColor: "#5E5971"
    property string devColorDis: "#7E7991"
//    property int   value//: mSwitch.value
    property string nameValue: "eq_pre"

    property bool isAvaliable: true
    
    Rectangle
    {
        anchors.fill:  parent
        color: main.enabled?devColor:devColorDis
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

            //value: main.value

            onChValue: _uiCore.setParameter(main.nameValue, mSwitch.value)

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
                    _uiCore.setParameter(main.nameValue, mSwitch.value)
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
                    _uiCore.setParameter(main.nameValue, mSwitch.value)
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
                color: fonColor
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
            color: main.enabled?devColor:devColorDis
        }
    }

    //TODO switch аналогичен IR, но работает по-разному!
    Connections
    {
        target: mSwitch
        function onChValue(value)
        {
            mSwitch.value=value;
            _uiCore.setParameter(main.nameValue, mSwitch.value)

        }
    }

    Connections
    {
        target: _uiCore
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
