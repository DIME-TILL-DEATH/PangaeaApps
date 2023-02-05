import QtQuick 2.15
////import QtQuick.Controls 1.5
import "../Modules/"

MText
{
    id: main
    signal clicked();
    signal wheel(int angleDelta);
    property bool check: false
    property bool leftText: false

    text:  "TEXT"
    color: check?"Salmon":fonColor
    horizontalAlignment: leftText?Text.AlignHLeft:Text.AlignHCenter
    font.bold: true
    font.pixelSize: parent.height/1.5
    opacity: main.enabled?1:0.5
    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: main.clicked()
        onWheel:   main.wheel(wheel.angleDelta.y);
    }
}
