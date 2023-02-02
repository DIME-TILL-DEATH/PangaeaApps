import QtQuick 2.15
import QtQuick.Controls 2.15

Item
{
    id: main
    property string fonColor: "#EBECEC"
    property string devColor: "#5E5971"

    property int curVal: -1
    property int maxPresetBank: 10

    Column
    {
        anchors.fill: parent
        Rectangle
        {
            width:  parent.width
            height: parent.height / 2

            color: fonColor
            radius: parent.height
            MText
            {
                anchors.fill: parent
                text: "PRESET"
                color: devColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
                font.bold: true
                font.pixelSize: parent.height/1.5
            }
        }

        Row
        {
            width:  parent.width
            height: parent.height /2

            Repeater
            {
                anchors.fill: parent
                model: maxPresetBank
                Item
                {
                    width:  parent.width/maxPresetBank
                    height: parent.height
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
                        font.pixelSize: parent.height/1.5
                    }
                }
            }
        }
    }
}
