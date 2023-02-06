import QtQuick 2.15

Item
{
    id: topItem
    anchors.fill: parent

    property bool on: false

    Rectangle
    {
        id: colorRect

        width: topItem.parent.height*3
        height: topItem.parent.height*3
        radius: topItem.parent.height*3

        //TODO: цвет что и у модуля при первом запуске анимации.В итоге эффект не виден
        color: topItem.on ? devColor : devColorDis

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
        to: topItem.parent.height*3
        duration: 300
        onStopped: {
            topItem.parent.color = topItem.parent.on ? devColor : devColorDis
        }
    }

    function start(xM, yM)
    {
        colorRect.x = xM;
        colorRect.y = yM;
        circleAnimation.start();
    }
}
