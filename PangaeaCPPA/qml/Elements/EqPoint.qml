import QtQuick

import CppObjects

Rectangle{
    id: root

    property int selectedBandIndex

    property EqBand eqBand
    property int gainRange: 40

    property int xmin: EqResponse.points[0].x
    property int xmax: EqResponse.points[EqResponse.points.length-1].x

    x: parent.width*((Math.log10(eqBand.Fc)-Math.log10(xmin))
                     /(Math.log10(xmax)-Math.log10(xmin))) - width/2;
    y: parent.height/2 - eqBand.gain * parent.height/gainRange - height/2;

    transformOrigin: Item.Center

    width: Math.min(parent.width/25, parent.height/25)
    height: width

    radius: width/2

    color: (selectedBandIndex === index) ? "red" : "transparent"

    border.width: Math.max(2, width/20)
    border.color: (true) ? "Salmon" : "#EBECEC"

    signal pointSelected(var index);

    MouseArea{
        anchors.fill: parent

        onClicked: {
            pointSelected(index);
        }
    }
}
