import QtQuick 2.15
import QtQuick.Controls as Controls

import StyleSettings

import Elements

import CppObjects
import PangaeaBackend

BaseModule{
    id: main

    isHeaderVisible: false

    property EqParametric module

    contentItem: Rectangle{
        id: _eqp

        // property bool on: module.moduleEnabled

        property int currentBandIndex

        property int gainRange: 40

        color: main.on ? Style.mainEnabledColor : Style.mainDisabledColor


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

                    onClicked: bandSelected(5)
                }

                MButton{
                    text: "1"

                    width: parent.width * 1/10
                    height: parent.height * 0.9

                    scaleText: 2

                    highlighted: _eqp.currentBandIndex === 0

                    onClicked: bandSelected(0)
                }

                MButton{
                    text: "2"

                    width: parent.width * 1/10
                    height: parent.height * 0.9

                    scaleText: 2

                    highlighted: _eqp.currentBandIndex === 1

                    onClicked: bandSelected(1)
                }

                MButton{
                    text: "3"

                    width: parent.width * 1/10
                    height: parent.height * 0.9

                    scaleText: 2

                    highlighted: _eqp.currentBandIndex === 2

                    onClicked: bandSelected(2)
                }

                MButton{
                    text: "4"

                    width: parent.width * 1/10
                    height: parent.height * 0.9

                    scaleText: 2

                    highlighted: _eqp.currentBandIndex === 3

                    onClicked: bandSelected(3)
                }

                MButton{
                    text: "5"

                    width: parent.width * 1/10
                    height: parent.height * 0.9

                    scaleText: 2

                    highlighted: _eqp.currentBandIndex === 4

                    onClicked: bandSelected(4)
                }

                MButton{
                    text: "LP"

                    width: parent.width * 1/10
                    height: parent.height * 0.9

                    scaleText: 2

                    highlightColor: "green"
                    highlighted: _eqp.currentBandIndex === 6

                    onClicked: bandSelected(6)
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
                    property real coefY : height / gainRange

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

                        for(i=0; i<module.points.length; i++)
                        {
                            x = _canvas.width*((Math.log10(points[i].x)-Math.log10(xmin))
                                               /(Math.log10(xmax)-Math.log10(xmin)));
                            y = points[i].y*coefY

                            ctx.lineTo(x, -y);
                        }
                        ctx.stroke()

                        // band draw
                        xmin = module.EqBands[currentBandIndex].bandPoints[0].x;
                        xmax = module.EqBands[currentBandIndex].bandPoints[module.EqBands[currentBandIndex].bandPoints.length-1].x
                        ctx.lineWidth = 1;
                        ctx.strokeStyle = "salmon"
                        ctx.fillStyle = "salmon";
                        ctx.globalAlpha = 0.4;

                        ctx.beginPath();
                        ctx.moveTo(xmax,0)
                        ctx.lineTo(0,0)
                        for(i=0; i<module.EqBands[currentBandIndex].bandPoints.length; i++)
                        {
                            x = _canvas.width*((Math.log10(module.EqBands[currentBandIndex].bandPoints[i].x)-Math.log10(xmin))
                                               /(Math.log10(xmax)-Math.log10(xmin)));
                            y = module.EqBands[currentBandIndex].bandPoints[i].y*coefY

                            ctx.lineTo(x, -y);
                        }
                        ctx.fill()
                    }
                }

                Repeater{
                   id: repeater

                   anchors.fill: parent
                   delegate: EqPoint{
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
                        var pointRadius = repeater.itemAt(currentBandIndex).height/2;

                        var freq = Math.pow(10, (drag.source.x+pointRadius)/_canvas.width * (Math.log10(xmax)-Math.log10(xmin)) + Math.log10(xmin))
                        var gain = (-drag.source.y + _canvas.height/2 - pointRadius) * _eqp.gainRange/_canvas.height;

                        module.EqBands[currentBandIndex].gain.displayValue = Math.round(gain);
                        module.EqBands[currentBandIndex].Fc.displayValue = Math.round(freq);
                    }
                }
            }

            Loader{
                id: loaderBandControl

                width: parent.width
                height: parent.height * 1/4
            }
        }


        function bandSelected(index){
            currentBandIndex = index

            switch(currentBandIndex)
            {
            case 0:
                loaderBandControl.source = "../Layouts/BPeakingShelfControl.qml";
                loaderBandControl.item.shelfType = FilterType.LOW_SHELF
                break;
            case 4:
                loaderBandControl.source = "../Layouts/BPeakingShelfControl.qml";
                loaderBandControl.item.shelfType = FilterType.HIGH_SHELF
                break;
            case 5:
                loaderBandControl.source = "../Layouts/BCutControl.qml";
                break;
            case 6:
                loaderBandControl.source = "../Layouts/BCutControl.qml";
                break;
            default:
                loaderBandControl.source = "../Layouts/BPeakingControl.qml";
            }
            loaderBandControl.item.eqBand = module.EqBands[currentBandIndex];

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

        Connections{
            target: main

            function onModuleChanged() {
               bandSelected(0)
            }
        }
    }

}
