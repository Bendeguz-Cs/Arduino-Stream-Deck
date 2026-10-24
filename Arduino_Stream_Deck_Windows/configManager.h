#pragma once

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QStandardPaths>
#include <QSet>
#include <QObject>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <algorithm>

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;

    Q_INVOKABLE QVariantList load()
    {
        QSettings settings;
        QJsonDocument doc = QJsonDocument::fromJson(
            settings.value("buttonConfigs").toByteArray());

        QVariantList result = doc.array().toVariantList();

        if (result.isEmpty()) {
            for (int i = 0; i < 8; ++i)
                result.append(QVariantMap{
                    {"action", "none"},
                    {"value", ""}
                });
        }

        return result;
    }

    Q_INVOKABLE void save(const QVariantList &configs)
    {
        QJsonArray array;

        for (const auto &item : configs)
            array.append(QJsonObject::fromVariantMap(item.toMap()));

        QSettings settings;
        settings.setValue("buttonConfigs",
                          QJsonDocument(array).toJson(QJsonDocument::Compact));
    }

    Q_INVOKABLE QVariantList availableApplications() const
    {
        QVariantList result;
        QSet<QString> seenPaths;

        QStringList folders =
            QStandardPaths::standardLocations(
                QStandardPaths::ApplicationsLocation);

        // Also include the system-wide Start Menu.
        const QString commonStartMenu =
            qEnvironmentVariable("PROGRAMDATA")
            + "/Microsoft/Windows/Start Menu/Programs";

        if (!commonStartMenu.isEmpty())
            folders.append(commonStartMenu);

        for (const QString &folder : folders) {
            QDirIterator it(
                folder,
                QStringList() << "*.lnk",
                QDir::Files,
                QDirIterator::Subdirectories);

            while (it.hasNext()) {
                const QString path = it.next();
                const QString canonical =
                    QFileInfo(path).canonicalFilePath();

                // Skip duplicate shortcuts.
                const QString key = canonical.isEmpty()
                                        ? QDir::cleanPath(path).toLower()
                                        : canonical.toLower();

                if (seenPaths.contains(key))
                    continue;

                seenPaths.insert(key);

                QString name = QFileInfo(path).completeBaseName();

                // Ignore empty names and uninstall shortcuts.
                if (name.isEmpty())
                    continue;

                if (name.contains("uninstall",
                                  Qt::CaseInsensitive))
                    continue;

                result.append(QVariantMap{
                    {"name", name},
                    {"path", path}
                });
            }
        }

        std::sort(result.begin(), result.end(),
                  [](const QVariant &a, const QVariant &b) {
                      return a.toMap().value("name").toString()
                      .compare(
                          b.toMap().value("name").toString(),
                          Qt::CaseInsensitive) < 0;
                  });

        return result;
    }
};