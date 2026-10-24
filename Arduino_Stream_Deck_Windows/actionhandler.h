
#pragma once

#include <QObject>
#include <QString>

class ActionHandler : public QObject
{
    Q_OBJECT

public:
    explicit ActionHandler(QObject *parent = nullptr);

    Q_INVOKABLE void executeAction(const QString &action,
                                   const QString &value = "");
};
