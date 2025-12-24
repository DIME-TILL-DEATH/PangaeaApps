#ifndef UISETTINGS_H
#define UISETTINGS_H

#include <QObject>
#include <QSettings>
#include <QTranslator>
#include <QQmlEngine>

class UiSettings : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(bool autoConnectEnabled READ autoConnectEnabled NOTIFY sgUiSettingsChanged)
    Q_PROPERTY(bool checkUpdatesEnabled READ checkUpdatesEnabled NOTIFY sgUiSettingsChanged)

    Q_PROPERTY(bool autoConvertWav READ autoConvertWav NOTIFY sgUiSettingsChanged FINAL)
    Q_PROPERTY(bool autoTrimWav READ autoTrimWav NOTIFY sgUiSettingsChanged FINAL)

    Q_PROPERTY(bool isModulesRightAligned READ isModulesRightAligned NOTIFY sgUiSettingsChanged)
    Q_PROPERTY(bool eqClassicView READ eqClassicView NOTIFY sgUiSettingsChanged FINAL)

    Q_PROPERTY(int windowWidth READ windowWidth NOTIFY sgUiSettingsChanged)
    Q_PROPERTY(int windowHeight READ windowHeight NOTIFY sgUiSettingsChanged)

    Q_PROPERTY(QString appLanguageCode READ appLanguageCode NOTIFY sgUiSettingsChanged)

    Q_PROPERTY(ColorTheme colorTheme READ colorTheme WRITE setColorTheme NOTIFY colorThemeChanged FINAL)
public:
    explicit UiSettings(QObject *parent = nullptr);

    enum ColorTheme
    {
        ClassicBlue,
        DarkOrange,
        DarkBlue,
        DarkGreen
    };
    Q_ENUM(ColorTheme)

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

    ColorTheme colorTheme() const;
    void setColorTheme(ColorTheme newColorTheme);

    bool autoConvertWav() const;
    bool autoTrimWav() const;

signals:
    void sgUiSettingsChanged();
    void sgApplicationStarted();
    void sgTranslatorChanged(QString langauageCode);

    void colorThemeChanged();

private:
    QSettings* appSettings;

    int m_windowWidth;
    int m_windowHeight;

    QTranslator m_translator;

    // TODO only CPPA, not mobile!
    QMap<QString, QString> pathFromCode
    {
        {"en", ":/translations/Pangaea-desktop_en.qm"},
        {"ru", ":/translations/Pangaea-desktop_ru.qm"},
        {"it", ":/translations/Pangaea-desktop_it.qm"},
        {"de", ":/translations/Pangaea-desktop_de.qm"}
    };
    void loadTranslator(QString languageCode);
    void loadDefaultTranslator();
};
typedef UiSettings::ColorTheme ColorTheme;
Q_DECLARE_METATYPE(ColorTheme)

#endif // UISETTINGS_H
