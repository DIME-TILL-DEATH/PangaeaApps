import QtQuick

import CppObjects

Rectangle{
    id: root

    property int selectedBandIndex

    property EqBand eqBand
    property int gainRange: 40

    property int xmin: EqResponse.points[0].x
    property int xmax: EqResponse.points[EqResponse.points.length-1].x

    x: parent.width*((Math.log10(eqBand.Fc.value)-Math.log10(xmin))
                     /(Math.log10(xmax)-Math.log10(xmin))) - width/2;
    y: parent.height/2 - eqBand.gain.value * parent.height/gainRange - height/2;

    transformOrigin: Item.Center

    width: Math.min(parent.width/15, parent.height/15)
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

    MouseArea{
        id: ma

        anchors.fill: parent
        cursorShape: Qt.SizeAllCursor

        drag.target: root

        property real xmin: EqResponse.points[0].x;
        property real xmax: EqResponse.points[EqResponse.points.length-1].x

        drag.minimumX: _canvas.width*((Math.log10(eqBand.Fc.minValue)-Math.log10(xmin))
                                      /(Math.log10(xmax)-Math.log10(xmin))) - root.width/2;
        drag.maximumX: _canvas.width*((Math.log10(eqBand.Fc.maxValue)-Math.log10(xmin))
                                      /(Math.log10(xmax)-Math.log10(xmin))) - root.width/2;

        drag.minimumY: root.parent.height/2 - 15 * root.parent.height/gainRange - root.height/2
        drag.maximumY: root.parent.height/2 + 15 * root.parent.height/gainRange - root.height/2

        drag.smoothed: false

        onPressed: {
            pointSelected(index);
        }

        onWheel: function(wheel){
            var resultQ = EqResponse.EqBands[currentBandIndex].Q.value;

            if(wheel.angleDelta.y < 0)
            {
                resultQ *= 0.9;
                if(resultQ < EqResponse.EqBands[currentBandIndex].Q.minValue) resultQ = EqResponse.EqBands[currentBandIndex].Q.minValue;
            }

            if(wheel.angleDelta.y > 0)
            {
                resultQ *= 1.1;
                if(resultQ > EqResponse.EqBands[currentBandIndex].Q.maxValue) resultQ = EqResponse.EqBands[currentBandIndex].Q.maxValue;
            }
            EqResponse.EqBands[currentBandIndex].Q.value = resultQ;
        }
    }
}
