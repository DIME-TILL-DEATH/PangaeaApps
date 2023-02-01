import QtQuick 2.15
import QtQuick.Controls 2.15

Item
{
    id: main
    property string fonColor: "#EBECEC"
    property string devColor: "#5E5971"

    property int curVal: -1
    property int maxPresetBank: 10

    Row
    {
        anchors.fill: parent
        Rectangle
        {
            width:  parent.width/2
            height: parent.height

            color:  fonColor
            radius: parent.height
            MText
            {
                anchors.fill: parent
                text: "BANK"
                color: devColor
                rotation: 270
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
                font.bold: true
                font.pixelSize: parent.width/1.5
            }
        }

        Column
        {
            width:  parent.width /2
            height: parent.height

            Repeater
            {
                anchors.fill: parent
                model: maxPresetBank
                Item
                {
                    width:  parent.width
                    height: parent.height/maxPresetBank
                    Rectangle
                    {
                        anchors.fill: parent
                        color: devColor
                    }
                    MText
                    {
                        anchors.fill: parent
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment:   Qt.AlignVCenter
                        text: index
                        color: index===curVal ? "Salmon":fonColor
                        font.bold: true
                        font.pixelSize: parent.width/1.5
                    }
                }
            }
        }
    }
}
