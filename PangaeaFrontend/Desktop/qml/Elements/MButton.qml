import QtQuick

import Elements
import StyleSettings

Item
{
    id: main

    property string text: text
    property int    value: 0
    property double scaleText: 1

    property bool  highlighted: false
    property color highlightColor: Style.highlightColor

    signal clicked();

    property alias radius: _rectangle.radius

    Rectangle
    {
        id: _rectangle

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter:  parent.verticalCenter
        width:  parent.width/1.3
        height: parent.height/1.3 //width/2

        border.width: 1
        border.color: Style.mainEnabledColor

        radius: height/2
        color:  main.highlighted ? highlightColor : Style.backgroundColor
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

                radius: main.height/2

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
                properties: "width,height"//",radius"
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

            font.pixelSize: Math.min(parent.height/2, parent.width/5) * scaleText
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:   Text.AlignVCenter
            text: main.text
            color: Style.mainEnabledColor
            opacity: main.enabled?1:0.3
            Behavior on opacity  {NumberAnimation { duration:500 }}

            elide: Text.ElideMiddle
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
