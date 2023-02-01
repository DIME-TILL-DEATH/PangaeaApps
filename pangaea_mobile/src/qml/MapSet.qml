import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2

import "modules/"
import "elements/"

Item
{
    id: main

    property color colorFon: "#2B2A29"
    property color colorModul: "#5B5B5B"
    property color colorSlider: "#769897"
    property color colorModulOn: "#FEFEFE"
    property color colorModulOff: "#009846"
    property color colorText: "#FFFFFF"
    property color colorTextActive: "#009846"
    property int  presetNom: bank.value*4+preset.value
    property bool copyFirst: false
    property bool wait: false
    property bool edit: false
    property bool editable:false
    property string devName: "CP-16"
    property string devVersion: "CP-16"
    property int mode: 0
    property string  lastSelectFile: ""
    property string  fwSelectFile: ""
    property string  version: "1.3"
    property string  firmwareVersion: "2.3"
    property string  poVersion: devName + " Version: " + version + " Firmware version: " + firmwareVersion


    property int countElements: 28
    //    anchors.fill: parent

    Rectangle
    {
        anchors.fill: parent
        color: colorFon
    }

    Column
    {
        id: modulesColumn
        width: parent.width
//        Item
//        {
//            width:  parent.width
//            height: main.height*1/countElements
//        }
        Item
        {
            width:  parent.width
            height: main.height*5/countElements
            Row
            {
                anchors.fill: parent
                Item
                {
                    width:  parent.width*1/6
                    height: parent.height
                    Presets
                    {
                        id: bank
                        anchors.fill: parent
                        text: "BANK"
                        nameValue: "bank"
                        onChPreset:
                        {
                            _core.slSetValue("preset_change", presetNom);
//                            console.log(bank.value);
                        }
                    }
                }
                Item
                {
                    width:  parent.width*4/6
                    height: parent.height
                    Row
                    {
                        anchors.fill: parent
                        Item
                        {
//                            anchors.horizontalCenter: parent.horizontalCenter
//                            anchors.verticalCenter: parent.verticalCenter
                            width:  parent.width/10*4 //45*10*11/10
                            height: parent.height/15*10*11/10
                            anchors.bottom: parent.bottom
                            Column
                            {
                                anchors.fill: parent
                                MButton
                                {
                                    width:  parent.width
                                    height: parent.height/3
                                    textButton: "Phones"
                                    active: (mode==0)
                                    onMbPressed:
                                    {
                                        mode = 0;
                                        _core.slSetValue("mode", mode);
                                    }
                                }

                                MButton
                                {
                                    width:  parent.width
                                    height: parent.height/3
                                    textButton: "Balance"
                                    active: (mode==2)
                                    onMbPressed:
                                    {
                                        mode = 2;
                                        _core.slSetValue("mode", mode);
                                    }
                                }

                                MButton
                                {
                                    width:  parent.width
                                    height: parent.height/3
                                    textButton: "Line"
                                    active: (mode==1)
                                    onMbPressed:
                                    {
                                        mode = 1;
                                        _core.slSetValue("mode", mode);
                                    }
                                }
                            }
                        }

                        Item
                        {
//                            anchors.horizontalCenter: parent.horizontalCenter
//                            anchors.verticalCenter: parent.verticalCenter
                            width:  parent.width/10*3 //45*10*11/10
                            height: parent.height/15*10*11/10
                            anchors.bottom: parent.bottom
                            Column
                            {
                                anchors.fill: parent
                                MButton
                                {
                                    enabled: main.edit //true //main.editable //& //main.edit //
                                    textButton: "Save"
                                    onMbPressed:
                                    {
                                        console.log("23425");
                                        _core.slSetValue("save_change", (-1));
                                    }
                                }
                                MButton
                                {
                                    enabled: main.edit
                                    textButton: "Comp"
                                    onMbPressed: _core.slSetValue("comp", 0);
                                }
                            }
                        }

                        Item
                        {
//                            anchors.left: parent.horizontalCenter
//                            anchors.leftMargin: parent.width/2/45*9
//                            anchors.horizontalCenter: parent.horizontalCenter
//                            anchors.verticalCenter: parent.verticalCenter
                            width:  parent.width/10*3 //45*10*11/10
                            height: parent.height/15*10*11/10
                            anchors.bottom: parent.bottom
                            Column
                            {
                                anchors.fill: parent
                                MButton
                                {
                                    textButton: "Copy"
                                    enabled: editable
                                    onMbPressed:
                                    {
                                        _core.slSetValue("copy", 0);
                                        copyFirst=true;
                                    }
                                }
                                MButton
                                {
                                    textButton: "Paste"
                                    enabled: editable & copyFirst
                                    onMbPressed:
                                    {
                                        _core.slSetValue("past", 0)
                                    }
                                }
                            }
                        }
                    }
                }
                Item
                {
                    width:  parent.width*1/6
                    height: parent.height
                    Presets
                    {
                        id: preset
                        anchors.fill: parent
                        text: "PRESET"
                        nameValue: "preset"
                        onChPreset:
                        {
                            _core.slSetValue("preset_change", presetNom);
                        }
                    }
                }
            }
        }
        Item
        {
            width:  parent.width
            height: main.height*1/countElements
        }
        Ng
        {
            colorFon: main.colorFon
            colorModul: main.colorModul
            colorSlider: main.colorSlider
            colorModulOn: main.colorModulOn
            colorModulOff: main.colorModulOff

            width:  parent.width
            height: main.height*2/countElements
        }
        Cm
        {
            width:  parent.width
            height: main.height*2/countElements
        }
        Pr
        {
            width:  parent.width
            height: main.height*4/countElements
        }
        Pa
        {
            colorFon: main.colorFon
            colorModul: main.colorModul
            colorSlider: main.colorSlider
            colorModulOn: main.colorModulOn
            colorModulOff: main.colorModulOff

            width: parent.width
            height: main.height*4/countElements

        }
        Ir // Здесь должна быть IR!
        {
            id: ir
            name: "IR"
            width:  parent.width
            height: main.height*1/countElements
            onGetImpuls:
            {
                   timerImpuls.start();
                   fileOpen.open();
            }
        }
        Hr
        {
            width:  parent.width
            height: main.height*1/countElements
        }

        Eqs // Здесь должна быть EQ!
        {
            id: eqs
            eq1: eqsExt.eq1
            eq2: eqsExt.eq2
            eq3: eqsExt.eq3
            eq4: eqsExt.eq4
            eq5: eqsExt.eq5
            name: "EQ"
            width:  parent.width
            height: main.height*3/countElements
            onExtVisible:
            {
                eqsExt.visible = true;
            }
        }
        Lp
        {
            width:  parent.width
            height: main.height*1/countElements
        }
        Ps
        {
            width:  parent.width
            height: main.height*1/countElements
        }
        Er
        {
            width:  parent.width
            height: main.height*2/countElements
        }
        Vl
        {
            width:  parent.width
            height: main.height*1/countElements
        }


        //            Rectangle
        //            {
        //                width:  parent.width
        //                height: main.height*8/10
        //                color: "black"
        //            }
    }
    EqsExt
    {
        id: eqsExt
        visible: false
        anchors.centerIn: parent
        height: parent.height/2
        width:  parent.width/5*4
        onHide:
        {
            visible = false;
        }
    }

//    Rectangle
//    {
//        height: 30
//        width: parent.width
//        color: "blue"
//    }
/*
        Row
        {
            height: parent.height
            width: parent.width
            Item
            {
                height: parent.height
                width: parent.width/5*2
                MText
                {
                    text: poVersion
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Item
            {
                height: parent.height
                width: parent.width/5*1
                Rectangle
                {
                    anchors.fill: parent
                    color: "red"
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            _core.slSetValue("format", 0);
                        }
                    }
                }
            }
            Item
            {
                height: parent.height
                width: parent.width/5*1
                Rectangle
                {
                    anchors.fill: parent
                    color: "green"
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            fileFirmWare.setVisible(true);
                        }
                    }
                }
            }

            Item
            {
                height: parent.height
                width: parent.width/5*1
                Rectangle
                {
                    anchors.fill: parent
                    color: "blue"
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            _core.slSendTest();
                        }
                    }
                }
            }
        }
    }
*/

    Dialog
    {
        id: msgPresetUpDownSave
        property int saveParam: 0
        title: "Save preset"
        standardButtons: StandardButton.Save | StandardButton.No | StandardButton.Cancel
        onAccepted: _core.slSetValue("save_up_down", saveParam)
        onNo:       _core.slSetValue("set_preset_updown", saveParam)
    }
    Dialog
    {
        id: msgPresetChangeSave
        property int saveParam: 0
        title: "Save preset"
        standardButtons: StandardButton.Save | StandardButton.No | StandardButton.Cancel
        onAccepted: _core.slSetValue("save_change", saveParam);
        onNo:
        {
//            console.log("saveParam", saveParam);
            if(saveParam==(-2))
                _core.slSetValue("esc", 0);
            else
                _core.slSetValue("set_preset_change", saveParam);
        }
        onRejected: saveParam = 0;
    }

    Timer
    {
        id: timerImpuls
        interval: 250
        repeat: true
//        running: fileOpen.visible
        onTriggered:
        {
//            ir.setName(fileOpen.curentFile)

//            ir.setName(fileOpen.fileUrl);
//            ir.setName(fileOpen.curentFile);

//            ir.setName("1212");



            if((lastSelectFile!=fileOpen.fileUrl))// || (lastSelectFile!==fileOpen.curentFile))
            {
                console.log("Start impuls", lastSelectFile, fileOpen.fileUrl);
                var cleanPath;
                ir.setName("0");
                lastSelectFile=fileOpen.fileUrl;
                cleanPath = (Qt.platform.os=="windows")?decodeURIComponent(lastSelectFile.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(lastSelectFile.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));
                _core.setImpuls(cleanPath);
            }
        }
    }

//    MFileOpen
    FileDialog
    {
        id: fileOpen
        onVisibleChanged:
        {
            lastSelectFile = "";
            console.log("!!!!!!!!!!!!!!!!!!!!!!!!!!! onVisibleChanged ");
        }
        onAccepted:
        {
//            moduls.irEnable(true);
//            ir.setName(fileUrl);
        }
        onRejected:
        {
            _core.slEscImpuls();
        }
    }

    FileDialog
    {
        id: fileFirmWare
        onAccepted:
        {
            var cleanPath;
            fwSelectFile = fileFirmWare.fileUrl;
            cleanPath = (Qt.platform.os=="windows")?decodeURIComponent(fwSelectFile.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(fwSelectFile.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));
            _core.setFirmware(cleanPath);
        }
        onRejected:
        {
//            _core.slEscImpuls();
        }
    }


    Dialog
    {
        id: msgAnswerError
        standardButtons: StandardButton.Ok
        width: parent.width/1.2
    }

    MessageDialog
    {
        id: msgAnswerErrSave
        standardButtons: MessageDialog.Ok
        visible: false
        detailedText: ""
        text: "Exchange error"
    }

    MBusy
    {
        id: mBusy
        busy: main.wait
        z:1
    }

    function startUpdateFW()
    {
        fileFirmWare.setVisible(true);
    }

    Connections
    {
        target: _core
        onSignal: console.log("The application data changed!");

        onSgSetProgress:
        {
            console.log(val);
            mBusy.pbValue = val;
        }
        onSgNotSupport:
        {
//            main.color = "#FF5050";
//            timerColor.start();
        }
        onSgAnswerErrSave:
        {
            msgAnswerErrSave.visible = true;
            msgAnswerErrSave.detailedText = "Command: " + strCom;
        }
        onSgPresetUpDownStage1:
        {
            msgPresetUpDownSave.saveParam = inChangePreset;
            msgPresetUpDownSave.visible = true;
        }
        onSgPresetChangeStage1:
        {
            msgPresetChangeSave.saveParam = inChangePreset;
            msgPresetChangeSave.visible = true;
        }
        onSgReadText:
        {
            if(nameParam==("open_port"))
            {
                connect = true;
                msgAnswerError.close();
            }
            if(nameParam==("close_port"))
            {
                connect = false;
                msgAnswerError.title = markConnect;
                msgAnswerError.visible = true;
                main.editable = false;
            }
            if(nameParam==("devVersion"))
            {
                devVersion = value;
            }
        }
        onSgReadValue:
        {
            if(nameParam==("mode"))
                mode = inValue;

            if(nameParam==("preset_edit"))
                edit = inValue;
            if(nameParam==("wait"))
            {
                console.log("wait inWait=",inValue, msgPresetChangeSave.saveParam)
                if(inValue)
                {
                    main.wait = true;
                }
                else
                {
                    main.wait = false;
                }

                if(msgPresetChangeSave.saveParam==(-2))
                {
                    if(!inValue && (!msgPresetChangeSave.visible) && (msgPresetChangeSave.saveParam==(-2)))
                        Qt.quit();
                }
            }
            if(nameParam=="editable")
                main.editable=inValue

            if(nameParam==("type_dev"))
            {
                switch (inValue)
                {
                case 0: devName = "";  break;
                case 1: devName = "CP-100";  break;
                case 2: devName = "CP-16M";  break;
                case 3: devName = "CP-16PA"; break;
                case 4: devName = "CP-100PA"; break;
                }
            }
        }
    }

    //    onClosing:
    //    {
    //        if(main.edit && main.connect)
    //        {
    //            msgPresetChangeSave.saveParam = (-2);
    //            msgPresetChangeSave.visible = true;
    //            close.accepted = false;
    //        }
    //    }

}
