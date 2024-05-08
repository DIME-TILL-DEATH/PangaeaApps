#ifndef UISETTINGS_H
#define UISETTINGS_H

#include <QObject>
#include <QSettings>
#include <QTranslator>

class UiSettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool autoConnectEnabled READ autoConnectEnabled NOTIFY sgUiSettingsChanged)
    Q_PROPERTY(bool checkUpdatesEnabled READ checkUpdatesEnabled NOTIFY sgUiSettingsChanged)
    Q_PROPERTY(bool isModulesRightAligned READ isModulesRightAligned NOTIFY sgUiSettingsChanged)
    Q_PROPERTY(bool eqClassicView READ eqClassicView NOTIFY sgUiSettingsChanged FINAL)

    Q_PROPERTY(int windowWidth READ windowWidth NOTIFY sgUiSettingsChanged)
    Q_PROPERTY(int windowHeight READ windowHeight NOTIFY sgUiSettingsChanged)

    Q_PROPERTY(QString appLanguageCode READ appLanguageCode NOTIFY sgUiSettingsChanged)
public:
    explicit UiSettings(QObject *parent = nullptr);

    Q_INVOKABLE void setupApplication();
    Q_INVOKABLE void saveSetting(QString settingName, QVariant settingValue);
    Q_INVOKABLE void setLanguage(QString languageCode);

    bool autoConnectEnabled();
    bool checkUpdatesEnabled();
    bool isModulesRightAligned();
    bool eqClassicView();

    int windowWidth();
    int windowHeight();

    QString appLanguageCode();

signals:
    void sgUiSettingsChanged();
    void sgApplicationStarted();
    void sgTranslatorChanged(QString langauageCode);

private:
    QSettings* appSettings;

    int m_windowWidth;
    int m_windowHeight;

    QTranslator m_translator;

    QMap<QString, QString> pathFromCode
    {
        {"en", ":/translations/PangaeaCPPA_en.qm"},
        {"ru", ":/translations/PangaeaCPPA_ru.qm"},
        {"it", ":/translations/PangaeaCPPA_it.qm"},
        {"de", ":/translations/PangaeaCPPA_de.qm"}
    };
    void loadTranslator(QString languageCode);
    void loadDefaultTranslator();
};

#endif // UISETTINGS_H
