import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0

import Layouts 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property EqParametric module: _moduleLoader.selectedModuleInstance

    EqWindow{
        id: _eqWindow

        module: main.module
    }

    contentItem: Row
    {
        width: main.dialWidth * 5
        height: main.dialHeight

        Rectangle{
            id: canvasBackground

            height: parent.height
            width: parent.width * 3/5 //+ Style.mainSpacing * 2

            color: "black"
            Canvas{
                id: _canvas

                anchors.fill: parent
                renderStrategy: Canvas.Threaded

                property int gainRange: 40

                property real coefY : height / gainRange
                property int yGridSize: 5

                // z: canvasBackground.z+1

                property real xmin: module.minFreq;
                property real xmax: module.maxFreq;

                onPaint: {

                    var ctx = getContext("2d");

                    var points = module.points;

                    ctx.reset();

                    ctx.lineWidth = 1;
                    ctx.strokeStyle = "gray";

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
                    }

                    // response draw
                    ctx.lineWidth = 3;
                    ctx.strokeStyle = (main.on)? "white" : "darkgray";

                    ctx.translate(0, _canvas.height/2);

                    ctx.beginPath();

                    var x=0;
                    var y=_canvas.height/2;

                    for(i=0; i<module.points.length; i++)
                    {
                        x = _canvas.width*((Math.log10(points[i].x)-Math.log10(xmin))
                                           /(Math.log10(xmax)-Math.log10(xmin)));
                        y = points[i].y*coefY

                        ctx.lineTo(x, -y);
                    }
                    ctx.stroke()
                }
            }

            Connections{
                target: module

                function onPointsChanged()
                {
                    _canvas.requestPaint();

                    // console.log("points changed")
                }

                function onModuleEnabledChanged()
                {
                    _canvas.requestPaint();

                    // console.log("module enabled changed")
                }
            }
        }

        Rectangle{
            width: 1
            height: parent.height
        }

        Item{
            width: main.dialWidth * 3
            height: parent.height

            MButton{
                id: _showEqBtn

                text: qsTr("Controls")

                width: parent.width * 0.75
                height: dialHeight/2

                anchors.centerIn: parent

                onClicked:{
                    _eqWindow.show();
                }
            }
        }


        Rectangle{
            width: 1
            height: parent.height
        }

        ParameterSwitch{
            width: main.dialWidth
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            invertedValue: 1

            textLeft: "PRE"
            textRight: "POST"

            ctrlValInstance: module.position

            moduleOn: module.moduleEnabled
        }
    }

}
