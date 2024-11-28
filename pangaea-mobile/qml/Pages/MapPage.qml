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

    Rectangle
    {
        anchors.fill: parent
        color: Style.colorFon
    }

    Loader{
        id: _mapContentLoader

        anchors.fill: parent
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
                    // _masterControlsLoader.source = "../ControlGroups/MasterControls_CP.qml";
                    _mapContentLoader.source = "../ControlGroups/MapCPLegacy.qml";
                    break;
                }
                case DeviceClass.CP_MODERN:
                {
                    // _masterControlsLoader.source = "../ControlGroups/MasterControls_CP.qml";
                    _mapContentLoader.source = "../ControlGroups/MapCPModern.qml";
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
