import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15
import QtQuick.Dialogs
import QtCore

import StyleSettings 1.0

import Elements 1.0
import Modules 1.0
import CustomOverlays

import CppObjects
import PangaeaBackend

GridLayout
{
    id: _root

    property int mode: UiCore.currentDevice.outputMode

    rows: 2
    columns: 3

    property bool copyFirst: false
    property bool presetEdited: UiCore.currentDevice.deviceParamsModified

    signal openPresetsList()

    ColumnLayout
    {
        Layout.preferredWidth: parent.width*1/3 - borderSpacings*2
        Layout.preferredHeight: parent.height*9/15
        Layout.fillWidth: true
        Layout.fillHeight: true

        spacing: borderSpacings

        MButton
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Почему-то именно кнопка save при смене своего состояния
            // через свойство "enabled" отключала обновление SwipeView до любого следующего действия
            // поэтому добавлено своё свойство "enabled" отключающее только нужные свойства
            enabledKostyl: _root.presetEdited & (UiCore.currentDevice.presetManager.currentState !== PresetState.Compare)

            //: Button text. Save preset
            textButton: qsTr("Save")

            onMbPressed:
            {
                UiCore.currentDevice.saveChanges();
            }
        }
        MButton
        {
            id: _btnCompare

            Layout.fillWidth: true
            Layout.fillHeight: true

            enabled: _root.presetEdited
            highlighted: UiCore.currentDevice.presetManager.currentState === PresetState.Compare

            //: Button text. Compare preset
            textButton: qsTr("Comp")
            onMbPressed: UiCore.currentDevice.comparePreset();
        }
    }

    ColumnLayout
    {
        Layout.preferredWidth: parent.width*1/3
        Layout.preferredHeight: parent.height*9/15
        spacing: borderSpacings

        MButton
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            //: Button text. Copy preset.
            textButton: qsTr("Copy")
            enabled: UiCore.currentDevice.presetManager.currentState !== PresetState.Compare

            onMbPressed:
            {
                UiCore.currentDevice.copyPreset();
                copyFirst=true;
            }
        }
        MButton
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            //: Button text. Paste preset
            textButton: qsTr("Paste")
            enabled: copyFirst & (UiCore.currentDevice.presetManager.currentState !== PresetState.Compare)

            onMbPressed:
            {
                UiCore.currentDevice.pastePreset();
            }
        }
    }

    ColumnLayout
    {
        Layout.preferredWidth: parent.width*1/3
        Layout.preferredHeight: parent.height*9/15
        spacing: borderSpacings

        MButton
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            //: Button text. Import preset
            textButton: qsTr("Import")
            enabled: UiCore.currentDevice.presetManager.currentState !== PresetState.Compare

            onMbPressed:
            {
                if(Qt.platform.os === "android")
                    UiCore.importPreset("");
                else
                    _iosFileImportDialog.open();
            }

            FileDialog{
                id: _iosFileImportDialog

                onAccepted:
                {
                    UiCore.importPreset(_iosFileImportDialog.currentFile, _root.dstIrPath);

                }
            }
        }

        MButton
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            //: Button text. Export preset
            textButton: qsTr("Export")
            enabled: UiCore.currentDevice.presetManager.currentState !== PresetState.Compare

            onMbPressed: {
                if(_root.presetEdited){
                    UiCore.currentDevice.sgDeviceError(DeviceErrorType.PresetNotSaved);
                }
                else{
                    if(Qt.platform.os === "android")
                        UiCore.exportPreset("");
                    else
                        _fileNameDialog.open();
                }
            }
        }
    }

    EditTextDialog
    {
        id: _fileNameDialog

        contentText: UiCore.moduleName

        width: Screen.width * 0.75

        headerText: qsTr("Export preset")
        text: qsTr("Enter file name:")

        onAccepted:
        {
            var path = StandardPaths.writableLocation(StandardPaths.DocumentsLocation)

            var name = contentText
            if(name === "") name = "preset"

            var fullPath = path + "/" + name + ".pst"

            UiCore.exportPreset(fullPath);
        }
    }


    OutputMode {
        id: _outputModeModule

        Layout.preferredWidth: parent.width
        Layout.preferredHeight:  parent.height*3/15
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.columnSpan: 3
    }

    VL
    {
        Layout.preferredWidth: parent.width
        Layout.preferredHeight:  parent.height*3/15
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.columnSpan: 3
    }
}
