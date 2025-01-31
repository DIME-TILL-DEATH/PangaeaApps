import QtQuick

import CppObjects
import PangaeaBackend

Rectangle{
    id: root

    property int selectedBandIndex

    property EqBand eqBand
    property int gainRange: 40

    property real xmin: module.minFreq
    property real xmax: module.maxFreq

    x: parent.width*((Math.log10(eqBand.Fc.displayValue)-Math.log10(xmin))
                     /(Math.log10(xmax)-Math.log10(xmin))) - width/2;
    y: parent.height/2 - eqBand.gain.displayValue * parent.height/gainRange - height/2;

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

        property real xmin: module.points[0].x;
        property real xmax: module.points[module.points.length-1].x

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

        onWheel: function(wheel){
            var resultQ = module.EqBands[currentBandIndex].Q.displayValue;

            if(wheel.angleDelta.y < 0)
            {
                resultQ *= 0.9;
                if(resultQ < module.EqBands[currentBandIndex].Q.minDisplayValue) resultQ = module.EqBands[currentBandIndex].Q.minDisplayValue;
            }

            if(wheel.angleDelta.y > 0)
            {
                resultQ *= 1.1;
                if(resultQ > module.EqBands[currentBandIndex].Q.maxDisplayValue) resultQ = module.EqBands[currentBandIndex].Q.maxDisplayValue;
            }
            module.EqBands[currentBandIndex].Q.displayValue = resultQ;
        }
    }
}
