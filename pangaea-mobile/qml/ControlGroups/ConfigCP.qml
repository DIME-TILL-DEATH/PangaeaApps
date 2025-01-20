// pragma ComponentBehavior: Bound

import QtQuick 2.15
// import QtQuick.Effects
import QtQuick.Controls

import Elements
import StyleSettings
import CustomOverlays

import CppObjects
import CppEnums

Item {
    id: _main

    enabled: UiCore.currentDevice.presetManager.currentState !== PresetState.Compare

    property alias modulesModel: _visualModel.model

    Rectangle
    {
        anchors.fill: parent
        color: Style.colorFon
    }

    Rectangle{
        id: _maskCompare

        anchors.fill: parent

        opacity: 0.25
        visible: !_main.enabled
        z: _modulesManagementWindow.z + 5

        SequentialAnimation on color{
            running: true
            loops: Animation.Infinite
            ColorAnimation {from: "white"; to: "lightcoral"; duration: 1000}
            ColorAnimation {from: "lightcoral"; to: "white"; duration: 1000}
        }
    }

    Column{
        id: _column

        anchors.fill: parent

        spacing: 4

        Button{
            id: _btnAddModule

            text: qsTr("Add/Remove module")

            width: parent.width/2
            height: parent.height/12

            anchors.horizontalCenter: parent.horizontalCenter

            onClicked:{
                 _modulesManagementWindow.visible = true
            }
        }

        Rectangle{
            id: _rectConf

            width: parent.width
            height: parent.height * 1/15

            property bool confAcceplatble: UiCore.currentDevice.processingUsed < UiCore.currentDevice.processingBudget
            radius: Style.baseRadius
            border.width: 1
            border.color: Style.currentTheme.colorBorderOn

            color: confAcceplatble ? "green" : "red"

            MText{
                text: "Used " + UiCore.currentDevice.processingUsed
                      + qsTr(" of ") + UiCore.currentDevice.processingBudget

                color: "white"

                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        ListView{
            id: _listConfig

            width: parent.width
            height: parent.height * 9/16

            spacing: 1

            z: _rectConf.z + 5

            interactive: false

            // clip: true

            model: DelegateModel{
                id: _visualModel

                delegate: DropArea{
                    id: _delegateRoot

                    property int modelIndex
                    property int visualIndex: DelegateModel.itemsIndex

                    width: _listConfig.width
                    height: _listConfig.height * 1/8

                    onEntered: function (drag) {
                        var from = drag.source.visualIndex;
                        var to = _thing.visualIndex;
                        _visualModel.items.move(from, to);
                    }

                    onDropped: function (drag) {
                        var from = modelIndex;
                        var to = (drag.source as Item).visualIndex;
                        UiCore.currentDevice.modulesListModel.moveModule(from, to);
                    }

                    MouseArea {
                        id: _mouseAreaDrag
                        anchors.fill: parent

                        drag.target: _thing
                        drag.axis: Drag.YAxis //Drag.XAndYAxis
                        drag.minimumY: 2
                        drag.maximumY: _listConfig.y + _listConfig.height + _rectDelete.height

                        onPressed: _delegateRoot.modelIndex = visualIndex

                        onReleased: _thing.Drag.drop()
                    }

                    Rectangle{
                        id: _thing

                        property Item dragParent: _delegateRoot
                        property int visualIndex: _delegateRoot.visualIndex

                        width: _delegateRoot.width
                        height: _delegateRoot.height

                        gradient: Gradient{
                            GradientStop{
                                position: 0.0; color: Style.colorModul
                            }
                            GradientStop{position: 1.0; color: "black"}

                        }

                        radius: Style.baseRadius
                        border.color: Style.currentTheme.colorBorderOn
                        border.width: Drag.active? 5 : 1

                        Drag.active: _mouseAreaDrag.drag.active
                        Drag.source: _thing
                        Drag.hotSpot.x: width / 2
                        Drag.hotSpot.y: height / 2


                        MText{
                            text: moduleInstance.moduleName + qsTr(", cost: ") + moduleInstance.processingTime

                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter

                            color: Style.currentTheme.colorTextEnabled
                        }

                        states: State {
                                when: _mouseAreaDrag.drag.active
                                ParentChange {
                                    target: _thing
                                    parent: _listConfig
                                }

                                AnchorChanges {
                                    target: _thing
                                    anchors.horizontalCenter: undefined
                                    anchors.verticalCenter: undefined
                                }
                            }
                    }
                }
            }

            //==================Animations=================================
            displaced: Transition {
                NumberAnimation {
                    properties: "x,y";
                    easing.type: Easing.OutQuad;}
            }

            moveDisplaced: Transition {
                     NumberAnimation { properties: "y"; duration: 150 }
            }

            remove: Transition{
                PropertyAction {
                    property: "ListView.delayRemove"
                    value: true
                }
                NumberAnimation {
                    property: "height"
                    to: 0
                    easing.type: Easing.InOutQuad
                }
                PropertyAction {
                    property: "ListView.delayRemove"
                    value: false
                }
            }

            DropArea{
                anchors.fill: parent
                z: -10
                onDropped: function (drag) {
                    var from = drag.source.dragParent.modelIndex;
                    var to = (drag.source as Item).visualIndex;
                    UiCore.currentDevice.modulesListModel.moveModule(from, to);
                }
            }

        }

        Rectangle{
            id: _rectDelete

            width: parent.width
            height: _listConfig.height * 1/8

            radius: Style.baseRadius
            border.width: 1
            border.color: Style.currentTheme.colorBorderOn

            color: _dropDelete.containsDrag ? "#60FFFFFF" : "transparent"

            Image
            {
                id: _scanImg
                source: "qrc:/images/bin.svg"

                anchors.centerIn: parent

                height: parent.height * 0.5

                fillMode: Image.PreserveAspectFit
            }

            DropArea{
                id: _dropDelete

                anchors.fill: parent

                onDropped:{
                    UiCore.currentDevice.modulesListModel.removeModule(drag.source.dragParent.modelIndex);
                }
            }
        }

        Rectangle {
            id: _spatialConf

            property bool deviceUpdatingValues: false

            color: Style.colorModul

            radius: Style.baseRadius
            border.width: 1
            border.color: Style.currentTheme.colorBorderOn

            width: _listConfig.width
            height: _listConfig.height * 6/16 * 0.8
            Column{
                anchors.fill: parent
                Item{
                    width: _listConfig.width
                    height: _listConfig.height * 1/16

                    MText{
                        anchors.centerIn: parent
                        text: "Delay/Reverb config"
                        color: Style.colorText
                    }
                }

                MComboBox{
                    id: _delayCombo
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: _listConfig.width * 0.7
                    height: _listConfig.height * 1/8 * 0.9

                    model: ["Delay disabled",
                            "Delay enabled(cost " + UiCore.currentDevice.DL.processingTime + ")"]

                    currentIndex: UiCore.currentDevice.DL.used

                    onActivated:
                    {
                        if(!_spatialConf.deviceUpdatingValues)
                        {
                            if(UiCore.currentDevice.processingUsed + UiCore.currentDevice.DL.processing > UiCore.currentDevice.processingBudget)
                            {
                                _delayCombo.currentIndex = 0;
                                _msgBudegetError.open();
                            }
                            UiCore.currentDevice.DL.used = _delayCombo.currentIndex;
                            UiCore.currentDevice.modulesListModel.sgModulesReconfigured()
                        }
                    }
                }

                MComboBox{
                    id: _earlyCombo
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: _listConfig.width * 0.7
                    height: _listConfig.height * 1/8 * 0.9

                    model: ["Early reflections disabled",
                        "Early reflections mono(cost " + UiCore.currentDevice.ER.processingTimeMono + ")",
                        "Early reflections stereo(cost) " + UiCore.currentDevice.ER.processingTimeStereo + ")"]

                    onActivated:
                    {
                        if(!_spatialConf.deviceUpdatingValues)
                        {
                            var revType;
                            switch(_earlyCombo.currentIndex)
                            {
                            case 0: revType = ModuleType.BYPASS; break;
                            case 1:
                            {
                                revType = ModuleType.ER_MONO;
                                if(UiCore.currentDevice.processingUsed + UiCore.currentDevice.ER.processingTimeMono > UiCore.currentDevice.processingBudget)
                                {
                                    _earlyCombo.currentIndex = 0;
                                    revType = ModuleType.BYPASS;
                                    _msgBudegetError.open();
                                }
                                break;
                            }
                            case 2:
                            {
                                revType = ModuleType.ER_STEREO;
                                if(UiCore.currentDevice.processingUsed + UiCore.currentDevice.ER.processingTimeStereo > UiCore.currentDevice.processingBudget)
                                {
                                    _earlyCombo.currentIndex = 0;
                                    revType = ModuleType.BYPASS;
                                    _msgBudegetError.open();
                                }
                                break;
                            }
                            }

                            UiCore.currentDevice.ER.reverbType = revType;
                            UiCore.currentDevice.modulesListModel.sgModulesReconfigured()
                        }
                    }

                    function revTypeToIndex()
                    {
                        switch(UiCore.currentDevice.ER.reverbType)
                        {
                        case ModuleType.ER_MONO: _earlyCombo.currentIndex = 1; break;
                        case ModuleType.ER_STEREO: _earlyCombo.currentIndex = 2; break;
                        default: _earlyCombo.currentIndex = 0;
                        }
                    }

                    Component.onCompleted: {
                        revTypeToIndex();
                    }
                }
            }

            Connections{
                target: UiCore.currentDevice

                function onDeviceUpdatingValues()
                {
                    _spatialConf.deviceUpdatingValues = true;
                    _delayCombo.currentIndex = UiCore.currentDevice.DL.used;
                    _earlyCombo.revTypeToIndex();
                    _spatialConf.deviceUpdatingValues = false;
                }
            }
        }
    }

    ModulsManagementWindow{
        id: _modulesManagementWindow

        visible: false
    }

    CustomMessageDialog
    {
        id: _msgBudegetError

        buttons: Dialog.Ok

        text: qsTr("Device doesn't have enough processing budget to use selected configuration");

        headerText: qsTr("Processing budget error")
    }
}
