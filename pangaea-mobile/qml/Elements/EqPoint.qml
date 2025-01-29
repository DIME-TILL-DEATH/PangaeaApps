import QtQuick

import StyleSettings

import CppObjects
import CppEnums

Rectangle{
    id: root

    property int selectedBandIndex

    property EqBand eqBand
    property int gainRange: 40

    property real xmin: eqModule.minFreq
    property real xmax: eqModule.maxFreq

    x: parent.width*((Math.log10(eqBand.Fc.displayValue)-Math.log10(xmin))
                     /(Math.log10(xmax)-Math.log10(xmin))) - width/2
    y: parent.height/2 - eqBand.gain.displayValue * parent.height/gainRange - height/2

    transformOrigin: Item.Center

    width: Math.min(parent.width/6, parent.height/6)
    height: width

    radius: width/2

    property color selectedColor: "Salmon"

    color: (selectedBandIndex === index) ?
               (main.on ? selectedColor : "darkgrey") :
               "transparent"

    border.width: Math.max(2, width/20)
    border.color: main.on ? selectedColor  : "darkgrey"

    signal pointSelected(var index);

    Drag.active: ma.drag.active
    Drag.hotSpot.x: width/2
    Drag.hotSpot.y: height/2

    MText{
        id: _textBandNum

        anchors.centerIn: parent
        // text: index + 1

        color:  (selectedBandIndex === index) ? "black" : (main.on ? selectedColor : "darkgrey")
    }

    MouseArea{
        id: ma

        anchors.fill: parent
        cursorShape: Qt.SizeAllCursor

        drag.target: root

        // drag.axis: (eqBand.type === FilterType.HIGH_CUT || eqBand.type === FilterType.LOW_CUT) ? Drag.XAxis : Drag.XAndYAxis

        // property real xmin: eqModule.points[0].x;
        // property real xmax: eqModule.points[eqModule.points.length-1].x
        // property real xmin: eqModule.minFreq;
        // property real xmax: eqModule.maxFreq;

        drag.minimumX: _canvas.width*((Math.log10(eqBand.Fc.minDisplayValue)-Math.log10(xmin))
                                      /(Math.log10(xmax)-Math.log10(xmin))) - root.width/2;
        drag.maximumX: _canvas.width*((Math.log10(eqBand.Fc.maxDisplayValue)-Math.log10(xmin))
                                      /(Math.log10(xmax)-Math.log10(xmin))) - root.width/2;

        drag.minimumY: root.parent.height/2 - 15 * root.parent.height/gainRange - root.height/2
        drag.maximumY: root.parent.height/2 + 15 * root.parent.height/gainRange - root.height/2

        drag.smoothed: false

        // drag.threshold

        onPressed: {
            pointSelected(index);
        }

        // Component.onCompleted:{
        //     console.log("Drag threshold: ", ma.drag.threshold)
        //     console.log("Drag minimum-maximum X ", ma.drag.minimumX, ma.drag.maximumX)
        // }
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
