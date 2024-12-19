pragma ComponentBehavior: Bound

import QtQuick 2.15
import QtQuick.Controls 2.12

import QtQuick.Layouts

import StyleSettings 1.0

import Elements 1.0

import CppObjects
import CppEnums

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
            height: parent.height/10*9
            anchors.centerIn: parent

            spacing: height/45
            Item {
                width: parent.width
                height: parent.height/15

                MButton{
                    textButton: "EQ ON"
                    textColor: main.on ? Style.colorText : Style.currentTheme.colorTextDisabled

                    width: parent.width * 2/5
                    height: parent.height
                    anchors.left: parent.left

                    highlighted: main.on

                    onMbPressed: eqModule.moduleEnabled = !eqModule.moduleEnabled
                }

                MButton{
                    textButton: qsTr("RESET")

                    width: parent.width * 2/5
                    height: parent.height
                    anchors.right: parent.right

                    onMbPressed: eqModule.reset()
                }
            }

            Item {
                width: parent.width
                height: parent.height*4/10
                Canvas{
                    id: _canvas

                    anchors.fill: parent

                    renderStrategy: Canvas.Threaded

                    property int yGridSize: 5
                    property real coefY : height / main.gainRange

                    property real xmin: main.eqModule.minFreq;
                    property real xmax: main.eqModule.maxFreq;

                    onPaint: {

                        var points = main.eqModule.points; // One access more efficient

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
                            x = _canvas.width*((Math.log10(points[i].x)-Math.log10(xmin))
                                               /(Math.log10(xmax)-Math.log10(xmin)));
                            y = points[i].y*coefY

                            ctx.lineTo(x, -y);
                        }
                        ctx.stroke()

                        // band draw
                        ctx.lineWidth = 1;
                        ctx.strokeStyle = "salmon"
                        ctx.fillStyle = "salmon";
                        ctx.globalAlpha = 0.4;

                        ctx.beginPath();
                        ctx.moveTo(xmax,0)
                        ctx.lineTo(0,0)

                        var bandPoints = eqModule.EqBands[currentBandIndex].bandPoints
                        for(i=0; i<bandPoints.length; i++)
                        {
                            x = _canvas.width*((Math.log10(bandPoints[i].x)-Math.log10(xmin))
                                               /(Math.log10(xmax)-Math.log10(xmin)));
                            y = bandPoints[i].y*coefY

                            ctx.lineTo(x, -y);
                        }
                        ctx.fill()
                    }
                }

                Repeater{
                   id: repeater

                   anchors.fill: parent
                   delegate: EqPoint{
                       eqBand: main.eqModule.EqBands[index]
                       gainRange: main.gainRange
                       selectedBandIndex: main.currentBandIndex

                       Component.onCompleted: {
                           pointSelected.connect(main.bandSelected)
                       }
                   }

                   model: main.eqModule.EqBands
                }

                DropArea{
                    id: dropArea

                    anchors.fill: parent

                    property var xmin: main.eqModule.minFreq;
                    property var xmax: main.eqModule.maxFreq;

                    onPositionChanged: function(drag){

                        var pointRadius = repeater.itemAt(main.currentBandIndex).height/2;

                        var freq = Math.pow(10, (drag.source.x+pointRadius)/_canvas.width * (Math.log10(xmax)-Math.log10(xmin)) + Math.log10(xmin))
                        var gain = (-drag.source.y + _canvas.height/2 - pointRadius) * main.gainRange/_canvas.height;

                        if(gain > 15) gain = 15;
                        if(gain < -15) gain = -15;

                        eqModule.EqBands[currentBandIndex].gain.displayValue = Math.round(gain);
                        eqModule.EqBands[currentBandIndex].Fc.displayValue = Math.round(freq);

                        // console.log("Drag update: ", freq, gain)
                    }
                }


                PinchArea{
                    id: pinchArea

                    anchors.fill: parent
                    pinch.target: _canvas
                    pinch.dragAxis: Pinch.XAxis

                    z: repeater.z-1

                    property var startPinchQ

                    enabled: (main.eqModule.EqBands[currentBandIndex].filterType !== FilterType.HIGH_CUT) &
                             (eqModule.EqBands[currentBandIndex].filterType !== FilterType.LOW_CUT)

                    onPinchStarted: {
                        startPinchQ = main.eqModule.EqBands[currentBandIndex].Q.displayValue;
                    }

                    onPinchUpdated: function(pinch){
                        var resultQ = startPinchQ / pinch.scale;
                        if(resultQ > eqModule.EqBands[currentBandIndex].Q.maxDisplayValue) resultQ = eqModule.EqBands[currentBandIndex].Q.maxDisplayValue;
                        if(resultQ < eqModule.EqBands[currentBandIndex].Q.minDisplayValue) resultQ = eqModule.EqBands[currentBandIndex].Q.minDisplayValue;

                        eqModule.EqBands[currentBandIndex].Q.displayValue = resultQ
                    }
                }
            }

            GridLayout{
                width: parent.width
                height: parent.height/7.5

                Layout.preferredHeight: parent.height/2
                Layout.preferredWidth: parent.width/5

                rows: 2
                columns: 5

                MButton{
                    textButton: "HP"
                    textColor: main.eqModule.EqBands[5].enabled ? Style.colorText : Style.currentTheme.colorTextDisabled

                    highlightColor: "blue"

                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height/2.1
                    Layout.columnSpan: 2

                    highlighted: main.currentBandIndex === 5
                    onMbPressed: main.bandSelected(5)
                }

                Item{
                    Layout.preferredWidth: parent.width/6
                    Layout.preferredHeight: parent.height/2.1
                }

                MButton{
                    textButton: "LP"
                    textColor: eqModule.EqBands[6].enabled ? Style.colorText : Style.currentTheme.colorTextDisabled

                    highlightColor: "green"

                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height/2.1
                    Layout.columnSpan: 2

                    highlighted: main.currentBandIndex === 6
                    onMbPressed: main.bandSelected(6)
                }

                MButton{
                    textColor: main.on ? Style.colorText : Style.currentTheme.colorTextDisabled

                    Layout.fillWidth: true
                    Layout.preferredWidth: parent.width/6
                    Layout.preferredHeight: parent.height/2.1
                    textButton: "B1"

                    highlighted: main.currentBandIndex === 0
                    onMbPressed: main.bandSelected(0)
                }
                MButton{
                    textColor: main.on ? Style.colorText : Style.currentTheme.colorTextDisabled

                    Layout.fillWidth: true
                    Layout.preferredWidth: parent.width/6
                    Layout.preferredHeight: parent.height/2.1
                    textButton: "B2"

                    highlighted: main.currentBandIndex === 1
                    onMbPressed: main.bandSelected(1)
                }
                MButton{
                    textColor: main.on ? Style.colorText : Style.currentTheme.colorTextDisabled

                    Layout.fillWidth: true
                    Layout.preferredWidth: parent.width/6
                    Layout.preferredHeight: parent.height/2.1
                    textButton: "B3"

                    highlighted: main.currentBandIndex === 2
                    onMbPressed: main.bandSelected(2)
                }
                MButton{
                    textColor: main.on ? Style.colorText : Style.currentTheme.colorTextDisabled

                    Layout.fillWidth: true
                    Layout.preferredWidth: parent.width/6
                    Layout.preferredHeight: parent.height/2.1
                    textButton: "B4"

                    highlighted: main.currentBandIndex === 3
                    onMbPressed: main.bandSelected(3)
                }
                MButton{
                    textColor: main.on ? Style.colorText : Style.currentTheme.colorTextDisabled

                    Layout.fillWidth: true
                    Layout.preferredWidth: parent.width/6
                    Layout.preferredHeight: parent.height/2.1
                    textButton: "B5"

                    highlighted: main.currentBandIndex === 4
                    onMbPressed: main.bandSelected(4)
                }
            }

            Loader{
                id: loaderBandControl

                width: parent.width
                height: parent.height*4/15
            }

            Button
            {
                id: btn

                width: parent.width
                height: parent.height/15

                text: qsTr("HIDE")

                onClicked:
                {
                    main.hide();
                }
            }
        }
    }

    function bandSelected(index)
    {
        currentBandIndex = index
        switch(currentBandIndex)
        {
        case 0:
            loaderBandControl.source = "/ControlGroups/qml/ControlGroups/BPeakingShelfControl.qml";
            loaderBandControl.item.shelfType = FilterType.LOW_SHELF
            break;
        case 4:
            loaderBandControl.source = "/ControlGroups/qml/ControlGroups/BPeakingShelfControl.qml";
            loaderBandControl.item.shelfType = FilterType.HIGH_SHELF
            break;
        case 5:
            loaderBandControl.source = "/ControlGroups/qml/ControlGroups/BCutControl.qml";
            break;
        case 6:
            loaderBandControl.source = "/ControlGroups/qml/ControlGroups/BCutControl.qml";
            break;
        default:
            loaderBandControl.source = "/ControlGroups/qml/ControlGroups/BPeakingControl.qml";
        }
        loaderBandControl.item.eqBand = eqModule.EqBands[currentBandIndex];

        main.update();
    }

    Component.onCompleted: {
        bandSelected(0);
    }

    Keys.onReleased: function(event)
    {
        if(event.key === Qt.Key_Back)
        {
            main.hide();
        }
    }

    Connections{
        target: main.eqModule

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
