import QtQuick 2.15
import QtQuick.Controls as Controls

import StyleSettings

import Elements

import CppObjects

Rectangle
{
    id: main

    property bool on: true
    property int paramType: DeviceParameter.EQ_ON

    property int currentBandIndex

    property real lineWidth: 0.25

    property int gainRange: 40

    color: main.on ? Style.mainEnabledColor : Style.mainDisabledColor


    function update()
    {
        _canvas.requestPaint();
    }

    Column{
        anchors.fill: parent

        Item{
            width: parent.width
            height: parent.height*3/4

            MouseArea{
                id: mAsteaalModuleOnOf
                anchors.fill: parent
                z: parent.z-1
            }

            Canvas{
                id: _canvas

                anchors.fill: parent

                renderStrategy: Canvas.Threaded

                property int yGridSize: 5
                property real coefY : height / gainRange

                onPaint: {
                    var xmin = EqResponse.points[0].x;
                    var xmax = EqResponse.points[EqResponse.points.length-1].x

                    var ctx = getContext("2d");

                    ctx.reset();

                    ctx.lineWidth = 1;
                    ctx.strokeStyle = "gray";
                    ctx.font = "8px Arial";
                    ctx.fillStyle = "gray";

                    // y-grid draw
                    for(var i=0; i<gainRange/yGridSize; i++)
                    {
                        if(i == gainRange/yGridSize/2)
                        {
                            ctx.strokeStyle =  (main.on)? "darkgrey" : "gray";
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
                    ctx.strokeStyle = (main.on)? "white" : "darkgray";

                    ctx.translate(0, _canvas.height/2);

                    ctx.beginPath();

                    var x=0;
                    var y=_canvas.height/2;

                    for(i=0; i<EqResponse.points.length; i++)
                    {
                        /*To draw ticks at simple values, first determine the full decades that are spanned,
                        from 10^floor(log(V0)) to 10^floor(log(V1)), then get the most significant digits by

                        ceil(10^(log(V0) - floor(log(V0)))
                        floor(10^(log(V1) - floor(log(V1)))
                        to get the starting/ending digit in these decades.*/

                        x = _canvas.width*((Math.log10(EqResponse.points[i].x)-Math.log10(xmin))
                                           /(Math.log10(xmax)-Math.log10(xmin)));
                        y = EqResponse.points[i].y*coefY

                        ctx.lineTo(x, -y);
                    }
                    ctx.stroke()
                }
            }

            Repeater{
               id: repeater

               anchors.fill: parent
               delegate: EqPoint{
                   eqBand: EqResponse.EqBands[index]
                   gainRange: main.gainRange
                   selectedBandIndex: main.currentBandIndex

                   Component.onCompleted: {
                       pointSelected.connect(main.bandSelected)
                   }
               }

               model: EqResponse.EqBands
            }

            DropArea{
                id: dropArea

                anchors.fill: parent


                onPositionChanged: function(drag){
                    var xmin = EqResponse.points[0].x;
                    var xmax = EqResponse.points[EqResponse.points.length-1].x;
                    var pointRadius = repeater.itemAt(currentBandIndex).height/2;

                    var freq = Math.pow(10, (drag.source.x+pointRadius)/_canvas.width * (Math.log10(xmax)-Math.log10(xmin)) + Math.log10(xmin))
                    var gain = (-drag.source.y + _canvas.height/2 - pointRadius) * main.gainRange/_canvas.height;

                    EqResponse.EqBands[currentBandIndex].gain = Math.round(gain);
                    EqResponse.EqBands[currentBandIndex].Fc = Math.round(freq);
                    // console.log(Math.round(freq), Math.round(gain))
                }
            }
        }

        Rectangle{
            id: bandInfoRect

            width: parent.width
            height: parent.height - _canvas.height - border.width*2

            z:-1

            color: "transparent"

            border.width: 2
            border.color: (main.on) ? "white" : "darkgrey"

            Row{
                width: parent.width
                height: parent.height*0.9
                anchors.verticalCenter: parent.verticalCenter
                Item{
                    width: parent.width/6
                    height: parent.height*0.8
                    anchors.verticalCenter: parent.verticalCenter

                    MText{
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: (main.on) ? "white" : "darkgrey"
                        font.pixelSize: parent.width/6

                        text: "BAND " + (currentBandIndex + 1) + "\n" +
                              EqResponse.EqBands[currentBandIndex].fStart + "Hz\n" +
                              EqResponse.EqBands[currentBandIndex].fStop + "Hz\n"
                    }
                }

                EqDial{
                    id: freqDial

                    annotation: "Freq"

                    from: EqResponse.EqBands[currentBandIndex].fStart
                    to: EqResponse.EqBands[currentBandIndex].fStop

                    step: 1
                    value: EqResponse.EqBands[currentBandIndex].Fc

                    onMoved: {
                        EqResponse.EqBands[currentBandIndex].Fc = value
                    }
                }
                EqDial{
                    annotation: "Gain"

                    from: -15
                    to: 15
                    step: 1

                    value: EqResponse.EqBands[currentBandIndex].gain

                    onMoved: {
                        EqResponse.EqBands[currentBandIndex].gain = value
                    }
                }
                EqDial{
                    annotation: "Q"

                    from: 0.1
                    to: 20
                    step: 0.1
                    value: EqResponse.EqBands[currentBandIndex].Q

                    onMoved: {
                        EqResponse.EqBands[currentBandIndex].Q = value
                    }
                }
            }
        }
    }


    function bandSelected(index){
        currentBandIndex = index
    }

    Connections{
        target: UiCore

        function onSgSetDeviceParameter(paramType, value)
        {
            if(paramType === DeviceParameter.EQ_ON)
            {
                main.update();
            }
        }

        function onSetDeviceParameter(paramType, value)
        {
            if(paramType === DeviceParameter.EQ_ON)
            {
                main.update();
            }
        }
    }

    Connections{
        target: EqResponse

        function onPointsChanged()
        {
            main.update();
        }
    }
}
