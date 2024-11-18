import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    // anchors.centerIn: parent

    property EqParametric eqModule

    property bool on: eqModule.moduleEnabled

    property int currentBandIndex

    property int gainRange: 40

    property bool extVisible: false
    signal hide

    function update()
    {
        _canvas.requestPaint();
    }

    Rectangle
    {
        color: Style.colorFon
        anchors.fill: parent
        border.width: 3
        border.color: "black"
        radius: 5

        Column
        {
            width: parent.width/10*9
            height: parent.height/10*8
            anchors.centerIn: parent

            spacing: height/36

            Item {
                width: parent.width
                height: parent.height*18/36
                Canvas{
                    id: _canvas

                    anchors.fill: parent

                    renderStrategy: Canvas.Threaded

                    property int yGridSize: 5
                    property real coefY : height / gainRange

                    onPaint: {
                        var xmin = eqModule.points[0].x;
                        var xmax = eqModule.points[eqModule.points.length-1].x

                        var ctx = getContext("2d");

                        ctx.reset();

                        ctx.lineWidth = 1;
                        ctx.strokeStyle = "gray";
                        ctx.font = "6px sans-serif";
                        ctx.fillStyle = "gray";

                        // y-grid draw
                        for(var i=0; i<gainRange/yGridSize; i++)
                        {
                            if(i == gainRange/yGridSize/2)
                            {
                                ctx.strokeStyle = "darkgrey"
                                ctx.lineWidth = 3
                            }
                            else
                            {
                                ctx.strokeStyle = "gray";
                                ctx.lineWidth=1
                            }
                            ctx.beginPath();
                            var ypos = coefY*yGridSize*i;
                            ctx.moveTo(0, ypos);
                            ctx.lineTo(_canvas.width, ypos);
                            ctx.stroke();
                            ctx.fillText((gainRange/2 - yGridSize*i)+"dB", 2, ypos-4);
                        }

                        // x-grid draw
                        for(i=1; i<5; i++)
                        {
                            ctx.beginPath();
                            var xpos = _canvas.width*((Math.log10(Math.pow(10, i))-Math.log10(xmin))
                                                      /(Math.log10(xmax)-Math.log10(xmin)));
                            ctx.moveTo(xpos, 0);
                            ctx.lineTo(xpos, _canvas.height);
                            ctx.stroke();
                            ctx.fillText(Math.pow(10, i)+"Hz", xpos+2, _canvas.height-2);
                        }

                        // response draw
                        ctx.lineWidth = 3;
                        ctx.strokeStyle = on ? "white" : "darkgray" ;

                        ctx.translate(0, _canvas.height/2);

                        ctx.beginPath();

                        var x = 0;
                        var y = _canvas.height/2;

                        for(i=0; i<eqModule.points.length; i++)
                        {
                            x = _canvas.width*((Math.log10(eqModule.points[i].x)-Math.log10(xmin))
                                               /(Math.log10(xmax)-Math.log10(xmin)));
                            y = eqModule.points[i].y*coefY

                            ctx.lineTo(x, -y);
                        }
                        ctx.stroke()

                        // band draw
                        xmin = eqModule.EqBands[currentBandIndex].bandPoints[0].x;
                        xmax = eqModule.EqBands[currentBandIndex].bandPoints[eqModule.EqBands[currentBandIndex].bandPoints.length-1].x
                        ctx.lineWidth = 1;
                        ctx.strokeStyle = "salmon"
                        ctx.fillStyle = "salmon";
                        ctx.globalAlpha = 0.4;

                        ctx.beginPath();
                        ctx.moveTo(xmax,0)
                        ctx.lineTo(0,0)
                        for(i=0; i<eqModule.EqBands[currentBandIndex].bandPoints.length; i++)
                        {
                            x = _canvas.width*((Math.log10(eqModule.EqBands[currentBandIndex].bandPoints[i].x)-Math.log10(xmin))
                                               /(Math.log10(xmax)-Math.log10(xmin)));
                            y = eqModule.EqBands[currentBandIndex].bandPoints[i].y*coefY

                            ctx.lineTo(x, -y);
                        }
                        ctx.fill()
                    }
                }

                Repeater{
                   id: repeater

                   anchors.fill: parent
                   delegate: EqPoint{
                       eqBand: eqModule.EqBands[index]
                       gainRange: main.gainRange
                       selectedBandIndex: main.currentBandIndex

                       Component.onCompleted: {
                           pointSelected.connect(main.bandSelected)
                       }
                   }

                   model: eqModule.EqBands
                }

                DropArea{
                    id: dropArea

                    anchors.fill: parent


                    onPositionChanged: function(drag){
                        var xmin = eqModule.points[0].x;
                        var xmax = eqModule.points[eqModule.points.length-1].x;
                        var pointRadius = repeater.itemAt(currentBandIndex).height/2;

                        var freq = Math.pow(10, (drag.source.x+pointRadius)/_canvas.width * (Math.log10(xmax)-Math.log10(xmin)) + Math.log10(xmin))
                        var gain = (-drag.source.y + _canvas.height/2 - pointRadius) * main.gainRange/_canvas.height;

                        if(gain > 15) gain = 15;
                        if(gain < -15) gain = -15;

                        eqModule.EqBands[currentBandIndex].gain.displayValue = Math.round(gain);
                        eqModule.EqBands[currentBandIndex].Fc.displayValue = Math.round(freq);
                    }
                }


                PinchArea{
                    id: pinchArea

                    anchors.fill: parent
                    pinch.target: _canvas
                    pinch.dragAxis: Pinch.XAxis

                    z: repeater.z-1

                    property var startPinchQ

                    onPinchStarted: {
                        startPinchQ = eqModule.EqBands[currentBandIndex].Q.displayValue;
                    }

                    onPinchUpdated: function(pinch){
                        var delta1 = pinch.point1.x - pinch.startPoint1.x
                        var delta2 = pinch.point2.x - pinch.startPoint2.x

                        var resultQ = startPinchQ / pinch.scale;
                        if(resultQ > eqModule.EqBands[currentBandIndex].Q.maxDisplayValue) resultQ = eqModule.EqBands[currentBandIndex].Q.maxDisplayValue;
                        if(resultQ < eqModule.EqBands[currentBandIndex].Q.minDisplayValue) resultQ = eqModule.EqBands[currentBandIndex].Q.minDisplayValue;

                        eqModule.EqBands[currentBandIndex].Q.displayValue = resultQ
                    }
                }
            }

            Item{
                width: parent.width*0.9
                height: parent.height*1/10

                MText{
                    anchors.verticalCenter: parent.verticalCenter

                    color: Style.colorText

                    text: "BAND " + (currentBandIndex+1)
                          + ", frequency span: " + eqModule.EqBands[currentBandIndex].Fc.minDisplayValue + "Hz-"
                           + eqModule.EqBands[currentBandIndex].Fc.maxDisplayValue + "Hz"
                }
            }

            CustomSlider
            {
                ctrlValInstance: eqModule.EqBands[currentBandIndex].Fc
            }

            CustomSlider
            {
                ctrlValInstance: eqModule.EqBands[currentBandIndex].gain

            }

            CustomSlider
            {
                ctrlValInstance: eqModule.EqBands[currentBandIndex].Q
                precision: 1
            }

            Button
            {
                id: btn

                width: parent.width
                height: parent.height*2/15

                text: qsTr("HIDE")

                onClicked:
                {
                    hide();
                }
            }
        }
    }

    function bandSelected(index)
    {
        currentBandIndex = index
        main.update();
    }

    Keys.onReleased: function(event)
    {
        if(event.key === Qt.Key_Back)
        {
            main.hide();
        }
    }

    Connections{
        target: eqModule

        function onPointsChanged()
        {
            main.update();
        }

        function onModuleEnabledChanged()
        {
            main.update();
        }
    }
}
