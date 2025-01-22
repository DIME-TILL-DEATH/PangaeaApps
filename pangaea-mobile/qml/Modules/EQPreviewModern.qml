import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0
import Tutorials

import CppObjects


BaseModule{
    id: main

    signal extVisible()

    property bool isPrePostVisible: true

    property bool isPreEQ: (UiCore.currentDevice.isPreEq !== undefined) ? UiCore.currentDevice.isPreEq : false

    property EqParametric module

    contentItem: Row
    {
        id: _content
        spacing: 0

        Item{
            height: parent.height
            width: parent.width

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    extVisible();
                }
            }

            Row
            {
                anchors.fill: parent

                spacing: 2

                Rectangle{
                    id: canvasBackground

                    height: parent.height
                    width: parent.width * 4/5 //+ Style.mainSpacing * 2

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
                        }

                        function onModuleEnabledChanged()
                        {
                            _canvas.requestPaint();
                        }
                    }
                }

                Rectangle{
                    id: _separator

                    width: Style.mainSpacing
                    height: parent.height

                    color: main.on ? Style.currentTheme.colorBorderOn : Style.currentTheme.colorTextDisabled
                }

                Item{
                    height: parent.height
                    width: parent.width - canvasBackground.width - _separator.width

                    Column
                    {
                        width: parent.width
                        height: parent.height*0.9

                        anchors.centerIn: parent
                        spacing: parent.height*0.1
                        MButton
                        {
                            width: parent.width * 0.9
                            height: parent.height * 0.45

                            anchors.horizontalCenter: parent.horizontalCenter

                            textButton: "HP"
                            textColor: module.hpf.enabled? Style.colorText : Style.currentTheme.colorTextDisabled

                            highlightColor: "blue"

                            highlighted: module.hpf.enabled
                            onMbPressed: module.hpf.enabled = !module.hpf.enabled
                        }

                        MButton
                        {
                            width: parent.width * 0.9
                            height: parent.height * 0.45

                            anchors.horizontalCenter: parent.horizontalCenter

                            textButton: "LP"
                            textColor: module.lpf.enabled ? Style.colorText : Style.currentTheme.colorTextDisabled

                            highlightColor: "green"

                            highlighted: module.lpf.enabled
                            onMbPressed: module.lpf.enabled = !module.lpf.enabled
                        }
                    }
                }
            }
        }

    }

    tutorialItem: TutorialEQ{}
}
