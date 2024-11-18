import QtQuick

import StyleSettings

import CppObjects

Rectangle{
    id: root

    property int selectedBandIndex

    property EqBand eqBand
    property int gainRange: 40

    property int xmin: eqModule.points[0].x
    property int xmax: eqModule.points[eqModule.points.length-1].x

    x: parent.width*((Math.log10(eqBand.Fc.displayValue)-Math.log10(xmin))
                     /(Math.log10(xmax)-Math.log10(xmin))) - width/2;
    y: parent.height/2 - eqBand.gain.displayValue * parent.height/gainRange - height/2;

    transformOrigin: Item.Center

    width: Math.min(parent.width/6, parent.height/6)
    height: width

    radius: width/2

    color: (selectedBandIndex === index) ?
               (main.on ? "red" : "darkgrey") :
               "transparent"

    border.width: Math.max(2, width/20)
    border.color: main.on ? "Salmon" : "darkgrey"

    signal pointSelected(var index);

    Drag.active: ma.drag.active
    Drag.hotSpot.x: width/2
    Drag.hotSpot.y: height/2

    MText{
        id: _textBandNum

        anchors.centerIn: parent
        text: index + 1

        color:  (selectedBandIndex === index) ? "black" : (main.on ? Style.currentTheme.colorLedOn : "darkgrey")
    }

    MouseArea{
        id: ma

        anchors.fill: parent
        cursorShape: Qt.SizeAllCursor

        drag.target: root

        property real xmin: eqModule.points[0].x;
        property real xmax: eqModule.points[eqModule.points.length-1].x

        drag.minimumX: _canvas.width*((Math.log10(eqBand.Fc.minDisplayValue)-Math.log10(xmin))
                                      /(Math.log10(xmax)-Math.log10(xmin))) - root.width/2;
        drag.maximumX: _canvas.width*((Math.log10(eqBand.Fc.maxDisplayValue)-Math.log10(xmin))
                                      /(Math.log10(xmax)-Math.log10(xmin))) - root.width/2;

        drag.minimumY: root.parent.height/2 - 15 * root.parent.height/gainRange - root.height/2
        drag.maximumY: root.parent.height/2 + 15 * root.parent.height/gainRange - root.height/2

        drag.smoothed: false

        onPressed: {
            pointSelected(index);
        }
    }
}
