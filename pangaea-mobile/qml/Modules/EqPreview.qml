import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    signal extVisible()

    // property int paramType: DeviceParameter.EQ_ON

    property bool on: EqResponse.moduleEnabled
    property bool isPrePostVisible: true

    property bool isPreEQ: true

    BaseModule{
        id: _baseModule

        module: EqResponse
        // moduleName: qsTr("EQ")
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
                        width: parent.width*8/10

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
                                var xmin = EqResponse.points[0].x;
                                var xmax = EqResponse.points[EqResponse.points.length-1].x

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

                                for(i=0; i<EqResponse.points.length; i++)
                                {
                                    x = _canvas.width*((Math.log10(EqResponse.points[i].x)-Math.log10(xmin))
                                                       /(Math.log10(xmax)-Math.log10(xmin)));
                                    y = EqResponse.points[i].y*coefY

                                    ctx.lineTo(x, -y);
                                }
                                ctx.stroke()
                            }
                        }

                        Connections{
                            target: EqResponse

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

                    Item{
                        id: _prePostEQ

                        height: parent.height
                        width: parent.width*2/10

                        MouseArea{
                           anchors.fill: parent
                           enabled: isPrePostVisible

                            onClicked: {
                                isPreEQ = !isPreEQ
                                UiCore.setDeviceParameter(DeviceParameter.EQ_PRE, isPreEQ);
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
    }

    // Connections
    // {
    //     target: UiCore
    //     function onSgSetUiDeviceParameter(paramType, value)
    //     {
    //         if(paramType === DeviceParameter.EQ_PRE)
    //         {
    //             isPreEQ=value
    //         }
    //     }
    // }
}
