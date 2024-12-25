import QtQuick 2.15
import QtQuick.Controls 2.15
import StyleSettings 1.0

import CustomOverlays 1.0
import ControlGroups 1.0
import Modules 1.0
import Elements 1.0

import CppObjects
import CppEnums

Item
{
    id: _main

    function setMapContent(){
        switch(UiCore.currentDevice.deviceClass)
        {
            case DeviceClass.CP_LEGACY:
            {
                _mapContentLoader.source = "/ControlGroups/qml/ControlGroups/MapCPLegacy.qml";
                break;
            }
            case DeviceClass.CP_MODERN:
            {
                _mapContentLoader.source = "/ControlGroups/qml/ControlGroups/MapCPModern.qml";
                break;
            }
        }
    }

    function setConfigContent(){
        switch(UiCore.currentDevice.deviceClass)
        {
            case DeviceClass.CP_LEGACY:
            {
                _mapContentLoader.source = "/ControlGroups/qml/ControlGroups/ConfigUnavaliable.qml";
                break;
            }
            case DeviceClass.CP_MODERN:
            {
                _mapContentLoader.source = "/ControlGroups/qml/ControlGroups/ConfigCP.qml";
                break;
            }
        }
    }

    Rectangle
    {
        anchors.fill: parent
        color: Style.colorFon
    }

    PresetsWindow
    {
        id: _presetsList

        // topHeaderSize: _mapContentLoader.height
    }

    Column{
        anchors.fill: parent
        spacing: 2

        Loader{
            id: _masterControlsLoader

            width:  parent.width
            height: parent.height * 1/5
        }


        Loader{
            id: _mapContentLoader

            width: parent.width
            height: parent.height - _masterControlsLoader.height

            onLoaded:{
                _mapContentLoader.item.modulesModel = UiCore.currentDevice.modulesListModel
            }
        }
    }

    Connections{
        target: UiCore.currentDevice.modulesListModel

        function onModelReset(){
            _mapContentLoader.item.modulesModel = undefined // без этого пропускает первое обновление
            _mapContentLoader.item.modulesModel = UiCore.currentDevice.modulesListModel
        }
    }

    CustomMessageDialog
    {
        id: msgPresetChangeSave

        property int newBank
        property int newPreset

        headerText: qsTr("Save preset")
        text: qsTr("Do you want to save your changes?")

        buttons: Dialog.Yes | Dialog.Discard | Dialog.Cancel

        closeOnDisconnect: true

        onAccepted:
        {
            UiCore.currentDevice.saveChanges();
            UiCore.currentDevice.changePreset(newBank, newPreset, true);
        }
        onDiscarded:
        {
            UiCore.currentDevice.changePreset(newBank, newPreset, true);
            visible = false;
        }
        onRejected:
        {
            // // TODO: more accurate way
            UiCore.currentDevice.restoreValue("bank-preset")
        }
    }

    Connections
    {
        target: UiCore

        function onCurrentDeviceChanged()
        {
            switch(UiCore.currentDevice.deviceClass)
            {
                case DeviceClass.ABSTRACT:
                {
                    _mapContentLoader.sourceComponent = undefined
                    break;
                }
                case DeviceClass.CP_LEGACY:
                {
                    _masterControlsLoader.source = "/ControlGroups/qml/ControlGroups/MasterControls_CP.qml";
                    _masterControlsLoader.item.openPresetsList.connect(_presetsList.open);
                    // _mapContentLoader.source = "/ControlGroups/qml/ControlGroups/MapCPLegacy.qml";
                    setMapContent();
                    break;
                }
                case DeviceClass.CP_MODERN:
                {
                    _masterControlsLoader.source = "/ControlGroups/qml/ControlGroups/MasterControls_CP.qml";
                    _masterControlsLoader.item.openPresetsList.connect(_presetsList.open);
                    // _mapContentLoader.source = "/ControlGroups/qml/ControlGroups/MapCPModern.qml";
                    _main.setMapContent();
                    break;
                }
            }
        }

        function onSgQmlRequestChangePreset(bank, preset)
        {
            if(UiCore.currentDevice.bank !== bank || UiCore.currentDevice.preset !== preset)
            {
                if(UiCore.currentDevice.deviceParamsModified)
                {
                    msgPresetChangeSave.newBank = bank;
                    msgPresetChangeSave.newPreset = preset;
                    msgPresetChangeSave.open();
                }
                else
                {
                    UiCore.currentDevice.changePreset(bank, preset);
                }
            }
        }
    }
}
