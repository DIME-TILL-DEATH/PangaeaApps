import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15

import StyleSettings 1.0

import Elements 1.0
import Modules 1.0

import CppObjects


GridLayout
{
    id: _root

    property int mode: UiCore.currentDevice.outputMode

    rows: 2
    columns: 3

    property bool copyFirst: false
    property bool presetEdited: UiCore.currentDevice.deviceParamsModified
    property bool compareState: AppProperties.compareState

    signal openPresetsList()

    ColumnLayout
    {
        Layout.preferredWidth: parent.width*2/5 - borderSpacings*2
        Layout.preferredHeight: parent.height*11/15
        Layout.fillWidth: true
        Layout.fillHeight: true

        spacing: borderSpacings

        MButton
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            //: Output mode. Phones.
            textButton: qsTr("Phones")
            active: (mode == 0)
            onMbPressed:
            {
                mode = 0;
                UiCore.currentDevice.outputMode = 0;
            }
        }

        MButton
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            //: Output mode. Balanced out.
            textButton: qsTr("Balanced")
            active: (mode == 2)
            onMbPressed:
            {
                UiCore.currentDevice.outputMode = 2;
            }
        }

        MButton
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            //: Output mode. Line out.
            textButton: qsTr("Line")
            active: (mode == 1)
            onMbPressed:
            {
                UiCore.currentDevice.outputMode = 1;
            }
        }
    }

    ColumnLayout
    {
        Layout.preferredWidth: parent.width*3/10
        Layout.preferredHeight: parent.height*11/15
        spacing: borderSpacings

        MButton
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            //: Button text. Export preset
            textButton: qsTr("Export")
            enabled: !AppProperties.compareState

            onMbPressed: {
                if(_root.presetEdited)
                    UiCore.sgSetUIText("preset_not_saved", "");
                else
                    UiCore.exportPreset("");
            }


        }

        MButton
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Почему-то именно кнопка save при смене своего состояния
            // через свойство "enabled" отключала обновление SwipeView до любого следующего действия
            // поэтому добавлено своё свойство "enabled" отключающее только нужные свойства
            enabledKostyl: _root.presetEdited & !AppProperties.compareState

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
            highlighted: AppProperties.compareState

            //: Button text. Compare preset
            textButton: qsTr("Comp")
            onMbPressed: AppProperties.comparePreset();
        }
    }

    ColumnLayout
    {
        Layout.preferredWidth: parent.width*3/10
        Layout.preferredHeight: parent.height*11/15
        spacing: borderSpacings

        MButton
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            //: Button text. Import preset
            textButton: qsTr("Import")
            enabled: !AppProperties.compareState

            onMbPressed:
            {
                UiCore.importPreset("");
            }
        }

        MButton
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            //: Button text. Copy preset.
            textButton: qsTr("Copy")
            enabled: !AppProperties.compareState

            onMbPressed:
            {
                AppProperties.copyPreset();
                copyFirst=true;
            }
        }
        MButton
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            //: Button text. Paste preset
            textButton: qsTr("Paste")
            enabled: copyFirst & !AppProperties.compareState

            onMbPressed:
            {
                AppProperties.pastePreset();
            }
        }
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