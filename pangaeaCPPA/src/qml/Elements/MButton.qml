import QtQuick 2.7
import QtQuick.Controls 1.5
import "../Elements/"

Item
{
    id: main
    property string fonColor: "#EBECEC"
    property string devColor: "#5E5971"
    property string text: text
    property int    value: 0
    property double scaleText: 1

    property bool  highlighted: false
    signal clicked();
    anchors.fill:  parent

    Rectangle
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter:  parent.verticalCenter
        width:  parent.width/1.5
        height: width/2
        radius: height/2
        color:  main.highlighted ? "Salmon" : fonColor
        clip: true

        Item
        {
            id: material

            anchors.fill: parent
            Rectangle
            {
                id: colorRect
                color: "grey"
                opacity: 0.3
                transform: Translate
                {
                    x: -colorRect.width / 2
                    y: -colorRect.height / 2
                }
            }

            PropertyAnimation
            {
                id: circleAnimation
                target: colorRect
                properties: "width,height,radius"
                from: 0
                to: main.width
                duration: 400
                onStopped: {colorRect.width = 0; colorRect.height = 0;}
            }


            function start(xM, yM)
            {
                colorRect.x = parent.width/2;  //xM;
                colorRect.y = parent.height/2; //yM;
                circleAnimation.start();
            }

            function stop()
            {
                circleAnimation.stop();
            }
        }

        MText
        {
            anchors.fill:  parent
            font.family: "Arial Black"
            font.bold: true
            font.pixelSize: parent.height/2*scaleText
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:   Text.AlignVCenter
            text: main.text
            color: devColor
            opacity: main.enabled?1:0.3
            Behavior on opacity  {NumberAnimation { duration:500 }}
        }
        MouseArea
        {
            anchors.fill:  parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {main.clicked(); material.start(mouseX, mouseY);}
        }
    }
}