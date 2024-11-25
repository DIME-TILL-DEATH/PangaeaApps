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

    moduleDescription: qsTr("Equalizer")

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

                        onPaint: {
                            var xmin = module.points[0].x;
                            var xmax = module.points[module.points.length-1].x

                            var ctx = getContext("2d");

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
                                x = _canvas.width*((Math.log10(module.points[i].x)-Math.log10(xmin))
                                                   /(Math.log10(xmax)-Math.log10(xmin)));
                                y = module.points[i].y*coefY

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
                    id: _prePostEQ

                    visible: (UiCore.currentDevice.isPaFw !== undefined) ? UiCore.currentDevice.isPaFw : false

                    height: parent.height
                    width: parent.width - canvasBackground.width - _separator.width

                    MouseArea{
                       anchors.fill: parent
                       enabled: isPrePostVisible

                        onClicked: {
                            UiCore.currentDevice.isPreEq = !isPreEQ
                        }
                    }

                    Column
                    {
                        visible: isPrePostVisible
                        anchors.fill: parent
                        MText
                        {
                            height: parent.height/2
                            width: parent.width

                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                            text: qsTr("PRE")

                            color: isPreEQ ? (on ? Style.currentTheme.colorLedOn : Style.currentTheme.colorTextEnabled)
                                           : (on ? Style.currentTheme.colorTextDisabled : Style.currentTheme.colorModulOff)
                            font.bold: true
                        }

                        MText
                        {
                            height: parent.height/2
                            width: parent.width

                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                            text: qsTr("POST")
                            color: isPreEQ ? (on ? Style.currentTheme.colorTextDisabled : Style.currentTheme.colorModulOff)
                                           : (on ? Style.currentTheme.colorLedOn : Style.currentTheme.colorTextEnabled)

                            font.bold: true
                        }
                    }
                }
            }
        }

    }

    tutorialItem: SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Parametric EQ")

        text: qsTr("This effect block provides 5-band parametric equalizer for fine-tuning your tone.

Parameters for each band:

FREQUENCY: Central frequency of the band.

GAIN: Amplification/attenuation factor of the band.

Q(quality): Bandwidth - the area around the set frequency that the EQ will amplify or attenuate.");
    }
}

