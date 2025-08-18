import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import Elements
import StyleSettings

import CppObjects
import PangaeaBackend

Window
{
    id: main

    width: 400
    height: 500

    title: qsTr("Add/Remove module")

    Rectangle
    {
        anchors.fill: parent
        color: Style.currentTheme.mainEnabledColor

        enabled: UiCore.currentDevice.presetManager.currentState === PresetState.Idle
        opacity: enabled ? 1 : 0.5

        Column{
            id: _configColumn

            width: parent.width*0.95
            height: parent.height// - _dialogButtonBox.height

            anchors.horizontalCenter: parent.horizontalCenter

            spacing: parent.height * 0.01

            Item{
                width: parent.width
                height: parent.height * 0.05
                MText{
                    text: qsTr("Processing points used ") + UiCore.currentDevice.processingUsed
                                + qsTr(" of ") + UiCore.currentDevice.processingBudget
                    color: Style.currentTheme.textEnabled
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Rectangle {
                id: _sepTop

                width: parent.width
                height: 2
            }

            ListView{
                id: _moduleListView

                width: parent.width * 0.9
                height: parent.height * 0.7

                anchors.horizontalCenter: parent.horizontalCenter

              //  model: UiCore.currentDevice.avaliableModulesList


                boundsBehavior: Flickable.StopAtBounds
                clip: true

                ScrollBar.vertical: ScrollBar{
                    active: true
                    visible: true
                }

                spacing: 3

                snapMode: ListView.SnapToItem

                delegate: Rectangle{
                    id: _rectDelegate

                    width: _moduleListView.width
                    height: _moduleListView.height/11

                    color: Style.currentTheme.mainEnabledColor

                    radius: height/10

                    border.width: Drag.active? 5 : 2
                    border.color: Style.currentTheme.borderOn


                    Row{
                        id: _delegateRow

                        width: parent.width * 0.9
                        height: parent.height * 0.8

                        spacing: parent.width / 32

                        anchors.centerIn: parent

                        property bool isResourcesEnough: (UiCore.currentDevice.processingUsed + modelData.processingTime < UiCore.currentDevice.processingBudget)

                        Item{
                            width: parent.width * 6/16
                            height: parent.height
                            MText{
                                text: modelData.fullModuleName


                                color:  modelData.used ? Style.currentTheme.textDisabled : Style.currentTheme.textEnabled

                                anchors.fill: parent

                                elide: Text.ElideMiddle

                                horizontalAlignment: Qt.AlignHCenter
                                verticalAlignment: Qt.AlignVCenter
                            }
                        }

                        Item{
                            width: parent.width * 1/4
                            height: parent.height
                            MText{
                                text: qsTr("cost: ") + modelData.processingTime

                                anchors.centerIn: parent
                                color: modelData.used ? (Style.currentTheme.textDisabled) :
                                                        (_delegateRow.isResourcesEnough ? Style.currentTheme.textEnabled : "red")


                                elide: Text.ElideMiddle

                                horizontalAlignment: Qt.AlignHCenter
                                verticalAlignment: Qt.AlignVCenter
                            }
                        }

                        MButton{
                            text: "?"

                            width: parent.width * 2/16
                            height: parent.height

                            scaleText: 2

                            onClicked:{
                                switch(modelData.moduleType)
                                {
                                case ModuleType.NG: _tutorialDialog.text = _tutorials.ngTutorial; break;
                                case ModuleType.CM: _tutorialDialog.text = _tutorials.cmTutorial; break;
                                case ModuleType.PR: _tutorialDialog.text = _tutorials.prTutorial; break;
                                case ModuleType.PA: _tutorialDialog.text = _tutorials.paTutorial; break;
                                case ModuleType.IR: _tutorialDialog.text = _tutorials.irTutorial; break;
                                case ModuleType.TR: _tutorialDialog.text = _tutorials.trTutorial; break;
                                case ModuleType.CH: _tutorialDialog.text = _tutorials.chTutorial; break;
                                case ModuleType.PH: _tutorialDialog.text = _tutorials.phTutorial; break;
                                case ModuleType.EQ1:
                                case ModuleType.EQ2: _tutorialDialog.text = _tutorials.eqTutorial; break;
                                }

                                _tutorialDialog.open()
                            }

                            Tutorials{
                                id: _tutorials
                            }
                        }

                        MButton{
                            id: _ctrlBtn

                            text: modelData.used ? "-" : "+"

                            scaleText: 2

                            width: parent.width * 2/16
                            height: parent.height

                            enabled: modelData.used ? "true" : _delegateRow.isResourcesEnough

                            onClicked:{
                                if(!modelData.used )
                                    UiCore.currentDevice.modulesListModel.appendModule(modelData)
                                else
                                    UiCore.currentDevice.modulesListModel.removeModuleByType(modelData.moduleType)
                            }

                        }

                    }
                }
            }

            Rectangle {
                id: _sepBtm

                width: parent.width
                height: 2
            }

            property bool deviceUpdatingValues

            MComboVertical{
                id: _delayCombo
                anchors.horizontalCenter: parent.horizontalCenter
                width: _moduleListView.width
                height: _moduleListView.height * 1/8 * 0.9

                model: [qsTr("Delay(DL) disabled"),
                        qsTr("Delay(DL) enabled(cost ") + UiCore.currentDevice.DL.processingTime + ")"]

                currentIndex: UiCore.currentDevice.DL.used

                onActivated:
                {
                    if(!_configColumn.deviceUpdatingValues)
                    {
                        if(UiCore.currentDevice.processingUsed + UiCore.currentDevice.DL.processingTime > UiCore.currentDevice.processingBudget)
                        {
                            if(_delayCombo.currentIndex === 1)
                                _msgBudegetError.open();

                            _delayCombo.currentIndex = 0;
                        }

                        UiCore.currentDevice.DL.used = _delayCombo.currentIndex;
                        UiCore.currentDevice.modulesListModel.sgModulesReconfigured()
                    }
                }
            }

            MComboVertical{
                id: _earlyCombo
                anchors.horizontalCenter: parent.horizontalCenter
                width: _moduleListView.width
                height: _moduleListView.height * 1/8 * 0.9

                model: [qsTr("Early reflections(ER) disabled"),
                    qsTr("Early reflections(ER) mono(cost ") + UiCore.currentDevice.ER.processingTimeMono + ")",
                    qsTr("Early reflections(ER) stereo(cost) ") + UiCore.currentDevice.ER.processingTimeStereo + ")"]

                onActivated:
                {
                    if(!_configColumn.deviceUpdatingValues)
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

            Connections{
                target: UiCore.currentDevice

                function onDeviceUpdatingValues()
                {
                    _configColumn.deviceUpdatingValues = true;
                    _delayCombo.currentIndex = UiCore.currentDevice.DL.used;
                    _earlyCombo.revTypeToIndex();
                    _configColumn.deviceUpdatingValues = false;
                }
            }
        }
    }



    Component.onCompleted:{
       _moduleListView.model = UiCore.currentDevice.avaliableModulesList
    }

    MessageDialog{
        id: _tutorialDialog

        buttons: Dialog.Ok
    }

    MessageDialog
    {
        id: _msgBudegetError

        buttons: Dialog.Ok

        title: qsTr("Processing budget error")
        text: qsTr("Device doesn't have enough processing budget to use selected configuration");
    }
}
