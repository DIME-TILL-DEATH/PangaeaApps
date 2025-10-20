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

    width: Math.min(parent.width/12, parent.height/12)
    height: width

    radius: width/2

    property color selectedColor: "red"

    color: (selectedBandIndex === index) ?
               (main.on ? selectedColor : "darkgrey") :
               "transparent"


    border.width: Math.max(2, width/20)
    border.color: main.on ? selectedColor : "darkgrey"

    signal pointSelected(var index);

    Drag.active: ma.drag.active
    Drag.hotSpot.x: width/2
    Drag.hotSpot.y: height/2

    MText{
        id: _textBandNum

        anchors.centerIn: parent
        color:  (selectedBandIndex === index) ? "black" : (main.on ? selectedColor : "darkgrey")
    }

    MouseArea{
        id: ma

        anchors.fill: parent
        cursorShape: Qt.SizeAllCursor

        focusPolicy: Qt.StrongFocus

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
            var resultQ = module.EqBands[selectedBandIndex].Q.displayValue;

            if(wheel.angleDelta.y < 0)
            {
                resultQ *= 0.9;
                if(resultQ < module.EqBands[selectedBandIndex].Q.minDisplayValue) resultQ = module.EqBands[selectedBandIndex].Q.minDisplayValue;
            }

            if(wheel.angleDelta.y > 0)
            {
                resultQ *= 1.1;
                if(resultQ > module.EqBands[selectedBandIndex].Q.maxDisplayValue) resultQ = module.EqBands[selectedBandIndex].Q.maxDisplayValue;
            }

            if(!(filterType == FilterType.LOW_CUT | filterType == FilterType.HIGH_CUT))
                module.EqBands[selectedBandIndex].Q.displayValue = resultQ;
        }
    }

    Component.onCompleted: {
        switch(filterType)
        {
        case FilterType.HIGH_CUT:
            _textBandNum.text = "LP"
            ma.drag.axis = Drag.XAxis
            selectedColor = "green"
            break;
        case FilterType.LOW_CUT:
            _textBandNum.text = "HP"
            ma.drag.axis = Drag.XAxis
            selectedColor = "blue"
            break;
        default:
            _textBandNum.text = index+1
            ma.drag.axis = Drag.XAndYAxis
            selectedColor = "salmon"
        }
    }
}
