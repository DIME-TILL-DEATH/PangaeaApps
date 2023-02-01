import QtQuick 2.5
import QtQuick.Controls 2.12

Item
{
    property bool busy: false
    property bool pbVisible: true
    property double  pbValue: 0
    property bool  txtVisible: false
    property bool  txtExtVisible: true
    property string extText: ""

    id : rWait
    width:  busy?(parent.width):0
    height: busy?(parent.height):0
    anchors.centerIn: parent

    Column
    {
        anchors.fill: parent

        Item
        {
            width: parent.width
            height: parent.height*5/10
            BusyIndicator
            {
//                anchors.fill: parent
                id : bI
                visible: busy
                width:  busy?(parent.width/2):0
                height: busy?(parent.width/2):0
                anchors.centerIn: parent
                palette.dark: "lightgrey"
                onVisibleChanged:
                {

                }
            }
        }
        Item
        {
            width: parent.width
            height: parent.height*1/10
            Text
            {
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignBottom
                visible: txtVisible
                text: (Math.round(pbValue*10000)/100)+"%"
                anchors.fill: parent
                color: "white"
            }
        }
        Item
        {
                width: parent.width/2
                height: parent.height*1/10
                anchors.horizontalCenter: parent.horizontalCenter
                ProgressBar
                {
                    visible: pbVisible
                    id: progressBar
                    anchors.fill: parent
                    value: pbValue
                    anchors.horizontalCenter: parent.horizontalCenter
                }
          }
        Item
        {
            width: parent.width
            height: parent.height*1/10
            Text
            {
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignBottom
                visible: txtExtVisible
                text: extText
                anchors.fill: parent
                color: "white"
            }
        }

    }

    MouseArea
    {
        id : mWait
        enabled: busy
        anchors.fill: parent
        onEnabledChanged:
        {

        }
    }

//    Connections
//    {
//        target: _core
//        onSignal: console.log("The application data changed!");
//        onSgSetProgress:
//        {
//            progressBar.value = val;
//        }
//    }

}
