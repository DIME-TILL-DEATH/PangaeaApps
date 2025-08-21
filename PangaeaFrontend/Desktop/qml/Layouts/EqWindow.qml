import QtQuick 2.15
import QtQuick.Controls as Controls

import StyleSettings

import Elements

import CppObjects
import PangaeaBackend

Window{
    id: main

    property EqParametric module
    property bool on: module.moduleEnabled

    width: Screen.width * 0.3
    height: width * 0.5

    title: "Parametric EQ"

    minimumWidth: Screen.width * 0.3 * 0.75
    minimumHeight: Screen.width * 0.3 * 0.5 * 0.75
    maximumWidth: Screen.width * 0.3 * 1.25
    maximumHeight: Screen.width * 0.3 * 0.5 * 1.25

    Rectangle{
        id: _eqp

        anchors.fill: parent

        property int currentBandIndex: 0

        property int gainRange: 40

        color: main.on ? Style.currentTheme.mainEnabledColor : Style.currentTheme.mainDisabledColor


        function update()
        {
            _canvas.requestPaint();
        }

        Column{
            anchors.fill: parent

            Row{
                width: parent.width
                height: parent.height*1/8

                opacity: module.moduleEnabled ? 1 : 0.5

                MButton{
                    text: qsTr("RESET")

                    width: parent.width * 1/5
                    height: parent.height * 0.9

                    onClicked: module.reset()
                }

                Item{
                    width: parent.width * 1/10
                    height: parent.height
                }

                MButton{
                    text: "HP"

                    width: parent.width * 1/10
                    height: parent.height * 0.9

                    scaleText: 2

                    highlightColor: "blue"
                    highlighted: _eqp.currentBandIndex === 5

                    onClicked: _eqp.currentBandIndex = 5
                }

                MButton{
                    text: "1"

                    width: parent.width * 1/10
                    height: parent.height * 0.9

                    scaleText: 2

                    highlighted: _eqp.currentBandIndex === 0

                    onClicked: _eqp.currentBandIndex = 0
                }

                MButton{
                    text: "2"

                    width: parent.width * 1/10
                    height: parent.height * 0.9

                    scaleText: 2

                    highlighted: _eqp.currentBandIndex === 1

                    onClicked: _eqp.currentBandIndex = 1
                }

                MButton{
                    text: "3"

                    width: parent.width * 1/10
                    height: parent.height * 0.9

                    scaleText: 2

                    highlighted: _eqp.currentBandIndex === 2

                    onClicked: _eqp.currentBandIndex = 2
                }

                MButton{
                    text: "4"

                    width: parent.width * 1/10
                    height: parent.height * 0.9

                    scaleText: 2

                    highlighted: _eqp.currentBandIndex === 3

                    onClicked: _eqp.currentBandIndex = 3
                }

                MButton{
                    text: "5"

                    width: parent.width * 1/10
                    height: parent.height * 0.9

                    scaleText: 2

                    highlighted: _eqp.currentBandIndex === 4

                    onClicked: _eqp.currentBandIndex = 4
                }

                MButton{
                    text: "LP"

                    width: parent.width * 1/10
                    height: parent.height * 0.9

                    scaleText: 2

                    highlightColor: "green"
                    highlighted: _eqp.currentBandIndex === 6

                    onClicked: _eqp.currentBandIndex = 6
                }
            }

            Item{
                width: parent.width
                height: parent.height*5/8

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
                    property real coefY : height / _eqp.gainRange

                    property real xmin: module.minFreq;
                    property real xmax: module.maxFreq;

                    onPaint: {
                        // var xmin = module.points[0].x;
                        // var xmax = module.points[module.points.length-1].x

                        var ctx = getContext("2d");

                        var points = module.points;

                        ctx.reset();

                        ctx.lineWidth = 1;
                        ctx.strokeStyle = "gray";
                        ctx.font = "8px Arial";
                        ctx.fillStyle = "gray";

                        // y-grid draw
                        for(var i=0; i<_eqp.gainRange/yGridSize; i++)
                        {
                            if(i == _eqp.gainRange/yGridSize/2)
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
                            ctx.fillText((_eqp.gainRange/2 - yGridSize*i)+"dB", 2, ypos-4);
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

                        for(i=0; i<module.points.length; i++)
                        {
                            x = _canvas.width*((Math.log10(points[i].x)-Math.log10(xmin))
                                               /(Math.log10(xmax)-Math.log10(xmin)));
                            y = points[i].y*coefY

                            ctx.lineTo(x, -y);
                        }
                        ctx.stroke()

                        // band draw
                        xmin = module.EqBands[_eqp.currentBandIndex].bandPoints[0].x;
                        xmax = module.EqBands[_eqp.currentBandIndex].bandPoints[module.EqBands[_eqp.currentBandIndex].bandPoints.length-1].x
                        ctx.lineWidth = 1;
                        ctx.strokeStyle = "salmon"
                        ctx.fillStyle = "salmon";
                        ctx.globalAlpha = 0.4;

                        ctx.beginPath();
                        ctx.moveTo(xmax,0)
                        ctx.lineTo(0,0)
                        for(i=0; i<module.EqBands[_eqp.currentBandIndex].bandPoints.length; i++)
                        {
                            x = _canvas.width*((Math.log10(module.EqBands[_eqp.currentBandIndex].bandPoints[i].x)-Math.log10(xmin))
                                               /(Math.log10(xmax)-Math.log10(xmin)));
                            y = module.EqBands[_eqp.currentBandIndex].bandPoints[i].y*coefY

                            ctx.lineTo(x, -y);
                        }
                        ctx.fill()
                    }
                }

                Repeater{
                   id: repeater

                   anchors.fill: parent
                   delegate: EqPoint{
                       width: Math.min(parent.width/6, parent.height/6)
                       height: width

                       eqBand: main.module.EqBands[index]
                       gainRange: _eqp.gainRange
                       selectedBandIndex: _eqp.currentBandIndex

                       Component.onCompleted: {
                           pointSelected.connect(_eqp.bandSelected)
                       }
                   }

                   model: module.EqBands
                }

                DropArea{
                    id: dropArea

                    anchors.fill: parent


                    onPositionChanged: function(drag){
                        var xmin = module.points[0].x;
                        var xmax = module.points[module.points.length-1].x;
                        var pointRadius = repeater.itemAt(_eqp.currentBandIndex).height/2;

                        var freq = Math.pow(10, (drag.source.x+pointRadius)/_canvas.width * (Math.log10(xmax)-Math.log10(xmin)) + Math.log10(xmin))
                        var gain = (-drag.source.y + _canvas.height/2 - pointRadius) * _eqp.gainRange/_canvas.height;

                        module.EqBands[_eqp.currentBandIndex].gain.displayValue = Math.round(gain);
                        module.EqBands[_eqp.currentBandIndex].Fc.displayValue = Math.round(freq);
                    }
                }
            }

            Rectangle{
                id: bandInfoRect

                width: parent.width
                height: parent.height * 2/8

                z:-1

                color: "transparent"

                border.width: 2
                border.color: (main.on) ? Style.currentTheme.textEnabled : Style.currentTheme.textDisabled

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
                            color: (main.on) ? Style.currentTheme.textEnabled : Style.currentTheme.textDisabled
                            font.pixelSize: parent.width/7

                            text: "BAND " + (_eqp.currentBandIndex + 1) + "\n" +
                                  module.EqBands[_eqp.currentBandIndex].Fc.minDisplayValue + "Hz\n" +
                                  module.EqBands[_eqp.currentBandIndex].Fc.maxDisplayValue + "Hz\n"
                        }
                    }

                    ParameterDial{
                        id: dialFreq
                        width: parent.width/3.5
                        height: parent.height*0.85
                        anchors.verticalCenter: parent.verticalCenter

                        property EqParametric module: main.module

                        controlValue: module.EqBands[_eqp.currentBandIndex].Fc
                    }
                    ParameterDial{
                        id: dialGain
                        width: parent.width/3.5
                        height: parent.height*0.85
                        anchors.verticalCenter: parent.verticalCenter

                        visible: (_eqp.currentBandIndex !== 5) & (_eqp.currentBandIndex !== 6)

                        property EqParametric module: main.module

                        controlValue: module.EqBands[_eqp.currentBandIndex].gain
                    }
                    ParameterDial{
                        id: dialQ
                        width: parent.width/3.5
                        height: parent.height*0.85
                        anchors.verticalCenter: parent.verticalCenter

                        visible: (_eqp.currentBandIndex !== 5) & (_eqp.currentBandIndex !== 6)

                        floatDigits: 2

                        property EqParametric module: main.module

                        step: 0.1
                        controlValue: module.EqBands[_eqp.currentBandIndex].Q
                    }
                }
            }
        }


        function bandSelected(index){
            _eqp.currentBandIndex = index
            _eqp.update();
        }

        Connections{
            target: module

            function onPointsChanged()
            {
                _eqp.update();
            }

            function onModuleEnabledChanged()
            {
                _eqp.update();
            }
        }
    }
}
