import QtQuick 2.12
import QtQuick.Controls 2.12

import StyleSettings 1.0

import CppObjects

Item
{
    id : rWait

    property bool busy: false
    property bool pbVisible: true
    property double  pbValue: 0
    property bool  txtVisible: false
    property bool  txtExtVisible: false

    //TODO для firmware отдельный экран. Нужно?
    property string firmwareFile: "*.ble"

    width:  busy?(parent.width):0
    height: busy?(parent.height):0
    anchors.centerIn: parent

    visible: busy


    Rectangle{
        id: background

        anchors.fill: parent
        opacity: 0.5
    }

    Column
    {
        anchors.centerIn: parent
        height: parent.height*0.8
        width: parent.width

        Item
        {
            width: parent.width
            height: parent.height*7/10
            BusyIndicator
            {
                id : bI
                visible: busy
                width:  busy?(parent.width/2):0
                height: busy?(parent.width/2):0

                anchors.centerIn: parent

                onVisibleChanged:
                {

                }


            }
        }

        Text
        {
            width: parent.width*0.75
            height: parent.height*1/10

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom

            font.pixelSize: parent.width*0.05

            anchors.horizontalCenter: parent.horizontalCenter

            visible: txtExtVisible

            text: firmwareFile

            wrapMode: Text.Text.Wrap

            color: Style.colorFon
        }

        Text
        {
            width: parent.width
            height: parent.height*1/10

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom

            font.pixelSize: parent.width*0.05

            anchors.horizontalCenter: parent.horizontalCenter

            visible: txtVisible

            text: (Math.round(pbValue*10000)/100)+"%"

            color: Style.colorFon
        }

        ProgressBar
        {
            id: progressBar

            width: parent.width/2
            height: parent.height*0.5/10
            anchors.horizontalCenter: parent.horizontalCenter

            visible: pbVisible
            value: pbValue

            background: Rectangle {
                     implicitWidth: 200
                     implicitHeight: 6
                     color: Style.colorFon
                     radius: 3

                     border.width: 1
                 }

             contentItem: Item {
                 implicitWidth: 200
                 implicitHeight: 4

                 Rectangle {
                     width: progressBar.visualPosition * parent.width
                     height: parent.height
                     radius: 2
                     color: Style.currentTheme.colorModulOn
                     border.width: 1
                }
            }
        }

    }

//TODO а он нужен?
    MouseArea{
        id: mWait

        enabled: busy
        anchors.fill: parent

        onEnabledChanged: {

        }
    }

    Connections
    {
        target: UiCore

        function onSgSetProgress(val)
        {
            rWait.pbValue = val;
        }


        function onSgSetUIParameter(nameParam, inValue)
        {
            if(nameParam === ("wait"))
            {

                rWait.busy = inValue;
            }
        }
    }
}
