#include <QDebug>
#include <QStandardPaths>
#include <QDesktopServices>

#include <QProcess>
#include <QCoreApplication>
#include <QUrl>
#include <QLocale>

#include "uisettings.h"

UiSettings::UiSettings(QObject *parent)
    : QObject{parent}
{
#if defined(Q_OS_ANDROID)
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                    + "/settings.conf", QSettings::NativeFormat);
#elif defined(Q_OS_IOS)
    appSettings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                                    + "/settings.plist", QSettings::NativeFormat);
#else
    appSettings = new QSettings(QSettings::UserScope);
#endif
}

void UiSettings::setupApplication()
{
    m_windowWidth = appSettings->value("window_width", 0).toUInt();
    m_windowHeight = appSettings->value("window_height", 0).toUInt();

    //    QString colorTheme = appSettings->value("color_theme", "dark_orange").toString();
    //    emit sgSetUIText("color_theme", colorTheme);
    QString appLanguage = appSettings->value("application_language", "autoselect").toString();

    if(appLanguage=="autoselect")
    {
        appLanguage = QLocale().name().left(2);
    }
    setLanguage(appLanguage);

    emit sgApplicationStarted();
}

void UiSettings::saveSetting(QString settingName, QVariant settingValue)
{
    appSettings->setValue(settingName, settingValue);
    appSettings->sync();

    emit sgUiSettingsChanged();
    qDebug() << __FUNCTION__ << "Setting name: " << settingName << "Setting value:" << settingValue;
}


void UiSettings::setLanguage(QString languageCode)
{
    appSettings->setValue("application_language", languageCode);
    appSettings->sync();

    loadTranslator(languageCode);
    emit sgUiSettingsChanged();
}

void UiSettings::loadTranslator(QString languageCode)
{
    if(QCoreApplication::removeTranslator(&m_translator)) qDebug() << "Old translator removed";

    if(languageCode=="autoselect")
    {
        loadDefaultTranslator();
        return;
    }

    if (m_translator.load(pathFromCode.value(languageCode)))
    {
        qDebug() << "Translator loaded. Language: " << m_translator.language();
        QCoreApplication::installTranslator(&m_translator);

        emit sgTranslatorChanged(languageCode);
    }
    else qDebug() << "Translator not found. Using english";
}

void UiSettings::loadDefaultTranslator()
{
    if (m_translator.load(QLocale(), QLatin1String("Pangaea-desktop"),
                        QLatin1String("_"), ":/translations/"))
    {
        qDebug() << "Default ranslator loaded. Locale: " << QLocale();
        QCoreApplication::installTranslator(&m_translator);
        emit sgTranslatorChanged(QLocale().nativeLanguageName());
    }
}

bool UiSettings::autoConnectEnabled()
{
    return appSettings->value("autoconnect_enable", false).toBool();
}

bool UiSettings::checkUpdatesEnabled()
{
    return appSettings->value("check_updates_enable", false).toBool();
}

bool UiSettings::isModulesRightAligned()
{
    return appSettings->value("modules_right_aligned", false).toBool();
}

bool UiSettings::eqClassicView()
{
    return appSettings->value("eq_classic_view", false).toBool();
}

int UiSettings::windowWidth()
{
    return m_windowWidth;
}

int UiSettings::windowHeight()
{
    return m_windowHeight;
}

QString UiSettings::appLanguageCode()
{
    return appSettings->value("application_language", "autoselect").toString();
}

ColorTheme UiSettings::colorTheme() const
{
    QVariant currentTheme = appSettings->value("Color_theme", ColorTheme::DarkOrange);
    return currentTheme.value<ColorTheme>();
}

void UiSettings::setColorTheme(ColorTheme newColorTheme)
{
    saveSetting("Color_theme", newColorTheme);

    emit colorThemeChanged();
}

bool UiSettings::autoConvertWav() const
{
    return appSettings->value("auto_convert_wav", false).toBool();
}

bool UiSettings::autoTrimWav() const
{
    return appSettings->value("auto_trim_wav", false).toBool();
}
